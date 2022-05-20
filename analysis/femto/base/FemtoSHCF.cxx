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

#include "CorrFitSHCF.h"
#include "Cout.h"
#include "FemtoPair.h"
#include "FemtoYlm.h"
#include "Std.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlObject.h"
#include "HtmlTable.h"

//#define FULL_CALC
//#define _FINISH_DEBUG_
namespace Hal {
  FemtoSHCF::FemtoSHCF() :
    DividedHisto1D(),
    fMaxJM(0),
    fNumReal(NULL),
    fNumImag(NULL),
    fDenReal(NULL),
    fDenImag(NULL),
    fFrame(Femto::EKinematics::kLCMS),
    fCFReal(NULL),
    fCFImag(NULL),
    fFactorialsSize(0),
    fMaxL(0),
    covmnum(NULL),
    covmden(NULL),
    covmcfc(NULL),
    fNormPurity(0),
    fNormRadius(0),
    fNormBohr(0),
    fEls(NULL),
    fEms(NULL),
    fElsi(NULL),
    fEmsi(NULL),
    fFactorials(NULL),
    fCfcov(NULL) {
    gSystem->Load("libgsl.so");
    gSystem->Load("libgslcblas.so");
  }

  FemtoSHCF::FemtoSHCF(TString name, Int_t maxL, Int_t bins, Double_t min, Double_t max, Femto::EKinematics kinematics) :
    DividedHisto1D(name, bins, min, max),
    fMaxJM((maxL + 1) * (maxL + 1)),
    fNumReal(NULL),
    fNumImag(NULL),
    fDenReal(NULL),
    fDenImag(NULL),
    fFrame(kinematics),
    fCFReal(NULL),
    fCFImag(NULL),
    fFactorialsSize((maxL + 1) * 4),
    fMaxL(maxL),
    covmnum(NULL),
    covmden(NULL),
    covmcfc(NULL),
    fNormPurity(0),
    fNormRadius(0),
    fNormBohr(0),
    fEls(NULL),
    fEms(NULL),
    fElsi(NULL),
    fEmsi(NULL),
    fFactorials(NULL),
    fCfcov(NULL) {
    SetNorm(0, 0.5, 0);
    FemtoYlm* ylm = FemtoYlm::Instance();
    ylm->InitializeYlms();
    int el = 0;
    int em = 0;
    int il = 0;
    fEls   = new Double_t[fMaxJM];
    fEms   = new Double_t[fMaxJM];
    fElsi  = new Double_t[fMaxJM];
    fEmsi  = new Double_t[fMaxJM];
    do {
      fEls[il]  = el;
      fEms[il]  = em;
      fElsi[il] = (int) el;
      fEmsi[il] = (int) em;

#ifdef _FINISH_DEBUG_
      std::cout << "il el em " << il << " " << fElsi[il] << " " << fEmsi[il] << std::endl;
#endif
      em++;
      il++;
      if (em > el) {
        el++;
        em = -el;
      }
    } while (el <= maxL);
    fNumReal = new TH1D*[fMaxJM];
    fNumImag = new TH1D*[fMaxJM];
    fDenReal = new TH1D*[fMaxJM];
    fDenImag = new TH1D*[fMaxJM];

    TString x_name;
    TString y_name;
    switch (fFrame) {
      case Femto::EKinematics::kPRF: {
        x_name = "k* [GeV/c]";
        y_name = "dN/dK*";
      } break;
      case Femto::EKinematics::kLCMS: {
        x_name = "q [GeV/c]";
        y_name = "dN/dq";
      } break;
      default: break;
    }

    for (int ihist = 0; ihist < fMaxJM; ihist++) {
      TString hname   = Form("NumReYlm%i%i", (int) fElsi[ihist], (int) fEmsi[ihist]);
      fNumReal[ihist] = new TH1D(hname, hname, bins, min, max);
      hname           = Form("NumImYlm%i%i", (int) fElsi[ihist], (int) fEmsi[ihist]);
      fNumImag[ihist] = new TH1D(hname, hname, bins, min, max);
      hname           = Form("DenReYlm%i%i", (int) fElsi[ihist], (int) fEmsi[ihist]);
      fDenReal[ihist] = new TH1D(hname, hname, bins, min, max);
      hname           = Form("DenImYlm%i%i", (int) fElsi[ihist], (int) fEmsi[ihist]);
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

    fFactorials    = new Double_t[fFactorialsSize];
    Double_t fac   = 1;
    fFactorials[0] = 1;
    for (int iter = 1; iter < fFactorialsSize; iter++) {
      fac *= iter;
      fFactorials[iter] = fac;
    }
    covmnum = new Array_1<Double_t>();
    covmden = new Array_1<Double_t>();
    covmcfc = new Array_1<Double_t>();
    covmnum->MakeBigger(fMaxJM * 4 * fMaxJM * bins);
    covmden->MakeBigger(fMaxJM * 4 * fMaxJM * bins);
    covmcfc->MakeBigger(fMaxJM * 4 * fMaxJM * bins);

    gSystem->Load("libgsl.so");
    gSystem->Load("libgslcblas.so");
    switch (fFrame) {
      case Femto::EKinematics::kPRF: AddLabel("prf"); break;
      case Femto::EKinematics::kLCMS: AddLabel("lcms"); break;
      default: break;
    }
    std::cout << "ARRAYS\t" << (*covmnum)[0] << " " << (*covmden)[0] << std::endl;
    RecalculateCF();
    std::cout << "ARRAYS\t" << (*covmnum)[0] << " " << (*covmden)[0] << std::endl;
  }

  FemtoSHCF::FemtoSHCF(const FemtoSHCF& other) :
    DividedHisto1D(other),
    fMaxJM(other.fMaxJM),
    fFrame(other.fFrame),
    fFactorialsSize(other.fFactorialsSize),
    fMaxL(other.fMaxL),
    fNormPurity(other.fNormPurity),
    fNormRadius(other.fNormRadius),
    fNormBohr(other.fNormBohr) {
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
      fEls          = new Double_t[fMaxJM];
      fEms          = new Double_t[fMaxJM];
      fElsi         = new Double_t[fMaxJM];
      fEmsi         = new Double_t[fMaxJM];
      FemtoYlm* ylm = FemtoYlm::Instance();
      ylm->InitializeYlms();
      int el = 0;
      int em = 0;
      int il = 0;
      do {
        fEls[il]  = el;
        fEms[il]  = em;
        fElsi[il] = (int) el;
        fEmsi[il] = (int) em;

#ifdef _FINISH_DEBUG_
        std::cout << "il el em " << il << " " << fElsi[il] << " " << fEmsi[il] << std::endl;
#endif
        em++;
        il++;
        if (em > el) {
          el++;
          em = -el;
        }
      } while (el <= fMaxL);
      Double_t fac   = 1;
      fFactorials[0] = 1;
      for (int iter = 1; iter < fFactorialsSize; iter++) {
        fac *= iter;
        fFactorials[iter] = fac;
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
    if (other.covmden) {
      covmden = new Array_1<Double_t>(*other.covmden);
      covmnum = new Array_1<Double_t>(*other.covmnum);
      covmcfc = new Array_1<Double_t>(*other.covmcfc);
    }
    if (other.fCfcov) { fCfcov = (TH3D*) other.fCfcov->Clone(); }
    UnpackCovariances();
    RecalculateCF();
  }

  TH1D* FemtoSHCF::GetCFRe(int el, int em) const {
    if (GetIndexForLM(el, em) >= 0 && fCFReal != NULL) {
      return fCFReal[GetIndexForLM(el, em)];
    } else {
      return NULL;
    }
  }

  TH1D* FemtoSHCF::GetCFIm(int el, int em) const {
    if (GetIndexForLM(el, em) >= 0 && fCFImag != NULL) {
      return fCFImag[GetIndexForLM(el, em)];
    } else {
      return NULL;
    }
  }

  TH1D* FemtoSHCF::GetNumRe(int el, int em) const {
    if (GetIndexForLM(el, em) >= 0)
      return fNumReal[GetIndexForLM(el, em)];
    else
      return NULL;
  }

  TH1D* FemtoSHCF::GetNumIm(int el, int em) const {
    if (GetIndexForLM(el, em) >= 0)
      return fNumImag[GetIndexForLM(el, em)];
    else
      return NULL;
  }

  TH1D* FemtoSHCF::GetDenRe(int el, int em) const {
    if (GetIndexForLM(el, em) >= 0)
      return fDenReal[GetIndexForLM(el, em)];
    else
      return NULL;
  }

  TH1D* FemtoSHCF::GetDenIm(int el, int em) const {
    if (GetIndexForLM(el, em) >= 0)
      return fDenImag[GetIndexForLM(el, em)];
    else
      return NULL;
  }

  Int_t FemtoSHCF::GetIndexForLM(int el, int em) const {
    for (int iter = 0; iter < fMaxJM; iter++)
      if ((el == fElsi[iter]) && (em == fEmsi[iter])) return iter;
    return -1;
  }

  Int_t FemtoSHCF::GetBin(int qbin, int ilmzero, int zeroimag, int ilmprim, int primimag) const {
    return (qbin * fMaxJM * fMaxJM * 4 + (ilmprim * 2 + primimag) * fMaxJM * 2 + ilmzero * 2 + zeroimag);
  }

  void FemtoSHCF::SetNumRe(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fNumReal == NULL) fNumReal = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fNumReal[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fNumReal = histograms;
    }
  }

  void FemtoSHCF::SetNumIm(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fNumImag == NULL) fNumImag = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fNumImag[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fNumImag = histograms;
    }
  }

