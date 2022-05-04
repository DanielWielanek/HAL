/*
 * HalChiSqMap.cxx
 *
 *  Created on: 23 paź 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalChiSqMap2D.h"
#include "HalSplines.h"
#include "HalStdMath.h"
#include <TAttLine.h>
#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TLine.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TMatrix.h>
#include <TMatrixDfwd.h>

#include <cfloat>


HalChiSqMap2D::HalChiSqMap2D() :
  fHist(NULL),
  fLineX(NULL),
  fLineY(NULL),
  fLineXlow(NULL),
  fLineXhigh(NULL),
  fLineYlow(NULL),
  fLineYhigh(NULL),
  fLabel("#chi/NDF") {}

HalChiSqMap2D::HalChiSqMap2D(TString name,
                             Int_t xbins,
                             Double_t xlow,
                             Double_t xhigh,
                             Int_t ybins,
                             Double_t ylow,
                             Double_t yhigh) :
  HalChiSqMap2D() {
  fHist = new TH2D(name, "", xbins, xlow, xhigh, ybins, ylow, yhigh);
}

void HalChiSqMap2D::SetXFit(Double_t Value, Double_t errLow, Double_t errHigh) {
  if (errHigh == -1) errHigh = errLow;
  if (fLineX) delete fLineX;
  if (fLineXlow) delete fLineXlow;
  if (fLineXhigh) delete fLineXhigh;

  fLineX     = MakeXLine(Value, 1);
  fLineXlow  = MakeXLine(Value - errLow, 10);
  fLineXhigh = MakeXLine(Value + errHigh, 10);
}

void HalChiSqMap2D::SetYFit(Double_t Value, Double_t errLow, Double_t errHigh) {
  if (errHigh == -1) errHigh = errLow;
  if (fLineY) delete fLineY;
  if (fLineYlow) delete fLineYlow;
  if (fLineYhigh) delete fLineYhigh;

  fLineY     = MakeYLine(Value, 1);
  fLineYlow  = MakeYLine(Value - errLow, 10);
  fLineYhigh = MakeYLine(Value + errHigh, 10);
}

void HalChiSqMap2D::Draw(Option_t* opt) {
  if (fHist == NULL) return;
  fHist->SetStats(kFALSE);
  fHist->Draw("colz");
  TString option = opt;
  if (!option.Contains("nolin")) {
    DrawLineX(fLineX);
    DrawLineX(fLineXlow);
    DrawLineX(fLineXhigh);
    DrawLineY(fLineY);
    DrawLineY(fLineYlow);
    DrawLineY(fLineYhigh);
  }
  if (option.Contains("min")) {
    Int_t xb = 1, yb = 1;
    Double_t min = DBL_MAX;
    for (int i = 1; i <= fHist->GetNbinsX(); i++) {
      for (int j = 1; j <= fHist->GetNbinsY(); j++) {
        if (fHist->GetBinContent(i, j) < min) {
          min = fHist->GetBinContent(i, j);
          xb  = i;
          yb  = j;
        }
      }
    }
    Double_t x1  = fHist->GetXaxis()->GetBinLowEdge(xb);
    Double_t y1  = fHist->GetYaxis()->GetBinLowEdge(yb);
    Double_t x2  = fHist->GetXaxis()->GetBinUpEdge(xb);
    Double_t y2  = fHist->GetYaxis()->GetBinUpEdge(yb);
    TLine** line = new TLine*[6];
    line[0]      = new TLine(x1, y1, x1, y2);
    line[1]      = new TLine(x2, y1, x2, y2);
    line[2]      = new TLine(x1, y1, x2, y1);
    line[3]      = new TLine(x1, y2, x2, y2);
    line[4]      = new TLine(x1, y1, x2, y2);
    line[5]      = new TLine(x2, y1, x1, y2);

    for (int i = 0; i < 6; i++) {
      line[i]->SetLineColor(kGray);
      line[i]->Draw("SAME");
    }
  }
  if (option.Contains("fit")) {
    Double_t x1  = fHist->GetXaxis()->GetBinLowEdge(1);
    Double_t y1  = fHist->GetYaxis()->GetBinLowEdge(1);
    Double_t x2  = fHist->GetXaxis()->GetBinUpEdge(fHist->GetNbinsX());
    Double_t y2  = fHist->GetYaxis()->GetBinUpEdge(fHist->GetNbinsY());
    Double_t X   = GetEstX();
    Double_t Y   = GetEstY();
    TLine** line = new TLine*[2];
    line[0]      = new TLine(x1, Y, x2, Y);
    line[1]      = new TLine(X, y1, X, y2);
    for (int i = 0; i < 2; i++) {
      line[i]->SetLineColor(kGray + 2);
      line[i]->Draw("SAME");
    }
  }
}

void HalChiSqMap2D::SetParNames(TString xpar, TString ypar) {
  if (fHist == NULL) return;
  fHist->GetXaxis()->SetTitle(xpar);
  fHist->GetYaxis()->SetTitle(ypar);
}

Bool_t HalChiSqMap2D::CheckX(Double_t x) const {
  Double_t x_low  = fHist->GetXaxis()->GetBinLowEdge(1);
  Double_t x_high = fHist->GetXaxis()->GetBinUpEdge(fHist->GetNbinsX());
  if (x < x_low) return kFALSE;
  if (x > x_high) return kFALSE;
  return kTRUE;
}

Bool_t HalChiSqMap2D::CheckY(Double_t y) const {
  Double_t y_low  = fHist->GetYaxis()->GetBinLowEdge(1);
  Double_t y_high = fHist->GetYaxis()->GetBinUpEdge(fHist->GetNbinsY());
  if (y < y_low) return kFALSE;
  if (y > y_high) return kFALSE;
  return kTRUE;
}

TLine* HalChiSqMap2D::MakeXLine(Double_t x, Style_t style) const {
  Double_t y_low  = fHist->GetYaxis()->GetBinLowEdge(1);
  Double_t y_high = fHist->GetYaxis()->GetBinUpEdge(fHist->GetNbinsY());
  TLine* l        = new TLine(x, y_low, x, y_high);
  l->SetLineStyle(style);
  return l;
}

TLine* HalChiSqMap2D::MakeYLine(Double_t y, Style_t style) const {
  Double_t x_low  = fHist->GetXaxis()->GetBinLowEdge(1);
  Double_t x_high = fHist->GetXaxis()->GetBinUpEdge(fHist->GetNbinsX());
  TLine* l        = new TLine(x_low, y, x_high, y);
  l->SetLineStyle(style);
  return l;
}

Double_t HalChiSqMap2D::GetFitX() const {
  if (fLineX == NULL) return 0;
  return fLineX->GetX1();
}

Double_t HalChiSqMap2D::GetFitY() const {
  if (fLineY == NULL) return 0;
  return fLineY->GetY1();
}

Double_t HalChiSqMap2D::GetFitXErrorUp() const {
  if (fLineYhigh == NULL) return 0;
  return fLineYhigh->GetX1();
}

Double_t HalChiSqMap2D::GetFitXErrorLow() const {
  if (fLineXlow == NULL) return 0;
  return fLineXlow->GetX1();
}

Double_t HalChiSqMap2D::GetFitYErrorUp() const {
  if (fLineYhigh == NULL) return 0;
  return fLineYhigh->GetY1();
}

Double_t HalChiSqMap2D::GetFitYErrorLow() const {
  if (fLineYlow == NULL) return 0;
  return fLineYlow->GetY1();
}

void HalChiSqMap2D::DrawLineX(TLine* l) {
  if (l == NULL) return;
  if (CheckX(l->GetX1())) l->Draw("SAME");
}

void HalChiSqMap2D::DrawLineY(TLine* l) {
  if (l == NULL) return;
  if (CheckY(l->GetY1())) l->Draw("SAME");
}

void HalChiSqMap2D::GetMin(Double_t& x, Double_t& y) const {
  Int_t xb = 0, yb = 0;
  Double_t min = DBL_MAX;
  for (int i = 1; i <= fHist->GetNbinsX(); i++) {
    for (int j = 1; j <= fHist->GetNbinsY(); j++) {
      if (fHist->GetBinContent(i, j) < min) {
        min = fHist->GetBinContent(i, j);
        xb  = i;
        yb  = j;
      }
    }
  }
  x = fHist->GetXaxis()->GetBinCenter(xb);
  y = fHist->GetYaxis()->GetBinCenter(yb);
}

HalChiSqMap2D::~HalChiSqMap2D() {
  if (fHist) delete fHist;
  if (fLineX) delete fLineX;
  if (fLineY) delete fLineY;
  if (fLineXlow) delete fLineXlow;
  if (fLineXhigh) delete fLineXhigh;
  if (fLineYlow) delete fLineYlow;
  if (fLineYhigh) delete fLineYhigh;
}

Double_t HalChiSqMap2D::GetEstErrorX(Double_t thres, Bool_t around_fit) const {
  Int_t binX, binY;
  if (around_fit) {
    binX = fHist->GetXaxis()->FindBin(GetFitX());
    binY = fHist->GetYaxis()->FindBin(GetFitY());
  } else {
    Double_t x, y;
    GetMin(x, y);
    binX = fHist->GetXaxis()->FindBin(x);
    binY = fHist->GetYaxis()->FindBin(y);
  }
  if (binX <= 1 || binX >= fHist->GetNbinsX()) return 0;
  Double_t dX = fHist->GetXaxis()->GetBinWidth(binX);
  Double_t X  = fHist->GetXaxis()->GetBinCenter(binX);
  Double_t y1 = fHist->GetBinContent(binX - 1, binY);
  Double_t y2 = fHist->GetBinContent(binX, binY);
  Double_t y3 = fHist->GetBinContent(binX + 1, binY);
  Double_t x1 = X - dX;
  Double_t x2 = X;
  Double_t x3 = X + dX;
  Double_t a, b, c;
  HalStd::FitParabola(x1, x2, x3, y1, y2, y3, a, b, c);
  Double_t X1, X2;
  if (thres <= 0) return 1.0 / TMath::Sqrt(a);
  HalStd::SolveParabola(a, b, c - y2 * (1.0 + thres), X1, X2);
  return TMath::Max(TMath::Abs(X1 - X), TMath::Abs(X2 - X));
}

Double_t HalChiSqMap2D::GetEstErrorY(Double_t thres, Bool_t around_fit) const {
  Int_t binX, binY;
  if (around_fit) {
    binX = fHist->GetXaxis()->FindBin(GetFitX());
    binY = fHist->GetYaxis()->FindBin(GetFitY());
  } else {
    Double_t x, y;
    GetMin(x, y);
    binX = fHist->GetXaxis()->FindBin(x);
    binY = fHist->GetYaxis()->FindBin(y);
  }
  if (binY <= 1 || binY >= fHist->GetNbinsY()) return 0;
  Double_t dX = fHist->GetYaxis()->GetBinWidth(binY);
  Double_t X  = fHist->GetYaxis()->GetBinCenter(binY);
  Double_t y1 = fHist->GetBinContent(binX, binY - 1);
  Double_t y2 = fHist->GetBinContent(binX, binY);
  Double_t y3 = fHist->GetBinContent(binX, binY + 1);
  Double_t x1 = X - dX;
  Double_t x2 = X;
  Double_t x3 = X + dX;
  Double_t a, b, c;
  HalStd::FitParabola(x1, x2, x3, y1, y2, y3, a, b, c);
  Double_t X1, X2;
  if (thres <= 0) return 1.0 / TMath::Sqrt(a);
  HalStd::SolveParabola(a, b, c - y2 * (1.0 + thres), X1, X2);
  return TMath::Max(TMath::Abs(X1 - X), TMath::Abs(X2 - X));
}

Double_t HalChiSqMap2D::GetEstX() const {
  Int_t binX, binY;
  Double_t x, y;
  GetMin(x, y);
  binX = fHist->GetXaxis()->FindBin(x);
  binY = fHist->GetYaxis()->FindBin(y);
  if (binY <= 1 || binY >= fHist->GetNbinsY()) return 0;
  Double_t dX = fHist->GetXaxis()->GetBinWidth(binX);
  Double_t X  = fHist->GetXaxis()->GetBinCenter(binX);
  Double_t y1 = fHist->GetBinContent(binX - 1, binY);
  Double_t y2 = fHist->GetBinContent(binX, binY);
  Double_t y3 = fHist->GetBinContent(binX + 1, binY);
  Double_t x1 = X - dX;
  Double_t x2 = X;
  Double_t x3 = X + dX;
  Double_t a, b, c;
  HalStd::FitParabola(x1, x2, x3, y1, y2, y3, a, b, c);
  return -b / (2.0 * a);
}

Double_t HalChiSqMap2D::GetEstY() const {
  Int_t binX, binY;
  Double_t x, y;
  GetMin(x, y);
  binX = fHist->GetXaxis()->FindBin(x);
  binY = fHist->GetYaxis()->FindBin(y);
  if (binY <= 1 || binY >= fHist->GetNbinsY()) return 0;
  Double_t dX = fHist->GetYaxis()->GetBinWidth(binY);
  Double_t X  = fHist->GetYaxis()->GetBinCenter(binY);
  Double_t y1 = fHist->GetBinContent(binX, binY - 1);
  Double_t y2 = fHist->GetBinContent(binX, binY);
  Double_t y3 = fHist->GetBinContent(binX, binY + 1);
  Double_t x1 = X - dX;
  Double_t x2 = X;
  Double_t x3 = X + dX;
  Double_t a, b, c;
  HalStd::FitParabola(x1, x2, x3, y1, y2, y3, a, b, c);
  return -b / (2.0 * a);
}

void HalChiSqMap2D::GetXYE(Double_t& x, Double_t& eX, Double_t& y, Double_t& eY) const {
  GetMin(x, y);
  Int_t gBinX      = fHist->GetXaxis()->FindBin(x);
  Int_t gBinY      = fHist->GetYaxis()->FindBin(y);
  Double_t xlow    = fHist->GetXaxis()->GetBinCenter(gBinX - 1);
  Double_t xhig    = fHist->GetXaxis()->GetBinCenter(gBinX + 1);
  Double_t ylow    = fHist->GetYaxis()->GetBinCenter(gBinY - 1);
  Double_t yhig    = fHist->GetYaxis()->GetBinCenter(gBinY + 1);
  Double_t chi2ndf = fHist->GetBinContent(gBinX, gBinY);
  TH2D* h          = new TH2D("tempHes", "tempHes", 3, xlow, xhig, 3, ylow, yhig);
  for (int i = 1; i <= 3; i++) {
    for (int j = 1; j <= 3; j++) {
      h->SetBinContent(i, j, fHist->GetBinContent(gBinX + i - 2, gBinY + j - 2));
    }
  }
  HalSpline2D sp(h);
  delete h;

  Double_t a3 = sp.GetExtrapolationParam(x, y, 3);
  Double_t a4 = sp.GetExtrapolationParam(x, y, 4);
  Double_t a5 = sp.GetExtrapolationParam(x, y, 5);
  Double_t a6 = sp.GetExtrapolationParam(x, y, 6);
  Double_t a7 = sp.GetExtrapolationParam(x, y, 7);
  Double_t a8 = sp.GetExtrapolationParam(x, y, 8);

  x = GetEstX();
  y = GetEstY();

  Double_t fXY = a3 + 2. * a6 * x + 2.0 * a7 * y + 4.0 * a8 * x * y;
  Double_t fYX = fXY;
  Double_t fXX = 2.0 * a4 + 2.0 * a6 * y + 2.0 * a8 * y * y;
  Double_t fYY = 2.0 * a5 + 2.0 * a7 * x + 2.0 * a8 * x * x;

  TMatrixD m(2, 2);
  m[0][0]      = fXX;
  m[1][0]      = fYX;
  m[0][1]      = fXY;
  m[1][1]      = fYY;
  TMatrixD inv = m.Invert();

  eX = TMath::Sqrt(inv[0][0]) * chi2ndf;
  eY = TMath::Sqrt(inv[1][1]) * chi2ndf;
}
