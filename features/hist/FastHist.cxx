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
    fAxisNameX(""),
    fAxisNameY(""),
    fAxisNameZ(""),
    fTitle(title),
    fBinsNoX(0),
    fBinsNoY(0),
    fBinsNoZ(0),
    fTotalBinsNo(0),
    fValues(nullptr) {
    SetName(name);
  }

  FastHist::~FastHist() {
    if (fValues) delete[] fValues;
  }

  FastHist1D::FastHist1D(TString name, TString title, Int_t nbins, Double_t min, Double_t max) : FastHist(name, title) {
    fBinsNoX = nbins + 2;
    if (nbins != 0) {
      fValues = new Double_t[fBinsNoX];
      fMinX   = min;
      fMaxX   = max;
      fStepX  = (fMaxX - fMinX) / nbins;
      fStepX  = 1.0 / fStepX;
    }
    for (int i = 0; i < fTotalBinsNo; i++)
      fValues[i] = 0;
    fTotalBinsNo = fBinsNoX;
  }

  Int_t FastHist1D::FindBin(Double_t x) const {
    Int_t bin = (x - fMinX) * fStepX + 1;
    if (bin < 0) {
      bin = 0;
    } else if (bin > fBinsNoX - 1) {
      bin = fBinsNoX - 1;
    }
    return bin;
  }

  void FastHist1D::Fill(Double_t x, Double_t w) {
    Int_t bin = (x - fMinX) * fStepX + 1;
    if (bin < 0) {
      bin = 0;
    } else if (bin > fBinsNoX - 1) {
      bin = fBinsNoX - 1;
    }
    fValues[bin] += w;
  }

  TH1* FastHist1D::GetTH1() const {
    TH1D* h = new TH1D(GetName(), GetTitle(), fBinsNoX - 2, fMinX, fMaxX);
    if (fAxisNameX.Length() > 0) h->GetXaxis()->SetTitle(fAxisNameX);
    if (fAxisNameY.Length() > 0) h->GetYaxis()->SetTitle(fAxisNameY);
    for (int i = 0; i < fBinsNoX; i++) {
      h->SetBinContent(i, fValues[i]);
    }
    return h;
  }

  FastHist2D::FastHist2D(TString name,
                         TString title,
                         Int_t nbinsX,
                         Double_t xmin,
                         Double_t xmax,
                         Int_t nbinsY,
                         Double_t ymin,
                         Double_t ymax) :
    FastHist(name, title) {
    fBinsNoX     = nbinsX + 2;
    fBinsNoY     = nbinsY + 2;
    fMinX        = xmin;
    fMinY        = ymin;
    fMaxX        = xmax;
    fMaxY        = ymax;
    fTotalBinsNo = fBinsNoX * fBinsNoY;
    fValues      = new Double_t[fTotalBinsNo];
    fStepX       = (fMaxX - fMinX) / (Double_t) nbinsX;
    fStepY       = (fMaxY - fMinY) / (Double_t) nbinsY;
    fStepX       = 1.0 / fStepX;
    fStepY       = 1.0 / fStepY;
    for (int i = 0; i < fTotalBinsNo; i++)
      fValues[i] = 0;
  }

  Int_t FastHist2D::FindBin(Double_t x, Double_t y) const {
    Int_t binX = (x - fMinX) * fStepX + 1;
    Int_t binY = (y - fMinY) * fStepY + 1;

    if (binX < 0) {
      binX = 0;
    } else if (binX + 1 > fBinsNoX) {
      binX = fBinsNoX - 1;
    }
    if (binY < 0) {
      binY = 0;
    } else if (binY + 1 > fBinsNoY) {
      binY = fBinsNoY - 1;
    }
    return binX * fBinsNoY + binY;
  }

  void FastHist2D::Fill(Double_t x, Double_t y, Double_t w) {
    Int_t binX = (x - fMinX) * fStepX + 1;
    Int_t binY = (y - fMinY) * fStepY + 1;

    if (binX < 0) {
      binX = 0;
    } else if (binX + 1 > fBinsNoX) {
      binX = fBinsNoX - 1;
    }
    if (binY < 0) {
      binY = 0;
    } else if (binY + 1 > fBinsNoY) {
      binY = fBinsNoY - 1;
    }
    fValues[binX * fBinsNoY + binY] += w;
  }

  Double_t FastHist2D::GetBinContent(Int_t i, Int_t j) const { return fValues[i * fBinsNoY + j]; }

  TH1* FastHist2D::GetTH1() const {
    TH2D* h = new TH2D(GetName(), GetTitle(), fBinsNoX - 2, fMinX, fMaxX, fBinsNoY - 2, fMinY, fMaxY);
    if (fAxisNameX.Length() > 0) h->GetXaxis()->SetTitle(fAxisNameX);
    if (fAxisNameY.Length() > 0) h->GetYaxis()->SetTitle(fAxisNameY);
    if (fAxisNameZ.Length() > 0) h->GetZaxis()->SetTitle(fAxisNameZ);
    for (int i = 0; i < fBinsNoX; i++) {
      for (int j = 0; j < fBinsNoY; j++) {
        h->SetBinContent(i, j, fValues[i * fBinsNoY + j]);
      }
    }
    return h;
  }

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
    fBinsNoX     = nbinsX + 2;
    fBinsNoY     = nbinsY + 2;
    fBinsNoZ     = nbinsZ + 2;
    fMinX        = xmin;
    fMinY        = ymin;
    fMinZ        = zmin;
    fMaxX        = xmax;
    fMaxY        = ymax;
    fMaxZ        = zmax;
    fTotalBinsNo = fBinsNoX * fBinsNoY * fBinsNoZ;
    fValues      = new Double_t[fTotalBinsNo];
    fStepX       = (fMaxX - fMinX) / nbinsX;
    fStepY       = (fMaxY - fMinY) / nbinsY;
    fStepZ       = (fMaxZ - fMinZ) / nbinsZ;
    fStepX       = 1.0 / fStepX;
    fStepY       = 1.0 / fStepY;
    fStepZ       = 1.0 / fStepZ;
    fBinsYZ      = fBinsNoY * fBinsNoZ;
    for (int i = 0; i < fTotalBinsNo; i++)
      fValues[i] = 0;
  }

  Int_t FastHist3D::FindBin(Double_t x, Double_t y, Double_t z) const {
    Int_t binX = (x - fMinX) * fStepX + 1;
    Int_t binY = (y - fMinY) * fStepY + 1;
    Int_t binZ = (z - fMinZ) * fStepZ + 1;
    if (binX < 0) {
      binX = 0;
    } else if (binX + 1 > fBinsNoX) {
      binX = fBinsNoX - 1;
    }
    if (binY < 0) {
      binY = 0;
    } else if (binY + 1 > fBinsNoY) {
      binY = fBinsNoY - 1;
    }
    if (binZ < 0) {
      binZ = 0;
    } else if (binZ + 1 > fBinsNoZ) {
      binZ = fBinsNoZ - 1;
    }
    return binX * fBinsYZ + binY * fBinsNoZ + binZ;
  }


  void FastHist3D::Fill(Double_t x, Double_t y, Double_t z, Double_t w) {
    Int_t binX = (x - fMinX) * fStepX + 1;
    Int_t binY = (y - fMinY) * fStepY + 1;
    Int_t binZ = (z - fMinZ) * fStepZ + 1;
    if (binX < 0) {
      binX = 0;
    } else if (binX + 1 > fBinsNoX) {
      binX = fBinsNoX - 1;
    }
    if (binY < 0) {
      binY = 0;
    } else if (binY + 1 > fBinsNoY) {
      binY = fBinsNoY - 1;
    }
    if (binZ < 0) {
      binZ = 0;
    } else if (binZ + 1 > fBinsNoZ) {
      binZ = fBinsNoZ - 1;
    }

    fValues[binX * fBinsYZ + binY * fBinsNoZ + binZ] += w;
  }
  Double_t FastHist3D::GetBinContent(Int_t i, Int_t j, Int_t k) const { return fValues[i * fBinsYZ + j * fBinsNoZ + k]; }

  TH1* FastHist3D::GetTH1() const {
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
    return h;
  }
} /* namespace Hal */
