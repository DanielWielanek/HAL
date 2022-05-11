/*
 * SubCut.cxx
 *
 *  Created on: 10 gru 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "SubCut.h"

#include <TAxis.h>
#include <TH1.h>
#include <TMathBase.h>
#include <TRandom.h>

namespace Hal {

  SubCut::SubCut(Int_t size) : TObject(), fMin(nullptr), fMax(nullptr), fValue(nullptr), fUnitName(nullptr) {
    fSize = size;
    if (fSize > 0) {
      fMin      = new Double_t[fSize];
      fMax      = new Double_t[fSize];
      fValue    = new Double_t[fSize];
      fUnitName = new TString[fSize];
    } else {
      fSize     = 1;
      fMin      = new Double_t[1];
      fMax      = new Double_t[1];
      fValue    = new Double_t[1];
      fUnitName = new TString[1];
    }
    for (int i = 0; i < fSize; i++) {
      fMin[i]      = -1E+9;
      fMax[i]      = 1E+9;
      fValue[i]    = 0;
      fUnitName[i] = "unknown [XU]";
    }
  }

  SubCut::SubCut(const SubCut& other) : TObject(other) {
    this->fSize     = other.fSize;
    this->fMin      = new Double_t[fSize];
    this->fMax      = new Double_t[fSize];
    this->fValue    = new Double_t[fSize];
    this->fUnitName = new TString[fSize];
    for (int i = 0; i < fSize; i++) {
      this->fMin[i]      = other.fMin[i];
      this->fMax[i]      = other.fMax[i];
      this->fValue[i]    = other.fValue[i];
      this->fUnitName[i] = other.fUnitName[i];
    }
  }

  SubCut::~SubCut() {
    delete[] fMin;
    delete[] fMax;
    delete[] fUnitName;
    delete[] fValue;
  }

  SubCut& SubCut::operator=(const SubCut& other) {
    if (this == &other) {
      return *this;
    } else {
      this->fSize     = other.fSize;
      this->fMin      = new Double_t[fSize];
      this->fMax      = new Double_t[fSize];
      this->fValue    = new Double_t[fSize];
      this->fUnitName = new TString[fSize];
      for (int i = 0; i < fSize; i++) {
        this->fMin[i]      = other.fMin[i];
        this->fMax[i]      = other.fMax[i];
        this->fValue[i]    = other.fValue[i];
        this->fUnitName[i] = other.fUnitName[i];
      }
      return *this;
    }
  }

  void SubCut::SetMin(Double_t val, Int_t i) { fMin[i] = val; }

  void SubCut::SetMax(Double_t val, Int_t i) { fMax[i] = val; }

  Bool_t SubCut::Validate() {
    for (Int_t i = 0; i < fSize; i++) {
      if (fValue[i] > fMax[i] || fValue[i] < fMin[i]) { return kFALSE; }
    }
    return kTRUE;
  }

  Bool_t SubCut::ValidateAbs() {
    for (Int_t i = 0; i < fSize; i++) {
      Double_t val = TMath::Abs(fValue[i]);
      if (val > fMax[i] || val < fMin[i]) { return kFALSE; }
    }
    return kTRUE;
  }

  SubCutHisto::SubCutHisto(Int_t size) : fSize(size), fAcceptanceHistogram(NULL) {
    if (fSize > 0) {
      fValue    = new Double_t[fSize];
      fUnitName = new TString[fSize];
    } else {
      fSize     = 1;
      fValue    = new Double_t[1];
      fUnitName = new TString[1];
    }
    for (int i = 0; i < fSize; i++) {
      fUnitName[i] = "unknown [XU]";
    }
  }

  SubCutHisto::SubCutHisto(const SubCutHisto& other) : TObject(other), fSize(other.fSize) {
    this->fSize     = other.fSize;
    this->fValue    = new Double_t[fSize];
    this->fUnitName = new TString[fSize];
    for (int i = 0; i < fSize; i++) {
      this->fValue[i]    = other.fValue[i];
      this->fUnitName[i] = other.fUnitName[i];
    }
    if (other.fAcceptanceHistogram != NULL) {
      this->fAcceptanceHistogram = (TH1*) other.fAcceptanceHistogram->Clone();
      fAcceptanceHistogram->SetDirectory(0);
    }
  }

  SubCutHisto::~SubCutHisto() {
    if (fAcceptanceHistogram) delete fAcceptanceHistogram;
    delete[] fValue;
    delete[] fUnitName;
  }

  Bool_t SubCutHisto::Validate() {
    Double_t prob = 0;
    switch (fSize) {
      case 1: prob = fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindBin(fValue[0])); break;
      case 2: prob = fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindFixBin(fValue[0], fValue[1])); break;
      case 3:
        prob = fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindFixBin(fValue[0], fValue[1], fValue[2]));
        break;
      default: break;
    }
    if (prob == 0) return kFALSE;
    if (prob == 1) return kTRUE;
    Double_t randval = gRandom->Rndm();
    if (randval <= prob) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  Bool_t SubCutHisto::ValidateAbs() {
    Double_t prob = 0;
    switch (fSize) {
      case 1: prob = fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindBin(TMath::Abs(fValue[0]))); break;
      case 2:
        prob =
          fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindFixBin(TMath::Abs(fValue[0]), TMath::Abs(fValue[1])));
        break;
      case 3:
        prob = fAcceptanceHistogram->GetBinContent(
          fAcceptanceHistogram->FindFixBin(TMath::Abs(fValue[0]), TMath::Abs(fValue[1]), TMath::Abs(fValue[2])));
        break;
      default: break;
    }
    if (prob == 0) return kFALSE;
    if (prob == 1) return kTRUE;
    Double_t randval = gRandom->Rndm();
    if (randval <= prob) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  Bool_t SubCutHisto::Init() {
    if (fAcceptanceHistogram == nullptr) return kFALSE;
    fAcceptanceHistogram->GetXaxis()->SetTitle(fUnitName[0]);
    if (fSize > 1) fAcceptanceHistogram->GetYaxis()->SetTitle(fUnitName[1]);
    if (fSize > 2) fAcceptanceHistogram->GetZaxis()->SetTitle(fUnitName[2]);
    return kFALSE;
  }

  Bool_t SubCutHisto::SetAcceptanceHistogram(const TH1& h) {
    if (h.InheritsFrom("TH3")) {
      if (fSize != 3) return kFALSE;
      for (int i = 0; i <= h.GetNbinsX() + 1; i++) {
        for (int j = 0; j <= h.GetNbinsY() + 1; j++) {
          for (int k = 0; k <= h.GetNbinsZ() + 1; k++) {
            Double_t cont = h.GetBinContent(i, j, k);
            if (cont < 0 || cont > 1) return kFALSE;
          }
        }
      }
    } else if (h.InheritsFrom("TH2")) {
      if (fSize != 2) return kFALSE;
      for (int i = 0; i <= h.GetNbinsX() + 1; i++) {
        for (int j = 0; j <= h.GetNbinsY() + 1; j++) {
          Double_t cont = h.GetBinContent(i, j);
          if (cont < 0 || cont > 1) return kFALSE;
        }
      }
    } else {
      if (fSize != 1) return kFALSE;
      for (int i = 0; i <= h.GetNbinsX() + 1; i++) {
        Double_t cont = h.GetBinContent(i);
        if (cont < 0 || cont > 1) return kFALSE;
      }
    }
    fAcceptanceHistogram = (TH1*) h.Clone();
    fAcceptanceHistogram->SetDirectory(0);
    return kTRUE;
  }

  SubCutHisto& SubCutHisto::operator=(const SubCutHisto& other) {
    if (this == &other) {
      return *this;
    } else {
      if (fValue) {
        delete[] fValue;
        delete[] fUnitName;
        fValue    = nullptr;
        fUnitName = nullptr;
      }
      this->fSize     = other.fSize;
      this->fValue    = new Double_t[fSize];
      this->fUnitName = new TString[fSize];
      for (int i = 0; i < fSize; i++) {
        this->fValue[i]    = other.fValue[i];
        this->fUnitName[i] = other.fUnitName[i];
      }
      if (other.fAcceptanceHistogram != nullptr) {
        this->fAcceptanceHistogram = (TH1*) other.fAcceptanceHistogram->Clone();
        fAcceptanceHistogram->SetDirectory(0);
      }
      return *this;
    }
  }
}  // namespace Hal
