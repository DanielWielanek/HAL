/*
 * FastHist.cxx
 *
 *  Created on: 6 cze 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FastHist.h"

#include <iostream>

namespace Hal {
  FastHist::FastHist(TString name, TString title) :
    fAxisNameX(""), fAxisNameY(""), fAxisNameZ(""), fTitle(title), fBinsNoX(0), fBinsNoY(0), fBinsNoZ(0), fTotalBinsNo(0) {
    TNamed::SetName(name);
  }

  Double_t FastHist::GetLabelValue(const std::vector<TString>& input, TString& val) const {
    for (unsigned int i = 0; i < input.size(); i++) {
      if (input[i] == val) return (Double_t) i;
    }
    return -1.0;
  }

  FastHist::~FastHist() {}

  //====================================================================================================================

  FastHist1D::FastHist1D(TString name, TString title, Int_t nbins, Double_t min, Double_t max) : FastHist(name, title) {
    fBinsNoX     = nbins + 2;
    fTotalBinsNo = fBinsNoX;
    fMinX        = min;
    fMaxX        = max;
    Initialize();
  }

  void FastHist1D::Initialize() {
    fValues.resize(fTotalBinsNo);
    fStepX   = (fMaxX - fMinX) / (fBinsNoX - 2);
    fStepX   = 1.0 / fStepX;
    fOffsetX = 1.0 - fMinX * fStepX;
    Reset();
  }

  void FastHist1D::LabelizeAxis(const std::vector<TString>& labels) {
    fLabelsX = labels;
    fMinX    = -0.5;
    fMaxX    = -0.5 + labels.size();
    Initialize();
  }

  TH1* FastHist1D::GetTH1() const {
    TH1::AddDirectory(kFALSE);
    TH1D* h = new TH1D(GetName(), GetTitle(), fBinsNoX - 2, fMinX, fMaxX);
    if (fAxisNameX.Length() > 0) h->GetXaxis()->SetTitle(fAxisNameX);
    if (fAxisNameY.Length() > 0) h->GetYaxis()->SetTitle(fAxisNameY);
    for (int i = 0; i < fBinsNoX; i++) {
      h->SetBinContent(i, fValues[i]);
    }
    h->SetFillColor(fFillColor);
    if (fLabelsX.size()) {
      int count = 0;
      for (auto label : fLabelsX) {
        h->GetXaxis()->SetBinLabel(++count, label);
      }
    }
    TH1::AddDirectory(kTRUE);
    return h;
  }

  //====================================================================================================================

  FastHist2D::FastHist2D(TString name,
                         TString title,
                         Int_t nbinsX,
                         Double_t xmin,
                         Double_t xmax,
                         Int_t nbinsY,
                         Double_t ymin,
                         Double_t ymax) :
    FastHist(name, title) {
    fBinsNoX = nbinsX + 2;
    fBinsNoY = nbinsY + 2;
    fMinX    = xmin;
    fMinY    = ymin;
    fMaxX    = xmax;
    fMaxY    = ymax;
    Initialize();
  }

  Double_t FastHist2D::GetBinContent(Int_t i, Int_t j) const { return fValues[i * fBinsNoY + j]; }

  void FastHist2D::Initialize() {
    fTotalBinsNo = fBinsNoX * fBinsNoY;
    fValues.resize(fTotalBinsNo);
    fStepX   = (fMaxX - fMinX) / (Double_t) (fBinsNoX - 2);
    fStepY   = (fMaxY - fMinY) / (Double_t) (fBinsNoY - 2);
    fStepX   = 1.0 / fStepX;
    fStepY   = 1.0 / fStepY;
    fOffsetX = 1.0 - fMinX * fStepX;
    fOffsetY = 1.0 - fMinY * fStepY;
    Reset();
  }

  void FastHist2D::LabelizeAxis(const std::vector<TString>& labels, Char_t axis) {
    if (axis == 'x') {
      fMinX    = -0.5;
      fMaxX    = -0.5 + labels.size();
      fLabelsX = labels;
    } else {
      fMinY    = -0.5;
      fMaxY    = -0.5 + labels.size();
      fLabelsY = labels;
    }
    Initialize();
  }

  TH1* FastHist2D::GetTH1() const {
    TH1::AddDirectory(kFALSE);
    TH2D* h = new TH2D(GetName(), GetTitle(), fBinsNoX - 2, fMinX, fMaxX, fBinsNoY - 2, fMinY, fMaxY);
    if (fAxisNameX.Length() > 0) h->GetXaxis()->SetTitle(fAxisNameX);
    if (fAxisNameY.Length() > 0) h->GetYaxis()->SetTitle(fAxisNameY);
    if (fAxisNameZ.Length() > 0) h->GetZaxis()->SetTitle(fAxisNameZ);
    for (int i = 0; i < fBinsNoX; i++) {
      for (int j = 0; j < fBinsNoY; j++) {
        h->SetBinContent(i, j, fValues[i * fBinsNoY + j]);
      }
    }
    h->SetFillColor(fFillColor);
    TH1::AddDirectory(kTRUE);
    return h;
  }

  //====================================================================================================================

  FastHist3D::FastHist3D(TString name,
                         TString title,
                         Int_t nbinsX,
                         Double_t xmin,
                         Double_t xmax,
                         Int_t nbinsY,
                         Double_t ymin,
                         Double_t ymax,
                         Int_t nbinsZ,
                         Double_t zmin,
                         Double_t zmax) :
    FastHist(name, title) {
    fBinsNoX = nbinsX + 2;
    fBinsNoY = nbinsY + 2;
    fBinsNoZ = nbinsZ + 2;
    fMinX    = xmin;
    fMinY    = ymin;
    fMinZ    = zmin;
    fMaxX    = xmax;
    fMaxY    = ymax;
    fMaxZ    = zmax;
    Initialize();
  }

  Double_t FastHist3D::GetBinContent(Int_t i, Int_t j, Int_t k) const { return fValues[i * fBinsYZ + j * fBinsNoZ + k]; }

  TH1* FastHist3D::GetTH1() const {
    TH1::AddDirectory(kFALSE);
    TH3D* h = new TH3D(GetName(), GetTitle(), fBinsNoX - 2, fMinX, fMaxX, fBinsNoY - 2, fMinY, fMaxY, fBinsNoZ - 2, fMinZ, fMaxZ);
    if (fAxisNameX.Length() > 0) h->GetXaxis()->SetTitle(fAxisNameX);
    if (fAxisNameY.Length() > 0) h->GetYaxis()->SetTitle(fAxisNameY);
    if (fAxisNameZ.Length() > 0) h->GetZaxis()->SetTitle(fAxisNameZ);
    for (int i = 0; i < fBinsNoX; i++) {
      for (int j = 0; j < fBinsNoY; j++) {
        for (int k = 0; k < fBinsNoZ; k++) {
          h->SetBinContent(i, j, k, fValues[i * fBinsYZ + j * fBinsNoZ + k]);
        }
      }
    }
    h->SetFillColor(fFillColor);
    TH1::AddDirectory(kTRUE);
    return h;
  }

  void FastHist3D::LabelizeAxis(const std::vector<TString>& labels, Char_t axis) {
    if (axis == 'x') {
      fMinX    = -0.5;
      fMaxX    = -0.5 + labels.size();
      fLabelsX = labels;
    } else if (axis == 'y') {
      fMinY    = -0.5;
      fMaxY    = -0.5 + labels.size();
      fLabelsY = labels;
    } else {
      fMinZ    = -0.5;
      fMaxZ    = -0.5 + labels.size();
      fLabelsZ = labels;
    }
    Initialize();
  }

  void FastHist3D::Initialize() {
    fTotalBinsNo = fBinsNoX * fBinsNoY * fBinsNoZ;
    fValues.resize(fTotalBinsNo);
    fStepX   = (fMaxX - fMinX) / (fBinsNoX - 2);
    fStepY   = (fMaxY - fMinY) / (fBinsNoY - 2);
    fStepZ   = (fMaxZ - fMinZ) / (fBinsNoZ - 2);
    fStepX   = 1.0 / fStepX;
    fStepY   = 1.0 / fStepY;
    fStepZ   = 1.0 / fStepZ;
    fBinsYZ  = fBinsNoY * fBinsNoZ;
    fOffsetX = 1. - fMinX * fStepX;
    fOffsetY = 1. - fMinY * fStepY;
    fOffsetZ = 1. - fMinZ * fStepZ;
    Reset();
  }

} /* namespace Hal */
