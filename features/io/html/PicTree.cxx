/*
 * PicTree.cxx
 *
 *  Created on: 4 lip 2024
 *      Author: daniel
 */

#include "PicTree.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TArc.h>
#include <TAttFill.h>
#include <TAttLine.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLine.h>
#include <TROOT.h>
#include <TString.h>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <vector>

#include "Std.h"


namespace Hal {

  PicTree::PicTree() {}

  void PicTree::AddLink(std::initializer_list<Int_t> depths, std::initializer_list<Int_t> ids, Int_t style) {
    fLinks.push_back(link(depths, ids, style));
  }
  void PicTree::AddPoint(Int_t depth, Int_t number, Int_t style, Double_t ratio) {
    fCircles.push_back(circle(depth, number, style, ratio));
  };

  void PicTree::SaveAs(TString path) {
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    TCanvas* c1 = new TCanvas("canv", "canv", fWindowWidth, fWindowHeight);
    c1->Range(0, 0, fWindowWidth, fWindowHeight);
    fDepthMax = 0;
    for (int i = 0; i < fMaxExpDepth; i++) {
      for (auto& acirc : fCircles) {
        if (acirc.sDepth == i) fDepthMax = i;
      }
    }
    ++fDepthMax;
    fPoints.resize(fDepthMax);
    for (int i = 0; i < fDepthMax; i++) {
      int tiers[] = {0, 0, 0, 0};
      for (auto& acirc : fCircles) {
        if (acirc.sDepth == i) tiers[acirc.sStyles]++;
      }
      int maxLev = 0;
      for (int j = 0; j < 4; j++) {
        if (maxLev < tiers[j]) maxLev = tiers[j];
      }
      fPoints[i].resize(maxLev);
    }
    for (int i = 0; i < fDepthMax; i++) {
      int maxLev   = fPoints[i].size();
      double depth = (double) i;
      double maxNo = (double) maxLev;
      for (int j = 0; j < fPoints[i].size(); j++) {
        double collection    = j;
        Double_t Y_step      = fWindowHeight / maxNo;
        Double_t X_step      = fWindowWidth / (double) fDepthMax;
        Double_t X_center    = X_step * (depth + 0.5);
        Double_t Y_center    = fWindowHeight - Y_step * (collection + 0.5);
        fPoints[i][j].first  = X_center;
        fPoints[i][j].second = Y_center;
      }
    }

    std::sort(fLinks.begin(), fLinks.end(), [](const link& a, const link& b) { return a.sStyles > b.sStyles; });
    std::sort(fCircles.begin(), fCircles.end(), [](const circle& a, const circle& b) { return a.sStyles > b.sStyles; });


    for (auto& aline : fLinks) {
      aline.Draw(*this);
    }
    for (auto& acirc : fCircles) {
      acirc.Draw(*this);
    }

    c1->SaveAs(path.Data());
    gROOT->SetBatch(batch);
  }

  PicTree::link::link(std::initializer_list<Int_t> depths, std::initializer_list<Int_t> ids, Int_t style) {
    {
      sStyles     = style;
      auto vec    = Hal::Std::GetVector(depths);
      sFromDepth  = vec[0];
      sToDepth    = vec[1];
      auto vec2   = Hal::Std::GetVector(ids);
      sFromNumber = vec2[0];
      sToNumber   = vec2[1];
    }
  }

  void PicTree::link::Draw(const PicTree& tree) {

    Double_t X1 = tree.GetCoords()[sFromDepth][sFromNumber].first;
    Double_t Y1 = tree.GetCoords()[sFromDepth][sFromNumber].second;

    Double_t X2 = tree.GetCoords()[sToDepth][sToNumber].first;
    Double_t Y2 = tree.GetCoords()[sToDepth][sToNumber].second;
    TLine* line = new TLine(X1, Y1, X2, Y2);
    line->SetLineWidth(4.0);
    switch (sStyles) {
      case 0: {
        line->SetLineWidth(2);
        line->SetLineStyle(1);
        line->SetLineColor(kBlack);
      } break;
      case 1: {
        line->SetLineWidth(4);
        line->SetLineStyle(9);
        line->SetLineColor(kBlue);
      } break;
      default: break;
    }
    line->Draw();
  }

}  // namespace Hal

void Hal::PicTree::circle::Draw(const PicTree& tree) {

  Double_t angle_pass   = 360.0 * sRatio;
  const Double_t X_coor = tree.GetCoords()[sDepth][sNumber].first;
  const Double_t Y_coor = tree.GetCoords()[sDepth][sNumber].second;
  switch (sStyles) {
    case 0: {
      TArc* b2 = new TArc(X_coor, Y_coor, 40, 0, 360);
      b2->SetFillColor(kRed);
      b2->Draw();
      if (sRatio > 0.001) {
        auto b1 = new TArc(X_coor, Y_coor, 40, 90, 90 - angle_pass);
        b1->SetFillColor(kGreen);
        b1->Draw("SAME");
      }

    } break;
    case 1: {
      TArc* b2 = new TArc(X_coor, Y_coor, 50, 0, 360);
      b2->SetFillColor(kRed + 2);
      b2->Draw();
      if (sRatio > 0.001) {
        auto b1 = new TArc(X_coor, Y_coor, 50, 90, 90 - angle_pass);
        b1->SetFillColor(kGreen + 2);
        b1->Draw("SAME");
      }
    } break;
    default: break;
  }
  TLatex t;
  if (sNumber < 10)
    t.DrawText(X_coor - 10, Y_coor - 12, Form("%i", sNumber));
  else
    t.DrawText(X_coor - 15, Y_coor - 17, Form("%i", sNumber));
  //  t.DrawText(0, 0, Form("%i", sNumber));
}
