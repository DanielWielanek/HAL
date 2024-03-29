/*
 * Femto3DCF.cpp
 *
 *  Created on: 12-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto3DCF.h"

#include "Cout.h"
#include "FemtoPair.h"
#include "FemtoSerializationInterface3D.h"
#include "StdString.h"

#include <TAttLine.h>
#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TCollection.h>
#include <TH1.h>
#include <TH3.h>
#include <TList.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TRegexp.h>
#include <TSystem.h>
#include <iostream>

#include "CorrFit3DCF.h"
#include "StdString.h"

namespace Hal {
  Femto3DCF::Femto3DCF(TString name, Femto::EKinematics frame) : DividedHisto3D(name), fFrame(frame) {
    SetName(name);
    AddLabel(Femto::KinematicsToLabel(fFrame));
  }

  void Femto3DCF::FillNumObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    if (pair->IsAbs()) {
      ((TH3*) fNum)->Fill(TMath::Abs(pair->GetX()), TMath::Abs(pair->GetY()), TMath::Abs(pair->GetZ()), pair->GetWeight());
    } else {
      ((TH3*) fNum)->Fill(pair->GetX(), pair->GetY(), pair->GetZ(), pair->GetWeight());
    }
  }

  void Femto3DCF::FillDenObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    if (pair->IsAbs()) {
      ((TH3*) fDen)->Fill(TMath::Abs(pair->GetX()), TMath::Abs(pair->GetY()), TMath::Abs(pair->GetZ()), pair->GetWeight());
    } else {
      ((TH3*) fDen)->Fill(pair->GetX(), pair->GetY(), pair->GetZ(), pair->GetWeight());
    }
  }

  Femto3DCF::Femto3DCF(const Femto3DCF& other) : DividedHisto3D(other), fFrame(other.fFrame) {}

  Femto3DCF::~Femto3DCF() {}

  TString Femto3DCF::HTMLExtract(Int_t counter, TString dir) const {
    TString path = Form("%s/divided_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);

    Int_t middle_x   = fNum->GetXaxis()->FindBin(0.0);
    Int_t middle_y   = fNum->GetYaxis()->FindBin(0.0);
    Int_t middle_z   = fNum->GetZaxis()->FindBin(0.0);
    TString names[]  = {"out", "side", "long"};
    TString dirs[]   = {"x", "y", "z"};
    TString titles[] = {"cf", "num", "den"};
    Color_t colz[]   = {kRed, kGreen, kBlue};
    Int_t mxx[]      = {middle_y, middle_x, middle_x};
    Int_t myy[]      = {middle_z, middle_z, middle_y};
    TCanvas* c1      = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
    c1->Divide(3, 3);
    TH1D** histos = new TH1D*[9];
    for (int padId = 0; padId < 9; padId++) {
      c1->cd(padId + 1);
      int optId     = padId % 3;
      int flagDir   = (padId - optId) / 3;
      TString opt   = titles[optId] + "+" + dirs[flagDir] + "+scale+bins";
      histos[padId] = GetProjection1D(mxx[flagDir], mxx[flagDir], myy[flagDir], myy[flagDir], opt);
      histos[padId]->SetLineColor(colz[flagDir]);
      histos[padId]->SetTitle(Form("%s %s", names[flagDir].Data(), titles[optId].Data()));
      histos[padId]->SetMinimum(0);
      histos[padId]->Draw();
    }
    c1->Update();
    c1->SaveAs(Form("%s/divided.root", path.Data()));
    gROOT->SetBatch(batch);

    delete c1;
    for (int i = 0; i < 9; i++) {
      delete histos[i];
    }
    delete[] histos;
    TString page = CommonExtract(counter, dir);
    return page;
  }

  void Femto3DCF::AddNum(TH1* h, Option_t* opt) {
    h->GetXaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 0, 3));
    h->GetYaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 1, 3));
    h->GetZaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 2, 3));
    SetAxisName(Femto::KinematicsToAxisLabel(fFrame, 3, 3));
    DividedHisto3D::AddNum(h, opt);
  }

  void Femto3DCF::AddDen(TH1* h, Option_t* opt) {
    h->GetXaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 0, 3));
    h->GetYaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 1, 3));
    h->GetZaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 2, 3));
    SetAxisName(Femto::KinematicsToAxisLabel(fFrame, 3, 3));
    DividedHisto3D::AddDen(h, opt);
  }

  TH1D* Femto3DCF::Projection3DTo1D(Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) const {
    TH1D* res      = DividedHisto3D::Projection3DTo1D(min1, max1, min2, max2, opt);
    Int_t axis     = 0;
    TString option = opt;
    if (Hal::Std::FindParam(option, "y")) {
      axis = 1;
    } else if (Hal::Std::FindParam(option, "z")) {
      axis = 2;
    } else {
      axis = 0;
    }
    TString titleY;
    if (Hal::Std::FindParam(option, "num") || Hal::Std::FindParam(option, "den")) {
      titleY = "dN_{pairs}/d%s";
    } else {
      titleY = "CF(%s)";
    }
    TString axisTitle = Femto::KinematicsToAxisLabel(fFrame, axis, 3);
    axisTitle         = Hal::Std::RemoveUnits(axisTitle);
    axisTitle.ReplaceAll(" ", "");
    titleY = Form(titleY.Data(), axisTitle.Data());

    res->GetYaxis()->SetTitle(titleY);
    return res;
  }

  void Femto3DCF::Fit(CorrFit3DCF* fit) { fit->Fit(this); }

  void Femto3DCF::FitDummy(CorrFit3DCF* fit) { fit->FitDummy(this); }

  void Femto3DCF::Browse(TBrowser* b) {
    TVirtualPad* c1 = gPad;
    if (gPad == nullptr) { new TCanvas(); }
    gPad->Clear();
    Draw("all");
    gPad = c1;
    b->Add(fNum);
    b->Add(fDen);
  }

  void Femto3DCF::Draw(Option_t* opt) {
    if (gPad == nullptr) new TCanvas();
    TString option = opt;
    Double_t draw_min, draw_max;
    Bool_t set_limits = Hal::Std::FindExpressionTwoFloats(option, draw_min, draw_max, kFALSE);
    if (Hal::Std::FindParam(option, "num", kTRUE)) {
      fNum->Draw(option);
    } else if (Hal::Std::FindParam(option, "den", kTRUE)) {
      fDen->Draw(option);
    } else if (Hal::Std::FindParam(option, "all", kTRUE)) {
      TString name = "Divided 1D";
      Int_t middle_x[2];
      Int_t middle_y[2];
      Int_t middle_z[2];
      middle_x[0] = middle_x[1] = fNum->GetXaxis()->FindBin(0.0);
      middle_y[0] = middle_y[1] = fNum->GetYaxis()->FindBin(0.0);
      middle_z[0] = middle_z[1] = fNum->GetZaxis()->FindBin(0.0);
      if (fFrame == Femto::EKinematics::kSH_LCMS || fFrame == Femto::EKinematics::kSH_PRF) {
        middle_x[0] = middle_y[0] = middle_z[0] = 1;
        middle_x[1]                             = fNum->GetXaxis()->GetNbins();
        middle_y[1]                             = fNum->GetYaxis()->GetNbins();
        middle_z[1]                             = fNum->GetZaxis()->GetNbins();
      } else {
        if (fNum->GetXaxis()->GetBinLowEdge(middle_x[0]) == 0.0 && middle_x[0] > 1) --middle_x[0];
        if (fNum->GetYaxis()->GetBinLowEdge(middle_y[0]) == 0.0 && middle_y[0] > 1) --middle_y[0];
        if (fNum->GetZaxis()->GetBinLowEdge(middle_z[0]) == 0.0 && middle_z[0] > 1) --middle_z[0];
      }
      TString names[]  = {"out", "side", "long"};
      TString dirs[]   = {"x", "y", "z"};
      TString titles[] = {"cf", "num", "den"};
      Color_t colz[]   = {kRed, kGreen, kBlue};
      Int_t mxxLow[]   = {middle_y[0], middle_x[0], middle_x[0]};
      Int_t myyLow[]   = {middle_z[0], middle_z[0], middle_y[0]};
      Int_t mxxHi[]    = {middle_y[1], middle_x[1], middle_x[1]};
      Int_t myyHi[]    = {middle_z[1], middle_z[1], middle_y[1]};
      TVirtualPad* c1  = gPad;
      if (c1->GetListOfPrimitives()->GetEntries() < 9) c1->Divide(3, 3);
      TH1D** histos = new TH1D*[9];
      for (int padId = 0; padId < 9; padId++) {
        c1->cd(padId + 1);
        int optId      = padId % 3;
        int flagDir    = (padId - optId) / 3;
        TString optLoc = titles[optId] + "+" + dirs[flagDir] + "+scale+bins";
        histos[padId]  = GetProjection1D(mxxLow[flagDir], mxxHi[flagDir], myyLow[flagDir], myyHi[flagDir], optLoc);
        histos[padId]->SetLineColor(colz[flagDir]);
        histos[padId]->SetTitle(Form("%s %s", names[flagDir].Data(), titles[optId].Data()));
        histos[padId]->SetMinimum(0);
        if (set_limits && optId == 0) {
          histos[padId]->SetMaximum(draw_max);
          histos[padId]->SetMinimum(draw_min);
        }
        histos[padId]->Draw();
      }
      delete[] histos;
      gPad = c1;
      gPad->cd();
    } else if (Hal::Std::FindParam(option, "diag2", kTRUE)) {
      TString drawOpt = option;
      TH1D** array    = GetDiagProj("diag2");
      TVirtualPad* c1 = gPad;
      if (c1->GetListOfPrimitives()->GetEntries() < 13) c1->Divide(4, 4);
      for (int i = 0; i < 13; i++) {
        c1->cd(i + 1);
        array[i]->Draw(option);
      }
    } else if (Hal::Std::FindParam(option, "diag1", kTRUE)) {
      TVirtualPad* c1 = gPad;
      if (c1->GetListOfPrimitives()->GetEntries() < 7) c1->Divide(4, 2);
      TH1D** array = GetDiagProj("diag1");
      for (int i = 0; i < 7; i++) {
        c1->cd(i + 1);
        array[i]->Draw();
      }
    } else {
      option.ReplaceAll("all", "");
      TString name = "Divided 1D";
      Bool_t norm  = kFALSE;
      if (Hal::Std::FindParam(option, "norm", kTRUE)) { norm = kTRUE; }
      option.ReplaceAll("norm", "");
      TH1D** arr = GetDiagProj(option, norm);
      option.ReplaceAll("rgb", "");
      TVirtualPad* c1 = gPad;
      if (c1->GetListOfPrimitives()->GetEntries() < 4) c1->Divide(2, 2);
      for (int i = 0; i < 3; i++) {
        c1->cd(i + 1);
        if (set_limits) {
          arr[i]->SetMinimum(draw_min);
          arr[i]->SetMaximum(draw_max);
        }
        arr[i]->Draw(option);
      }
      c1->cd(4);
      gPad = c1;
      gPad->cd();
    }
  }

  void Femto3DCF::DrawScaled(Double_t /*scale*/, Option_t* opt) {
    TString option = opt;
    TString name   = "Divided 1D";
    Int_t middle_x[2];
    Int_t middle_y[2];
    Int_t middle_z[2];
    middle_x[0] = middle_x[1] = fNum->GetXaxis()->FindBin(0.0);
    middle_y[0] = middle_y[1] = fNum->GetYaxis()->FindBin(0.0);
    middle_z[0] = middle_z[1] = fNum->GetZaxis()->FindBin(0.0);
    if (fFrame == Femto::EKinematics::kSH_LCMS || fFrame == Femto::EKinematics::kSH_PRF) {
      middle_x[0] = middle_y[0] = middle_z[0] = 1;
      middle_x[1]                             = fNum->GetXaxis()->GetNbins();
      middle_y[1]                             = fNum->GetYaxis()->GetNbins();
      middle_z[1]                             = fNum->GetZaxis()->GetNbins();
    }
    if (fNum->GetXaxis()->GetBinLowEdge(middle_x[0]) == 0.0 && middle_x[0] > 1) --middle_x[0];
    if (fNum->GetYaxis()->GetBinLowEdge(middle_y[0]) == 0.0 && middle_y[0] > 1) --middle_y[0];
    if (fNum->GetZaxis()->GetBinLowEdge(middle_z[0]) == 0.0 && middle_z[0] > 1) --middle_z[0];
    TVirtualPad* c1 = gPad;
    if (c1->GetListOfPrimitives()->GetEntries() < 4) c1->Divide(2, 2);
    c1->cd(1);
    TH1D* out_func = GetProjection1D(middle_y[0], middle_y[1], middle_z[0], middle_z[1], "x+scale+bins+autoname");
    out_func->SetLineColor(kRed);
    out_func->SetTitle("out function");
    out_func->SetMinimum(0);
    out_func->Draw(option);
    c1->cd(2);
    TH1D* side_func = GetProjection1D(middle_x[0], middle_x[1], middle_z[0], middle_z[1], "y+scale+bins+autoname");
    side_func->SetLineColor(kBlue);
    side_func->SetTitle("side function");
    side_func->SetMinimum(0);
    side_func->Draw(option);
    c1->cd(3);
    TH1D* long_func = GetProjection1D(middle_x[0], middle_x[1], middle_y[0], middle_y[1], "z+scale+bins+autoname");
    long_func->SetMinimum(0);
    long_func->SetLineColor(kGreen);
    long_func->SetTitle("long function");
    long_func->Draw(option);
    c1->cd(4);
    gPad = c1;
  }

  Femto3DCF::Femto3DCF(TString name,
                       Int_t binsX,
                       Double_t minX,
                       Double_t maxX,
                       Int_t binsY,
                       Double_t minY,
                       Double_t maxY,
                       Int_t binsZ,
                       Double_t minZ,
                       Double_t maxZ,
                       Femto::EKinematics frame) :
    DividedHisto3D(name, binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ, 'D'), fFrame(frame) {
    fNum->Sumw2();
    fDen->Sumw2();
    if (fFrame == Femto::EKinematics::kSH_PRF || fFrame == Femto::EKinematics::kSH_LCMS) {
      TH3* old_num     = (TH3*) fNum;
      TH3* old_den     = (TH3*) fDen;
      TString old_name = old_num->GetName();
      name             = old_name(0, old_name.Length() - 4);
      fNum             = new TH3D(name + "_Num",
                      name + "_Num",
                      binsX,
                      0,
                      maxX,
                      binsY,
                      -TMath::Pi(),
                      TMath::Pi(),
                      binsZ,
                      -TMath::Pi() * 0.5,
                      TMath::Pi() * 0.5);
      fDen             = new TH3D(name + "_Den",
                      name + "_Den",
                      binsX,
                      0,
                      maxX,
                      binsY,
                      -TMath::Pi(),
                      TMath::Pi(),
                      binsZ,
                      -TMath::Pi() * 0.5,
                      TMath::Pi() * 0.5);
      delete old_num;
      delete old_den;
    }
    SetAxisNames(fNum);
    SetAxisNames(fDen);
    AddLabel(Femto::KinematicsToLabel(fFrame));
  }

  void Femto3DCF::SetAxisNames(TH1* h) {
    if (h == NULL) return;
    h->GetXaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 0, 3));
    h->GetYaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 1, 3));
    h->GetZaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 2, 3));
    SetAxisName(Femto::KinematicsToAxisLabel(fFrame, 3, 3));
  }

  void Femto3DCF::Print(Option_t* opt) const {
    DividedHisto1D::Print(opt);
    TString text = Form("Frame : %s", Femto::KinematicsToLabel(fFrame).Data());
    Cout::Text(text, "L", kWhite);
  }

  TH1D** Femto3DCF::GetDiagProj(Option_t* opt, Bool_t normalized) const {
    TString option = opt;
    if (Hal::Std::FindParam(option, "cf", kTRUE)) {
      TH3D* h            = (TH3D*) GetHist(normalized);
      TString options[3] = {"x", "y", "z"};
      TString titles[3]  = {"out", "side", "long"};
      TH1D** array       = new TH1D*[3];
      Bool_t rgb         = kFALSE;
      Color_t colors[3]  = {kRed, kBlue, kGreen};
      if (Hal::Std::FindParam(option, "rgb", kTRUE)) rgb = kTRUE;
      for (int i = 0; i < 3; i++) {
        array[i] = Hal::Std::GetDiagonalProjection1D(h, options[i], 0, 0);
        array[i]->SetTitle(titles[i]);
        array[i]->SetStats(0);
        TString xTitle = array[i]->GetXaxis()->GetTitle();
        xTitle         = xTitle.ReplaceAll("[GeV/c]", "");
        array[i]->SetYTitle(Form("C(%s)", xTitle.Data()));
        array[i]->SetXTitle(Form("%s [GeV/c]", xTitle.Data()));
        if (rgb) {
          array[i]->SetMarkerColor(colors[i]);
          array[i]->SetLineColor(colors[i]);
        }
      }
      delete h;
      return array;
    } else if (option.EqualTo("diag1")) {
      TH3D* h            = (TH3D*) GetHist(normalized);
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
      TH1D** array = new TH1D*[7];
      for (int i = 0; i < 7; i++) {
        array[i] = Hal::Std::GetDiagonalProjection1D(h, options[i], 0, 0);
        array[i]->SetTitle(titles[i]);
        array[i]->SetStats(0);
        TString xTitle = array[i]->GetXaxis()->GetTitle();
        xTitle         = xTitle.ReplaceAll("[GeV/c]", "");
        array[i]->SetYTitle(Form("C(%s)", xTitle.Data()));
        array[i]->SetXTitle(Form("%s [GeV/c]", xTitle.Data()));
      }
      delete h;
      return array;

    } else if (option.EqualTo("diag2")) {
      TString drawOpt     = option;
      TH3D* h             = (TH3D*) GetHist(normalized);
      TString options[13] = {
        "x", "y", "z", "xy++", "xy+-", "yz++", "yz+-", "xz++", "xz+-", "xyz+++", "xyz+-+", "xyz+--", "xyz++-"};
      TString titles[13] = {"out",
                            "side",
                            "long",
                            "out+side+",
                            "out+side-",
                            "side+long+",
                            "side+long-",
                            "out+long+",
                            "out+long-",
                            "out+side+long+",
                            "out+side-long+",
                            "out+side-long-",
                            "out+side+long-"};
      TH1D** array       = new TH1D*[13];
      for (int i = 0; i < 13; i++) {
        array[i] = Hal::Std::GetDiagonalProjection1D(h, options[i], 0, 0);
        array[i]->SetTitle(titles[i]);
        array[i]->SetStats(0);
        TString xTitle = array[i]->GetXaxis()->GetTitle();
        xTitle         = xTitle.ReplaceAll("[GeV/c]", "");
        array[i]->SetYTitle(Form("C(%s)", xTitle.Data()));
        array[i]->SetXTitle(Form("%s [GeV/c]", xTitle.Data()));
      }
      delete h;
      return array;
    }
    return nullptr;
  }

  TObject* Femto3DCF::GetSpecial(TString opt) const {
    if (opt == "serialization") return new FemtoSerializationInterface3D();
    return nullptr;
  }

}  // namespace Hal