  void FemtoSHCF::SetDenRe(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fDenReal == NULL) fDenReal = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fDenReal[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fDenReal = histograms;
    }
  }

  void FemtoSHCF::SetDenIm(TH1D** histograms, Bool_t clone) {
    if (clone) {
      if (fDenImag == NULL) fDenImag = new TH1D*[fMaxJM];
      for (int i = 0; i < fMaxJM; i++) {
        fDenImag[i] = (TH1D*) histograms[i]->Clone();
      }
    } else {
      fDenImag = histograms;
    }
  }

  void FemtoSHCF::Draw(Option_t* /*opt*/) {
    if (gPad == NULL) { new TCanvas(); }
    TVirtualPad* pad = gPad;
    gPad->Clear();
    TVirtualPad* temp_pad = gPad;
    pad->Divide(GetL() + 1, GetL() + 1);
    int pad_id = 1;
    for (int i = 0; i <= GetL(); i++) {
      int I = i;
      for (int j = 0; j <= GetL(); j++) {
        int J = j - i;
        if (j <= i) I = i;
        pad->cd(pad_id++);
        gPad->SetBottomMargin(0.025);
        gPad->SetTopMargin(0.025);
        gPad->SetLeftMargin(0.025);
        gPad->SetRightMargin(0.025);
        TH1D* cfr = GetCFRe(I, J);  // GetHisto(I,J,kTRUE,"re");
        TH1D* cfi = GetCFIm(I, J);  // GetHisto(I,J,kTRUE,"im");
        if (cfi) cfi->SetLineColor(kRed);
        if (cfi && cfr) {
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
          cfr->Draw();
          cfi->Draw("SAME");
        }
        I++;
      }
    }
    gPad = temp_pad;
  }

  void FemtoSHCF::PackCfcCovariance() {
    char bufname[200];

    if (fCfcov) delete fCfcov;
    sprintf(bufname, "CovCfc%s", fNumReal[0]->GetName() + 10);
    fCfcov = new TH3D(bufname,
                      bufname,
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
    for (int ibin = 1; ibin <= fCfcov->GetNbinsX(); ibin++)
      for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++)
        for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
          tK = (*covmcfc)[GetBin(ibin - 1, ilmz / 2, ilmz % 2, ilmp / 2, ilmp % 2)];
          // 	tE = fCFReal[0]->GetEntries();
          // 	if (ilmz%2) {
          // 	  if (ilmp%2) {
          // 	    tB =
          // fCFImag[ilmz/2]->GetBinContent(ibin)*fCFImag[ilmp/2]->GetBinContent(ibin);
          // 	  }
          // 	  else {
          // 	    tB =
          // fCFImag[ilmz/2]->GetBinContent(ibin)*fCFReal[ilmp/2]->GetBinContent(ibin);
          // 	  }
          // 	}
          // 	else {
          // 	  if (ilmp%2) {
          // 	    tB =
          // fCFReal[ilmz/2]->GetBinContent(ibin)*fCFImag[ilmp/2]->GetBinContent(ibin);
          // 	  }
          // 	  else {
          // 	    tB =
          // fCFReal[ilmz/2]->GetBinContent(ibin)*fCFReal[ilmp/2]->GetBinContent(ibin);
          // 	  }
          // 	}

          fCfcov->SetBinContent(ibin, ilmz + 1, ilmp + 1, tK);
        }
    fCfcov->SetBinContent(0, 0, 0, 1.0);
  }

  void FemtoSHCF::GetElEmForIndex(int aIndex, double& aEl, double& aEm) const {
    aEl = fEls[aIndex];
    aEm = fEms[aIndex];
  }

  void FemtoSHCF::GetElEmForIndex(int aIndex, int& aEl, int& aEm) const {
    aEl = fEls[aIndex];
    aEm = fEms[aIndex];
  }

  void FemtoSHCF::Browse(TBrowser* b) {
    if (fCFReal == NULL) {
      Cout::PrintInfo("No correlation functions, call RecalculateCF", EInfo::kLessError);
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
    TH1D* num      = NULL;
    TH1D* den      = NULL;
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
    if (fLabels == NULL) {
      fLabels = new TList();
      fLabels->SetName("Labels");
    }
    fLabels->AddLast(new TObjString(label));
  }

  void FemtoSHCF::GetIndependentLM(int ibin, int& el, int& em, int& im) const {
    int cbin = ibin;
    if (cbin == 0) {
      el = 0;
      em = 0;
      im = 0;
      return;
    } else
      cbin--;
    if (cbin == 0) {
      el = 2;
      em = 0;
      im = 0;
      return;
    } else
      cbin--;
    im = cbin % 2;
    el = 2;
    em = cbin / 2 + 1;
    return;
  }

  void FemtoSHCF::InvertYlmIndependentMatrix(double* inmat, double* outmat) {
    // Invert the Ylm matrix by inverting only the matrix
    // with independent elements and filling in the rest
    // according to sign rules
    double mU[GetMaxJM() * GetMaxJM() * 4];
    int isize = PackYlmMatrixIndependentOnly(inmat, mU);
    //  cout << "Independent count " << isize << std::endl;
#ifndef DISABLE_GSL
    gsl_matrix_view matU = gsl_matrix_view_array(mU, isize, isize);
#endif
    // Identity matrix helper for inversion
    double mI[GetMaxJM() * GetMaxJM() * 4];
    for (int iterm = 0; iterm < isize; iterm++)
      for (int iterp = 0; iterp < isize; iterp++)
        if (iterm == iterp)
          mI[iterm * isize + iterp] = 1.0;
        else
          mI[iterm * isize + iterp] = 0.0;
#ifndef DISABLE_GSL
    gsl_matrix_view matI = gsl_matrix_view_array(mI, isize, isize);
    // Invert the matrix
    gsl_blas_dtrsm(CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, 1.0, &matU.matrix, &matI.matrix);
#else
    std::cout << "GLS NOT ENABLED!" << std::endl;
#endif
    UnPackYlmMatrixIndependentOnly(mI, outmat, isize);
  }

  void FemtoSHCF::UnpackCovariances() {
    Bool_t nanfound = kFALSE;
    for (int i = 0; i < covmnum->GetSize(); i++) {
      Double_t val = (*covmnum)[i];
      if (TMath::IsNaN(val)) {
        std::cout << "Nan at " << i << std::endl;
        nanfound = kTRUE;
      }
      val = (*covmden)[i];
      if (TMath::IsNaN(val)) {
        std::cout << "Dan at " << i << std::endl;
        nanfound = kTRUE;
      }
      // val= (*covmcfc)[i];
      // if(TMath::IsNaN(val))nanfound =kTRUE;
    }
    if (nanfound) { Cout::PrintInfo("NaN found in  numerator or denominator covariance matrix !", EInfo::kLessError); }
    return;
    Cout::PrintInfo("Unpacking covariances", EInfo::kLessInfo);
    if (covmnum) { delete covmnum; }
    if (covmden) { delete covmden; }
    if (covmcfc) { delete covmcfc; }
    covmnum = new Array_1<Double_t>();
    covmden = new Array_1<Double_t>();
    covmcfc = new Array_1<Double_t>();
    covmnum->MakeBigger(fMaxJM * 4 * fMaxJM * GetNum()->GetNbinsX());
    covmden->MakeBigger(fMaxJM * 4 * fMaxJM * GetNum()->GetNbinsX());
    covmcfc->MakeBigger(fMaxJM * 4 * fMaxJM * GetNum()->GetNbinsX());
    //	Bool_t nanfound = kFALSE;
    /*
     if (fCovNum) {
     for (int ibin = 1; ibin <= fCovNum->GetNbinsX(); ibin++){
     for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++){
     for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++){
     (*covmnum)[GetBin(ibin - 1, ilmz / 2, ilmz % 2, ilmp / 2,
     ilmp % 2)] = fCovNum->GetBinContent(ibin, ilmz + 1,
     ilmp + 1);
     if(TMath::IsNaN(fCovNum->GetBinContent(ibin, ilmz + 1,
     ilmp + 1))){
     nanfound = kTRUE;
     }
     }
     }
     }
     }

     if(nanfound){
     Cout::InStars("NaN found in  numerator covariance matrix
     !",EInfo::kLessError);
     }
     nanfound = kFALSE;
     if (fCovDen) {
     for (int ibin = 1; ibin <= fCovDen->GetNbinsX(); ibin++){
     for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++){
     for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++){
     (*covmden)[GetBin(ibin - 1, ilmz / 2, ilmz % 2, ilmp / 2,
     ilmp % 2)] = fCovDen->GetBinContent(ibin, ilmz + 1,
     ilmp + 1);
     if(TMath::IsNaN( fCovDen->GetBinContent(ibin, ilmz + 1,ilmp + 1))){
     nanfound = kTRUE;
     }
     }
     }
     }
     }
     if(nanfound){
     Cout::InStars("NaN found in  denominator covariance matrix
     !",EInfo::kLessError);
     }
     */
    /*  if (covcfc) {
     for (int ibin=1; ibin<=covcfc->GetNbinsX(); ibin++)
     for (int ilmz=0; ilmz<GetMaxJM()*2; ilmz++)
     for (int ilmp=0; ilmp<GetMaxJM()*2; ilmp++)
     covmcfc[GetBin(ibin-1, ilmz/2, ilmz%2, ilmp/2, ilmp%2)] =
     covcfc->GetBinContent(ibin, ilmz+1, ilmp+1);
     }*/
  }

  void FemtoSHCF::GetMtilde(std::complex<double>* aMat, double* aMTilde) {
    // Create the Mtilde for a given q bin
    double lzero, mzero;
    double lprim, mprim;
    double lbis, mbis;

    int lzeroi, mzeroi;
    int lprimi, mprimi;
    int lbisi, mbisi;

    for (int iz = 0; iz < GetMaxJM() * 2; iz++)
      for (int ip = 0; ip < GetMaxJM() * 2; ip++)
        aMTilde[iz * GetMaxJM() * 2 + ip] = 0.0;

    for (int izero = 0; izero < GetMaxJM(); izero++) {
      GetElEmForIndex(izero, lzero, mzero);
      GetElEmForIndex(izero, lzeroi, mzeroi);
      //     if (mzero < 0)
      //       continue;
      for (int ibis = 0; ibis < GetMaxJM(); ibis++) {
        GetElEmForIndex(ibis, lbis, mbis);
        GetElEmForIndex(ibis, lbisi, mbisi);

        //       if (mbis<0) continue;

        std::complex<double> val = std::complex<double>(0.0, 0.0);
        std::complex<double> mcomp[fMaxJM];
        for (int iprim = 0; iprim < GetMaxJM(); iprim++) {
          GetElEmForIndex(iprim, lprim, mprim);
          GetElEmForIndex(iprim, lprimi, mprimi);

          // 	if (mprim < 0 ) continue;

          if (abs(mzeroi) % 2)
            mcomp[iprim] = std::complex<double>(-1.0, 0.0);  // (-1)^m
          else
            mcomp[iprim] = std::complex<double>(1.0, 0.0);

          mcomp[iprim] *= sqrt((2 * lzero + 1) * (2 * lprim + 1) * (2 * lbis + 1));  // P1
          mcomp[iprim] *= WignerSymbol(lzero, 0, lprim, 0, lbis, 0);                 // W1
          mcomp[iprim] *= WignerSymbol(lzero, -mzero, lprim, mprim, lbis,
                                       mbis);  // W2
          mcomp[iprim] *= aMat[iprim];
          //	if (
          val += mcomp[iprim];
        }

        aMTilde[(izero * 2) * (2 * GetMaxJM()) + (ibis * 2)]     = real(val);
        aMTilde[(izero * 2 + 1) * (2 * GetMaxJM()) + (ibis * 2)] = imag(val);
        if (imag(val) != 0.0)
          aMTilde[(izero * 2) * (2 * GetMaxJM()) + (ibis * 2 + 1)] = -imag(val);
        else
          aMTilde[(izero * 2) * (2 * GetMaxJM()) + (ibis * 2 + 1)] = 0.0;
        aMTilde[(izero * 2 + 1) * (2 * GetMaxJM()) + (ibis * 2 + 1)] = real(val);
      }
    }
  }

  void FemtoSHCF::UnPackYlmMatrixIndependentOnly(double* inmat, double* outmat, int insize) {
    int lmax = sqrt(insize) - 1;
    //  cout << "lmax is  " << lmax << std::endl;
    if (0) { lmax *= 2; }
    int tmax = (lmax + 1) * (lmax + 1) * 2;
    int indexfrom[tmax];
    int multfrom[tmax];

    int el, em;
    for (int iter = 0; iter < tmax; iter++) {
      int im = iter % 2;
      GetElEmForIndex(iter / 2, el, em);
      if (em == 0) {
        if (im == 1) {
          indexfrom[iter] = 0;
          multfrom[iter]  = 0;
        } else {
          indexfrom[iter] = el * el;
          multfrom[iter]  = 1;
        }
      } else if (em < 0) {
        indexfrom[iter] = (el * el) + (-em) * 2 - 1;
        if (im) indexfrom[iter]++;
        if ((-em) % 2)
          if (im)
            multfrom[iter] = 1;
          else
            multfrom[iter] = -1;
        else if (im)
          multfrom[iter] = -1;
        else
          multfrom[iter] = 1;
      } else if (em > 0) {
        indexfrom[iter] = (el * el) + (em) *2 - 1;
        if (im) indexfrom[iter]++;
        multfrom[iter] = 1;
      }
    }

    //   cout << "From Mult " << std::endl;
    //   for (int iter=0; iter<tmax; iter++)
    //     cout << indexfrom[iter] << " ";
    //   cout << std::endl;
    //   for (int iter=0; iter<tmax; iter++)
    //     cout << multfrom[iter] << " ";
    //   cout << std::endl;

    for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++)
      for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++)
        outmat[ilmz * GetMaxJM() * 2 + ilmp] =
          inmat[(indexfrom[ilmz] * insize) + indexfrom[ilmp]] * multfrom[ilmz] * multfrom[ilmp];
  }

  void FemtoSHCF::Add(const Object* pack) {
    FemtoSHCF* cf = (FemtoSHCF*) pack;
    if (cf->fMaxJM != fMaxJM) {
      Cout::PrintInfo("Not compatible maxL numbers during merging FemtoSHCF", EInfo::kLessError);
      return;
    }
    if (cf->fFrame != this->fFrame) {
      Cout::PrintInfo("Incompatible frames !", EInfo::kLessError);
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
    RecalculateCF();
  }

  TH1D* FemtoSHCF::GetHisto(int el, int em, Bool_t norm, Option_t* opt) const {
    TString option = opt;
    TH1D* cf       = NULL;
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
    if (fCFReal == NULL) {
      Cout::PrintInfo("No correlation functions, call RecalculateCF", EInfo::kLessError);
      return NULL;
    }
    TString option = opt;
    if (option.Length() > GetL()) return NULL;
    TH1D* cf  = NULL;
    TH1D* cf2 = NULL;
    TH1D* cf3 = NULL;
    TH1D* cf4 = NULL;
    TH1D* cf5 = NULL;
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
    if (cf == NULL) return NULL;
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

  int FemtoSHCF::PackYlmVectorIndependentOnly(double* invec, double* outvec) const {
    int ioutcount = 0;
    int em, el;
    for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
      GetElEmForIndex(ilm, el, em);
      if (em < 0) continue;
      outvec[ioutcount++] = invec[ilm * 2];
      if (em == 0) continue;
      outvec[ioutcount++] = invec[ilm * 2 + 1];
    }
    return ioutcount;
  }

  double FemtoSHCF::ClebschGordan(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const {
    int mint, maxt;
    double cgc = 0.0;
    int titer;
    double coef;

    maxt = lrint(aJot1 + aJot2 - aJot);
    mint = 0;
    if (lrint(aJot1 - aEm1) < maxt) maxt = lrint(aJot1 - aEm1);
    if (lrint(aJot2 + aEm2) < maxt) maxt = lrint(aJot2 + aEm2);
    if (lrint(-(aJot - aJot2 + aEm1)) > mint) mint = lrint(-(aJot - aJot2 + aEm1));
    if (lrint(-(aJot - aJot1 - aEm2)) > mint) mint = lrint(-(aJot - aJot1 - aEm2));

    for (titer = mint; titer <= maxt; titer++) {
      coef = TMath::Power(-1, titer);
      coef *= TMath::Sqrt((2 * aJot + 1) * fFactorials[lrint(aJot1 + aEm1)] * fFactorials[lrint(aJot1 - aEm1)]
                          * fFactorials[lrint(aJot2 + aEm2)] * fFactorials[lrint(aJot2 - aEm2)] * fFactorials[lrint(aJot + aEm)]
                          * fFactorials[lrint(aJot - aEm)]);
      coef /= (fFactorials[titer] * fFactorials[lrint(aJot1 + aJot2 - aJot - titer)] * fFactorials[lrint(aJot1 - aEm1 - titer)]
               * fFactorials[lrint(aJot2 + aEm2 - titer)] * fFactorials[lrint(aJot - aJot2 + aEm1 + titer)]
               * fFactorials[lrint(aJot - aJot1 - aEm2 + titer)]);

      cgc += coef;
    }

    cgc *= DeltaJ(aJot1, aJot2, aJot);

    return cgc;
  }

  double FemtoSHCF::WignerSymbol(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const {
    if (lrint(aEm1 + aEm2 + aEm) != 0.0) return 0.0;
    double cge = ClebschGordan(aJot1, aEm1, aJot2, aEm2, aJot, -aEm);
    if (lrint(abs(aJot1 - aJot2 - aEm)) % 2) cge *= -1.0;
    cge /= sqrt(2 * aJot + 1);

    if (cge == -0.0) cge = 0.0;

    return cge;
  }

  double FemtoSHCF::DeltaJ(double aJot1, double aJot2, double aJot) const {
    if ((aJot1 + aJot2 - aJot) < 0) {
      //    cout << "J1+J2-J3 < 0 !!!" << " " << aJot1 << " " << aJot2 << " " <<
      //    aJot << std::endl;
      return 0;
    }
    if ((aJot1 - aJot2 + aJot) < 0) {
      //    cout << "J1-J2+J3 < 0 !!!" << " " << aJot1 << " " << aJot2 << " " <<
      //    aJot << std::endl;
      return 0;
    }
    if ((-aJot1 + aJot2 + aJot) < 0) {
      //    cout << "-J1+J2+J3 < 0 !!!" << " " << aJot1 << " " << aJot2 << " " <<
      //    aJot << std::endl;
      return 0;
    }
    if ((aJot1 + aJot2 + aJot + 1) < 0) {
      //    cout << "J1+J2+J3+1 < 0 !!!" << " " << aJot1 << " " << aJot2 << " " <<
      //    aJot << std::endl;
      return 0;
    }
    double res = TMath::Sqrt(1.0 * fFactorials[lrint(aJot1 + aJot2 - aJot)] * fFactorials[lrint(aJot1 - aJot2 + aJot)]
                             * fFactorials[lrint(-aJot1 + aJot2 + aJot)] / fFactorials[lrint(aJot1 + aJot2 + aJot + 1)]);

    return res;
  }

  int FemtoSHCF::PackYlmMatrixIndependentOnly(double* inmat, double* outmat) const {
    int ioutcountz = 0;
    int ioutcountp = 0;
    int emz, elz;
    int emp, elp;
    int finalsize = 0;

    for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
      GetElEmForIndex(ilm, elz, emz);
      if (emz < 0) continue;
      finalsize++;
      if (emz == 0) continue;
      finalsize++;
    }

    //  cout << "Final size " << finalsize << std::endl;

    for (int ilmz = 0; ilmz < GetMaxJM(); ilmz++) {
      GetElEmForIndex(ilmz, elz, emz);
      ioutcountp = 0;

      if (emz < 0) continue;
      for (int ilmp = 0; ilmp < GetMaxJM(); ilmp++) {
        GetElEmForIndex(ilmp, elp, emp);
        if (emp < 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(0, ilmz, 0, ilmp, 0)];
        ioutcountp++;
        if (emp == 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(0, ilmz, 0, ilmp, 1)];
        ioutcountp++;
      }
      ioutcountz++;

      if (emz == 0) continue;
      ioutcountp = 0;
      for (int ilmp = 0; ilmp < GetMaxJM(); ilmp++) {
        GetElEmForIndex(ilmp, elp, emp);
        if (emp < 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(0, ilmz, 1, ilmp, 0)];
        ioutcountp++;
        if (emp == 0) continue;
        outmat[ioutcountz * finalsize + ioutcountp] = inmat[GetBin(0, ilmz, 1, ilmp, 1)];
        ioutcountp++;
      }
      ioutcountz++;
    }

    return ioutcountz;
  }

  TH3D* FemtoSHCF::GetCovCF() const { return fCfcov; }

  void FemtoSHCF::RecalculateCF() {
#ifndef DISABLE_GSL
    if (fDenImag == NULL) {
      Cout::PrintInfo("No imaginary denominators!", EInfo::kLessError);
      return;
    }
    if (fDenReal == NULL) {
      Cout::PrintInfo("No real denominators", EInfo::kLessError);
      return;
    }
    if (fNumReal == NULL) {
      Cout::PrintInfo("No real numeraotrs", EInfo::kLessError);
      return;
    }
    if (fNumImag == NULL) {
      Cout::PrintInfo("No imaginary numerators", EInfo::kLessError);
      return;
    }

    Cout::PrintInfo("Computing SF correlation functions", EInfo::kLessInfo);
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
    UnpackCovariances();
    for (int i = 0; i < fMaxJM; i++) {
      TString name = fNumReal[i]->GetName();
      name.ReplaceAll("Num", "CF");
      fCFReal[i] = new TH1D(name,
                            name,
                            fNumReal[i]->GetNbinsX(),
                            fNumReal[i]->GetXaxis()->GetBinLowEdge(1),
                            fNumReal[i]->GetXaxis()->GetBinUpEdge(fNumReal[i]->GetNbinsX()));
      name       = fNumImag[i]->GetName();
      name.ReplaceAll("Num", "CF");
      fCFImag[i] = new TH1D(name,
                            name,
                            fNumImag[i]->GetNbinsX(),
                            fNumImag[i]->GetXaxis()->GetBinLowEdge(1),
                            fNumImag[i]->GetXaxis()->GetBinUpEdge(fNumReal[i]->GetNbinsX()));
      fCFImag[i]->SetLineColor(kRed);
      fCFImag[i]->SetLineColor(kRed);
      int el, em;
      GetElEmForIndex(i, el, em);
      fCFReal[i]->GetYaxis()->SetTitle(Form("C^{%i}_{%i}", el, em));
      fCFReal[i]->SetTitle(Form("CF^{%i}_{%i} Re", el, em));
      fCFImag[i]->GetYaxis()->SetTitle(Form("C^{%i}_{%i}", el, em));
      fCFImag[i]->SetTitle(Form("CF^{%i}_{%i} Im", el, em));
    }

    std::complex<double> tMq0[fMaxJM];
    std::complex<double> tTq0[fMaxJM];
    double tMTilde[fMaxJM * fMaxJM * 4];
    //	std::complex<double> tCq0[fMaxJM];
    int recalccov = 1;
    if ((covmnum) && ((*covmnum)[0] > 0.0)) {
      std::cout << "Detected calculated covariance matrix. Do not recalculate !!!" << std::endl;
      //  recalccov = 0;
    }
    if (fNum->GetEntries() == 0) return;  // no data, CF is empdy
    double normfactor = 1.0;
    // TODO Fix/improvenormalization
    double normbinmax = 0;  // fDenReal[0]->FindBin(fNormMax);
    double normbinmin = 0;  // fDenReal[0]->FindBin(fNormMin);
    if (normbinmax > 0) {
      double sksum = 0.0;
      double wksum = 0.0;

      double sk, wk, ks;
      if (normbinmin < 1) normbinmin = 1;
      if (normbinmax > fDenReal[0]->GetNbinsX()) normbinmax = fDenReal[0]->GetNbinsX();
      for (int ib = normbinmin; ib <= normbinmax; ib++) {
        ks = fDenReal[0]->GetXaxis()->GetBinCenter(ib);
        sk = fNumReal[0]->GetBinContent(ib)
             / (fDenReal[0]->GetBinContent(ib) * (1.0 - fNormPurity / (fNormRadius * fNormBohr * ks * ks)));
        wk = fNumReal[0]->GetBinContent(ib);
        sksum += sk * wk;
        wksum += wk;
      }
      normfactor *= sksum / wksum;
      normfactor /= fNumReal[0]->GetEntries() / fDenReal[0]->GetEntries();
    }

    for (int ibin = 1; ibin <= fNumReal[0]->GetNbinsX(); ibin++) {
      for (int ilm = 0; ilm < fMaxJM; ilm++) {
        //      cout << fNumImag[ilm]->GetBinContent(ibin) << std::endl;
        if (recalccov) {
          tMq0[ilm] = std::complex<double>(fDenReal[ilm]->GetBinContent(ibin) / (fDenReal[0]->GetEntries() / normfactor),
                                           fDenImag[ilm]->GetBinContent(ibin) / (fDenReal[0]->GetEntries() / normfactor));
          tTq0[ilm] = std::complex<double>(fNumReal[ilm]->GetBinContent(ibin) / fNumReal[0]->GetEntries(),
                                           fNumImag[ilm]->GetBinContent(ibin) / fNumReal[0]->GetEntries());
        } else {
          tMq0[ilm] = std::complex<double>(fDenReal[ilm]->GetBinContent(ibin) / normfactor,
                                           fDenImag[ilm]->GetBinContent(ibin) / normfactor);
          tTq0[ilm] = std::complex<double>(fNumReal[ilm]->GetBinContent(ibin), fNumImag[ilm]->GetBinContent(ibin));
        }
        //      cout << imag(tTq0[ilm]) << std::endl;
      }

      // Calculate the proper error matrix for T
      // from the temporary covariance matrices
      //    int tabshift = (ibin-1)*GetMaxJM()*GetMaxJM()*4;
      if (recalccov) {
        for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++)
          for (int ilmprim = 0; ilmprim < GetMaxJM(); ilmprim++) {
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! RR " << ilmzero << " " << ilmprim <<
              // std::endl; 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! RI " << ilmzero << " " << ilmprim <<
              // std::endl; 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! IR " << ilmzero << " " << ilmprim <<
              // std::endl; 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! II " << ilmzero << " " << ilmprim <<
              // std::endl; 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            (*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 0)] /= fNumReal[0]->GetEntries();
            (*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 1)] /= fNumReal[0]->GetEntries();
            (*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 0)] /= fNumReal[0]->GetEntries();
            (*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 1)] /= fNumReal[0]->GetEntries();

            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! RR" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! RI" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! IR" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! II" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            (*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 0)] -= real(tTq0[ilmzero]) * real(tTq0[ilmprim]);
            (*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 1)] -= real(tTq0[ilmzero]) * imag(tTq0[ilmprim]);
            (*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 0)] -= imag(tTq0[ilmzero]) * real(tTq0[ilmprim]);
            (*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 1)] -= imag(tTq0[ilmzero]) * imag(tTq0[ilmprim]);

            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! RR" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! RI" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! IR" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! II" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            (*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 0)] /= (fNumReal[0]->GetEntries() - 1);
            (*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 1)] /= (fNumReal[0]->GetEntries() - 1);
            (*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 0)] /= (fNumReal[0]->GetEntries() - 1);
            (*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 1)] /= (fNumReal[0]->GetEntries() - 1);

            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! RR" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 0, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! RI" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 0)])) {
              // 	    cout << "NaN !!!! IR" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
            if (std::isnan((*covmnum)[GetBin(ibin - 1, ilmzero, 1, ilmprim, 1)])) {
              // 	    cout << "NaN !!!! II" << std::endl;
              // 	    cout << fNumReal[0]->GetEntries() << " "
              // 		 << real(tTq0[ilmzero]) << " "
              // 		 << real(tTq0[ilmprim]) << " "
              // 		 << imag(tTq0[ilmzero]) << " "
              // 		 << imag(tTq0[ilmprim]) << " " << std::endl;
            }
          }
      }

      GetMtilde(tMq0, tMTilde);

      // Perform the solution for the correlation function itself and the errors
      //     cout << "=============================" << std::endl;
      //     cout << "C calculation for bin " << (ibin-1) << std::endl;
      //     cout << std::endl;
      //     cout << "Input: " << std::endl;
      //     cout << "T vector " << std::endl;
      //     for (int ilm=0; ilm<GetMaxJM(); ilm++)
      //       cout << real(tTq0[ilm]) << " " << imag(tTq0[ilm]) << "   ";
      //     cout << std::endl << "M vector " << std::endl;
      //     for (int ilm=0; ilm<GetMaxJM(); ilm++)
      //       cout << real(tMq0[ilm]) << " " << imag(tMq0[ilm]) << "   ";
      //     cout << std::endl;

      if (fNumReal[0]->GetBinContent(ibin) > 0) {
        // Rewrite the new way to use the solving wherever there is inversion
        double mDeltaT[fMaxJM * fMaxJM * 4];
        for (int ilmzero = 0; ilmzero < GetMaxJM() * 2; ilmzero++)
          for (int ilmprim = 0; ilmprim < GetMaxJM() * 2; ilmprim++)
            mDeltaT[(ilmzero * fMaxJM * 2) + ilmprim] =
              ((*covmnum)[GetBin(ibin - 1, ilmzero / 2, ilmzero % 2, ilmprim / 2, ilmprim % 2)]);

#ifdef _FINISH_DEBUG_
        std::cout << "Delta T matrix " << std::endl;
        for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
          for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mDeltaT[ilmz * GetMaxJM() * 2 + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        double mDeltaTPacked[fMaxJM * fMaxJM * 4];
        int msize = PackYlmMatrixIndependentOnly(mDeltaT, mDeltaTPacked);

#ifdef _FINISH_DEBUG_
        std::cout << "Delta T matrix packed " << std::endl;
        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mDeltaTPacked[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        // (1) Solve (DeltaT)^1 Mtilde = Q

        // Prepare halper matrices

        double mM[fMaxJM * fMaxJM * 4];
        double mMPacked[fMaxJM * fMaxJM * 4];
        for (int iter = 0; iter < fMaxJM * fMaxJM * 4; iter++)
          mM[iter] = tMTilde[iter];
        PackYlmMatrixIndependentOnly(mM, mMPacked);

        gsl_matrix_view matM = gsl_matrix_view_array(mMPacked, msize, msize);
#ifdef _FINISH_DEBUG_
        std::cout << "Mtilde matrix " << std::endl;
        for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
          for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mM[ilmz * GetMaxJM() * 2 + ilmp];
          }
          std::cout << std::endl;
        }

        std::cout << "Mtilde matrix packed " << std::endl;
        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mMPacked[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        // Inverting matrix DeltaT.

        double mU[fMaxJM * fMaxJM * 4];
        InvertYlmIndependentMatrix(mDeltaT, mU);

        double mDTInvertedPacked[fMaxJM * fMaxJM * 4];
        PackYlmMatrixIndependentOnly(mU, mDTInvertedPacked);

        gsl_matrix_view matDTI = gsl_matrix_view_array(mDTInvertedPacked, msize, msize);

#ifdef _FINISH_DEBUG_
        std::cout << "Delta T matrix inverted packed " << std::endl;
        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mDTInvertedPacked[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        // (2) Multiply DeltaT^1 M = Q
        double mQ[fMaxJM * fMaxJM * 4];
        for (int iter = 0; iter < msize * msize; iter++)
          mQ[iter] = 0.0;
        gsl_matrix_view matQ = gsl_matrix_view_array(mQ, msize, msize);

        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &matDTI.matrix, &matM.matrix, 0.0, &matQ.matrix);

        double mTest[fMaxJM * fMaxJM * 4];
        gsl_matrix_view matTest = gsl_matrix_view_array(mTest, msize, msize);

        double mF[fMaxJM * fMaxJM * 4];
        for (int iter = 0; iter < fMaxJM * fMaxJM * 4; iter++)
          mF[iter] = mDeltaTPacked[iter];
        gsl_matrix_view matF = gsl_matrix_view_array(mF, msize, msize);

        gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, &matF.matrix, &matQ.matrix, 0.0, &matTest.matrix);

#ifdef _FINISH_DEBUG_
        std::cout << "Test matrix packed - compare to Mtilde" << std::endl;
        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mTest[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        // (2) Multiply Mtilde^T Q = P

        double mP[fMaxJM * fMaxJM * 4];
        for (int iter = 0; iter < fMaxJM * fMaxJM * 4; iter++)
          mP[iter] = 0;

        gsl_matrix_view matP = gsl_matrix_view_array(mP, msize, msize);

        gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, &matM.matrix, &matQ.matrix, 0.0, &matP.matrix);

#ifdef _FINISH_DEBUG_
        std::cout << "P matrix packed " << std::endl;
        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mP[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        // (3) Solve P^-1 Mtilde^T = R
        double mPUnpacked[fMaxJM * fMaxJM * 4];
        UnPackYlmMatrixIndependentOnly(mP, mPUnpacked, msize);

#ifdef _FINISH_DEBUG_
        std::cout << "P matrix unpacked " << std::endl;
        for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
          for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mPUnpacked[ilmz * GetMaxJM() * 2 + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        // Invert the P matrix

        double mPInverted[fMaxJM * fMaxJM * 4];
        InvertYlmIndependentMatrix(mPUnpacked, mPInverted);

        double mPInvertedPacked[fMaxJM * fMaxJM * 4];
        PackYlmMatrixIndependentOnly(mPInverted, mPInvertedPacked);

        gsl_matrix_view matPI = gsl_matrix_view_array(mPInvertedPacked, msize, msize);

#ifdef _FINISH_DEBUG_
        std::cout << "P matrix inverted packed " << std::endl;
        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mPInvertedPacked[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        //       //      gsl_matrix_view matR = gsl_matrix_view_array(mR, msize,
        //       msize);

        //       double mG[fMaxJM*fMaxJM*4];
        //       for (int iter=0; iter<fMaxJM*fMaxJM*4; iter++)
        // 	mG[iter] = mP[iter];
        //       gsl_matrix_view matG = gsl_matrix_view_array(mG, msize, msize);

        //       // Decomposing the M matrix
        //       gsl_linalg_SV_decomp(&matG.matrix, &matS.matrix, &vecST.vector,
        //       &vecWT.vector);

        //       for (int itert=0; itert<msize; itert++) {
        // 	for (int iterm=0; iterm<msize; iterm++)
        // 	  vCT[iterm] = mMPacked[iterm*msize + itert];
        // 	  // Transvere !!!      ^^^^^         ^^^^^

        // 	// Solving the problem
        // 	gsl_linalg_SV_solve(&matG.matrix, &matS.matrix, &vecST.vector,
        // &vecCT.vector, &vecXT.vector);

        // 	for (int iterm=0; iterm<msize; iterm++)
        // 	  mR[itert*msize + iterm] = vXT[iterm];
        //       }

        double mR[fMaxJM * fMaxJM * 4];
        for (int ir = 0; ir < fMaxJM * fMaxJM * 4; ir++)
          mR[ir] = 0.0;
        gsl_matrix_view matR = gsl_matrix_view_array(mR, msize, msize);

        // (2) Multiply P^-1 M (Trans) = R

#ifdef _FINISH_DEBUG_
        std::cout << "Matrix M Packed " << std::endl;
        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mMPacked[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &matPI.matrix, &matM.matrix, 1.0, &matR.matrix);

#ifdef _FINISH_DEBUG_
        std::cout << "R matrix packed " << std::endl;

        for (int ilmz = 0; ilmz < msize; ilmz++) {
          for (int ilmp = 0; ilmp < msize; ilmp++) {
            std::cout.precision(3);
            std::cout.width(10);
            std::cout << mR[ilmz * msize + ilmp];
          }
          std::cout << std::endl;
        }
#endif

        // (4) Solve DeltaT^-1 T = L
        double vL[fMaxJM * 2];
        gsl_vector_view vecL = gsl_vector_view_array(vL, msize);

        //       // Decomposing the M matrix
        //       gsl_linalg_SV_decomp(&matF.matrix, &matS.matrix, &vecST.vector,
        //       &vecWT.vector);

        double vB[fMaxJM * 2];
        for (int iter = 0; iter < GetMaxJM(); iter++) {
          vB[iter * 2]     = real(tTq0[iter]);
          vB[iter * 2 + 1] = imag(tTq0[iter]);
        }

        double vBPacked[fMaxJM * 2];
        PackYlmVectorIndependentOnly(vB, vBPacked);

        gsl_vector_view vecB = gsl_vector_view_array(vBPacked, msize);

        //       // Solving the problem
        //       gsl_linalg_SV_solve(&matF.matrix, &matS.matrix, &vecST.vector,
        //       &vecB.vector, &vecL.vector);

#ifdef _FINISH_DEBUG_
        std::cout << "L vector packed " << std::endl;
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << vL[ilmp];
        }
        std::cout << std::endl;
#endif

        // Multiply DeltaT^-1 T = L

        gsl_blas_dgemv(CblasNoTrans, 1.0, &matDTI.matrix, &vecB.vector, 0.0, &vecL.vector);

        // (5) Multiply R L = C

        double vY[fMaxJM * 2];
        for (int iter = 0; iter < GetMaxJM() * 2; iter++) {
          vY[iter] = 0.0;
        }

        // Prepare inputs for solving the problem
        gsl_vector_view vecY = gsl_vector_view_array(vY, msize);

        gsl_blas_dgemv(CblasNoTrans, 1.0, &matR.matrix, &vecL.vector, 0.0, &vecY.vector);

#ifdef _FINISH_DEBUG_
        std::cout << "C vector packed " << std::endl;
        for (int ilmp = 0; ilmp < msize; ilmp++) {
          std::cout.precision(3);
          std::cout.width(10);
          std::cout << vY[ilmp];
        }
        std::cout << std::endl;
#endif
        int mpack = 0;
        int el, em;
        for (int ilm = 0; ilm < fMaxJM; ilm++) {
          // 	fCFReal[ilm]->SetBinContent(ibin, vC[mpack++]);
          GetElEmForIndex(ilm, el, em);
          if ((el % 2) == 1) {
            fCFReal[ilm]->SetBinContent(ibin, 0.0);
            fCFImag[ilm]->SetBinContent(ibin, 0.0);
          }

#ifdef FULL_CALC
          fCFReal[ilm]->SetBinContent(ibin, fNumReal[ilm]->GetBinContent(ibin) / fDenReal[ilm]->GetBinContent(ibin));
          fCFImag[ilm]->SetBinContent(ibin, fNumImag[ilm]->GetBinContent(ibin) / fDenImag[ilm]->GetBinContent(ibin));

#else
          if (em < 0) {
            fCFReal[ilm]->SetBinContent(ibin, 0.0);
            fCFImag[ilm]->SetBinContent(ibin, 0.0);
          } else {
            fCFReal[ilm]->SetBinContent(ibin, vY[mpack++]);
            if (em == 0)
              fCFImag[ilm]->SetBinContent(ibin, 0);
            else
              //	  fCFImag[ilm]->SetBinContent(ibin, vC[mpack++]);
              fCFImag[ilm]->SetBinContent(ibin, vY[mpack++]);
          }

#endif
        }

        // invert the P matrix to get C errors
        //      double mS[fMaxJM*fMaxJM*4];

        //       for (int iterz=0; iterz<msize; iterz++)
        // 	for (int iterp=0; iterp<msize; iterp++)
        // 	  if (iterp == iterz)
        // 	    mS[iterz*msize + iterp] = 1.0;
        // 	  else
        // 	    mS[iterz*msize + iterp] = 0.0;

        //      gsl_matrix_view matS = gsl_matrix_view_array(mS, msize, msize);

        // Invert V

        //       gsl_blas_dtrsm(CblasLeft, CblasUpper, CblasNoTrans,
        //       CblasNonUnit, 1.0, &matP.matrix, &matS.matrix);

        mpack = 0;
        for (int ilm = 0; ilm < fMaxJM; ilm++) {
          GetElEmForIndex(ilm, el, em);
          if (em < 0) {
            fCFReal[ilm]->SetBinError(ibin, 0);
            fCFImag[ilm]->SetBinError(ibin, 0);
          } else {
            fCFReal[ilm]->SetBinError(ibin, sqrt(fabs(mPInvertedPacked[mpack * msize + mpack])));
            mpack++;
            if (em == 0)
              fCFImag[ilm]->SetBinError(ibin, 0);
            else {
              fCFImag[ilm]->SetBinError(ibin, sqrt(fabs(mPInvertedPacked[mpack * msize + mpack])));
              mpack++;
            }
          }
        }

        for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
          for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
            if (ilmp > ilmz)
              (*covmcfc)[GetBin(ibin - 1, ilmz / 2, ilmz % 2, ilmp / 2, ilmp % 2)] = mPInverted[ilmz * GetMaxJM() * 2 + ilmp];
            else
              (*covmcfc)[GetBin(ibin - 1, ilmz / 2, ilmz % 2, ilmp / 2, ilmp % 2)] = mPInverted[ilmp * GetMaxJM() * 2 + ilmz];
          }
        }
      } else {
        for (int ilm = 0; ilm < fMaxJM; ilm++) {
          fCFReal[ilm]->SetBinError(ibin, 0);
          fCFImag[ilm]->SetBinError(ibin, 0);
        }

        for (int ilmz = 0; ilmz < GetMaxJM() * 2; ilmz++) {
          for (int ilmp = 0; ilmp < GetMaxJM() * 2; ilmp++) {
            (*covmcfc)[GetBin(ibin - 1, ilmz / 2, ilmz % 2, ilmp / 2, ilmp % 2)] = 0.0;
          }
        }
      }
    }
    /// fix CF by using T(l'm) = T(l,-m)*
    for (int l = 0; l <= GetL(); l++) {
      for (int m = -l; m < 0; m++) {
        Int_t from = GetIndexForLM(l, -m);
        Int_t to   = GetIndexForLM(l, m);
        for (int i = 1; i <= fCFReal[from]->GetNbinsX(); i++) {
          fCFReal[to]->SetBinContent(i, fCFReal[from]->GetBinContent(i));
          fCFReal[to]->SetBinError(i, fCFReal[from]->GetBinError(i));
          fCFImag[to]->SetBinContent(i, -fCFImag[from]->GetBinContent(i));
          fCFImag[to]->SetBinError(i, fCFImag[from]->GetBinError(i));
        }
      }
    }

    PackCfcCovariance();
