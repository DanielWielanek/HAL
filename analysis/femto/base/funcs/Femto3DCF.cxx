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
#include "Femto3DCFPainter.h"
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
    TString options = opt;
    if (fPainter) {
      fPainter->SetOption(options);
      fPainter->Paint();
    } else {
      fPainter = new Hal::Femto3DCFPainter(this);
      fPainter->SetOption(options);
      fPainter->Paint();
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

  Femto3DCF::Femto3DCF(TString name, Int_t binsX, Double_t minX, Double_t maxX, Femto::EKinematics frame) :
    Femto3DCF(name, binsX, minX, maxX, binsX, minX, maxX, binsX, minX, maxX, frame) {}

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
    if (opt == "painter") return fPainter;
    return nullptr;
  }

}  // namespace Hal
