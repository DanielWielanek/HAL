/*
 * SmearAlgo.cxx
 *
 *  Created on: 3 cze 2025
 *      Author: daniel
 */

#include "SmearAlgo.h"

#include <TF1.h>
#include <TF2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TMatrixT.h>
#include <TMatrixTBase.h>

#include "Cout.h"
#include "Std.h"
#include "StdHist.h"

namespace Hal {
  TMatrixD SmearAlgo::GetVec(const TH2& vec) const {
    TMatrixD vect(vec.GetNbinsX() + 2, vec.GetNbinsY() + 2);  // on x - real, on y- smeared
    for (int iSim = 0; iSim <= vec.GetNbinsX() + 1; iSim++) {
      for (int iReco = 0; iReco <= vec.GetNbinsY() + 1; iReco++) {
        if (iSim == iReco)
          vect[iReco][iSim] = 1;
        else
          vect[iReco][iSim] = 0;
      }
    }
    for (int iSim = 1; iSim <= vec.GetNbinsX(); iSim++) {
      for (int iReco = 1; iReco <= vec.GetNbinsY(); iReco++)
        vect[iReco][iSim] = vec.GetBinContent(iSim, iReco);
    }
    return vect;
  }

  TMatrixD SmearAlgo::GetVec(const TH1& vec) const {
    TMatrixD vect(vec.GetNbinsX() + 2, 1);
    vect[0][0]                   = 0;
    vect[vec.GetNbinsX() + 1][0] = 0;
    for (int i = 1; i <= vec.GetNbinsX(); i++) {
      vect[i][0] = vec.GetBinContent(i);
    }
    return vect;
  }


  TH1D* SmearAlgo::GetSmeared(const TH1D& raw) {
    auto newMatrix = GetSmearedVec(raw);
    TH1D* res      = new TH1D("res", "res", fBins, fMin, fMax);
    newMatrix.Print();
    for (int i = 0; i < newMatrix.GetNrows(); i++) {
      res->SetBinContent(i, newMatrix[i][0]);
    }
    return res;
  }

  TH1D* SmearAlgo::GetUnsmeared(const TH1D& raw) {
    auto newMatrix = GetUnsmearedVec(raw);
    TH1D* res      = new TH1D("res", "res", fBins, fMin, fMax);
    for (int i = 0; i < newMatrix.GetNrows(); i++) {
      res->SetBinContent(i, newMatrix[i][0]);
    }
    return res;
  }

  void SmearAlgo::SetSmearMatrix(TH2& vec) {
    if (fSmearMatrixH) delete fSmearMatrixH;
    fSmearMatrixH = (TH2*) vec.Clone();
    fSmearMatrixH->SetDirectory(nullptr);
  }

  void SmearAlgo::SetSmearFunction(TF1* func) {
    if (fSmearFunc1d) delete fSmearFunc1d;
    fSmearFunc1d = (TF1*) func->Clone();
  }

  void SmearAlgo::NormalizeMatrix(TMatrixD& matrix) const {
    for (int iSim = 0; iSim < matrix.GetNcols(); iSim++) {
      Double_t sum = 0;
      for (int iReco = 0; iReco < matrix.GetNrows(); iReco++) {
        sum += matrix[iReco][iSim];
      }
      sum = 1.0 / sum;
      for (int iReco = 0; iReco < matrix.GetNrows(); iReco++) {
        matrix[iReco][iSim] = matrix[iReco][iSim] * sum;
      }
    }
  }

  SmearAlgo::~SmearAlgo() {
    if (fSmearMatrixH) delete fSmearMatrixH;
    if (fRawFunctionH) delete fRawFunctionH;
    if (fSmearFunc1d) delete fSmearFunc1d;
    if (fSmearFunc2d) delete fSmearFunc2d;
  }