#else
    std::cout << "GLS NOT ENABLED!" << std::endl;
#endif
  }

  Long64_t FemtoSHCF::Merge(TCollection* collection) {
    if (collection) {
      FemtoSHCF* pack = NULL;
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
      Cout::PrintInfo("Not compatible maxL numbers during merging FemtoSHCF", EInfo::kLessError);
      return;
    }
    if (cf->fFrame != this->fFrame) {
      Cout::PrintInfo("Incompatible frames !", EInfo::kLessError);
      return;
    }
    for (int i = 0; i < fMaxJM; i++) {
      fNumReal[i]->Add(cf->fNumReal[i]);
      fNumImag[i]->Add(cf->fNumImag[i]);
      fDenReal[i]->Add(cf->fDenReal[i]);
      fDenImag[i]->Add(cf->fDenImag[i]);
    }
    covmcfc->Add(*cf->covmcfc);
    covmden->Add(*cf->covmden);
    covmnum->Add(*cf->covmnum);
    fNum->Add(cf->fNum);
    fDen->Add(cf->fDen);
  }

  Double_t FemtoSHCF::Sil(Double_t n) const {
    if (n == 0)
      return 1;
    else
      return n * Sil(n - 1);
  }

  Double_t FemtoSHCF::Sil2(Double_t n) const {
    if (n <= 1) {
      return 1;
    } else {
      return n * Sil2(n - 2);
    }
  }

  FemtoSHCF::~FemtoSHCF() {
    if (fNumImag) delete[] fNumImag;
    if (fNumReal) delete[] fNumReal;
    if (fDenReal) delete[] fDenReal;
    if (fDenImag) delete[] fDenImag;
    if (fCFReal) delete[] fCFReal;
    if (fCFImag) delete[] fCFImag;
    if (covmnum) delete covmnum;
    if (covmden) delete covmden;
    if (covmcfc) delete covmcfc;
    if (fEls) delete[] fEls;
    if (fEms) delete[] fEms;
    if (fElsi) delete[] fElsi;
    if (fEmsi) delete[] fEmsi;
    if (fCfcov) delete fCfcov;
  }

  TString FemtoSHCF::HTMLExtract(Int_t counter, TString dir) const {
    gSystem->MakeDirectory(Form("%s/divided_%i", dir.Data(), counter));
    TString filename = Form("%s/divided_%i/divided.html", dir.Data(), counter);
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "nicatable", "");
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
    HtmlTable table2("", "nicatable", "");
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
    Int_t jmax   = GetL() * 2 + 1;
    Int_t width  = 966 / jmax;
    Int_t height = 966 / GetL();
    if (height > width) height = width;
    for (int i = 0; i <= GetL(); i++) {
      HtmlRow row7("", "dark_blue", "");

      int I = i;
      for (int j = 0; j <= GetL(); j++) {
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
    std::complex<double>* YlmBuffer = FemtoYlm::YlmUpToL(fMaxL, pair->GetX(), pair->GetY(), pair->GetZ());
    for (int ilm = 0; ilm < GetMaxJM(); ilm++) {
      fNumReal[ilm]->Fill(kv, real(YlmBuffer[ilm]) * pair->GetWeight());
      fNumImag[ilm]->Fill(kv, -imag(YlmBuffer[ilm]) * pair->GetWeight());
      fNum->Fill(kv, 1.0);
    }

    // Fill in the error matrix
    int nqbin = fNum->GetXaxis()->FindFixBin(kv) - 1;
    if (nqbin < fNum->GetNbinsX()) {
      Double_t weight2 = pair->GetWeight() * pair->GetWeight();
      for (int ilmzero = 0; ilmzero < GetMaxJM(); ilmzero++) {
        for (int ilmprim = 0; ilmprim < GetMaxJM(); ilmprim++) {
          Int_t gbin       = GetBin(nqbin, ilmzero, 0, ilmprim, 0);
          (*covmnum)[gbin] = (*covmnum)[gbin] + real(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          gbin             = GetBin(nqbin, ilmzero, 0, ilmprim, 1);
          (*covmnum)[gbin] = (*covmnum)[gbin] + real(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
          gbin             = GetBin(nqbin, ilmzero, 1, ilmprim, 0);
          (*covmnum)[gbin] = (*covmnum)[gbin] - imag(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          gbin             = GetBin(nqbin, ilmzero, 1, ilmprim, 1);
          (*covmnum)[gbin] = (*covmnum)[gbin] - imag(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
        }
      }
    }
  }

  void FemtoSHCF::FillDenObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    Double_t kv     = TMath::Sqrt(pair->GetX() * pair->GetX() + pair->GetY() * pair->GetY() + pair->GetZ() * pair->GetZ());
    std::complex<double>* YlmBuffer = FemtoYlm::YlmUpToL(fMaxL, pair->GetX(), pair->GetY(), pair->GetZ());
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
        for (int ilmprim = 0; ilmprim < GetMaxJM(); ilmprim++) {
          Int_t gbin       = GetBin(nqbin, ilmzero, 0, ilmprim, 0);
          (*covmden)[gbin] = (*covmden)[gbin] + real(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          gbin             = GetBin(nqbin, ilmzero, 0, ilmprim, 1);
          (*covmden)[gbin] = (*covmden)[gbin] + real(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
          gbin             = GetBin(nqbin, ilmzero, 1, ilmprim, 0);
          (*covmden)[gbin] = (*covmden)[gbin] - imag(YlmBuffer[ilmzero]) * real(YlmBuffer[ilmprim]) * weight2;
          gbin             = GetBin(nqbin, ilmzero, 1, ilmprim, 1);
          (*covmden)[gbin] = (*covmden)[gbin] - imag(YlmBuffer[ilmzero]) * -imag(YlmBuffer[ilmprim]) * weight2;
        }
      }
    }
  }

  Array_1<Float_t>* FemtoSHCF::ExportToFlatNum() const {
    Array_1<Float_t>* data = new Array_1<Float_t>(fNum->GetNbinsX() * fMaxJM * 2);
    Int_t bin              = 0;
    for (int l = 0; l < GetL(); l++) {
      for (int m = -l; l <= m; m++) {
        TH1D* h = GetCFRe(l, m);
        for (int i = 1; i <= h->GetNbinsX(); i++) {
          (*data)[bin++] = h->GetBinContent(i);
        }
      }
    }
    for (int l = 0; l < GetL(); l++) {
      for (int m = -l; l <= m; m++) {
        TH1D* h = GetCFIm(l, m);
        for (int i = 1; i <= h->GetNbinsX(); i++) {
          (*data)[bin++] = h->GetBinContent(i);
        }
      }
    }
    return data;
  }

  void FemtoSHCF::Rebin(Int_t ngroup, Option_t* opt) { std::cout << "REBIN of SHCF not implented !" << std::endl; }

  void FemtoSHCF::Fit(CorrFitSHCF* fit) { fit->Fit(this); }

  void FemtoSHCF::FitDummy(CorrFitSHCF* fit) { fit->FitDummy(this); }
}  // namespace Hal
