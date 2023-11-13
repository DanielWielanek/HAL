/*
 * CorrFitSmear1DCF.cxx
 *
 *  Created on: 4 sty 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitSmear1DCF.h"
#include <iostream>

#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Splines.h"
#include "StdHist.h"
#include "StdMath.h"

#include <TAxis.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>


namespace Hal {
  CorrFitSmear1DCF::CorrFitSmear1DCF() {
    fFunc =
      new TF1(Hal::Std::GetUniqueName("func_draw"), this, &CorrFitSmear1DCF::GaussNS, -10, 10, 2, this->ClassName(), "GaussNS");
  }

  CorrFitSmear1DCF::~CorrFitSmear1DCF() {
    if (fCF) delete fCF;
    if (fFunc) delete fFunc;
    if (fSpline) delete fSpline;
    if (fSplot) delete fSplot;
  }

  void CorrFitSmear1DCF::SetCF(Hal::Femto1DCF& cf) {
    if (fCF) {
      delete fCF;
      fCF = nullptr;
    }
    fCF = (Hal::Femto1DCF*) cf.Clone();
  }

  TH2D CorrFitSmear1DCF::RecalcSmearMatrix(TH2D& matrix) {
    if (!fCF) {
      Hal::Cout::PrintInfo("Cannot recalculate smear matrix", EInfo::kWarning);
      return TH2D();
    }
    int binsX;
    double minX, maxX;

    Hal::Std::GetAxisPar(*fCF->GetNum(), binsX, minX, maxX, "x");
    TH2D newMatrix("newMatrix", "newMatrix", binsX, minX, maxX, binsX, minX, maxX);
    Hal::Spline2D spline(&matrix, "const");
    for (int i = 1; i <= binsX; i++) {
      double x = matrix.GetXaxis()->GetBinCenter(i);
      for (int j = 1; j <= binsX; j++) {
        double y   = matrix.GetYaxis()->GetBinCenter(j);
        double val = spline.Eval(x, y);
        newMatrix.GetBinContent(i, j, val);
      }
    }
    return newMatrix;
  }

  CorrFitMapKstarRstar* CorrFitSmear1DCF::SmearMatrix(const TH2D& smearMatrix) {
    CorrFitMapKstarRstar* newMap = new CorrFitMapKstarRstar(*fMap);
    auto histo                   = newMap->GetHisto();
    histo->Reset();
    Int_t sizeQ  = newMap->GetHisto()->GetNbinsX();
    auto smearMT = Std::GetMatrix(smearMatrix, kFALSE);
    NormalizeMatrix(smearMT);
    std::vector<double> correction;
    if (fAutoFill) { correction = GetAutoFill(smearMT, (smearMatrix.GetXaxis()->GetBinCenter(1) < 0)); }
    auto print = [](const TMatrixD x, TString flag) {
#ifdef _DEBUG__CORRFITSMEAR1DCF_
      std::cout << flag << std::endl;
      for (int i = 0; i < x.GetNrows(); i++) {
        for (int j = 0; j < x.GetNcols(); j++) {
          std::cout << Form(" %4.4f ", x[i][j]);
        }
        std::cout << std::endl;
      }
#endif
    };


    if (fSeparateSmear) {
      auto den           = (TH1D*) fCF->GetDen();
      auto maph          = fMap->GetHisto();
      auto denVec        = GetVec(*den);
      auto denVecSmeared = smearMT * denVec;
      for (int i = 1; i <= maph->GetNbinsY(); i++) {
        auto sim           = GetVec(*maph, i);
        auto numVec        = Multiply(sim, denVec);
        auto numVecSmeared = smearMT * numVec;
        auto reco          = smearMT * sim;
        for (int j = 1; j <= sizeQ; j++) {
          if (fAutoFill) {
            histo->SetBinContent(j, i, numVecSmeared[j - 1][0] / denVecSmeared[j - 1][0] + correction[j - 1]);
          } else {
            histo->SetBinContent(j, i, numVecSmeared[j - 1][0] / denVecSmeared[j - 1][0]);
          }
        }
      }
    } else {
      auto maph = fMap->GetHisto();
      for (int i = 1; i <= maph->GetNbinsY(); i++) {
        auto sim = GetVec(*maph, i);

        auto reco = smearMT * sim;

        if (i == 1) {
          print(sim, "SIM MOM");
          print(smearMT, "MATRIX");
          print(reco, "RECO");
        }
        for (int j = 1; j <= sizeQ; j++) {
          if (fAutoFill) {
            histo->SetBinContent(j, i, reco[j - 1][0] + correction[j - 1]);
          } else {
            histo->SetBinContent(j, i, reco[j - 1][0]);
          }
        }
      }
    }

    return newMap;
  }

  CorrFitMapKstarRstar* CorrFitSmear1DCF::SmearFunction(TF1* func) {
    CorrFitMapKstarRstar* newMap = new CorrFitMapKstarRstar(*fMap);
    auto histo                   = newMap->GetHisto();
    histo->Reset();
    std::vector<double> norms = GetGausNorms(func, histo->GetXaxis());
    TAxis* y                  = histo->GetYaxis();
    TAxis* x                  = histo->GetXaxis();

    if (fSpline) {
      delete fSpline;
      fSpline = nullptr;
    }
    auto h  = Hal::Std::GetProjection1D(histo, 1, 1, "x+bins");
    fSpline = new Hal::Spline1D(h);
    delete h;
    if (!fSplot) {
      fSplot =
        new TF1(Hal::Std::GetUniqueName("func_splot"), this, &CorrFitSmear1DCF::Splot, -10, 10, 1, this->ClassName(), "Splot");
    }
    if (!fSeparateSmear) {
      for (int iR = 1; iR <= y->GetNbins(); iR++) {
        h = Hal::Std::GetProjection1D(fMap->GetHisto(), iR, iR, "x+bins+randname");
        fSpline->FastOverwrite(h, h->GetBinContent(1), h->GetBinContent(x->GetNbins()));
        fSpline->Refit();
        delete h;
        for (int jQ = 1; jQ <= x->GetNbins(); jQ++) {
          Double_t q     = x->GetBinCenter(jQ);
          Double_t sigma = func->Eval(q);
          fFunc->FixParameter(1, norms[jQ - 1]);  // normalize gauss
          fFunc->FixParameter(0, sigma);          // set sigma
          fSplot->FixParameter(0, q);
          Double_t integral = fSplot->Integral(-4.0 * sigma, 4.0 * sigma, 1e-3);
          histo->SetBinContent(jQ, iR, integral);
        }
      }
    } else {  // TODO make this
      for (int iR = 1; iR <= y->GetNbins(); iR++) {
        h = Hal::Std::GetProjection1D(fMap->GetHisto(), iR, iR, "x+bins+randname");
        fSpline->FastOverwrite(h, h->GetBinContent(1), h->GetBinContent(x->GetNbins()));
        fSpline->Refit();
        delete h;
        for (int jQ = 1; jQ <= x->GetNbins(); jQ++) {
          Double_t q     = x->GetBinCenter(jQ);
          Double_t sigma = func->Eval(q);
          fFunc->FixParameter(1, norms[jQ - 1]);  // normalize gauss
          fFunc->FixParameter(0, sigma);          // set sigma
          fSplot->FixParameter(0, q);
          Double_t integral = fSplot->Integral(-4.0 * sigma, 4.0 * sigma, 1e-3);
          histo->SetBinContent(jQ, iR, integral);
        }
      }
    }
    newMap->Recalc();
    return newMap;
  }

  TMatrixD CorrFitSmear1DCF::GetVec(const TH2D& vec, Int_t bin) const {
    TMatrixD vect(vec.GetNbinsX(), 1);
    for (int i = 1; i <= vec.GetNbinsX(); i++) {
      vect[i - 1][0] = vec.GetBinContent(i, bin);
    }
    return vect;
  }

  TMatrixD CorrFitSmear1DCF::GetVec(const TH1D& vec) const {
    TMatrixD vect(vec.GetNbinsX(), 1);
    for (int i = 1; i <= vec.GetNbinsX(); i++) {
      vect[i - 1][0] = vec.GetBinContent(i);
    }
    return vect;
  }

  void CorrFitSmear1DCF::NormalizeMatrix(TMatrixD& matrix) const {
    int rowcol = matrix.GetNrows();
    for (int i = 0; i < rowcol; i++) {
      double sum = 0;
      for (int j = 0; j < rowcol; j++) {
        sum += matrix[j][i];
      }
      if (sum != 0)
        sum = 1.0 / sum;
      else {
        sum          = 1;
        matrix[i][i] = 1;  // y,x
      }
      for (int j = 0; j < rowcol; j++) {
        matrix[j][i] = matrix[j][i] * sum;
      }
    }
  }

  TMatrixD CorrFitSmear1DCF::Multiply(TMatrixD A, TMatrixD B) const {
    TMatrixD res(A.GetNrows(), 1);
    for (int i = 0; i < A.GetNrows(); i++) {
      res[i][0] = A[i][0] * B[i][0];
    }
    return res;
  }

  TH2D CorrFitSmear1DCF::TransposeHistogram(TH2D& matrix) {
    TH2D h(matrix);
    for (int i = 1; i <= matrix.GetNbinsX(); i++) {
      for (int j = 1; j <= matrix.GetNbinsX(); j++) {
        h.SetBinContent(i, j, matrix.GetBinContent(j, i));
      }
    }
    return h;
  }

  Double_t CorrFitSmear1DCF::GaussNS(Double_t* x, Double_t* params) const {
    return params[1] * TMath::Exp(-(x[0] * x[0] / (2.0 * params[0] * params[0])));
  }

  Double_t CorrFitSmear1DCF::CorrFunc(Double_t* x, Double_t* params) const { return fSpline->Eval(x[0]); }

  Double_t CorrFitSmear1DCF::Splot(Double_t* x, Double_t* params) const {
    Double_t kstar = x[0] + params[0];
    if (kstar < 0) kstar = -kstar;
    return fSpline->Eval(kstar) * fFunc->Eval(x[0]);
  }

  std::vector<double> CorrFitSmear1DCF::GetAutoFill(const TMatrixD& smearMT, Bool_t lower) const {
    std::vector<double> correction;
    int size = smearMT.GetNrows();
    if (lower) {
      int half = size / 2;
      // lower part
      for (int i = 0; i < half; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) {
          sum -= smearMT[j][i];
        }
        for (int j = i + 1; j < size; j++) {
          sum += smearMT[j][i];
        }
        if (sum < 0) sum = 0;
        correction.push_back(sum);
      }

      // upper part
      for (int i = half; i < size; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) {
          sum += smearMT[j][i];
        }
        for (int j = i + 1; j < size; j++) {
          sum -= smearMT[j][i];
        }
        if (sum < 0) sum = 0;
        correction.push_back(sum);
      }
    } else {
      for (int i = 0; i < size; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) {
          sum += smearMT[j][i];
        }
        for (int j = i + 1; j < size; j++) {
          sum -= smearMT[j][i];
        }
        if (sum < 0) sum = 0;
        correction.push_back(sum);
      }
    }
    return correction;
  }

  std::vector<double> CorrFitSmear1DCF::GetGausNorms(TF1* f, TAxis* x) const {
    std::vector<double> norms;
    for (int i = 1; i <= x->GetNbins(); i++) {
      Double_t kstar = x->GetBinCenter(i);
      Double_t sigma = f->Eval(kstar);
      fFunc->FixParameter(0, sigma);
      fFunc->FixParameter(1, 1);
      Double_t lowerBoundary = -4.0 * sigma;
      Double_t upperBoundary = 4.0 * sigma;
      Double_t integral      = fFunc->Integral(lowerBoundary, upperBoundary);
      if (integral != 0) {
        norms.push_back(1.0 / integral);
      } else {
        norms.push_back(1.0);
      }
    }
    return norms;
  }

  TMatrixD CorrFitSmear1DCF::ReverseDenominator(TF1* smearfunc, TH2D* smearMatrix) const {
    Int_t nbins = fCF->GetNum()->GetNbinsX();
    TMatrixD reversed(nbins + 2, 1);
    for (int i = 1; i <= nbins; i++) {
      reversed[i][0] = fCF->GetDen()->GetBinContent(i);
    }
    if (fCF->GetNum()->GetXaxis()->GetBinCenter(1) < 0) {  // sym
      reversed[0][0] = fCF->GetDen()->GetBinContent(1);
    } else {
      reversed[0][0] = 0;
    }
    reversed[nbins + 1][0] = fCF->GetDen()->GetBinContent(nbins);
    TMatrixD smearMT(nbins + 2, nbins + 2);
    // row col
    smearMT[0][0] = smearMT[nbins + 1][nbins + 1] = 1;
    TAxis* x                                      = fCF->GetNum()->GetXaxis();
    if (smearfunc) {
      fFunc->FixParameter(0, 1);  // does'nt matter because matrix will be normalized
      auto norms = GetGausNorms(smearfunc, fCF->GetNum()->GetXaxis());
      for (int iSim = 1; iSim <= nbins; iSim++) {
        Double_t qsim  = x->GetBinCenter(iSim);
        Double_t sigma = smearfunc->Eval(qsim);
        for (int jReco = 1; jReco <= nbins; jReco++) {
          Double_t qreco       = x->GetBinCenter(jReco);
          Double_t density     = fFunc->Eval(sigma);
          smearMT[jReco][iSim] = density;
        }
      }
    } else {
      for (int iSim = 1; iSim <= nbins; iSim++) {
        for (int jReco = 1; jReco <= nbins; jReco++) {
          smearMT[jReco][iSim] = smearMatrix->GetBinContent(iSim, jReco);
        }
      }
    }
    NormalizeMatrix(smearMT);
    auto rev    = smearMT.Invert();
    auto newDen = reversed * rev;
    TMatrixD reversedDen(nbins, 1);
    for (int i = 0; i < nbins; i++) {
      reversedDen[i][0] = newDen[i + 1][0];
    }
    return reversedDen;
  }

}  // namespace Hal
