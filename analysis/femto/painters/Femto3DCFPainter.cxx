/*
 * Femto3DCFPainter.cxx
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#include "Femto3DCFPainter.h"

#include "Cout.h"
#include "Femto3DCF.h"
#include "HistoStyle.h"
#include "Options.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"

#include <TH3D.h>
#include <TVirtualPad.h>


namespace Hal {
  const int Femto3DCFPainter::kRgbBit        = FemtoCFPainter::LastBitPainter() + 1;
  const int Femto3DCFPainter::kRawBit        = FemtoCFPainter::LastBitPainter() + 2;
  const int Femto3DCFPainter::kDiag1Bit      = FemtoCFPainter::LastBitPainter() + 3;
  const int Femto3DCFPainter::kDiag2Bit      = FemtoCFPainter::LastBitPainter() + 4;
  const int Femto3DCFPainter::kTwoDimBit     = FemtoCFPainter::LastBitPainter() + 5;
  const int Femto3DCFPainter::kTwoDimPlusBit = FemtoCFPainter::LastBitPainter() + 6;
  const int Femto3DCFPainter::kAngles        = FemtoCFPainter::LastBitPainter() + 7;

  ULong64_t Femto3DCFPainter::SetOptionInternal(const Options& opt, ULong64_t newFlags) {
    newFlags        = FemtoCFPainter::SetOptionInternal(opt, newFlags);
    auto cleanFlags = [&](int setbit) { ResetFewBits(newFlags, {kDiag1Bit, kDiag2Bit, kTwoDimBit, kTwoDimPlusBit}, setbit); };
    if (opt.HasOption("diag1")) {
      cleanFlags(kDiag1Bit);
      fDefDrawFlag = "SAME+P";
    };
    if (opt.HasOption("diag2")) {
      cleanFlags(kDiag2Bit);
      fDefDrawFlag = "SAME+P";
    };
    if (opt.HasOption("2d")) {
      cleanFlags(kTwoDimBit);
      fDefDrawFlag = "SAME+colz";
    };
    if (opt.HasOption("3d")) {
      cleanFlags(kTwoDimPlusBit);
      fDefDrawFlag = "SAME+surf1";
      if (auto angles = opt.GetLabeledArray("ang"); angles.values.size() == 2) {
        fThetaPad = angles.values[0];
        fPhiPad   = angles.values[0];
      }
    };

    ContitionalPattern(opt, "hidetitles", newFlags, kHideTitlesBit);
    return newFlags;
  }

  void Femto3DCFPainter::MakeHistograms() {
    auto copyMarkerSize = [](TH1* cf, std::vector<std::vector<TH1*>>& arr) {
      for (auto& i : arr) {
        for (auto& j : i) {
          j->SetMarkerSize(cf->GetMarkerSize());
        }
      }
    };
    TH3* h     = nullptr;
    bool clean = false;
    if (CheckOpt(kNumBit)) {
      h = (TH3*) fCF->GetNum()->Clone();
    } else if (CheckOpt(kDenBit)) {
      h = (TH3*) fCF->GetDen()->Clone();
    } else {  // cf
      h     = (TH3*) fCF->GetHist(CheckOpt(kScaleBit));
      clean = true;
    }
    if (CheckOpt(kDiag1Bit)) {
      PrepareDiagonal1(h);
      copyMarkerSize(fCF->GetNum(), fHistograms);
    } else if (CheckOpt(kDiag2Bit)) {
      PrepareDiagonal2(h);
      copyMarkerSize(fCF->GetNum(), fHistograms);
    } else if (CheckOpt(kHtmlBit)) {
      PrepareHtml(h);
      copyMarkerSize(fCF->GetNum(), fHistograms);
    } else if (CheckOpt(kTwoDimBit) || CheckOpt(kTwoDimPlusBit)) {
      fDrawDim = 2;
      PrepareTwoDim(h);
    } else {  // standard
      if (AreSimiliar(GetDrawFlags(), PrepBitTemplate({kNumBit, kDenBit, kCFBit}))) {
        Prepare3DFull(h);
        copyMarkerSize(fCF->GetNum(), fHistograms);
      } else {
        Prepare3D(h);
        copyMarkerSize(fCF->GetNum(), fHistograms);
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
      auto histo     = GetProjection1D(h, mxxLow[flagDir], mxxHi[flagDir], myyLow[flagDir], myyHi[flagDir], optLoc);
      histo->SetTitle(Form("%s %s", names[flagDir].Data(), titles[optId].Data()));
      histo->SetMinimum(0);
      if (fRangeY[0] != fRangeY[1] && optId == 0) {
        histo->SetMaximum(fRangeY[1]);
        histo->SetMinimum(fRangeY[0]);
      }
      if (fRangeX[0] != fRangeX[1]) { histo->GetXaxis()->SetRangeUser(fRangeX[0], fRangeX[1]); }
      std::vector<TH1*> histVec;
      histVec.push_back(histo);
      Hal::Std::CopyAxisProp(fCF->GetNum()->GetXaxis(), histo->GetYaxis(), "!tit");
      if (GetHistoStyle()) GetHistoStyle()->Apply(*histo);
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
      TString titleY = "";
      if (CheckOpt(kNumBit) || CheckOpt(kDenBit)) {
        titleY = "dN_{pairs}/d%s";
      } else {
        titleY = "CF(%s)";
        if (fRangeY[0] != fRangeY[1]) {
          histo->SetMaximum(fRangeY[1]);
          histo->SetMinimum(fRangeY[0]);
        }
      }
      TString axisTitle = Femto::KinematicsToAxisLabel(fCF->GetFrame(), i, 3);
      axisTitle         = Hal::Std::RemoveUnits(axisTitle);
      axisTitle.ReplaceAll(" ", "");
      titleY = Form(titleY.Data(), axisTitle.Data());
      histo->GetYaxis()->SetTitle(titleY);
      std::vector<TH1*> histVec;
      histVec.push_back(histo);
      Hal::Std::CopyAxisProp(fCF->GetNum()->GetXaxis(), histo->GetYaxis(), "!tit");
      if (GetHistoStyle()) GetHistoStyle()->Apply(*histo);
      if (fRangeX[0] != fRangeX[1]) { histo->GetXaxis()->SetRangeUser(fRangeX[0], fRangeX[1]); }
      fHistograms.push_back(histVec);
    }
  }

  void Femto3DCFPainter::PrepareDiagonal1(TH1* h) {
    TString options[7] = {"x", "y", "z", "xy++", "xz++", "yz++", "xyz+++"};
    TString titles[7]  = {
      "out",
      "side",
      "long",
      "out+side+",
      "out+long+",
      "side+long+",
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
      if (GetHistoStyle()) GetHistoStyle()->Apply(*hist);
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
        if (GetHistoStyle()) GetHistoStyle()->Apply(*histo);
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
      if (fRangeX[0] != fRangeX[1]) { out_side->GetXaxis()->SetRangeUser(fRangeX[0], fRangeX[1]); }
      if (fRangeY[0] != fRangeY[1]) { out_side->GetYaxis()->SetRangeUser(fRangeY[0], fRangeY[1]); }
      if (fRangeZ[0] != fRangeZ[1]) {
        out_side->SetMaximum(fRangeZ[1]);
        out_side->SetMinimum(fRangeZ[0]);
      }
      if (GetHistoStyle()) GetHistoStyle()->Apply(*out_side);
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
    Double_t minDraw, maxDraw;
    if (fDrawDim == 1) {
      minDraw = fRangeY[0];
      maxDraw = fRangeY[1];
    }
    if (fDrawDim == 2) {
      minDraw = fRangeZ[0];
      maxDraw = fRangeZ[1];
    }

    if (minDraw != maxDraw)
      for (auto& x : fHistograms) {
        for (auto y : x) {
          if (y) {
            y->SetMinimum(minDraw);
            y->SetMaximum(maxDraw);
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

  std::pair<Double_t, Double_t> Femto3DCFPainter::GetXaxisRange() const {
    std::pair<Double_t, Double_t> res(0, 1);
    auto h = (TH3*) fCF->GetNum();
    if (!h) return res;
    res.first  = h->GetXaxis()->GetBinLowEdge(1);
    res.second = h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
    return res;
  }

  std::pair<Double_t, Double_t> Femto3DCFPainter::GetYaxisRange() const {
    std::pair<Double_t, Double_t> res(0, 1);
    auto h = (TH3*) fCF->GetNum();
    if (!h) return res;
    res.first  = h->GetYaxis()->GetBinLowEdge(1);
    res.second = h->GetYaxis()->GetBinUpEdge(h->GetNbinsY());
    return res;
  }

  std::pair<Double_t, Double_t> Femto3DCFPainter::GetZaxisRange() const {
    std::pair<Double_t, Double_t> res(0, 1);
    auto h = (TH3*) fCF->GetNum();
    if (!h) return res;
    res.first  = h->GetZaxis()->GetBinLowEdge(1);
    res.second = h->GetZaxis()->GetBinUpEdge(h->GetNbinsZ());
    return res;
  }

} /* namespace Hal */
