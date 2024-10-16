/*
 * Femto3DCFPainter.cxx
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#include "Femto3DCFPainter.h"

#include "Cout.h"
#include "Femto3DCF.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"

#include <TH3D.h>
#include <TVirtualPad.h>

namespace Hal {
  const int Femto3DCFPainter::kRgbBit        = 16;
  const int Femto3DCFPainter::kRawBit        = 17;
  const int Femto3DCFPainter::kDiag1Bit      = 18;
  const int Femto3DCFPainter::kDiag2Bit      = 19;
  const int Femto3DCFPainter::kTwoDimBit     = 20;
  const int Femto3DCFPainter::kTwoDimPlusBit = 21;
  const int Femto3DCFPainter::kAngles        = 22;
  ULong64_t Femto3DCFPainter::SetOptionInternal(TString opt, ULong64_t newFlags) {
    newFlags        = FemtoCFPainter::SetOptionInternal(opt, newFlags);
    auto cleanFlags = [&](int setbit) { ResetFewBits(newFlags, {kDiag1Bit, kDiag2Bit, kTwoDimBit, kTwoDimPlusBit}, setbit); };
    if (Hal::Std::FindParam(opt, "diag1")) {
      cleanFlags(kDiag1Bit);
      fDefDrawFlag = "SAME+P";
    };
    if (Hal::Std::FindParam(opt, "diag2")) {
      cleanFlags(kDiag2Bit);
      fDefDrawFlag = "SAME+P";
    };
    if (Hal::Std::FindParam(opt, "2d")) {
      cleanFlags(kTwoDimBit);
      fDefDrawFlag = "SAME+colz";
    };
    if (Hal::Std::FindParam(opt, "3d")) {
      cleanFlags(kTwoDimPlusBit);
      fDefDrawFlag = "SAME+surf1";
      auto bra     = Hal::Std::FindBrackets(opt, kTRUE, kTRUE);
      for (auto pat : bra) {
        std::vector<Double_t> vals;
        if (GetPatterns(pat, "ang", vals)) {
          SETBIT(newFlags, kAngles);
          if (vals.size() == 2) {
            fThetaPad = vals[0];
            fPhiPad   = vals[0];
          }
        }
      }
    };

    ContitionalPattern(opt, "hidetitles", newFlags, kHideTitles);
    return newFlags;
  }

  void Femto3DCFPainter::MakeHistograms() {
    TH3* h     = nullptr;
    bool clean = false;
    if (CheckOpt(kNumBit)) {
      h = (TH3*) fCF->GetNum()->Clone();
    } else if (CheckOpt(kDenBit)) {
      h = (TH3*) fCF->GetDen()->Clone();
    } else {  // cf
      h     = (TH3*) fCF->GetHist(kFALSE);
      clean = true;
    }
    if (CheckOpt(kDiag1Bit)) {
      PrepareDiagonal1(h);
    } else if (CheckOpt(kDiag2Bit)) {
      PrepareDiagonal2(h);
    } else if (CheckOpt(kHtmlBit)) {
      PrepareHtml(h);
    } else if (CheckOpt(kTwoDimBit) || CheckOpt(kTwoDimPlusBit)) {
      PrepareTwoDim(h);
    } else {  // standard
      if (AreSimiliar(GetDrawFlags(), PrepBitTemplate({kNumBit, kDenBit, kCFBit}))) {
        Prepare3DFull(h);
      } else {
        Prepare3D(h);
      }
    }
    if (clean) delete h;
  }

  Bool_t Femto3DCFPainter::CheckPads() const {
    Int_t req    = GetPadsRequired();
    Int_t padsNo = GetPadsNo();
    if (padsNo < req) return kFALSE;
    return kTRUE;
  };

  Femto3DCFPainter::Femto3DCFPainter(Femto3DCF* cf) : fCF(cf) {}

  void Femto3DCFPainter::Prepare3DFull(TH1* h) {
    Int_t middle_x[2];
    Int_t middle_y[2];
    Int_t middle_z[2];
    middle_x[0] = middle_x[1] = h->GetXaxis()->FindBin(0.0);
    middle_y[0] = middle_y[1] = h->GetYaxis()->FindBin(0.0);
    middle_z[0] = middle_z[1] = h->GetZaxis()->FindBin(0.0);
    auto frame                = fCF->GetFrame();
    if (frame == Femto::EKinematics::kSH_LCMS || frame == Femto::EKinematics::kSH_PRF) {
      middle_x[0] = middle_y[0] = middle_z[0] = 1;
      middle_x[1]                             = h->GetXaxis()->GetNbins();
      middle_y[1]                             = h->GetYaxis()->GetNbins();
      middle_z[1]                             = h->GetZaxis()->GetNbins();
    } else {
      if (h->GetXaxis()->GetBinLowEdge(middle_x[0]) == 0.0 && middle_x[0] > 1) --middle_x[0];
      if (h->GetYaxis()->GetBinLowEdge(middle_y[0]) == 0.0 && middle_y[0] > 1) --middle_y[0];
      if (h->GetZaxis()->GetBinLowEdge(middle_z[0]) == 0.0 && middle_z[0] > 1) --middle_z[0];
    }
    TString names[]  = {"out", "side", "long"};
    TString dirs[]   = {"x", "y", "z"};
    TString titles[] = {"cf", "num", "den"};
    Int_t mxxLow[]   = {middle_y[0], middle_x[0], middle_x[0]};
    Int_t myyLow[]   = {middle_z[0], middle_z[0], middle_y[0]};
    Int_t mxxHi[]    = {middle_y[1], middle_x[1], middle_x[1]};
    Int_t myyHi[]    = {middle_z[1], middle_z[1], middle_y[1]};
    for (int padId = 0; padId < 9; padId++) {
      int optId      = padId % 3;
      int flagDir    = (padId - optId) / 3;
      TString optLoc = titles[optId] + "+" + dirs[flagDir] + "+scale+bins";
      TH1* hx        = nullptr;
      switch (optId) {
        case 0: hx = h; break;
        case 1: hx = fCF->GetNum(); break;
        case 2: hx = fCF->GetDen(); break;
        default: break;
      }
      auto histo = GetProjection1D(h, mxxLow[flagDir], mxxHi[flagDir], myyLow[flagDir], myyHi[flagDir], optLoc);
      histo->SetTitle(Form("%s %s", names[flagDir].Data(), titles[optId].Data()));
      histo->SetMinimum(0);
      if (fRangeY[0] != fRangeY[1]) {
        histo->SetMaximum(fRangeY[1]);
        histo->SetMinimum(fRangeY[0]);
      }
      if (fRangeX[0] != fRangeX[1]) { histo->GetXaxis()->SetRangeUser(fRangeX[0], fRangeX[1]); }
      std::vector<TH1*> histVec;
      histVec.push_back(histo);
      fHistograms.push_back(histVec);
    }
  }

  TH1D*
  Femto3DCFPainter::GetProjection1D(TH1* h, Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) const {
    TString option = opt;
    TH1D* num1d    = Hal::Std::GetProjection1D((TH3*) h, min1, max1, min2, max2, option);
    return num1d;
  }

  void Femto3DCFPainter::Prepare3D(TH1* h) {
    Int_t middle_x[2];
    Int_t middle_y[2];
    Int_t middle_z[2];
    middle_x[0] = middle_x[1] = h->GetXaxis()->FindBin(0.0);
    middle_y[0] = middle_y[1] = h->GetYaxis()->FindBin(0.0);
    middle_z[0] = middle_z[1] = h->GetZaxis()->FindBin(0.0);
    auto frame                = fCF->GetFrame();
    if (frame == Femto::EKinematics::kSH_LCMS || frame == Femto::EKinematics::kSH_PRF) {
      middle_x[0] = middle_y[0] = middle_z[0] = 1;
      middle_x[1]                             = h->GetXaxis()->GetNbins();
      middle_y[1]                             = h->GetYaxis()->GetNbins();
      middle_z[1]                             = h->GetZaxis()->GetNbins();
    } else {
      if (h->GetXaxis()->GetBinLowEdge(middle_x[0]) == 0.0 && middle_x[0] > 1) --middle_x[0];
      if (h->GetYaxis()->GetBinLowEdge(middle_y[0]) == 0.0 && middle_y[0] > 1) --middle_y[0];
      if (h->GetZaxis()->GetBinLowEdge(middle_z[0]) == 0.0 && middle_z[0] > 1) --middle_z[0];
    }
    TString names[] = {"out", "side", "long"};
    TString dirs[]  = {"x", "y", "z"};
    Int_t mxxLow[]  = {middle_y[0], middle_x[0], middle_x[0]};
    Int_t myyLow[]  = {middle_z[0], middle_z[0], middle_y[0]};
    Int_t mxxHi[]   = {middle_y[1], middle_x[1], middle_x[1]};
    Int_t myyHi[]   = {middle_z[1], middle_z[1], middle_y[1]};
    for (int i = 0; i < 3; i++) {
      auto histo = GetProjection1D(h, mxxLow[i], mxxHi[i], myyLow[i], myyHi[i], dirs[i] + "+scale+bins");
      histo->SetTitle(names[i]);
      if (fRangeY[0] != fRangeY[1]) {
        histo->SetMaximum(fRangeY[1]);
        histo->SetMinimum(fRangeY[0]);
      }
      TString titleY = "";
      if (CheckOpt(kNumBit) || CheckOpt(kDenBit)) {
        titleY = "dN_{pairs}/d%s";
      } else {
        titleY = "CF(%s)";
      }
      TString axisTitle = Femto::KinematicsToAxisLabel(fCF->GetFrame(), i, 3);
      axisTitle         = Hal::Std::RemoveUnits(axisTitle);
      axisTitle.ReplaceAll(" ", "");
      titleY = Form(titleY.Data(), axisTitle.Data());
      histo->GetYaxis()->SetTitle(titleY);
      std::vector<TH1*> histVec;
      histVec.push_back(histo);
      fHistograms.push_back(histVec);
    }
  }

  void Femto3DCFPainter::PrepareDiagonal1(TH1* h) {
    TString options[7] = {"x", "y", "z", "xy++", "yz++", "xz++", "xyz+++"};
    TString titles[7]  = {
      "out",
      "side",
      "long",
      "out+side+",
      "side+long+",
      "out+long+",
      "out+side+long+",
    };
    for (int i = 0; i < 7; i++) {
      auto hist = Hal::Std::GetDiagonalProjection1D((TH3*) h, options[i], 0, 0);
      hist->SetTitle(titles[i]);
      hist->SetStats(0);
      TString xTitle = hist->GetXaxis()->GetTitle();
      xTitle         = xTitle.ReplaceAll("[GeV/c]", "");
      hist->SetYTitle(Form("C(%s)", xTitle.Data()));
      hist->SetXTitle(Form("%s [GeV/c]", xTitle.Data()));
      std::vector<TH1*> histVec;
      histVec.push_back(hist);
      if (fRangeY[0] != fRangeY[1]) {
        hist->SetMaximum(fRangeY[1]);
        hist->SetMinimum(fRangeY[0]);
      }
      if (fRangeX[0] != fRangeX[1]) { hist->GetXaxis()->SetRangeUser(fRangeX[0], fRangeX[1]); }
      fHistograms.push_back(histVec);
    }
  }

  void Femto3DCFPainter::PrepareDiagonal2(TH1* h) {
    TString options[16] = {
      "x", "y", "z", "", "xy++", "xy+-", "yz++", "", "yz+-", "xz++", "xz+-", "", "xyz+++", "xyz+-+", "xyz+--", "xyz++-"};
    TString titles[16] = {"out",
                          "side",
                          "long",
                          "",
                          "out+side+",
                          "out+side-",
                          "side+long+",
                          "",
                          "side+long-",
                          "out+long+",
                          "out+long-",
                          "",
                          "out+side+long+",
                          "out+side-long+",
                          "out+side-long-",
                          "out+side+long-"};

    for (int i = 0; i < 16; i++) {
      std::vector<TH1*> histVec;
      if (options[i] == "") {
        histVec.push_back(nullptr);
      } else {
        auto histo = Hal::Std::GetDiagonalProjection1D((TH3*) h, options[i], 0, 0);
        histo->SetTitle(titles[i]);
        histo->SetStats(0);
        TString xTitle = histo->GetXaxis()->GetTitle();
        xTitle         = xTitle.ReplaceAll("[GeV/c]", "");
        histo->SetYTitle(Form("C(%s)", xTitle.Data()));
        histo->SetXTitle(Form("%s [GeV/c]", xTitle.Data()));
        if (fRangeY[0] != fRangeY[1]) {
          histo->SetMaximum(fRangeY[1]);
          histo->SetMinimum(fRangeY[0]);
        }
        if (fRangeX[0] != fRangeX[1]) { histo->GetXaxis()->SetRangeUser(fRangeX[0], fRangeX[1]); }
        histVec.push_back(histo);
      }
      fHistograms.push_back(histVec);
    }
  }

  void Femto3DCFPainter::PrepareTwoDim(TH1* h) {

    TString opts[] = {"xy", "xz", "yz"};
    for (int i = 0; i < 3; i++) {
      auto out_side = Hal::Std::GetProjection2D((TH3*) h, 0, 0, opts[i]);
      if (fRangeY[0] != fRangeY[1]) {
        out_side->SetMaximum(fRangeY[1]);
        out_side->SetMinimum(fRangeY[0]);
      }
      std::vector<TH1*> histVec;
      histVec.push_back(out_side);
      fHistograms.push_back(histVec);
    }
  }

  Int_t Femto3DCFPainter::GetPadsRequired() const {
    if (CheckOpt(kDiag1Bit)) {
      return 9;
    } else if (CheckOpt(kDiag2Bit)) {
      return 16;
    } else if (CheckOpt(kHtmlBit)) {
      return 9;
    } else if (CheckOpt(kTwoDimBit) || CheckOpt(kTwoDimPlusBit)) {
      return 4;
    } else {  // standard
      if (AreSimiliar(GetDrawFlags(), PrepBitTemplate({kNumBit, kDenBit, kCFBit}))) {
        return 9;
      } else {
        return 4;
      }
    }
    return 4;
  }

  void Femto3DCFPainter::PrepareHtml(TH1* h) {
    Int_t middle_x[2];
    Int_t middle_y[2];
    Int_t middle_z[2];
    middle_x[0] = middle_x[1] = h->GetXaxis()->FindBin(0.0);
    middle_y[0] = middle_y[1] = h->GetYaxis()->FindBin(0.0);
    middle_z[0] = middle_z[1] = h->GetZaxis()->FindBin(0.0);
    auto frame                = fCF->GetFrame();
    if (frame == Femto::EKinematics::kSH_LCMS || frame == Femto::EKinematics::kSH_PRF) {
      middle_x[0] = middle_y[0] = middle_z[0] = 1;
      middle_x[1]                             = h->GetXaxis()->GetNbins();
      middle_y[1]                             = h->GetYaxis()->GetNbins();
      middle_z[1]                             = h->GetZaxis()->GetNbins();
    } else {
      if (h->GetXaxis()->GetBinLowEdge(middle_x[0]) == 0.0 && middle_x[0] > 1) --middle_x[0];
      if (h->GetYaxis()->GetBinLowEdge(middle_y[0]) == 0.0 && middle_y[0] > 1) --middle_y[0];
      if (h->GetZaxis()->GetBinLowEdge(middle_z[0]) == 0.0 && middle_z[0] > 1) --middle_z[0];
    }
    TString names[] = {"out", "side", "long"};
    TString dirs[]  = {"x", "y", "z"};
    Int_t mxxLow[]  = {middle_y[0], middle_x[0], middle_x[0]};
    Int_t myyLow[]  = {middle_z[0], middle_z[0], middle_y[0]};
    Int_t mxxHi[]   = {middle_y[1], middle_x[1], middle_x[1]};
    Int_t myyHi[]   = {middle_z[1], middle_z[1], middle_y[1]};
    for (int i = 0; i < 3; i++) {
      auto histo = GetProjection1D(h, mxxLow[i], mxxHi[i], myyLow[i], myyHi[i], dirs[i] + "+scale+bins");
      histo->SetTitle(names[i]);
      if (fRangeY[0] != fRangeY[1]) {
        histo->SetMaximum(fRangeY[1]);
        histo->SetMinimum(fRangeY[0]);
      }
      auto histoNum = GetProjection1D(fCF->GetNum(), mxxLow[i], mxxHi[i], myyLow[i], myyHi[i], dirs[i]);
      histoNum->SetTitle("Numerator" + names[i]);
      auto histoDen = GetProjection1D(fCF->GetDen(), mxxLow[i], mxxHi[i], myyLow[i], myyHi[i], dirs[i]);
      histoDen->SetTitle("Numerator" + names[i]);
      Hal::Std::SetColor(*histoNum, kGreen);
      Hal::Std::SetColor(*histoDen, kRed);
      Hal::Std::SetColor(*histo, kBlue);
      std::vector<TH1*> histVec1;
      histVec1.push_back(histo);
      std::vector<TH1*> histVec2;
      histVec2.push_back(histoNum);
      std::vector<TH1*> histVec3;
      histVec3.push_back(histoDen);
      fHistograms.push_back(histVec1);
      fHistograms.push_back(histVec2);
      fHistograms.push_back(histVec3);
    }
  }

  TString Femto3DCFPainter::GetOptionForCorrFit() const {
    if (CheckOpt(kDiag1Bit)) {
      HalCoutDebug("Diag1");
      return "fit2";
    }
    if (CheckOpt(kDiag2Bit)) {
      HalCoutDebug("Diag2");
      return "fit3";
    }
    if (CheckOpt(kTwoDimBit)) return "fit4";
    if (CheckOpt(kTwoDimPlusBit)) return "fit5";
    return "fit1";
  }

  void Femto3DCFPainter::ScaleHistograms() {
    FemtoCFPainter::ScaleHistograms();
    if (fRangeY[0] != fRangeY[1])
      for (auto& x : fHistograms) {
        for (auto y : x) {
          if (y) {
            y->SetMinimum(fRangeY[0]);
            y->SetMaximum(fRangeY[1]);
          }
        }
      }
  }

  Femto3DCFPainter::~Femto3DCFPainter() {}

  void Femto3DCFPainter::MakePadsAndCanvases() {
    Int_t req = TMath::Sqrt(GetPadsRequired());
    MakeCanvasPads(req, req, 0);
  }

  void Femto3DCFPainter::DrawHistograms() {
    FemtoCFPainter::DrawHistograms();
    if (!CheckOpt(kAngles)) return;
    LockPad();
    int count = 0;
    for (auto i : fHistograms) {
      GotoPad(++count);
      gPad->SetTheta(fThetaPad);
      gPad->SetPhi(fPhiPad);
    }
    UnlockPad();
  }

} /* namespace Hal */
