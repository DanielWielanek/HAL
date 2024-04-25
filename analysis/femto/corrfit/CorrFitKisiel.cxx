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

#include "CorrFitDrawOptions.h"
#include "Std.h"
#include "StdString.h"

namespace Hal {
  CorrFitKisiel::CorrFitKisiel(Int_t params) : CorrFit1DCF(params), fSkipError(kFALSE) {}

  void CorrFitKisiel::Check() {
    CorrFit1DCF::Check();
    if (fMaps.size() == 0) { Cout::PrintInfo("No maps loaded ", EInfo::kCriticalError); }
    if (IsParFixed(RadiusID())) {
      Cout::Text(Form("Radius fixed at %4.3f", GetRadius()));
    } else {
      Cout::Text(Form("Radius between %4.3f and %4.3f", fParameters[RadiusID()].GetMin(), fParameters[RadiusID()].GetMax()));
    }
    for (unsigned int i = 0; i < fMaps.size(); i++) {
      fMaps[i]->Recalc();
    }

    Double_t kstarMin  = fDenominatorHistogram->GetXaxis()->GetBinLowEdge(1);
    Double_t kstarMax  = fDenominatorHistogram->GetXaxis()->GetBinUpEdge(fDenominatorHistogram->GetNbinsX());
    Double_t kstarBins = fDenominatorHistogram->GetNbinsX();
    Double_t rMin      = fParameters[RadiusID()].GetMin();
    Double_t rMax      = fParameters[RadiusID()].GetMax();
    if (fKinematics == Femto::EKinematics::kLCMS) {
      kstarMin *= 0.5;
      kstarMax *= 0.5;
    }
    Double_t kstarDx = (kstarMax - kstarMin) / kstarBins;

    Cout::SetLineLenght(150);
    Cout::Text("CorrFitKisiel::Check test", "M", kWhite);
    Cout::Database({"ID", "Rmin", "Rmax", "Rstep", "KStarMin", "KstarMax", "KstarStep"});
    Cout::Database({"Req.",
                    Form("%4.3f", rMin),
                    Form("%4.3f", rMax),
                    "-",
                    Form("%4.3f", kstarMin),
                    Form("%4.3f", kstarMax),
                    Form("%4.3f", kstarDx)});

    auto colEn = [](Color_t Color) {
#ifdef __linux__
      if (Color == kRed) {
        std::cout << "\e[0;31m" << std::endl;
      } else if (Color == kGreen) {
        std::cout << "\e[0;32m" << std::endl;
      }
#endif
    };


    for (unsigned int i = 0; i < fMaps.size(); i++) {
      fMaps[i]->SetFrameScale(fKinematics);
      Double_t R_min = fMaps[i]->GetRmin();
      Double_t R_max = fMaps[i]->GetRmax();
      fParameters[RadiusID()].SetMapMin(R_min);
      fParameters[RadiusID()].SetMapMax(R_max);
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
      Cout::Database({id, txtRmin, txtRmax, txtRstep, txtKstarmin, txtKstarmax, txtKstardelta});
    }
#ifdef __linux__
    std::cout << "\e[0;0m" << std::endl;
#endif
    Cout::SetLineLenght(100);
  }

  Double_t CorrFitKisiel::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t r  = params[RadiusID()];
    Double_t l  = params[LambdaID()];
    Double_t n  = params[NormID()];
    Double_t cf = fMaps[0]->Eval(x[0], r);
    return n * ((cf - 1.0) * l + 1);
  }

  Double_t CorrFitKisiel::CalculateCFExtrapolated(const Double_t* x, const Double_t* params) const {
    return CalculateCF(x, params);
  }

  void CorrFitKisiel::Paint(Bool_t repaint, Bool_t refresh) {
    CorrFit1DCF::Paint(repaint, kFALSE);
    if (fDrawOptions.DrawNumErr() && fDrawOptions.DrawCf()) {
      if (fOldNumErr == nullptr) { fOldNumErr = (TH1D*) fDrawHistograms[0]->Clone("sys_err"); }
      fOldNumErr->SetFillStyle(3001);
      fOldNumErr->SetFillColor(kGray + 2);
      Double_t norm = 1;
      if (fDrawOptions.AutoNorm()) { norm = GetNorm(); }
      for (int iX = 1; iX <= fDrawHistograms[0]->GetNbinsX(); iX++) {
        fOldNumErr->SetBinContent(iX, fDrawHistograms[0]->GetBinContent(iX));
        fOldNumErr->SetBinError(iX, fMaps[0]->EvalNumErrorBin(iX, 1) * norm);
      }

      fOldNumErr->SetMarkerSize(0);
      fOldNumErr->SetMinimum(0);
      if (fDrawOptions.DrawMinMax()) {
        fOldNumErr->SetMinimum(fDrawOptions.GetMin());
        fOldNumErr->SetMaximum(fDrawOptions.GetMax());
      }
      fOldNumErr->Draw("SAME+E2");
    }
    if (refresh) {
      fTempPad->Modified(kTRUE);
      fTempPad->Update();
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

  void CorrFitKisiel::AddMap(CorrFitMapKstarRstar* map) { fMaps.push_back(map); }
}  // namespace Hal
