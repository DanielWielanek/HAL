/*
 * CorrFitKisiel.cxx
 *
 *  Created on: 27 lis 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitKisiel.h"

#include "AnaFile.h"
#include "CorrFit.h"
#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "Package.h"
#include "Parameter.h"

#include <TAttFill.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TClonesArray.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TList.h>
#include <TObjArray.h>
#include <TVirtualPad.h>

#include "Std.h"
#include "StdString.h"

namespace Hal {
  CorrFitKisiel::CorrFitKisiel(Int_t params) : CorrFit1DCF(params), fSkipError(kFALSE) {}

  void CorrFitKisiel::Check() {
    CorrFit1DCF::Check();
    if (fMaps.size() == 0) { Cout::PrintInfo("No maps loaded ", EInfo::kCriticalError); }
    if (IsParFixed(Radius())) {
      Cout::Text(Form("Radius fixed at %4.3f", GetRadius()));
    } else {
      Cout::Text(Form("Radius between %4.3f and %4.3f", fParameters[Radius()].GetMin(), fParameters[Radius()].GetMax()));
    }
    for (unsigned int i = 0; i < fMaps.size(); i++) {
      fMaps[i]->Recalc();
    }

    Double_t kstarMin  = fDenominatorHistogram->GetXaxis()->GetBinLowEdge(1);
    Double_t kstarMax  = fDenominatorHistogram->GetXaxis()->GetBinUpEdge(fDenominatorHistogram->GetNbinsX());
    Double_t kstarBins = fDenominatorHistogram->GetNbinsX();
    Double_t rMin      = fParameters[Radius()].GetMin();
    Double_t rMax      = fParameters[Radius()].GetMax();
    if (fKinematics == Femto::EKinematics::kLCMS) {
      kstarMin *= 0.5;
      kstarMax *= 0.5;
    }
    Double_t kstarDx = (kstarMax - kstarMin) / kstarBins;

    Cout::SetLineLenght(150);
    Cout::Text("CorrFitKisiel::Check test", "M", kWhite);
    Cout::Database(7, "ID", "Rmin", "Rmax", "Rstep", "KStarMin", "KstarMax", "KstarStep");
    Cout::Database(7,
                   "Req.",
                   Form("%4.3f", rMin),
                   Form("%4.3f", rMax),
                   "-",
                   Form("%4.3f", kstarMin),
                   Form("%4.3f", kstarMax),
                   Form("%4.3f", kstarDx));

    auto colEn = [](Color_t Color) {
#ifdef __linux__
      if (Color == kRed) {
        std::cout << "\e[0;31m" << std::endl;
      } else if (Color == kGreen) {
        std::cout << "\e[0;32m" << std::endl;
      }
#endif
    };
    auto colDis = []() {
#ifdef __linux__
      std::cout << "\e[0;0m" << std::endl;
#endif
    };

    for (unsigned int i = 0; i < fMaps.size(); i++) {
      fMaps[i]->SetFrameScale(fKinematics);
      Double_t R_min = fMaps[i]->GetRmin();
      Double_t R_max = fMaps[i]->GetRmax();
      Int_t R_bins   = fMaps[i]->GetNRbins();
      Double_t Kmin  = fMaps[i]->GetKstarMin();
      Double_t Kmax  = fMaps[i]->GetKstarMax();
      Double_t Kbins = fMaps[i]->GetNKstrbins();
      if (fMaps[i]->GetFrame() == Femto::EKinematics::kLCMS) {
        Kmin *= 0.5;
        Kmax *= 0.5;
      }
      Bool_t good = kTRUE;
      if (R_min > rMin) good = kFALSE;
      if (R_max < rMax) good = kFALSE;
      if (Kmin > kstarMin) good = kFALSE;
      if (Kmax < kstarMax) good = kFALSE;
      if (good) {
        colEn(kGreen);
      } else {
        colEn(kRed);
      }
      TString id            = Form("%i", i);
      TString txtRmin       = Form("%4.3f", R_min);
      TString txtRmax       = Form("%4.3f", R_max);
      TString txtRstep      = Form("%4.3f", (R_max - R_min) / R_bins);
      TString txtKstarmin   = Form("%4.3f", Kmin);
      TString txtKstarmax   = Form("%4.3f", Kmax);
      TString txtKstardelta = Form("%4.3f", (Kmax - Kmin) / Kbins);
      Cout::Database(7,
                     id.Data(),
                     txtRmin.Data(),
                     txtRmax.Data(),
                     txtRstep.Data(),
                     txtKstarmin.Data(),
                     txtKstarmax.Data(),
                     txtKstardelta.Data());
    }
#ifdef __linux__
    std::cout << "\e[0;0m" << std::endl;
#endif
    Cout::SetLineLenght(100);
  }

  Double_t CorrFitKisiel::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t r  = params[Radius()];
    Double_t l  = params[Lambda()];
    Double_t n  = params[Norm()];
    Double_t cf = fMaps[0]->Eval(x[0], r);
    return n * ((cf - 1.0) * l + 1);
  }

  Double_t CorrFitKisiel::CalculateCFExtrapolated(const Double_t* x, const Double_t* params) const {
    return CalculateCF(x, params);
  }

  void CorrFitKisiel::Draw(Option_t* draw_option) {
    TString option        = draw_option;
    Bool_t drawNormalized = Hal::Std::FindParam(option, "norm", kTRUE);
    Double_t Rmid         = 0.5 * (fMaps[0]->GetRmax() + fMaps[0]->GetKstarMin());
    Double_t draw_min, draw_max;
    Bool_t set_limits = ExtrDraw(option, draw_min, draw_max);

    if (Hal::Std::FindParam(option, "full") && Hal::Std::FindParam(option, "ee")) {
      if (fDrawFunc.size() == 0) fDrawFunc.resize(1);
      TH1* cf = GetTHForDrawing(drawNormalized);

      TF1* draw_func = GetFunctionForDrawing();

      cf->SetMarkerStyle(kFullSquare);
      TH1* cf2 = (TH1*) cf->Clone("sys_err");
      cf2->SetFillStyle(3001);
      cf2->SetFillColor(kGray + 2);
      for (int iX = 1; iX <= cf->GetNbinsX(); iX++) {
        Double_t k = cf2->GetXaxis()->GetBinCenter(iX);
        cf2->SetBinError(iX, fMaps[0]->EvalNumErrorBin(iX, 1));
      }

      cf2->SetMarkerSize(0);
      cf2->SetMinimum(0);
      if (set_limits) {
        cf2->SetMinimum(draw_min);
        cf2->SetMaximum(draw_max);
      }

      cf2->Draw("E2");
      cf->Draw("SAME");
      fDrawFunc[0].first  = draw_func;
      fDrawFunc[0].second = gPad;
      if (drawNormalized) draw_func->SetParameter(Norm(), 1);
      draw_func->Draw("SAME");
      TLegend* leg = GetLegendForDrawing();
      leg->Draw("SAME");
    } else {
      CorrFit1DCF::Draw(draw_option);
    }
  }

  CorrFitKisiel::~CorrFitKisiel() {
    for (unsigned int i = 0; i < fMaps.size(); i++) {
      delete fMaps[i];
    }
  }

  void CorrFitKisiel::LoadMap(TH2D* h, Femto::EKinematics mapFrame) {
    TString name              = Form("CF_map_%i", (int) fMaps.size());
    CorrFitMapKstarRstar* map = new CorrFitMapKstarRstar(*h, mapFrame);
    map->SetName(name);
    fMaps.push_back(map);
  }

  void CorrFitKisiel::AddMap(CorrFitMapKstarRstar* map) {
    map->Recalc();
    fMaps.push_back(map);
  }
}  // namespace Hal
