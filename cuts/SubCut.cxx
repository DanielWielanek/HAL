/*
 * SubCut.cxx
 *
 *  Created on: 10 gru 2016
 *      Author: Daniel Wielanek
 *      E-mail: daniel.wielanek@gmail.com
 *      Warsaw University of Technology, Faculty of Physics
 */

#include "SubCut.h"

#include "Cut.h"
#include "Package.h"
#include "Parameter.h"

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

  void SubCutHisto::AddToReport(Package* report) const {
    report->AddObject(new ParameterString("Label", "Acceptance Histogram"));
    report->AddObject(fAcceptanceHistogram->Clone());
  }
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

  //================================sub cut histo =============================================================================

  SubCutHisto::SubCutHisto(Int_t size) : fSize(size), fAcceptanceHistogram(NULL), fParX(-1), fParY(-1), fParZ(-1) {}

  SubCutHisto::SubCutHisto(const SubCutHisto& other) :
    TObject(other), fSize(other.fSize), fParX(other.fParX), fParY(other.fParY), fParZ(other.fParZ) {
    if (other.fAcceptanceHistogram != NULL) {
      this->fAcceptanceHistogram = (TH1*) other.fAcceptanceHistogram->Clone();
      fAcceptanceHistogram->SetDirectory(0);
    }
  }

  SubCutHisto::~SubCutHisto() {
    if (fAcceptanceHistogram) delete fAcceptanceHistogram;
  }

  Bool_t SubCutHisto::Validate(Double_t x, Double_t y, Double_t z) {
    Double_t prob = 0;
    switch (fSize) {
      case 1: prob = fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindBin(x)); break;
      case 2: prob = fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindFixBin(x, y)); break;
      case 3: prob = fAcceptanceHistogram->GetBinContent(fAcceptanceHistogram->FindFixBin(x, y, z)); break;
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

  Bool_t SubCutHisto::ValidateAbs(Double_t x, Double_t y, Double_t z) {
    return Validate(TMath::Abs(x), TMath::Abs(y), TMath::Abs(z));
  }

  Bool_t SubCutHisto::Init(const Cut& thisCut, Int_t par1, Int_t par2, Int_t par3) {
    if (fAcceptanceHistogram == nullptr) return kFALSE;
    fAcceptanceHistogram->GetXaxis()->SetTitle(thisCut.GetUnit(par1));
    if (par2 != -1) fAcceptanceHistogram->GetYaxis()->SetTitle(thisCut.GetUnit(par2));
    if (par3 != -1) fAcceptanceHistogram->GetZaxis()->SetTitle(thisCut.GetUnit(par3));
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
      this->fSize = other.fSize;
      if (other.fAcceptanceHistogram != nullptr) {
        this->fAcceptanceHistogram = (TH1*) other.fAcceptanceHistogram->Clone();
        fAcceptanceHistogram->SetDirectory(0);
      }
      return *this;
    }
  }


  SubCutRectangle::SubCutRectangle() : fParX(0), fParY(0) {}

  void SubCutRectangle::AddSquare(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax) {
    if (xmin > xmax) {
      fMinX.push_back(xmax);
      fMaxX.push_back(xmin);
    } else {
      fMinX.push_back(xmin);
      fMaxX.push_back(xmax);
    }
    if (ymin > ymax) {
      fMinY.push_back(ymax);
      fMaxY.push_back(ymin);
    } else {
      fMinY.push_back(ymin);
      fMaxY.push_back(ymax);
    }
  }

  Int_t SubCutRectangle::Validate(Double_t x, Double_t y) {
    Int_t res = 0;
    for (unsigned int i = 0; i < fMinX.size(); i++) {
      if (x >= fMinX[i] && x <= fMaxX[i] && y >= fMinY[i] && y <= fMaxY[i]) res++;
    }
    return res;
  }

  void SubCutRectangle::AddToReport(Package* report) const {
    report->AddObject(new ParameterString("Label", "SquareCutReport"));
    report->AddObject(new ParameterInt("XPar", fParX));
    report->AddObject(new ParameterInt("YPar", fParY));
    report->AddObject(new ParameterString("XParName", fNameX));
    report->AddObject(new ParameterString("YParName", fNameY));
    for (unsigned int i = 0; i < fMinX.size(); i++) {
      report->AddObject(new ParameterDouble(Form("x_{min} %i", i), fMinX[i]));
      report->AddObject(new ParameterDouble(Form("x_{max} %i", i), fMaxX[i]));
      report->AddObject(new ParameterDouble(Form("y_{min} %i", i), fMinY[i]));
      report->AddObject(new ParameterDouble(Form("y_{max} %i", i), fMaxY[i]));
    }
  }

  Bool_t SubCutRectangle::Init(const Cut& thisCut, Int_t par1, Int_t par2) {
    fParX  = par1;
    fParY  = par2;
    fNameX = thisCut.GetUnit(fParX);
    fNameY = thisCut.GetUnit(fParY);
    if (fMinX.size() > 0) return kTRUE;
    return kFALSE;
  }

  SubCutRectangle::~SubCutRectangle() {}
}  // namespace Hal