  void SmearAlgoMatrix::Compute() {
    if (fComputed) return;
    if (fSmearMatrixH) {  // use smear matrix directly
      auto temp = GetVec(*fSmearMatrixH);
      fSmearMatrix.ResizeTo(temp);
      fSmearMatrix = temp;

    } else if (fSmearFunc1d) {  // use smear function gaussian
      if (fFunction.GetNcols() == 0) {
        Hal::Cout::PrintInfo("SmearAlgo::SetSmearFunction - cf not set, cannot guess ranges", Hal::EInfo::kError);
        return;
      }
      double step = (fMax - fMin) / double(fBins);
      double low  = fMin - step;
      //     double high  = fMax + step;
      int rows     = fFunction.GetNrows();
      fSmearMatrix = TMatrixD(rows, rows);
      for (int i = 0; i < rows; i++) {
        double sim_q = low + double(i) * step + 0.5 * step;
        for (int j = 0; j < rows; j++) {
          double reco_q      = low + double(j) * step + 0.5 * step;
          double rms         = fSmearFunc1d->Eval(sim_q);
          double val         = TMath::Gaus(reco_q, 0, rms, false);
          fSmearMatrix[j][i] = val;
        }
      }
      /* cleaning the edges */
      for (int i = 0; i < rows; i++) {
        fSmearMatrix[i][0]        = 0;
        fSmearMatrix[0][i]        = 0;
        fSmearMatrix[rows - 1][0] = 0;
        fSmearMatrix[0][rows - 1] = 0;
      }
      fSmearMatrix[0][0] = fSmearMatrix[rows - 1][rows - 1] = 1;
    }

    NormalizeMatrix(fSmearMatrix);

    fSmearMatrixRev.ResizeTo(fSmearMatrix);
    fSmearMatrixRev = fSmearMatrix;
    fSmearMatrixRev.Invert();
    fComputed = kTRUE;
  }

  TMatrixD SmearAlgoMatrix::GetSmearedVec(const TH1D& raw) {
    auto temp = GetVec(raw);
    fFunction.ResizeTo(temp);
    fFunction = temp;
    Hal::Std::GetAxisPar(raw, fBins, fMin, fMax, "x");
    if (!fComputed) { Compute(); }
    return fSmearMatrix * fFunction;
  }

  TMatrixD SmearAlgoMatrix::GetUnsmearedVec(const TH1D& raw) {
    auto temp = GetVec(raw);
    fFunction.ResizeTo(temp);
    fFunction = temp;
    Hal::Std::GetAxisPar(raw, fBins, fMin, fMax, "x");
    if (!fComputed) { Compute(); }
    return fSmearMatrixRev * fFunction;
  }

  std::pair<TH2D*, TH1D*> SmearAlgoMatrix::GetFilledUpMatrix(TH2D& smear_matrix, TH1D& raw) {
    // TODO
    std::pair<TH2D*, TH1D*> res;
    res.first  = (TH2D*) smear_matrix.Clone();
    res.second = (TH1D*) raw.Clone();
    auto smear = res.first;
    auto data  = res.second;
    int size   = data->GetNbinsX();

    // normalize colums
    for (int i = 0; i <= size + 1; i++) {
      double sum = 0;
      for (int j = 0; j <= size + 1; j++) {
        sum += smear->GetBinContent(i, j);
      }
      if (sum > 0)
        for (int j = 0; j <= size + 1; j++) {
          smear->SetBinContent(i, j, smear->GetBinContent(i, j) / sum);
        }
      else {
        for (int j = 0; j <= size + 1; j++) {
          smear->SetBinContent(i, j, 0);
        }
        smear->SetBinContent(i, i, 1);
      }
    }
    double esc_data = 0;
    for (int i = 0; i <= size + 1; i++) {
      esc_data += smear->GetBinContent(i, size + 1) * data->GetBinContent(i);
    }
    double rawLast = data->GetBinContent(size, size);
    double last    = esc_data + data->GetBinContent(size, size);
    data->SetBinContent(size + 1, size + 1, last);
    double overFactor = esc_data / rawLast;
    for (int i = 0; i <= size + 1; i++) {
      smear->SetBinContent(size + 1, i, smear->GetBinContent(i, size + 1) * overFactor);
    }
    data->SetBinContent(size + 1, size + 1, 1 - overFactor);
    return res;
  }


} /* namespace Hal */
