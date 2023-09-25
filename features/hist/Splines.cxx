/*
 * HalSplines.cxx
 *
 *  Created on: 13 lis 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Splines.h"

#include "Cout.h"
#include "Std.h"
#include "StdString.h"

#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TMatrixDfwd.h>
#include <TMatrixT.h>
#include <iostream>

namespace Hal {
  Spline1D::Spline1D(TH1* h, Double_t begval, Double_t endval) :
    fXaxis(NULL), fNbins(0), fA(NULL), fB(NULL), fC(NULL), fAe(NULL) {
    if (h == NULL) return;
    fXaxis = (TAxis*) h->GetXaxis()->Clone();
    fNbins = fXaxis->GetNbins();
    fA     = new Double_t[fNbins + 1];
    fB     = new Double_t[fNbins + 1];
    fC     = new Double_t[fNbins + 1];
    fAe    = new Double_t[fNbins + 1];
    fBe    = new Double_t[fNbins + 1];
    SetFirstBin(h, begval, (Bool_t) begval);
    SetLastBin(h, endval, (Bool_t) endval);
    for (int i = 2; i < fXaxis->GetNbins(); i++) {
      Double_t x1 = h->GetBinCenter(i - 1);
      Double_t x2 = h->GetBinCenter(i);
      Double_t x3 = h->GetBinCenter(i + 1);
      Double_t y1 = h->GetBinContent(i - 1);
      Double_t y2 = h->GetBinContent(i);
      Double_t y3 = h->GetBinContent(i + 1);
      Double_t A, B, C;
      CalcParams(x1, y1, x2, y2, x3, y3, A, B, C);
      fA[i]  = A;
      fB[i]  = B;
      fC[i]  = C;
      fAe[i] = TMath::Abs(A * h->GetBinContent(i) / h->GetBinError(i));
    }
  }

  Double_t Spline1D::Eval(Double_t x) const {
    Int_t bin = fXaxis->FindBin(x);
    return fA[bin] + fB[bin] * x + fC[bin] * x * x;
  }

  Double_t Spline1D::GetError(Double_t x) const {
    Int_t bin = fXaxis->FindBin(x);
    return fAe[bin];
  }

  Double_t Spline1D::EvalBin(Int_t bin) const {
    Double_t x = fXaxis->GetBinCenter(bin);
    return fA[bin] + fB[bin] * x + fC[bin] * x * x;
  }

  Double_t Spline1D::ErrorBin(Int_t bin) const { return fAe[bin]; }

  void Spline1D::Eval(Double_t x, Double_t& f, Double_t& error) const {
    Int_t bin = fXaxis->FindBin(x);
    f         = fA[bin] + fB[bin] * x + fC[bin] * x * x;
    error     = fAe[bin];
  }

  void Spline1D::CalcParams(Double_t x1,
                            Double_t y1,
                            Double_t x2,
                            Double_t y2,
                            Double_t x3,
                            Double_t y3,
                            Double_t& A,
                            Double_t& B,
                            Double_t& C) {
    TMatrixD AM(3, 3);
    TMatrixD BM(3, 1);
    AM[0][0] = 1;
    AM[0][1] = x1;
    AM[0][2] = x1 * x1;
    AM[1][0] = 1;
    AM[1][1] = x2;
    AM[1][2] = x2 * x2;
    AM[2][0] = 1;
    AM[2][1] = x3;
    AM[2][2] = x3 * x3;

    BM[0][0] = y1;
    BM[1][0] = y2;
    BM[2][0] = y3;

    TMatrixD SOL(3, 1);
    AM = AM.InvertFast();

    SOL = AM * BM;

    A = SOL[0][0];
    B = SOL[1][0];
    C = SOL[2][0];
  }

  void Spline1D::Refit() {
    Double_t* tempA = new Double_t[fNbins + 1];
    Double_t* tempB = new Double_t[fNbins + 1];
    Double_t* tempC = new Double_t[fNbins + 1];
    for (int i = 1; i <= fNbins; i++) {
      Double_t bin_width = fXaxis->GetBinWidth(i);
      Double_t x2        = fXaxis->GetBinCenter(i);
      Double_t x1        = x2 - bin_width * 0.5;
      Double_t x3        = x2 + bin_width * 0.5;
      Double_t y2        = Eval(x2);
      Double_t epsilon   = bin_width * 0.001;
      Double_t y1_p      = Eval(x1 + epsilon);
      Double_t y1_m      = Eval(x1 - epsilon);
      if (i == 1) { y1_m = y1_p = Eval(x1 - bin_width * 0.5); }
      Double_t y3_p = Eval(x3 + epsilon);
      Double_t y3_m = Eval(x3 - epsilon);
      if (i == fNbins) { y3_p = y3_m = Eval(x3); }
      Double_t y1 = (y1_p + y1_m) * 0.5;
      Double_t y3 = (y3_p + y3_m) * 0.5;
      Double_t A, B, C;
      CalcParams(x1, y1, x2, y2, x3, y3, A, B, C);
      tempA[i] = A;
      tempB[i] = B;
      tempC[i] = C;
    }
    for (int i = 0; i <= fNbins; i++) {
      fA[i] = tempA[i];
      fB[i] = tempB[i];
      fC[i] = tempC[i];
    }
    delete[] tempA;
    delete[] tempB;
    delete[] tempC;
  }

  void Spline1D::SetFirstBin(TH1* h, Double_t begval, Bool_t use_begval) {
    Double_t y2 = h->GetBinContent(1);
    Double_t x2 = h->GetBinCenter(1);
    Double_t x1, x3;
    Double_t y1, y3;
    x1 = x2 - h->GetBinWidth(1) * 0.5 - h->GetBinWidth(2) * 0.5;
    x3 = h->GetBinCenter(2);
    y3 = h->GetBinContent(2);
    if (use_begval) {
      y1 = begval;
    } else {
      y1 = y2 - (y3 - y2);
    }
    Double_t A, B, C;
    CalcParams(x1, y1, x2, y2, x3, y3, A, B, C);
    fA[1]  = A;
    fB[1]  = B;
    fC[1]  = C;
    fAe[1] = TMath::Abs(A * h->GetBinContent(1) / h->GetBinError(1));
  }

  void Spline1D::SetLastBin(TH1* h, Double_t endval, Bool_t use_endval) {
    Double_t x2 = h->GetBinCenter(fNbins);
    Double_t y2 = h->GetBinContent(fNbins);
    Double_t y1 = h->GetBinContent(fNbins - 1);
    Double_t x1, x3, y3;
    if (use_endval) {
      x1 = h->GetBinCenter(fNbins - 1);
      x3 = h->GetXaxis()->GetBinUpEdge(fNbins);
      y3 = endval;
    } else {
      x1 = h->GetBinCenter(fNbins - 1);
      x3 = x2 + h->GetBinWidth(fNbins) * 0.5 + h->GetBinWidth(fNbins - 1) * 0.5;
      y3 = y1 + (y2 - y1);
    }
    Double_t A, B, C;
    CalcParams(x1, y1, x2, y2, x3, y3, A, B, C);
    fA[fNbins]  = A;
    fB[fNbins]  = B;
    fC[fNbins]  = C;
    fAe[fNbins] = TMath::Abs(A * h->GetBinContent(fNbins) / h->GetBinError(fNbins));
  }

  TH1D* Spline1D::GetTHD(TString name) const {
    TH1D* res = new TH1D(name, name, fXaxis->GetNbins(), fXaxis->GetXmin(), fXaxis->GetXmax());
    res->SetXTitle(fXaxis->GetTitle());
    for (int i = 0; i <= fXaxis->GetNbins() + 2; i++) {
      Double_t x = fXaxis->GetBinCenter(i);
      //	Double_t y =  fA[i]+fB[i]*x+fC[i]*x*x;
      res->SetBinContent(i, Eval(x));
      res->SetBinError(i, GetError(x));
    }
    return res;
  }

  Spline1D::~Spline1D() {
    if (fXaxis) {
      delete fXaxis;
      delete[] fA;
      delete[] fB;
      delete[] fAe;
      delete[] fC;
    }
  }

  Spline2D::Spline2D(TH2* h, Option_t* opt_int) : fXaxis(NULL), fYaxis(NULL), fNbinsX(0), fNbinsY(0), fA(), fAe(NULL) {
    if (h == nullptr) {
      Hal::Cout::Text("making copy Spline with null histogram");
      return;
    }
    fXaxis  = (TAxis*) h->GetXaxis()->Clone("Xspline");
    fYaxis  = (TAxis*) h->GetYaxis()->Clone("Yspline");
    fNbinsX = fXaxis->GetNbins();
    fNbinsY = fYaxis->GetNbins();
    fAe     = new Array_2<Double_t>();
    fA.MakeBigger(fXaxis->GetNbins() + 2, fYaxis->GetNbins() + 2, 9);
    fAe->MakeBigger(fXaxis->GetNbins() + 2, fYaxis->GetNbins() + 2);
    if (fXaxis->IsVariableBinSize() && fYaxis->IsVariableBinSize()) {
      Cout::PrintInfo("Bot axes have non-fixed bin size 2dim interpolation will probably not "
                      "work",
                      Hal::EInfo::kWarning);
    }
    TH2* temp_histo = (TH2*) h->Clone("temp_interpolation");
    Extrapolate(temp_histo, opt_int);
    Double_t Params[9];
    for (int i = 1; i <= fNbinsX; i++) {
      Double_t x[3];
      x[0] = fXaxis->GetBinCenter(i - 1);
      x[1] = fXaxis->GetBinCenter(i);
      x[2] = fXaxis->GetBinCenter(i + 1);
      for (int j = 1; j <= fYaxis->GetNbins(); j++) {
        Double_t y[3];
        y[0] = fYaxis->GetBinCenter(j - 1);
        y[1] = fYaxis->GetBinCenter(j);
        y[2] = fYaxis->GetBinCenter(j + 1);
        Double_t z[3][3];
        for (int a = -1; a < 2; a++)
          for (int b = -1; b < 2; b++)
            z[a + 1][b + 1] = temp_histo->GetBinContent(i + a, j + b);
        CalcParams(x, y, z, Params);
        (*fAe)[i][j] = temp_histo->GetBinError(i, j);
        for (int p = 0; p < 9; p++)
          fA[i][j][p] = Params[p];
      }
    }
    // "fix edges" (but no corners)
    for (int i = 1; i <= fNbinsX; i++) {
      for (int p = 1; p < 9; p++) {
        fA[i][0][p]       = 0;
        fA[i][fNbinsY][p] = 0;
      }
      fA[i][0][0]       = fA[i][1][0];
      fA[i][fNbinsY][0] = fA[i][fNbinsY - 1][0];
    }

    for (int i = 1; i <= fNbinsY; i++) {
      for (int p = 1; p < 9; p++) {
        fA[0][i][p]       = 0;
        fA[fNbinsX][i][p] = 0;
      }
      fA[0][i][0]       = fA[1][i][0];
      fA[fNbinsX][i][0] = fA[fNbinsX - 1][i][0];
    }
    delete temp_histo;
  }

  void Spline2D::Refit() {
    Double_t Params[9];
    Array_3<Double_t> tempA(fXaxis->GetNbins() + 2, fYaxis->GetNbins() + 2, 9);
    for (int i = 1; i <= fNbinsX; i++) {
      Double_t x[3];
      Double_t dx = fXaxis->GetBinWidth(i) * 0.5;
      x[0]        = fXaxis->GetBinCenter(i) - dx;
      x[1]        = fXaxis->GetBinCenter(i);
      x[2]        = fXaxis->GetBinCenter(i) + dx;

      Double_t ex  = dx * 0.01;
      Double_t x0p = x[0] + ex;
      Double_t x0m = x[0] - ex;
      Double_t x2p = x[2] + ex;
      Double_t x2m = x[2] - ex;
      Double_t X   = x[1];

      for (int j = 1; j <= fYaxis->GetNbins(); j++) {
        Double_t y[3];
        Double_t dy = fYaxis->GetBinWidth(j) * 0.5;
        Double_t ey = dy * 0.01;
        y[0]        = fYaxis->GetBinCenter(j) - dy;
        y[1]        = fYaxis->GetBinCenter(j);
        y[2]        = fYaxis->GetBinCenter(j) + dy;

        Double_t y0p = y[0] + ey;
        Double_t y0m = y[0] - ey;
        Double_t y2p = y[2] + ey;
        Double_t y2m = y[2] - ey;
        Double_t Y   = y[1];

        Double_t z[3][3];
        z[0][0] = 0.25 * (Eval(x0m, y0m) + Eval(x0p, y0m) + Eval(x0m, y0p) + Eval(x0p, y0p));
        z[0][1] = 0.5 * (Eval(x0p, Y) + Eval(x0m, Y));
        z[0][2] = 0.25 * (Eval(x0m, y2m) + Eval(x0p, y2m) + Eval(x0m, y2p) + Eval(x0p, y2p));

        z[1][0] = 0.5 * (Eval(X, y0m) + Eval(X, y0p));
        z[1][1] = Eval(X, Y);
        z[1][2] = 0.5 * (Eval(X, y2m) + Eval(X, y2p));

        z[2][0] = 0.25 * (Eval(x2m, y0p) + Eval(x2m, y0m) + Eval(x2p, y0p) + Eval(x2p, y0m));
        z[2][1] = 0.5 * (Eval(x2p, Y) + Eval(x2m, Y));
        z[2][2] = 0.25 * (Eval(x2m, y2m) + Eval(x2p, y2m) + Eval(x2m, y2p) + Eval(x2p, y2p));

        CalcParams(x, y, z, Params);
        for (int p = 0; p < 9; p++) {
          tempA[i][j][p] = Params[p];
        }
      }
    }
    fA = tempA;
  }

  Double_t Spline2D::Eval(Double_t x, Double_t y) const {
    Int_t ix     = fXaxis->FindFixBin(x);
    Int_t iy     = fYaxis->FindFixBin(y);
    Double_t x2  = x * x;
    Double_t y2  = y * y;
    Double_t res = fA.Get(ix, iy, 0) + fA.Get(ix, iy, 1) * x + fA.Get(ix, iy, 2) * y + fA.Get(ix, iy, 3) * x * y
                   + fA.Get(ix, iy, 4) * x2 + fA.Get(ix, iy, 5) * y2 + fA.Get(ix, iy, 6) * x2 * y + fA.Get(ix, iy, 7) * x * y2
                   + fA.Get(ix, iy, 8) * x2 * y2;
#ifdef SPLINE_DEBUG
    if (TMath::IsNaN(res))
      std::cout << "=>" << res << "(" << x << "," << y << ")\t" << fA.Get(ix, iy, 0) << " " << fA.Get(ix, iy, 1) * x << " "
                << fA.Get(ix, iy, 2) * y + fA.Get(ix, iy, 3) * x * y << " " << fA.Get(ix, iy, 4) * x2 << " "
                << fA.Get(ix, iy, 5) * y2 + fA.Get(ix, iy, 6) * x2 * y << " " << fA.Get(ix, iy, 7) * x * y2 << " "
                << fA.Get(ix, iy, 8) * x2 * y2 << std::endl;
#endif
    return res;
    return fA.Get(ix, iy, 0) + fA.Get(ix, iy, 1) * x + fA.Get(ix, iy, 2) * y + fA.Get(ix, iy, 3) * x * y + fA.Get(ix, iy, 4) * x2
           + fA.Get(ix, iy, 5) * y2 + fA.Get(ix, iy, 6) * x2 * y + fA.Get(ix, iy, 7) * x * y2 + fA.Get(ix, iy, 8) * x2 * y2;
  }

  Double_t Spline2D::EvalBin(Int_t ix, Int_t iy) const {
    Double_t x  = fXaxis->GetBinCenter(ix);
    Double_t y  = fYaxis->GetBinCenter(iy);
    Double_t x2 = x * x;
    Double_t y2 = y * y;
    return fA.Get(ix, iy, 0) + fA.Get(ix, iy, 1) * x + fA.Get(ix, iy, 2) * y + fA.Get(ix, iy, 3) * x * y + fA.Get(ix, iy, 4) * x2
           + fA.Get(ix, iy, 5) * y2 + fA.Get(ix, iy, 6) * x2 * y + fA.Get(ix, iy, 7) * x * y2 + fA.Get(ix, iy, 8) * x2 * y2;
  }

  Double_t Spline2D::ErrorBin(Int_t bin_x, Int_t bin_y) const { return fAe->Get(bin_x, bin_y); }

  void Spline2D::Eval(Double_t x, Double_t y, Double_t& f, Double_t& error) const {
    Int_t ix    = fXaxis->FindFixBin(x);
    Int_t iy    = fYaxis->FindFixBin(y);
    Double_t x2 = x * x;
    Double_t y2 = y * y;
    f = fA.Get(ix, iy, 0) + fA.Get(ix, iy, 1) * x + fA.Get(ix, iy, 2) * y + fA.Get(ix, iy, 3) * x * y + fA.Get(ix, iy, 4) * x2
        + fA.Get(ix, iy, 5) * y2 + fA.Get(ix, iy, 6) * x2 * y + fA.Get(ix, iy, 7) * x * y2 + fA.Get(ix, iy, 8) * x2 * y2;
    error = fAe->Get(ix, iy);
  }

  void Spline2D::CalcParams(Double_t x[3], Double_t y[3], Double_t z[3][3], Double_t params[9]) {
    TMatrixD AM(9, 9);
    TMatrixD BM(9, 1);
    for (int i = 0; i < 9; i++) {
      Int_t xi = i % 3;
      Int_t yi = 0;
      if (i >= 3) yi = 1;
      if (i >= 6) yi = 2;
      Double_t X = x[xi];
      Double_t Y = y[yi];
      AM[i][0]   = 1;              // A
      AM[i][1]   = X;              // B
      AM[i][2]   = Y;              // C
      AM[i][3]   = X * Y;          // D
      AM[i][4]   = X * X;          // E
      AM[i][5]   = Y * Y;          // F
      AM[i][6]   = X * X * Y;      // G
      AM[i][7]   = X * Y * Y;      // H
      AM[i][8]   = X * X * Y * Y;  // I
      BM[i][0]   = z[xi][yi];
    }

    TMatrixD SOL(9, 1);
    AM  = AM.Invert();
    SOL = AM * BM;

    for (int i = 0; i < 9; i++) {
      params[i] = SOL[i][0];
    }
  }

  void Spline2D::PrintParams(Double_t x, Double_t y) {
    std::cout << "PARAMS" << x << " " << y << std::endl;
    Int_t ix = fXaxis->FindFixBin(x);
    Int_t iy = fYaxis->FindFixBin(y);
    for (int i = 0; i < 9; i++) {
      std::cout << fA[ix][iy][i] << std::endl;
    }
  }

  void Spline2D::Extrapolate(TH2* h, Option_t* interpolation) const {
    TString option          = interpolation;
    Int_t interpolation_opt = 0;
    if (option.EqualTo("linear")) {
      interpolation_opt = 1;
    } else if (option.EqualTo("const")) {
      interpolation_opt = 2;
    } else {
      return;
    }
    for (int i = 0; i <= fNbinsX + 1; i++) {  // last row & first
      h->SetBinContent(i, 0, Extrapolate(h, i, i, i, 1, 2, 0, interpolation_opt));
      h->SetBinError(i, 0, h->GetBinError(i, 1));
      h->SetBinContent(i, fNbinsY + 1, Extrapolate(h, i, i, i, fNbinsY - 1, fNbinsY, fNbinsY + 1, interpolation_opt));
      h->SetBinError(i, fNbinsY + 1, h->GetBinError(i, fNbinsY));
    }
    for (int i = 0; i <= fNbinsY + 1; i++) {  // last column & first
      h->SetBinContent(0, i, Extrapolate(h, 1, 2, 0, i, i, i, interpolation_opt));
      h->SetBinError(0, i, h->GetBinError(1, i));
      h->SetBinContent(fNbinsX + 1, i, Extrapolate(h, fNbinsX - 1, fNbinsX, fNbinsX + 1, i, i, i, interpolation_opt));
      h->SetBinError(fNbinsX + 1, i, h->GetBinError(fNbinsX, i));
    }
    h->SetBinContent(0, 0, Extrapolate(h, 1, 2, 0, 1, 2, 0, interpolation_opt));  //->00
    h->SetBinError(0, 0, h->GetBinError(1, 1));
    h->SetBinContent(fNbinsX + 1, 0, Extrapolate(h, fNbinsX - 1, fNbinsX, fNbinsX + 1, 1, 2, 0,
                                                 interpolation_opt));  //->L0
    h->SetBinError(fNbinsX + 1, 0, h->GetBinError(fNbinsX, 1));
    h->SetBinContent(0, fNbinsY + 1, Extrapolate(h, 1, 2, 0, fNbinsY - 1, fNbinsY, fNbinsY + 1,
                                                 interpolation_opt));  //->0L
    h->SetBinError(0, fNbinsY + 1, h->GetBinError(1, fNbinsY));
    h->SetBinContent(
      fNbinsX + 1,
      fNbinsY + 1,
      Extrapolate(h, fNbinsX - 1, fNbinsX, fNbinsX + 1, fNbinsY - 1, fNbinsY, fNbinsY + 1, interpolation_opt));  //->LL
    h->SetBinError(fNbinsX + 1, fNbinsY + 1, h->GetBinError(fNbinsX, fNbinsY));
  }

  Double_t Spline2D::GetExtrapolationParam(Double_t x, Double_t y, Int_t param) const {
    Int_t ix = fXaxis->FindFixBin(x);
    Int_t iy = fYaxis->FindFixBin(y);
    return fA.Get(ix, iy, param);
  }

  Double_t Spline2D::Extrapolate(TH2* h, Int_t ix1, Int_t ix2, Int_t ix, Int_t iy1, Int_t iy2, Int_t iy, Int_t opt) const {
    Double_t z1, z2, dS, dI, Z;
    Double_t dX     = h->GetXaxis()->GetBinCenter(ix2) - h->GetXaxis()->GetBinCenter(ix1);
    Double_t dY     = h->GetYaxis()->GetBinCenter(iy2) - h->GetYaxis()->GetBinCenter(iy1);
    dS              = TMath::Sqrt(dX * dX + dY * dY);
    Int_t closest_x = ix1;
    Int_t closest_y = iy1;
    if (TMath::Abs(ix1 - ix) > TMath::Abs(ix2 - ix)) closest_x = ix2;
    if (TMath::Abs(iy1 - iy) > TMath::Abs(iy2 - iy)) closest_y = iy2;
    dX = h->GetXaxis()->GetBinCenter(closest_x) - h->GetXaxis()->GetBinCenter(ix);
    dY = h->GetYaxis()->GetBinCenter(closest_y) - h->GetYaxis()->GetBinCenter(iy);
    dI = TMath::Sqrt(dX * dX + dY * dY);
    z1 = h->GetBinContent(ix1, iy1);
    z2 = h->GetBinContent(ix2, iy2);
    switch (opt) {
      case 1: {
        Double_t dZ = (z2 - z1) * dI / dS;
        if (ix < ix1 || iy < iy1) {
          Z = h->GetBinContent(ix1, iy1);
          return Z - dZ;
        } else {
          Z = h->GetBinContent(ix2, iy2);
          return Z + dZ;
        }
      } break;
      case 2: return h->GetBinContent(closest_x, closest_y); break;
      default: return 0; break;
    }
  }

  Double_t Spline2D::GetError(Double_t x, Double_t y) const {
    Int_t ix = fXaxis->FindFixBin(x);
    Int_t iy = fYaxis->FindFixBin(y);
    return fAe->Get(ix, iy);
  }

  Spline2D::~Spline2D() {}

  void Spline1D::FastOverwrite(TH1D* h, Double_t begval, Double_t endval) {
    SetFirstBin(h, begval, (Bool_t) begval);
    SetLastBin(h, endval, (Bool_t) endval);
    for (int i = 2; i < fXaxis->GetNbins(); i++) {
      Double_t x1 = h->GetBinCenter(i - 1);
      Double_t x2 = h->GetBinCenter(i);
      Double_t x3 = h->GetBinCenter(i + 1);
      Double_t y1 = h->GetBinContent(i - 1);
      Double_t y2 = h->GetBinContent(i);
      Double_t y3 = h->GetBinContent(i + 1);
      Double_t A, B, C;
      CalcParams(x1, y1, x2, y2, x3, y3, A, B, C);
      fA[i]  = A;
      fB[i]  = B;
      fC[i]  = C;
      fAe[i] = TMath::Abs(A * h->GetBinContent(i) / h->GetBinError(i));
    }
  }

  void Spline2D::SetPoint(Int_t xbin, Int_t ybin, Double_t val, Double_t error) {
    if (xbin == 0 || ybin == 0 || xbin == fXaxis->GetNbins() || ybin == fYaxis->GetNbins()) {  //  this is edge reset parameters
      fA[xbin][ybin][0] = val;
      fAe[xbin][ybin]   = error;
      for (int i = 1; i < 9; i++) {
        fA[xbin][ybin][i] = 0;
      }
    }
  }

  void Spline3D::CalcParams(Double_t x[2], Double_t y[2], Double_t z[2], Double_t v[2][2][2], Double_t params[8]) {
    TMatrixD AM(8, 8);
    TMatrixD BM(8, 1);
    Int_t IX[8] = {0, 1, 0, 1, 0, 1, 0, 1};
    Int_t IY[8] = {0, 0, 1, 1, 0, 0, 1, 1};
    Int_t IZ[8] = {0, 0, 0, 0, 1, 1, 1, 1};
    for (int i = 0; i < 8; i++) {
      Int_t ix   = IX[i];
      Int_t iy   = IY[i];
      Int_t iz   = IZ[i];
      Double_t X = x[ix];
      Double_t Y = y[iy];
      Double_t Z = z[iz];
      AM[i][0]   = 1;
      AM[i][1]   = X;
      AM[i][2]   = Y;
      AM[i][3]   = Z;
      AM[i][4]   = X * Y;
      AM[i][5]   = X * Z;
      AM[i][6]   = Y * Z;
      AM[i][7]   = X * Y * Z;
      BM[i][0]   = v[ix][iy][iz];
    }
    TMatrixD SOL(8, 1);
    AM  = AM.Invert();
    SOL = AM * BM;

    for (int i = 0; i < 8; i++) {
      params[i] = SOL[i][0];
    }
  }

  Spline3D::Spline3D(TH3* h, Option_t* interpolation) {
    Bool_t mid = kFALSE;
    fXaxis     = (TAxis*) h->GetXaxis()->Clone();
    fYaxis     = (TAxis*) h->GetYaxis()->Clone();
    fZaxis     = (TAxis*) h->GetZaxis()->Clone();
    fNbinsX    = fXaxis->GetNbins();
    fNbinsY    = fYaxis->GetNbins();
    fNbinsZ    = fZaxis->GetNbins();
    fA.MakeBigger(fNbinsX + 2, fNbinsY + 2, fNbinsZ + 2, 8);
    fAe.MakeBigger(fNbinsX + 2, fNbinsY + 2, fNbinsZ + 2);
    TString opt      = interpolation;
    Int_t fix_method = 0;
    if (Hal::Std::FindParam(opt, "yes") || Hal::Std::FindParam(opt, "fix1")) { fix_method = 1; }
    if (Hal::Std::FindParam(opt, "nointerpolation")) {
      for (int i = 0; i <= fNbinsX + 1; i++) {
        for (int j = 0; j <= fNbinsY + 1; j++) {
          for (int k = 0; k <= fNbinsZ + 1; k++) {
            for (int a = 1; a < 8; a++) {
              fA[i][j][k][a] = 0;
            }
            fA[i][j][k][0] = h->GetBinContent(i, j, k);
          }
        }
      }
      return;
    }
    if (Hal::Std::FindParam(opt, "mid")) { mid = kTRUE; }
    Double_t x[2], y[2], z[2];
    Double_t v[2][2][2];
    Double_t params[8];
    Int_t idx[2] = {-1, 1};
    for (int i = 1; i <= fNbinsX; i++) {
      x[0] = fXaxis->GetBinLowEdge(i);
      x[1] = fXaxis->GetBinUpEdge(i);
      for (int j = 1; j <= fNbinsY; j++) {
        y[0] = fYaxis->GetBinLowEdge(j);
        y[1] = fYaxis->GetBinUpEdge(j);
        for (int k = 1; k <= fNbinsZ; k++) {
          z[0]       = fZaxis->GetBinLowEdge(k);
          z[1]       = fZaxis->GetBinUpEdge(k);
          Double_t C = h->GetBinContent(i, j, k);
          switch (fix_method) {
            case 0:  // do nothing
            {
              for (int a = 0; a < 2; a++) {
                Int_t I = i + idx[a];
                for (int b = 0; b < 2; b++) {
                  Int_t J = j + idx[b];
                  for (int c = 0; c < 2; c++) {
                    Int_t K    = k + idx[c];
                    v[a][b][c] = (C + h->GetBinContent(I, J, K)) * 0.5;
                  }
                }
              }
            } break;
            case 1:  // mean method
            {
              for (int a = 0; a < 2; a++) {
                Int_t I = i + idx[a];
                if (I == 0) I++;
                if (I == fNbinsX + 1) I--;
                for (int b = 0; b < 2; b++) {
                  Int_t J = j + idx[b];
                  if (J == 0) J++;
                  if (J == fNbinsY + 1) J--;
                  for (int c = 0; c < 2; c++) {
                    Int_t K = k + idx[c];
                    if (K == 0) K++;
                    if (K == fNbinsZ + 1) K--;
                    v[a][b][c] = 0.5 * (C + h->GetBinContent(I, J, K));
                  }
                }
              }
            } break;
            case 2:  // copy method
            {
              // TODO implement
            } break;
          }

          CalcParams(x, y, z, v, params);
          for (int a = 0; a < 8; a++) {
            fA[i][j][k][a] = params[a];
          }
          if (mid) {
            Double_t eval = Eval(0.5 * (x[0] + x[1]), 0.5 * (y[0] + y[1]), 0.5 * (z[0] + z[1]));
            Double_t val  = h->GetBinContent(i, j, k);
            fA[i][j][k][0] += val - eval;
          }
          fAe[i][j][k] = h->GetBinError(i, j, k);
        }
      }
    }
  }

  Double_t Spline3D::Eval(Double_t x, Double_t y, Double_t z) const {
    Int_t ix = fXaxis->FindFixBin(x);
    Int_t iy = fYaxis->FindFixBin(y);
    Int_t iz = fZaxis->FindFixBin(z);
    return fA.Get(ix, iy, iz, 0) + fA.Get(ix, iy, iz, 1) * x + fA.Get(ix, iy, iz, 2) * y + fA.Get(ix, iy, iz, 3) * z
           + fA.Get(ix, iy, iz, 4) * x * y + fA.Get(ix, iy, iz, 5) * x * z + fA.Get(ix, iy, iz, 6) * y * z
           + fA.Get(ix, iy, iz, 7) * x * y * z;
  }

  Double_t Spline3D::GetError(Double_t x, Double_t y, Double_t z) const {
    Int_t ix = fXaxis->FindFixBin(x);
    Int_t iy = fYaxis->FindFixBin(y);
    Int_t iz = fZaxis->FindFixBin(z);
    return fAe.Get(ix, iy, iz);
  }

  void Spline3D::PrintParams(Double_t x, Double_t y, Double_t z) const {
    Int_t ix = fXaxis->FindFixBin(x);
    Int_t iy = fYaxis->FindFixBin(y);
    Int_t iz = fZaxis->FindFixBin(z);
    for (int i = 0; i < 8; i++)
      std::cout << "Par " << i << " " << fA.Get(ix, iy, iz, i) << std::endl;
  }

  void Spline3D::Eval(Double_t x, Double_t y, Double_t z, Double_t& f, Double_t& error) const {
    Int_t ix = fXaxis->FindFixBin(x);
    Int_t iy = fYaxis->FindFixBin(y);
    Int_t iz = fZaxis->FindFixBin(z);
    f        = fA.Get(ix, iy, iz, 0) + fA.Get(ix, iy, iz, 1) * x + fA.Get(ix, iy, iz, 2) * y + fA.Get(ix, iy, iz, 3) * z
        + fA.Get(ix, iy, iz, 4) * x * y + fA.Get(ix, iy, iz, 5) * x * z + fA.Get(ix, iy, iz, 6) * y * z
        + fA.Get(ix, iy, iz, 7) * x * y * z;
    error = fAe.Get(ix, iy, iz);
  }

  Double_t Spline3D::EvalBin(Int_t ix, Int_t iy, Int_t iz) const {
    Double_t x = fXaxis->GetBinCenter(ix);
    Double_t y = fYaxis->GetBinCenter(iy);
    Double_t z = fZaxis->GetBinCenter(iz);
    return fA.Get(ix, iy, iz, 0) + fA.Get(ix, iy, iz, 1) * x + fA.Get(ix, iy, iz, 2) * y + fA.Get(ix, iy, iz, 3) * z
           + fA.Get(ix, iy, iz, 4) * x * y + fA.Get(ix, iy, iz, 5) * x * z + fA.Get(ix, iy, iz, 6) * y * z
           + fA.Get(ix, iy, iz, 7) * x * y * z;
  }

  Double_t Spline3D::ErrorBin(Int_t ix, Int_t iy, Int_t iz) const { return fAe.Get(ix, iy, iz); }

  Spline3D::~Spline3D() {
    if (fXaxis) {
      delete fXaxis;
      delete fYaxis;
      delete fZaxis;
    }
  }

  TH2D* Spline2D::GetTHD(TString name) const {
    TH2D* res = new TH2D(name,
                         name,
                         fXaxis->GetNbins(),
                         fXaxis->GetXmin(),
                         fXaxis->GetXmax(),
                         fYaxis->GetNbins(),
                         fYaxis->GetXmin(),
                         fYaxis->GetXmax());
    res->SetXTitle(fXaxis->GetTitle());
    res->SetYTitle(fYaxis->GetTitle());

    for (int i = 0; i <= fXaxis->GetNbins() + 1; i++) {
      Double_t x = fXaxis->GetBinCenter(i);
      for (int j = 0; j <= fYaxis->GetNbins() + 1; j++) {
        Double_t y = fYaxis->GetBinCenter(j);
        res->SetBinContent(i, j, Eval(x, y));
        res->SetBinError(i, j, GetError(x, y));
      }
    }
    return res;
  }

  TH3D* Spline3D::GetTHD(TString name) const {
    TH3D* res = new TH3D(name,
                         name,
                         fXaxis->GetNbins(),
                         fXaxis->GetXmin(),
                         fXaxis->GetXmax(),
                         fYaxis->GetNbins(),
                         fYaxis->GetXmin(),
                         fYaxis->GetXmax(),
                         fZaxis->GetNbins(),
                         fZaxis->GetXmin(),
                         fZaxis->GetXmax());
    res->SetXTitle(fXaxis->GetTitle());
    res->SetYTitle(fYaxis->GetTitle());
    res->SetZTitle(fZaxis->GetTitle());
    for (int i = 0; i <= fXaxis->GetNbins() + 1; i++) {
      Double_t x = fXaxis->GetBinCenter(i);
      for (int j = 0; j <= fYaxis->GetNbins() + 1; j++) {
        Double_t y = fYaxis->GetBinCenter(j);
        for (int k = 0; k <= fZaxis->GetNbins() + 1; k++) {
          Double_t z = fZaxis->GetBinCenter(k);
          res->SetBinContent(i, j, Eval(x, y, z));
          res->SetBinError(i, j, k, GetError(x, y, z));
        }
      }
    }
    return res;
  }

  void Spline3D::Refit() {
    Array_4<Double_t> ANew;
    ANew.MakeBigger(fNbinsX + 2, fNbinsY + 2, fNbinsZ + 2, 8);
    Double_t x[2], y[2], z[2];
    Double_t v[2][2][2];
    Double_t params[8];

    for (int i = 1; i <= fNbinsX; i++) {
      x[0]        = fXaxis->GetBinLowEdge(i);
      x[1]        = fXaxis->GetBinUpEdge(i);
      Double_t dx = fXaxis->GetBinWidth(i) * 0.1;
      for (int j = 1; j <= fNbinsY; j++) {
        y[0]        = fYaxis->GetBinLowEdge(j);
        y[1]        = fYaxis->GetBinUpEdge(j);
        Double_t dy = fYaxis->GetBinWidth(j) * 0.1;
        for (int k = 1; k <= fNbinsZ; k++) {
          z[0]        = fZaxis->GetBinLowEdge(k);
          z[1]        = fZaxis->GetBinUpEdge(k);
          Double_t dz = fZaxis->GetBinWidth(k) * 0.1;
          for (int a = 0; a < 2; a++) {
            for (int b = 0; b < 2; b++) {
              for (int c = 0; c < 2; c++) {
                v[a][b][c]   = 0;
                Double_t sum = 0;

                // calcuate "averaged corner"
                Int_t D[2], E[2], F[2];
                D[0] = fXaxis->FindBin(x[a] - dx);
                D[1] = fXaxis->FindBin(x[a] + dx);
                D[0] = TMath::Max(1, D[0]);
                D[1] = TMath::Min(fNbinsX, D[1]);

                E[0] = fYaxis->FindBin(y[b] - dy);
                E[1] = fYaxis->FindBin(y[b] + dy);
                E[0] = TMath::Max(1, E[0]);
                E[1] = TMath::Min(fNbinsY, E[1]);

                F[0] = fZaxis->FindBin(z[c] - dz);
                F[1] = fZaxis->FindBin(z[c] + dz);
                F[0] = TMath::Max(1, F[0]);
                F[1] = TMath::Min(fNbinsZ, F[1]);

                for (int d = 0; d < 2; d++) {
                  for (int e = 0; e < 2; e++) {
                    for (int f = 0; f < 2; f++) {
                      sum += EvalBin(D[d], E[e], F[f], x[a], y[b], z[c]);
                    }
                  }
                }

                v[a][b][c] = sum * 0.125;
              }
            }
          }
          CalcParams(x, y, z, v, params);
          for (int a = 0; a < 8; a++) {
            ANew[i][j][k][a] = params[a];
          }
        }
      }
    }
    fA = ANew;
  }

  Double_t Spline3D::EvalBin(Int_t ix, Int_t iy, Int_t iz, Double_t x, Double_t y, Double_t z) const {
    return fA.Get(ix, iy, iz, 0) + fA.Get(ix, iy, iz, 1) * x + fA.Get(ix, iy, iz, 2) * y + fA.Get(ix, iy, iz, 3) * z
           + fA.Get(ix, iy, iz, 4) * x * y + fA.Get(ix, iy, iz, 5) * x * z + fA.Get(ix, iy, iz, 6) * y * z
           + fA.Get(ix, iy, iz, 7) * x * y * z;
  }

  void Spline3D::FastOverwrite(TH3* h, Option_t* interpolation) {
    Bool_t mid       = kFALSE;
    TString opt      = interpolation;
    Int_t fix_method = 0;
    if (Hal::Std::FindParam(opt, "yes") || Hal::Std::FindParam(opt, "fix1")) { fix_method = 1; }
    if (Hal::Std::FindParam(opt, "nointerpolation")) {
      for (int i = 0; i <= fNbinsX + 1; i++) {
        for (int j = 0; j <= fNbinsY + 1; j++) {
          for (int k = 0; k <= fNbinsZ + 1; k++) {
            for (int a = 1; a < 8; a++) {
              fA[i][j][k][a] = 0;
            }
            fA[i][j][k][0] = h->GetBinContent(i, j, k);
          }
        }
      }
      return;
    }
    if (Hal::Std::FindParam(opt, "mid")) { mid = kTRUE; }
    Double_t x[2], y[2], z[2];
    Double_t v[2][2][2];
    Double_t params[8];
    Int_t idx[2] = {-1, 1};
    for (int i = 1; i <= fNbinsX; i++) {
      x[0] = fXaxis->GetBinLowEdge(i);
      x[1] = fXaxis->GetBinUpEdge(i);
      for (int j = 1; j <= fNbinsY; j++) {
        y[0] = fYaxis->GetBinLowEdge(j);
        y[1] = fYaxis->GetBinUpEdge(j);
        for (int k = 1; k <= fNbinsZ; k++) {
          z[0]       = fZaxis->GetBinLowEdge(k);
          z[1]       = fZaxis->GetBinUpEdge(k);
          Double_t C = h->GetBinContent(i, j, k);
          switch (fix_method) {
            case 0:  // do nothing
            {
              for (int a = 0; a < 2; a++) {
                Int_t I = i + idx[a];
                for (int b = 0; b < 2; b++) {
                  Int_t J = j + idx[b];
                  for (int c = 0; c < 2; c++) {
                    Int_t K    = k + idx[c];
                    v[a][b][c] = (C + h->GetBinContent(I, J, K)) * 0.5;
                  }
                }
              }
            } break;
            case 1:  // mean method
            {
              for (int a = 0; a < 2; a++) {
                Int_t I = i + idx[a];
                if (I == 0) I++;
                if (I == fNbinsX + 1) I--;
                for (int b = 0; b < 2; b++) {
                  Int_t J = j + idx[b];
                  if (J == 0) J++;
                  if (J == fNbinsY + 1) J--;
                  for (int c = 0; c < 2; c++) {
                    Int_t K = k + idx[c];
                    if (K == 0) K++;
                    if (K == fNbinsZ + 1) K--;
                    v[a][b][c] = 0.5 * (C + h->GetBinContent(I, J, K));
                  }
                }
              }
            } break;
            case 2:  // copy method
            {
              // TODO implement
            } break;
          }

          CalcParams(x, y, z, v, params);
          for (int a = 0; a < 8; a++) {
            fA[i][j][k][a] = params[a];
          }
          if (mid) {
            Double_t eval = Eval(0.5 * (x[0] + x[1]), 0.5 * (y[0] + y[1]), 0.5 * (z[0] + z[1]));
            Double_t val  = h->GetBinContent(i, j, k);
            fA[i][j][k][0] += val - eval;
          }
          fAe[i][j][k] = h->GetBinError(i, j, k);
        }
      }
    }
  }
}  // namespace Hal
