/*
 * FemtoSHCF.cxx
 *
 *  Created on: 28 kwi 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoSHCF.h"

#include <TAttLine.h>
#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TCollection.h>
#include <TH1.h>
#include <TH3.h>
#include <TList.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TSystem.h>
#ifndef DISABLE_GSL
#include <gsl/gsl_blas.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_vector_double.h>
#endif
#include <iostream>
#ifdef __CIA__
#include "CorrFitSHCF.h"
#endif
#include "Cout.h"
#include "FemtoPair.h"
#include "FemtoSHCF.h"
#include "FemtoSHSlice.h"
#include "FemtoSerializationInterfaceSH.h"
#include "FemtoYlmSolver.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlObject.h"
#include "HtmlTable.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"
#include "Style.h"

//#define FULL_CALC
//#define _FINISH_DEBUG_
namespace Hal {
  FemtoSHCF::FemtoSHCF() :
    DividedHisto1D(),
    fMaxJM(0),
    fFrame(Femto::EKinematics::kLCMS),
    fNormPurity(0),
    fNormRadius(0),
    fNormBohr(0),
    fLmVals(FemtoYlmIndexes(1)) {
    gSystem->Load("libgsl.so");
    gSystem->Load("libgslcblas.so");
  }

  FemtoSHCF::FemtoSHCF(TString name, Int_t maxL, Int_t bins, Double_t min, Double_t max, Femto::EKinematics kinematics) :
    DividedHisto1D(name, bins, min, max),
    fMaxJM((TMath::Min(maxL, 5) + 1) * (TMath::Min(maxL, 5) + 1)),
    fFrame(kinematics),
    fNormPurity(0),
    fNormRadius(0),
    fNormBohr(0),
    fLmVals(FemtoYlmIndexes(TMath::Min(maxL, 5))),
    fLmMath() {
    SetNorm(0, 0.5, 0);
    if (maxL > 5) { Hal::Cout::PrintInfo("MaxL > 5 not supported", EInfo::kError); }
    fNumReal = new TH1D*[fMaxJM];
    fNumImag = new TH1D*[fMaxJM];
    fDenReal = new TH1D*[fMaxJM];
    fDenImag = new TH1D*[fMaxJM];

    TString x_name = Hal::Femto::KinematicsToAxisLabel(fFrame, 0, 1);
    TString y_name = Hal::Femto::KinematicsToAxisLabel(fFrame, 1, 1);

    for (int ihist = 0; ihist < fMaxJM; ihist++) {
      TString hname   = Form("NumReYlm%i%i", fLmVals.GetElsi(ihist), fLmVals.GetEmsi(ihist));
      fNumReal[ihist] = new TH1D(hname, hname, bins, min, max);
      hname           = Form("NumImYlm%i%i", fLmVals.GetElsi(ihist), fLmVals.GetEmsi(ihist));
      fNumImag[ihist] = new TH1D(hname, hname, bins, min, max);
      hname           = Form("DenReYlm%i%i", fLmVals.GetElsi(ihist), fLmVals.GetEmsi(ihist));
      fDenReal[ihist] = new TH1D(hname, hname, bins, min, max);
      hname           = Form("DenImYlm%i%i", (int) fLmVals.GetElsi(ihist), fLmVals.GetEmsi(ihist));
      fDenImag[ihist] = new TH1D(hname, hname, bins, min, max);
      fNumReal[ihist]->GetXaxis()->SetName(x_name);
      fNumReal[ihist]->GetYaxis()->SetName(y_name);
      fNumImag[ihist]->GetXaxis()->SetName(x_name);
      fNumImag[ihist]->GetYaxis()->SetName(y_name);
      fDenReal[ihist]->GetXaxis()->SetName(x_name);
      fDenReal[ihist]->GetYaxis()->SetName(y_name);
      fDenImag[ihist]->GetXaxis()->SetName(x_name);
      fDenImag[ihist]->GetYaxis()->SetName(y_name);
      fNumReal[ihist]->Sumw2();
      fNumImag[ihist]->Sumw2();
      fDenReal[ihist]->Sumw2();
      fDenImag[ihist]->Sumw2();
    }

    fCovNum.MakeBigger(bins, fMaxJM * 2, fMaxJM * 2);
    fCovDen.MakeBigger(bins, fMaxJM * 2, fMaxJM * 2);
    fCovCf.MakeBigger(bins, fMaxJM * 2, fMaxJM * 2);


    // gSystem->Load("libgsl.so");
    // gSystem->Load("libgslcblas.so");
    AddLabel(Hal::Femto::KinematicsToLabel(fFrame));
    RecalculateCF();
  }

  FemtoSHCF::FemtoSHCF(const FemtoSHCF& other) :
    DividedHisto1D(other),
    fMaxJM(other.fMaxJM),
    fFrame(other.fFrame),
    fNormPurity(other.fNormPurity),
    fNormRadius(other.fNormRadius),
    fNormBohr(other.fNormBohr),
    fLmVals(other.fLmVals) {
    if (other.fNumReal) {
      fNumReal = new TH1D*[fMaxJM];
      fNumImag = new TH1D*[fMaxJM];
      fDenReal = new TH1D*[fMaxJM];
      fDenImag = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fNumReal[i] = (TH1D*) other.fNumReal[i]->Clone();
        fNumImag[i] = (TH1D*) other.fNumImag[i]->Clone();
        fDenReal[i] = (TH1D*) other.fDenReal[i]->Clone();
        fDenImag[i] = (TH1D*) other.fDenImag[i]->Clone();
      }
    }

    if (other.fCFReal) {
      fCFReal = new TH1D*[fMaxJM];
      fCFImag = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fCFReal[i] = (TH1D*) other.fCFReal[i]->Clone();
        fCFImag[i] = (TH1D*) other.fCFImag[i]->Clone();
      }
    }
    fCovNum = other.fCovNum;
    fCovDen = other.fCovDen;
    fCovCf  = other.fCovCf;
    if (other.fCfcov) { fCfcov = (TH3D*) other.fCfcov->Clone(); }
    UnpackCovariances();
    RecalculateCF();
  }

  TH1D* FemtoSHCF::GetCFRe(int el, int em) const {
    if (fLmVals.GetIndex(el, em) >= 0 && fCFReal != nullptr) {
      return fCFReal[fLmVals.GetIndex(el, em)];
    } else {
      return nullptr;
    }
  }

  TH1D* FemtoSHCF::GetCFIm(int el, int em) const {
    if (fLmVals.GetIndex(el, em) >= 0 && fCFImag != nullptr) {
      return fCFImag[fLmVals.GetIndex(el, em)];
    } else {
      return nullptr;
    }
  }

  TH1D* FemtoSHCF::GetNumRe(int el, int em) const {
    if (fLmVals.GetIndex(el, em) >= 0)
      return fNumReal[fLmVals.GetIndex(el, em)];
    else
      return nullptr;
  }

  TH1D* FemtoSHCF::GetNumIm(int el, int em) const {
    if (fLmVals.GetIndex(el, em) >= 0)
      return fNumImag[fLmVals.GetIndex(el, em)];
    else
      return nullptr;
  }

  TH1D* FemtoSHCF::GetDenRe(int el, int em) const {
    if (fLmVals.GetIndex(el, em) >= 0)
      return fDenReal[fLmVals.GetIndex(el, em)];
    else
      return nullptr;
  }

  TH1D* FemtoSHCF::GetDenIm(int el, int em) const {
    if (fLmVals.GetIndex(el, em) >= 0)
      return fDenImag[fLmVals.GetIndex(el, em)];
    else
      return nullptr;
  }

  void FemtoSHCF::SetNumRe(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fNumReal == nullptr) fNumReal = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fNumReal[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fNumReal = histograms;
    }
  }

  void FemtoSHCF::SetNumIm(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fNumImag == nullptr) fNumImag = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fNumImag[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fNumImag = histograms;
    }
  }

  void FemtoSHCF::SetDenRe(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fDenReal == nullptr) fDenReal = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fDenReal[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fDenReal = histograms;
    }
  }

  void FemtoSHCF::SetDenIm(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fDenImag == nullptr) fDenImag = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fDenImag[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fDenImag = histograms;
    }
  }

  void FemtoSHCF::Draw(Option_t* opt) { DrawRanges(opt, {}, {}); }

  void FemtoSHCF::PackCfcCovariance() {
    TString bufName;
    if (fCfcov) delete fCfcov;
    bufName = Form("CovCfc%s", fNumReal[0]->GetName());
    fCfcov  = new TH3D(bufName,
                      bufName,
                      fCFReal[0]->GetNbinsX(),
                      fCFReal[0]->GetXaxis()->GetXmin(),
                      fCFReal[0]->GetXaxis()->GetXmax(),
                      GetMaxJM() * 2,
                      -0.5,
                      GetMaxJM() * 2 - 0.5,
                      GetMaxJM() * 2,
                      -0.5,
                      GetMaxJM() * 2 - 0.5);

    double tK;
    for (int ibin = 1; ibin <= fCfcov->GetNbinsX(); ibin++) {
      const int binCov = ibin - 1;
      for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++)
        for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
          tK = fCovCf[binCov][ilmz][ilmp];
          fCfcov->SetBinContent(ibin, ilmz + 1, ilmp + 1, tK);
        }
    }
    fCfcov->SetBinContent(0, 0, 0, 1.0);
  }

  void FemtoSHCF::Browse(TBrowser* b) {
    if (fCFReal == nullptr) {
      Cout::PrintInfo("No correlation functions, call RecalculateCF", EInfo::kError);
      return;
    }
    for (int i = 0; i < fMaxJM; i++) {
      b->Add(fNumReal[i]);
      b->Add(fNumImag[i]);
      b->Add(fDenReal[i]);
      b->Add(fDenImag[i]);
    }
    for (int i = 0; i < fMaxJM; i++) {
      b->Add(fCFReal[i]);
      b->Add(fCFImag[i]);
    }
    b->Add(fCfcov);
    b->Add(fNum);
    b->Add(fDen);
    NormalizeBy(0, 0);
    Draw();
  }

  void FemtoSHCF::NormalizeBy(int el, int em, Option_t* opt) {
    TString option = opt;
    TH1D* num      = nullptr;
    TH1D* den      = nullptr;
    if (option.EqualTo("re")) {
      num = GetNumRe(el, em);
      den = GetDenRe(el, em);
    } else {
      num = GetNumIm(el, em);
      den = GetDenIm(el, em);
      if (num) num->SetLineColor(kRed);
    }
    fScale = 1.0;
    return;
    Double_t n_integral, d_integral;
    if (fNormMax == fNormMin) {
      n_integral = num->Integral(1, num->GetNbinsX());
      d_integral = den->Integral(1, den->GetNbinsX());
    } else {
      n_integral = num->Integral(num->GetXaxis()->FindBin(fNormMin[0]), num->GetXaxis()->FindBin(fNormMax[0]));
      d_integral = den->Integral(den->GetXaxis()->FindBin(fNormMin[0]), den->GetXaxis()->FindBin(fNormMax[0]));
    }
    fScale = d_integral / n_integral;
  }

  void FemtoSHCF::AddLabel(TString label) {
    if (fLabels == nullptr) {
      fLabels = new TList();
      fLabels->SetName("Labels");
    }
    fLabels->AddLast(new TObjString(label));
  }

  void FemtoSHCF::Add(const Object* pack) {
    FemtoSHCF* cf = (FemtoSHCF*) pack;
    if (cf->fMaxJM != fMaxJM) {
      Cout::PrintInfo("Not compatible maxL numbers during merging FemtoSHCF", EInfo::kError);
      return;
    }
    if (cf->fFrame != this->fFrame) {
      Cout::PrintInfo("Incompatible frames !", EInfo::kError);
      return;
    }
    for (int i = 0; i < fMaxJM; i++) {
      fNumReal[i]->Add(cf->fNumReal[i]);
      fNumImag[i]->Add(cf->fNumImag[i]);
      fDenReal[i]->Add(cf->fDenReal[i]);
      fDenImag[i]->Add(cf->fDenImag[i]);
    }
    fNum->Add(cf->fNum);
    fDen->Add(cf->fDen);
    for (int i = 0; i < fCovNum.GetSize(); i++) {
      for (int j = 0; j < fCovNum[0].GetSize(); j++) {
        for (int k = 0; k < fCovNum[0][0].GetSize(); k++) {
          fCovNum[i][j][k] = fCovNum[i][j][k] + cf->fCovNum[i][j][k];
          fCovDen[i][j][k] = fCovDen[i][j][k] + cf->fCovDen[i][j][k];
        }
      }
    }
    // RecalculateCF(); // DO NOT MERGE !!!
  }

  TH1D* FemtoSHCF::GetHisto(int el, int em, Bool_t norm, Option_t* opt) const {
    TString option = opt;
    TH1D* cf       = nullptr;
    if (option.EqualTo("re")) {
      cf = GetCFRe(el, em);
    } else {
      cf = GetCFIm(el, em);
    }
    cf = (TH1D*) cf->Clone();
    cf->GetYaxis()->SetTitle(Form("C^{%i}_{%i}", el, em));
    cf->SetTitle(Form("CF^{%i}_{%i}", el, em));
    cf->SetMinimum(0);
    cf->SetMaximum(cf->GetBinContent(cf->GetMaximumBin()) * 1.1);
    if (norm && fScale != 0) cf->Scale(fScale);
    return cf;
  }

  TH1D* FemtoSHCF::GetCubic(Option_t* opt, Bool_t R) const {
    if (fCFReal == nullptr) {
      Cout::PrintInfo("No correlation functions, call RecalculateCF", EInfo::kError);
      return nullptr;
    }
    TString option = opt;
    if (option.Length() > GetLMax()) return nullptr;
    TH1D* cf  = nullptr;
    TH1D* cf2 = nullptr;
    TH1D* cf3 = nullptr;
    TH1D* cf4 = nullptr;
    TH1D* cf5 = nullptr;
    if (option.Length() == 1) {
      Double_t scale1 = 2.0 * Sqr(1, 3);
      Double_t scale2 = Sqr(2, 3);
      //	Double_t scale4 = Sqr(1,4);
      if (option == "z") {
        cf = Histo(1, 0, "re", scale1);
      } else if (option == "y") {
        cf  = Histo(1, 1, "im", scale2);
        cf2 = Histo(1, -1, "im", scale2);
      } else if (option == "x") {
        cf  = Histo(1, 1, "re", -scale2);
        cf2 = Histo(1, -1, "re", scale2);
      } else if (option == "0") {
        cf = Histo(0, 0, "re", 1.0);
      }
    }
    if (option.Length() == 2) {
      Double_t scale = Sqr(2, 15);
      if (option == "zz") {
        cf = Histo(2, 0, "re", 4.0 / 3.0 * Sqr(1, 5));
      } else if (option == "yz") {
        cf  = Histo(2, 1, "im", scale);
        cf2 = Histo(2, -1, "im", scale);
      } else if (option == "yy") {
        cf  = Histo(2, 2, "re", -scale);
        cf2 = Histo(2, 0, "re", -2.0 / 3.0 * Sqr(1, 5));
        cf3 = Histo(2, -2, "re", -scale);
      } else if (option == "xz") {
        cf  = Histo(2, 1, "re", -scale);
        cf2 = Histo(2, -1, "re", scale);
      } else if (option == "xy") {
        cf  = Histo(2, 2, "im", -scale);
        cf2 = Histo(2, -2, "im", scale);
      } else if (option == "xx") {
        cf  = Histo(2, 2, "re", scale);
        cf2 = Histo(2, 0, "re", -2.0 / 3.0 * Sqr(1, 5));
        cf3 = Histo(2, -2, "re", scale);
      }
    }
    if (option.Length() == 3) {
      Double_t scale7   = 1.0 / 5.0 * Sqr(1, 7);
      Double_t scale21  = 1.0 / 5.0 * Sqr(1, 21);
      Double_t scale105 = Sqr(2, 105);
      Double_t scale35  = Sqr(1, 35);
      if (option == "zzz") {
        cf = Histo(3, 0, "re", 4.0 * scale7);
      } else if (option == "yzz") {
        cf  = Histo(3, 1, "im", 4.0 * scale21);
        cf2 = Histo(3, -1, "im", 4.0 * scale21);
      } else if (option == "yyz") {
        cf  = Histo(3, 2, "re", -scale105);
        cf2 = Histo(3, 0, "re", -2.0 * scale7);
        cf3 = Histo(3, -2, "re", -scale105);
      } else if (option == "yyy") {
        cf  = Histo(3, 3, "im", -scale35);
        cf2 = Histo(3, 1, "im", -Sqr(3, 7) / 5.0);
        cf3 = Histo(3, -1, "im", -Sqr(3, 7) / 5.0);
        cf4 = Histo(3, -3, "im", -scale35);
      } else if (option == "xzz") {
        cf  = Histo(3, 1, "re", -4.0 * scale21);
        cf2 = Histo(3, -1, "re", 4.0 * scale21);
      } else if (option == "xyz") {
        cf  = Histo(3, 2, "im", -scale105);
        cf2 = Histo(3, -2, "im", scale105);
      } else if (option == "xyy") {
        cf  = Histo(3, 3, "re", scale35);
        cf2 = Histo(3, 1, "re", scale21);
        cf3 = Histo(3, -1, "re", -scale21);
        cf4 = Histo(3, -3, "re", -scale35);
      } else if (option == "xxz") {
        cf  = Histo(3, 2, "re", scale105);
        cf2 = Histo(3, 0, "re", -2.0 * scale7);
        cf3 = Histo(3, -2, "re", scale105);
      } else if (option == "xxy") {
        cf  = Histo(3, 3, "im", scale35);
        cf2 = Histo(3, 1, "im", -scale21);
        cf3 = Histo(3, -1, "im", -scale21);
        cf4 = Histo(3, -3, "im", scale35);
      } else if (option == "xxx") {
        cf  = Histo(3, 3, "re", -scale35);
        cf2 = Histo(3, 1, "re", scale7);
        cf3 = Histo(3, -1, "re", -scale7);
        cf4 = Histo(3, -3, "re", scale35);
      }
    }
    if (option.Length() == 4) {
      Double_t scale5   = Sqr(1, 5) / 21.0;
      Double_t scale25  = Sqr(2, 5) / 21.0;
      Double_t scale35  = Sqr(1, 35) / 3.0;
      Double_t scale235 = Sqr(2, 35) / 3.0;
      Double_t scale57  = Sqr(1, 5) / 7.0;
      Double_t pis      = TMath::Sqrt(TMath::Pi());
      if (option == "zzzz") {
        cf = Histo(4, 0, "re", TMath::Sqrt(TMath::Pi()) * 16.0 / 105.0);
      } else if (option == "yzzz") {
        cf  = Histo(4, 1, "im", scale5 * 4);
        cf2 = Histo(4, -1, "im", scale5 * 4.0);
      } else if (option == "yyzz") {
        cf  = Histo(4, 2, "re", -2.0 * scale25);
        cf2 = Histo(4, 0, "re", -8.0 / 105.0 * pis);
        cf3 = Histo(4, -2, "re", -2.0 * scale25);
      } else if (option == "yyyz") {
        cf  = Histo(4, 3, "im", -scale35);
        cf2 = Histo(4, 1, "im", -scale57);
        cf3 = Histo(4, -1, "im", -scale57);
        cf4 = Histo(4, -3, "im", -scale35);
      } else if (option == "yyyy") {
        cf  = Histo(4, 4, "re", scale235);
        cf2 = Histo(4, 2, "re", scale25 * 2);
        cf3 = Histo(4, 0, "re", 2.0 / 35.0 * pis);
        cf4 = Histo(4, 2, "re", scale25 * 2);
        cf5 = Histo(4, -4, "re", scale235);
      } else if (option == "xzzz") {
        cf  = Histo(4, 1, "re", -4.0 * scale5);
        cf2 = Histo(4, -1, "re", 4.0 * scale5);
      } else if (option == "xyzz") {
        cf  = Histo(4, 2, "im", -2.0 * scale25);
        cf2 = Histo(4, -2, "im", 2.0 * scale25);
      } else if (option == "xyzz") {
        cf  = Histo(4, 3, "re", scale35);
        cf2 = Histo(4, 1, "re", scale5);
        cf3 = Histo(4, -1, "re", -scale5);
        cf4 = Histo(4, -3, "re", -scale35);
      } else if (option == "xyyz") {
        cf  = Histo(4, 3, "re", scale35);
        cf2 = Histo(4, 1, "re", scale5);
        cf3 = Histo(4, -1, "re", -scale5);
        cf4 = Histo(4, -3, "re", -scale35);
      } else if (option == "xyyy") {
        cf  = Histo(4, 4, "im", scale235);
        cf2 = Histo(4, 2, "im", scale25);
        cf3 = Histo(4, -2, "im", -scale25);
        cf4 = Histo(4, -4, "im", -scale235);
      } else if (option == "xxzz") {
        cf  = Histo(4, 2, "re", 2.0 * scale25);
        cf2 = Histo(4, 0, "re", -8.0 / 105.0 * pis);
        cf3 = Histo(4, -2, "re", 2.0 * scale25);
      } else if (option == "xxyz") {
        cf  = Histo(4, 3, "im", scale35);
        cf2 = Histo(4, 1, "im", -scale5);
        cf3 = Histo(4, -1, "im", -scale5);
        cf4 = Histo(4, -3, "im", scale35);
      } else if (option == "xxyy") {
        cf  = Histo(4, 4, "re", -scale235);
        cf2 = Histo(4, 0, "re", 2.0 / 105. * pis);
        cf3 = Histo(4, 4, "re", -scale235);
      } else if (option == "xxxz") {
        cf  = Histo(4, 3, "re", -scale35);
        cf2 = Histo(4, 1, "re", scale57);
        cf3 = Histo(4, -1, "re", -scale57);
        cf4 = Histo(4, -3, "re", scale35);
      } else if (option == "xxxy") {
        cf  = Histo(4, 4, "im", -scale235);
        cf2 = Histo(4, 2, "im", scale25);
        cf3 = Histo(4, -2, "im", -scale25);
        cf4 = Histo(4, -4, "im", scale235);
      } else if (option == "xxxx") {
        cf  = Histo(4, 4, "re", scale235);
        cf2 = Histo(4, 2, "re", -2.0 * scale25);
        cf3 = Histo(4, 0, "re", 2.0 / 35.0 * pis);
        cf4 = Histo(4, -2, "re", -2.0 * scale25);
        cf5 = Histo(4, -4, "re", scale235);
      }
    }
    if (cf == nullptr) return nullptr;
    if (cf2) {
      cf->Add(cf2);
      delete cf2;
      if (cf3) {
        cf->Add(cf3);
        delete cf3;
        if (cf4) {
          cf->Add(cf4);
          delete cf4;
          if (cf5) {
            cf->Add(cf5);
            delete cf5;
          }
        }
      }
    }
    cf->SetTitle(option);
    cf->SetName(option);
    if (R) {
      cf->SetTitle(option + " R(q)");
      for (int i = 1; i <= cf->GetNbinsX(); i++) {
        cf->SetBinContent(i, cf->GetBinContent(i) - fScale);
      }
    }
    return cf;
  }

  Double_t FemtoSHCF::Sqr(Double_t val1, Double_t val2) const { return TMath::Sqrt(TMath::Pi() * val1 / val2); }

  TH1D* FemtoSHCF::Histo(int lm, int em, Option_t* opt, Double_t scale) const {
    TH1D* cf;
    TString option = opt;
    if (option.EqualTo("im")) {
      cf = (TH1D*) GetCFIm(lm, TMath::Abs(em))->Clone();
    } else {
      cf = (TH1D*) GetCFRe(lm, TMath::Abs(em))->Clone();
    }
    if (option == "im") {
      cf->Scale(-scale);  // multiply by i
    } else {
      cf->Scale(scale);
    }
    return cf;
  }

  TH3D* FemtoSHCF::GetCovCF() const { return fCfcov; }

  void FemtoSHCF::RecalculateCF(Int_t debug) {
#ifndef DISABLE_GSL
    if (fDenImag == nullptr) {
      Cout::PrintInfo("No imaginary denominators!", EInfo::kError);
      return;
    }
    if (fDenReal == nullptr) {
      Cout::PrintInfo("No real denominators", EInfo::kError);
      return;
    }
    if (fNumReal == nullptr) {
      Cout::PrintInfo("No real numeraotrs", EInfo::kError);
      return;
    }
    if (fNumImag == nullptr) {
      Cout::PrintInfo("No imaginary numerators", EInfo::kError);
      return;
    }

    Cout::PrintInfo("Computing SF correlation functions", EInfo::kDebugInfo);
    if (fCFReal) {
      for (int i = 0; i < fMaxJM; i++)
        delete fCFReal[i];
      delete[] fCFReal;
    }
    if (fCFImag) {
      for (int i = 0; i < fMaxJM; i++)
        delete fCFImag[i];
      delete[] fCFImag;
    }
    fCFReal = new TH1D*[fMaxJM];
    fCFImag = new TH1D*[fMaxJM];
    // UnpackCovariances(); do nothing
    for (int i = 0; i < fMaxJM; i++) {
      TString name = fNumReal[i]->GetName();
      name.ReplaceAll("Num", "CF");
      Int_t nbins;
      Double_t min, max;
      Std::GetAxisPar(*fNumReal[i], nbins, min, max, "x");
      fCFReal[i] = new TH1D(name, name, nbins, min, max);
      name       = fNumImag[i]->GetName();
      name.ReplaceAll("Num", "CF");
      fCFImag[i] = new TH1D(name, name, nbins, min, max);
      fCFImag[i]->SetLineColor(kRed);
      fCFImag[i]->SetMarkerColor(kRed);
      int el, em;
      el = fLmVals.GetElsi(i);
      em = fLmVals.GetEmsi(i);
      fCFReal[i]->GetYaxis()->SetTitle(Form("C^{%i}_{%i}", el, em));
      fCFReal[i]->SetTitle(Form("CF^{%i}_{%i} Re", el, em));
      fCFImag[i]->GetYaxis()->SetTitle(Form("C^{%i}_{%i}", el, em));
      fCFImag[i]->SetTitle(Form("CF^{%i}_{%i} Im", el, em));
    }
    if ((fCovNum[0][0][0] > 0.0)) {
      std::cout << "Detected calculated covariance matrix. Do not recalculate !!!" << std::endl;
      //  recalccov = 0;
    }
    if (fNum->GetEntries() == 0) {
      std::cout << "Emtpy numerator, stop calculating cF" << std::endl;
      return;  // no data, CF is empdy
    }

    //=====================================================
    FemtoYlmSolver solver(GetLMax(), this);
    solver.SetDebugBin(debug);
    solver.SetNormalizationArea(GetNormMin(0), GetNormMax(0));
    solver.Solve(kTRUE);
    PackCfcCovariance();
#else
    std::cout << "GLS NOT ENABLED!" << std::endl;
#endif
  }

  Long64_t FemtoSHCF::Merge(TCollection* collection) {
    if (collection) {
      FemtoSHCF* pack = nullptr;
      TIter iterator(collection);
      while ((pack = (FemtoSHCF*) iterator())) {
        FastAdd(pack);
      }
    }
    RecalculateCF();
    return 1;
  }

  void FemtoSHCF::FastAdd(const FemtoSHCF* obj) {
    FemtoSHCF* cf = (FemtoSHCF*) obj;
    if (cf->fMaxJM != fMaxJM) {
      Cout::PrintInfo("Not compatible maxL numbers during merging FemtoSHCF", EInfo::kError);
      return;
    }
    if (cf->fFrame != this->fFrame) {
      Cout::PrintInfo("Incompatible frames !", EInfo::kError);
      return;
    }
    for (int i = 0; i < fMaxJM; i++) {
      fNumReal[i]->Add(cf->fNumReal[i]);
      fNumImag[i]->Add(cf->fNumImag[i]);
      fDenReal[i]->Add(cf->fDenReal[i]);
      fDenImag[i]->Add(cf->fDenImag[i]);
    }
    fCovCf += obj->fCovCf;
    fCovNum += obj->fCovNum;
    fCovDen += obj->fCovDen;
    fNum->Add(cf->fNum);
    fDen->Add(cf->fDen);
  }

  void FemtoSHCF::DrawRanges(TString option, std::initializer_list<double> lowMin, std::initializer_list<double> userRange) {
    auto rangesY = Hal::Std::GetVector(lowMin);
    auto rangesX = Hal::Std::GetVector(userRange);
    if (gPad == nullptr) { new TCanvas(); }
    TVirtualPad* pad = gPad;
    // gPad->Clear();??
    Bool_t drawImg  = kFALSE;
    Bool_t drawReal = kFALSE;
    Bool_t drawNeg  = kTRUE;
    Bool_t drawGrid = kFALSE;
    enum class drawType { kNum, kDen, kCF };

    if (Hal::Std::FindParam(option, "im", kTRUE)) drawReal = kTRUE;
    if (Hal::Std::FindParam(option, "re", kTRUE)) drawImg = kTRUE;
    if (!drawReal && !drawImg) { drawReal = drawImg = kTRUE; }
    if (Hal::Std::FindParam(option, "short", kTRUE)) drawNeg = kFALSE;
    if (Hal::Std::FindParam(option, "grid", kTRUE)) drawGrid = kFALSE;
    auto drawSub = [&](Int_t l, Int_t m, const FemtoSHCF* thiz, Bool_t dImg, Bool_t dReal, TString opT) {
      gPad->SetBottomMargin(0.045);
      gPad->SetTopMargin(0.025);
      gPad->SetLeftMargin(0.045);
      gPad->SetRightMargin(0.025);
      TH1D* cfr = thiz->GetCFRe(l, m);  // GetHisto(I,J,kTRUE,"re");
      TH1D* cfi = thiz->GetCFIm(l, m);  // GetHisto(I,J,kTRUE,"im");
      if (cfr == nullptr) return;
      if (cfi == nullptr) return;
      cfr->SetMinimum(0);
      double max = cfr->GetBinContent(cfr->GetMaximumBin());
      double min = cfr->GetBinContent(cfr->GetMinimumBin());
      if (min < 0) {
        cfr->SetMinimum(-1);
      } else {
        cfr->SetMinimum(0);
      }
      if (max < 1) { cfr->SetMaximum(1); }
      cfr->SetStats(kFALSE);
      cfr->SetName(Form("%4.5f", gRandom->Rndm()));
      if (fColzSet) {
        Hal::Std::SetColor(*cfr, fColRe);
        Hal::Std::SetColor(*cfi, fColIm);
      } else {
        Hal::Std::SetColor(*cfr, kBlue);
        Hal::Std::SetColor(*cfi, kRed);
      }
      cfr->SetMarkerStyle(GetNum()->GetMarkerStyle());
      cfi->SetMarkerStyle(GetNum()->GetMarkerStyle());
      cfr->SetMarkerSize(GetNum()->GetMarkerSize());
      cfi->SetMarkerSize(GetNum()->GetMarkerSize());
      cfr->SetObjectStat(kFALSE);
      cfi->SetObjectStat(kFALSE);
      if (rangesY.size() == 4) {
        if (l == 0) {
          cfr->SetMinimum(rangesY[0]);
          cfr->SetMaximum(rangesY[1]);
        } else {
          cfr->SetMinimum(rangesY[2]);
          cfr->SetMaximum(rangesY[3]);
        }
      }
      if (rangesX.size() == 2) { cfr->GetXaxis()->SetRangeUser(rangesX[0], rangesX[1]); }
      if (dImg && dReal) {
        cfr->Draw(opT);
        cfi->Draw("SAME" + opT);
      } else {
        if (dImg) cfi->Draw(opT);
        if (dReal) cfr->Draw(opT);
      }
      if (drawGrid) {
        gPad->SetGridx();
        gPad->SetGridy();
      }
    };

    TVirtualPad* temp_pad = gPad;
    Int_t padsNo          = Hal::Std::GetListOfSubPads(temp_pad);
    Int_t req             = (GetLMax() + 1) * (GetLMax() + 1);
    if (padsNo == req) {
      // do nothing we have enough pads
    } else {
      gPad->Clear();
      gPad->Divide(GetLMax() + 1, GetLMax() + 1);
    }

    for (int l = 0; l <= GetLMax(); l++) {
      for (int m = -l; m <= l; m++) {
        temp_pad->cd(fLmVals.GetPadId(l, m));
        if ((m < 0 && drawNeg) || m >= 0) drawSub(l, m, this, drawImg, drawReal, option);
      }
    }
    gPad = temp_pad;
  }

  FemtoSHCF::~FemtoSHCF() {
    auto cleanArray = [&](TH1D** x) {
      if (x == nullptr) return;
      for (int i = 0; i < fMaxJM; i++)
        delete x[i];
      delete[] x;
    };
    cleanArray(fNumImag);
    cleanArray(fNumReal);
    cleanArray(fDenReal);
    cleanArray(fDenImag);
    cleanArray(fCFReal);
    cleanArray(fCFImag);
    if (fCfcov) delete fCfcov;
  }

  TString FemtoSHCF::HTMLExtract(Int_t counter, TString dir) const {
    gSystem->MakeDirectory(Form("%s/divided_%i", dir.Data(), counter));
    TString filename = Form("%s/divided_%i/divided.html", dir.Data(), counter);
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "haltable", "");
    HtmlRow row1("", "dark_blue", "");
    row1.AddContent(HtmlCell("label"));
    row1.AddContent(HtmlCellCol("value", 2));
    table.AddContent(row1);
    HtmlRow row2("", "dark_blue", "");
    row2.AddContent(HtmlCell("Name"));
    row2.AddContent(HtmlCellCol(GetName(), 2));
    table.AddContent(row2);
    HtmlRow row3("", "dark_blue", "");
    row3.AddContent(HtmlCell("Comment"));
    row3.AddContent(HtmlCellCol(fComment, 2));
    table.AddContent(row3);
    if (fScale == 0) {
      HtmlRow row4("", "dark_blue", "");
      row4.AddContent(HtmlCell("Scale"));
      row4.AddContent(HtmlCellCol("0(disabled)", 2));
      table.AddContent(row4);
    } else {
      HtmlRow row4("", "dark_blue", "");
      row4.AddContent(HtmlCell("Norm on xaxis"));
      row4.AddContent(HtmlCell(Form("%4.2f", fNormMin[0])));
      row4.AddContent(HtmlCell(Form("%4.2f", fNormMax[0])));
      table.AddContent(row4);
    }
    file.AddContent(table);
    HtmlTable table2("", "haltable", "");
    HtmlRow row5("", "dark_blue", "");
    row5.AddContent(HtmlCell("No."));
    row5.AddContent(HtmlCell("Name/Value"));
    table2.AddContent(row5);

    if (fLabels)  // for backward compatilibiyt
      for (int i = 0; i < fLabels->GetEntries(); i++) {
        TObjString* obj = (TObjString*) fLabels->At(i);
        HtmlRow row6("", "light_blue", "");
        row6.AddContent(HtmlCell(Form("%i", i)));
        row6.AddContent(HtmlCell(obj->GetString()));
        table2.AddContent(row6);
      }
    file.AddContent(table2);
    // file<<"<img border=\"0\" src=\""<<Form("divided.png")<<"\"  width=\"996\"
    // height=\"1472\">"<<std::endl;
    HtmlTable table3;
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    Int_t jmax   = GetLMax() * 2 + 1;
    Int_t width  = 966 / jmax;
    Int_t height = 966 / GetLMax();
    if (height > width) height = width;
    for (int i = 0; i <= GetLMax(); i++) {
      HtmlRow row7("", "dark_blue", "");

      int I = i;
      for (int j = 0; j <= GetLMax(); j++) {
        int J = j - i;
        if (j <= i) I = i;
        TH1D* cfr = GetHisto(I, J, kFALSE, "re");
        TH1D* cfi = GetHisto(I, J, kFALSE, "im");
        if (cfi) cfi->SetLineColor(kRed);
        HtmlCell cell;
        if (cfi && cfr) {
          TCanvas* c1 = new TCanvas();
          cfr->Draw();
          cfi->Draw("SAME");
          c1->SaveAs(Form("%s/divided_%i/C_%i%i.png", dir.Data(), counter, I, J));
          TString cell_content = Form("<a href=\"C_%i%i.png\">", I, J);
          cell_content         = cell_content
                         + Form("<img border=\"0\" src=\"C_%i%i.png\"  "
                                "width=\"%i\" height=\"%i\"></a>",
                                I,
                                J,
                                width,
                                height);
          delete c1;
          cell.SetStringContent(cell_content);
        }
        row7.AddContent(cell);
        I++;
      }
      file.AddContent(row7);
    }

    file.AddContent(table3);
    file.Save();
    gROOT->SetBatch(batch);
    return HtmlCore::GetUrl(Form("divided_%i/divided.html", counter), this->ClassName());
  }

  void FemtoSHCF::FillNumObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    Double_t kv     = TMath::Sqrt(pair->GetX() * pair->GetX() + pair->GetY() * pair->GetY() + pair->GetZ() * pair->GetZ());
    std::complex<double>* YlmBuffer = fLmMath.YlmUpToL(fLmVals.GetMaxL(), pair->GetX(), pair->GetY(), pair->GetZ());
    for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
      fNumReal[ilm]->Fill(kv, real(YlmBuffer[ilm]) * pair->GetWeight());
      fNumImag[ilm]->Fill(kv, -imag(YlmBuffer[ilm]) * pair->GetWeight());
      fNum->Fill(kv, 1.0);  //?
    }

    // Fill in the error matrix
    int nqbin = fNum->GetXaxis()->FindFixBin(kv) - 1;
    if (nqbin < fNum->GetNbinsX()) {
      Double_t weight2 = pair->GetWeight() * pair->GetWeight();
      for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++) {
        const int twoilmzero = ilmzero * 2;
        for (int ilmprim = 0; ilmprim < GetMaxJM(); ilmprim++) {
          const int twoilmprim = ilmprim * 2;
          fCovNum[nqbin][twoilmzero][twoilmprim] += real(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          fCovNum[nqbin][twoilmzero][twoilmprim + 1] += real(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
          fCovNum[nqbin][twoilmzero + 1][twoilmprim] -= imag(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          fCovNum[nqbin][twoilmzero + 1][twoilmprim + 1] -= imag(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
        }
      }
    }
  }

  void FemtoSHCF::FillDenObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    Double_t kv     = TMath::Sqrt(pair->GetX() * pair->GetX() + pair->GetY() * pair->GetY() + pair->GetZ() * pair->GetZ());
    std::complex<double>* YlmBuffer = fLmMath.YlmUpToL(fLmVals.GetMaxL(), pair->GetX(), pair->GetY(), pair->GetZ());
    for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
      fDenReal[ilm]->Fill(kv, real(YlmBuffer[ilm]) * pair->GetWeight());
      fDenImag[ilm]->Fill(kv, -imag(YlmBuffer[ilm]) * pair->GetWeight());
      fDen->Fill(kv, 1.0);
    }

    // Fill in the error matrix
    int nqbin = fDen->GetXaxis()->FindFixBin(kv) - 1;
    if (nqbin < fDen->GetNbinsX()) {
      Double_t weight2 = pair->GetWeight() * pair->GetWeight();
      for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++) {
        const int twoilmzero = ilmzero * 2;
        for (int ilmprim = 0; ilmprim < GetMaxJM(); ilmprim++) {
          const int twoilmprim = ilmprim * 2;
          fCovDen[nqbin][twoilmzero][twoilmprim] += real(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          fCovDen[nqbin][twoilmzero][twoilmprim + 1] += real(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
          fCovDen[nqbin][twoilmzero + 1][twoilmprim] -= imag(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          fCovDen[nqbin][twoilmzero + 1][twoilmprim + 1] -= imag(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
        }
      }
    }
  }

  Array_1<Float_t>* FemtoSHCF::ExportToFlatNum() const {
    Array_1<Float_t>* data = new Array_1<Float_t>(fNum->GetNbinsX() * fMaxJM * 2);
    ExportIntoToFlatNum(data);
    return data;
  }

  void FemtoSHCF::ExportIntoToFlatNum(Array_1<Float_t>* output) const {
    Int_t bin   = 0;
    Int_t nbins = GetNum()->GetNbinsX();
    output->MakeBigger(fNum->GetNbinsX() * fMaxJM * 2);
    for (int ibin = 1; ibin <= nbins; ibin++) {
      for (int l = 0; l <= GetLMax(); l++) {
        for (int m = -l; m <= l; m++) {
          TH1D* h          = GetCFRe(l, m);
          (*output)[bin++] = h->GetBinContent(ibin);
          h                = GetCFIm(l, m);
          (*output)[bin++] = h->GetBinContent(ibin);
        }
      }
    }
  }

  void FemtoSHCF::ExportIntoToFlatNumValkyria(Array_1<Float_t>* output) const {
    Int_t bin   = 0;
    Int_t nbins = GetNum()->GetNbinsX();
    output->MakeBigger(fNum->GetNbinsX() * fMaxJM * 2);
    for (int ibin = 1; ibin <= nbins; ibin++) {
      for (int l = 0; l <= GetLMax(); l++) {
        for (int m = -l; m <= l; m++) {
          TH1D* h = GetNumRe(l, m);
          if (l == 0) { std::cout << "HI " << h->GetBinContent(ibin) << " " << ibin << std::endl; }
          (*output)[bin++] = h->GetBinContent(ibin);
          h                = GetNumIm(l, m);
          (*output)[bin++] = h->GetBinContent(ibin);
        }
      }
    }
  }

  void FemtoSHCF::MakeDummyCov() {
    Cout::Text("FemtoSHCF::MakeDummyCov", "M", kRed);
    for (int i = 0; i < fNum->GetNbinsX(); i++) {
      for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++) {
        for (int ilmzero2 = 0; ilmzero2 < GetMaxJM(); ilmzero2++) {
          //   Int_t gbin       = GetBin(i, ilmzero, 0, ilmzero2, 1);
          //   (*covmnum)[gbin] = 0;
        }
      }
      for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++) {
        //  Int_t gbin       = GetBin(i, ilmzero, 0, ilmzero, 1);
        // (*covmnum)[gbin] = 1;
      }
    }
  }

  void FemtoSHCF::Rebin(Int_t ngroup, Option_t* opt) {

    int oldBins = fNumReal[0]->GetNbinsX();
    for (int i = 0; i < fMaxJM; i++) {
      fNumReal[i]->Rebin(ngroup);
      fNumImag[i]->Rebin(ngroup);
      fDenReal[i]->Rebin(ngroup);
      fDenImag[i]->Rebin(ngroup);
    }
    Int_t newBins = oldBins / ngroup;
    Array_3<Double_t> CovNum, CovDen, CovCf;
    CovNum.MakeBigger(oldBins, fMaxJM * 2, fMaxJM * 2);
    CovDen.MakeBigger(oldBins, fMaxJM * 2, fMaxJM * 2);
    CovCf.MakeBigger(oldBins, fMaxJM * 2, fMaxJM * 2);
    for (int i = 0; i < newBins; i++) {
      for (int a = 0; a < fMaxJM * 2; a++) {
        for (int b = 0; b < fMaxJM * 2; b++) {
          for (int j = 0; j < ngroup; j++) {
            CovNum[i][a][b] = CovNum[i][a][b] + fCovNum[i * ngroup + j][a][b];
            CovDen[i][a][b] = CovDen[i][a][b] + fCovDen[i * ngroup + j][a][b];
            CovCf[i][a][b]  = CovCf[i][a][b] + fCovCf[i * ngroup + j][a][b];
          }
        }
      }
    }
    fCovNum = CovNum;
    fCovDen = CovDen;
    fCovCf  = CovCf;
    if (fCFReal[0]) { RecalculateCF(); }
  }
#ifdef __CIA__
  void FemtoSHCF::Fit(CorrFitSHCF* fit) { fit->Fit(this); }

  void FemtoSHCF::FitDummy(CorrFitSHCF* fit) { fit->FitDummy(this); }

  CorrFitMaskSH FemtoSHCF::MakeEmptyMask() const {
    CorrFitMaskSH mask;
    mask.Build(*this);
    return mask;
  }
#endif

  void FemtoSHCF::ImportSlice(Array_1<Float_t>* array, Int_t bin) {
    if (bin < 0) {
      return;
    } else {
      int count = 0;
      fNum->SetBinContent(bin, array->Get(count++));
      fDen->SetBinContent(bin, array->Get(count++));
      for (int i = 0; i < fMaxJM; i++) {
        fNumReal[i]->SetBinContent(bin, array->Get(count++));
        fDenReal[i]->SetBinContent(bin, array->Get(count++));
        fNumImag[i]->SetBinContent(bin, array->Get(count++));
        fDenImag[i]->SetBinContent(bin, array->Get(count++));
      }
      for (int i = 0; i < fMaxJM * 2; i++) {
        for (int j = 0; j < fMaxJM * 2; j++) {
          fCovNum[bin - 1][i][j] = array->Get(count++);
        }
      }
    }
  }

  void FemtoSHCF::ApplyStyle(const Hal::HistoStyle& h) {
    Hal::HistoStyle anti = h;
    anti.SetAntiColor();
    Hal::DividedHisto1D::ApplyStyle(h);
    for (int i = 0; i < fMaxJM; i++) {
      h.Apply(*fNumReal[i]);
      h.Apply(*fDenReal[i]);
      anti.Apply(*fNumImag[i]);
      anti.Apply(*fDenImag[i]);
      if (fCFReal[i]) h.Apply(*fCFReal[i]);
      if (fCFImag[i]) anti.Apply(*fCFImag[i]);
      if (i == 0 && (h.FindKey("mCol") || h.FindKey("lCol"))) {  // cols set by style, overwrite colz that are use by draw
        fColzSet = kTRUE;
        fColRe   = fCFReal[i]->GetMarkerColor();
        fColIm   = fCFImag[i]->GetMarkerColor();
        HalCoutDebug();
      }
    }
  }

  TObject* FemtoSHCF::GetSpecial(TString opt) const {
    if (opt == "serialization") return new FemtoSerializationInterfaceSH();
    return nullptr;
  }

}  // namespace Hal
