/*
 * Femto3DCFPainter.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Femto3DCFPainter.h"

#include "Femto3DCF.h"
#include "StdHist.h"
#include "StdString.h"
#include "Style.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TVirtualPad.h>

namespace Hal {

  Femto3DCFPainter::Femto3DCFPainter(Femto3DCF& other, FemtoDrawOptions opt) : FemtoPainter(opt) {
    TString name = "Divided 1D";
    Int_t middle_x[2];
    Int_t middle_y[2];
    Int_t middle_z[2];
    middle_x[0] = middle_x[1] = other.fNum->GetXaxis()->FindBin(0.0);
    middle_y[0] = middle_y[1] = other.fNum->GetYaxis()->FindBin(0.0);
    middle_z[0] = middle_z[1] = other.fNum->GetZaxis()->FindBin(0.0);
    if (other.fFrame == Femto::EKinematics::kSH_LCMS || other.fFrame == Femto::EKinematics::kSH_PRF) {
      middle_x[0] = middle_y[0] = middle_z[0] = 1;
      middle_x[1]                             = other.fNum->GetXaxis()->GetNbins();
      middle_y[1]                             = other.fNum->GetYaxis()->GetNbins();
      middle_z[1]                             = other.fNum->GetZaxis()->GetNbins();
    } else {
      if (other.fNum->GetXaxis()->GetBinLowEdge(middle_x[0]) == 0.0 && middle_x[0] > 1) --middle_x[0];
      if (other.fNum->GetYaxis()->GetBinLowEdge(middle_y[0]) == 0.0 && middle_y[0] > 1) --middle_y[0];
      if (other.fNum->GetZaxis()->GetBinLowEdge(middle_z[0]) == 0.0 && middle_z[0] > 1) --middle_z[0];
    }
    TString names[]  = {"out", "side", "long"};
    TString dirs[]   = {"x", "y", "z"};
    TString titles[] = {"cf", "num", "den"};
    Color_t colz[]   = {kRed, kGreen, kBlue};
    Int_t mxxLow[]   = {middle_y[0], middle_x[0], middle_x[0]};
    Int_t myyLow[]   = {middle_z[0], middle_z[0], middle_y[0]};
    Int_t mxxHi[]    = {middle_y[1], middle_x[1], middle_x[1]};
    Int_t myyHi[]    = {middle_z[1], middle_z[1], middle_y[1]};

    for (int optId = 0; optId < 3; optId++) {
      for (int dirId = 0; dirId < 3; dirId++) {
        TString optLoc = titles[optId] + "+" + dirs[dirId] + "+scale+bins";
        auto h         = other.GetProjection1D(mxxLow[optId], mxxHi[optId], myyLow[optId], myyHi[optId], optLoc);
        switch (optId) {
          case 0: fCFs.push_back(h); break;
          case 1: fNum.push_back(h); break;
          case 2: fDen.push_back(h); break;
        }
      }
    }
    TH1D** array = other.GetDiagProj("diag1");
    for (int i = 0; i < 7; i++) {
      fDiag1.push_back(array[i]);
    }
    delete[] array;
    array = other.GetDiagProj("diag2");
    for (int i = 0; i < 13; i++) {
      fDiag2.push_back(array[i]);
    }
    delete[] array;
  }

  void Femto3DCFPainter::Paint(TString option) {
    TVirtualPad* pad = gPad;
    if (gPad == nullptr) {
      fCanvas = new TCanvas();
    } else {
      fCanvas = gPad;
    }


    Double_t draw_min, draw_max;
    Bool_t set_limits = Hal::Std::FindExpressionTwoFloats(option, draw_min, draw_max, kFALSE);
    auto drawHist     = [](TH1* h) {
      fSubPads.push_back(gPad);
      if (set_limits) {
        h->SetMinimum(draw_min);
        h->SetMaximum(draw_max);
      }
      if (fHistoStyle) { fHistoStyle->Apply(*h); }
      h->Draw(option);
    };
    if (Hal::Std::FindParam(option, "num", kTRUE)) {
      if (Hal::Std::GetListOfSubPads(fCanvas) != 3) fCanvas->Divide(3, 1);
      for (unsigned int i = 0; i < fNum.size(); i++) {
        fCanvas->cd(i + 1);
        fSubPads.push_back(gPad);
        fNum[i]->Draw(option);
      }
      gPad = pad;
      return;
    }
    if (Hal::Std::FindParam(option, "den", kTRUE)) {
      if (Hal::Std::GetListOfSubPads(fCanvas) != 3) fCanvas->Divide(3, 1);
      for (unsigned int i = 0; i < fDen.size(); i++) {
        fCanvas->cd(i + 1);
        fSubPads.push_back(gPad);
        fDen[i]->Draw(option);
      }
      gPad = pad;
      return;
    }
    if (Hal::Std::FindParam(option, "html", kTRUE)) {
      if (Hal::Std::GetListOfSubPads(fCanvas) != 9) fCanvas->Divide(3, 3);
      for (unsigned int i = 0; i < fDen.size(); i++) {
        fCanvas->cd(i + 1);
        drawHist(fDen[i]);
      }
      for (unsigned int i = 0; i < fNum.size(); i++) {
        fCanvas->cd(i + 4);
        drawHist(fNum[i]);
      }
      for (unsigned int i = 0; i < fCFs.size(); i++) {
        fCanvas->cd(i + 9);
        drawHist(fCFs[i]);
      }
      gPad = pad;
      return;
    }
    if (Hal::Std::FindParam(option, "diag1", kTRUE)) {
      if (Hal::Std::GetListOfSubPads(fCanvas) < 7) fCanvas->Divide(4, 2);
      for (unsigned int i = 0; i < fDiag1.size(); i++) {
        fCanvas->cd(i + 1);
        drawHist(fDiag1[i]);
      }
      gPad = pad;
      return;
    }
    if (Hal::Std::FindParam(option, "diag2", kTRUE)) {
      if (Hal::Std::GetListOfSubPads(fCanvas) < 13) fCanvas->Divide(4, 4);
      for (unsigned int i = 0; i < fDiag2.size(); i++) {
        fCanvas->cd(i + 1);
        drawHist(fDiag2[i]);
      }
      gPad = pad;
      return;
    }
    if (Hal::Std::GetListOfSubPads(fCanvas) < 3) fCanvas->Divide(2, 2);
    for (unsigned int i = 0; i < fCFs.size(); i++) {
      fCanvas->cd(i + 1);
      drawHist(fCFs[i]);
    }
    gPad = pad;
  }

  void Femto3DCFPainter::Rescale(Double_t newScale) {
    FemtoPainter::Rescale(newScale);
    for (auto i : fDiag1)
      i->Scale(fNewScale);
    for (auto i : fDiag2)
      i->Scale(fNewScale);
    for (auto i : fCFs)
      i->Scale(fNewScale);
  }

  Femto3DCFPainter::~Femto3DCFPainter() {}

} /* namespace Hal */
