/*
 * CorrFit1DMomemtumSmearingCorrection.cxx
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#include "CorrFit1DSmearing.h"

#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "Femto1DCF.h"
#include "SmearAlgo.h"
#include "StdHist.h"
#include "StdTypes.h"

#include <iostream>

namespace Hal {

  void CorrFit1DSmearing::SetCorrelationFunction(const Hal::Femto1DCF& cf) { fCF = (Hal::Femto1DCF*) cf.Clone(); }

  void CorrFit1DSmearing::SetCorrFitMap(const Hal::CorrFitMapKstarRstar& map) {
    fInputMap = (Hal::CorrFitMapKstarRstar*) map.Clone();
  }

  void CorrFit1DSmearing::Calculate() {


    auto CF_unsmeared    = fInputMap->GetHisto();
    fSmearedMap          = (Hal::CorrFitMapKstarRstar*) fInputMap->Clone();
    auto C_smeared       = fSmearedMap->GetHisto();
    fRatio               = (TH2D*) CF_unsmeared->Clone();
    TH1D* rawDenominator = (TH1D*) fCF->GetDen();

    Int_t bin[3];
    Double_t max[3], min[3];
    Hal::Std::GetAxisPar(*rawDenominator, bin[0], min[0], max[0], "x");
    Hal::Std::GetAxisPar(*fRatio, bin[1], min[1], max[1], "x");
    Hal::Std::GetAxisPar(*fSmearingMap, bin[2], min[2], max[2], "x");

    if (bin[0] != bin[1] || bin[0] != bin[2]) {
      Hal::Cout::PrintInfo("Not equal number of bins in maps/smearing functions", EInfo::kError);
    }
    if (min[0] != min[1] || min[0] != min[2]) {
      Hal::Cout::PrintInfo("Not equal number of mins in maps/smearing functions", EInfo::kError);
    }
    if (max[0] != max[1] || max[0] != max[2]) {
      Hal::Cout::PrintInfo("Not equal number of maxes in maps/smearing functions", EInfo::kError);
    }

    Hal::SmearAlgoMatrix Algo(true);
    Algo.SetInvertionParameters({fLambda});
    Algo.SetSmearMatrix(*fSmearingMap);
    Algo.SetInvertionMethod(fInversion);
    Algo.Init();
    auto rawVector    = Std::Math::GetVector(*rawDenominator, true);
    rawVector[0]      = 0;
    auto unsmearedDen = Algo.GetUnsmeared(rawVector);
    // TFile* f                    = new TFile("debug.root", "recreate");
    auto div                    = dynamic_cast<Hal::CorrFitMapKstarRstarDiv*>(fSmearedMap);
    Hal::DividedHisto2D* ratioH = nullptr;
    if (div) { ratioH = (Hal::DividedHisto2D*) div->GetDividedHisto(); }


    TH1D* sliceNumRaw = Hal::Std::GetProjection1D(CF_unsmeared, 1, 1, "bins+x");

    auto smearedD = Algo.GetSmeared(unsmearedDen);

    for (int iRadius = 1; iRadius <= CF_unsmeared->GetNbinsY(); iRadius++) {
      for (int jKstar = 1; jKstar <= sliceNumRaw->GetNbinsX(); jKstar++) {
        sliceNumRaw->SetBinContent(jKstar, CF_unsmeared->GetBinContent(jKstar, iRadius) * unsmearedDen[jKstar]);
      }
      auto smearedN = Algo.GetSmeared(*sliceNumRaw);
      for (int jKstar = 1; jKstar <= CF_unsmeared->GetNbinsX(); jKstar++) {
        double DenVal = smearedD[jKstar];
        double NumVal = smearedN[jKstar];
        double OriVal = CF_unsmeared->GetBinContent(jKstar, iRadius);
        if (DenVal == 0) {
          C_smeared->SetBinContent(jKstar, iRadius, OriVal);
          fRatio->SetBinContent(jKstar, iRadius, 1);
          if (ratioH) {
            ratioH->GetNum()->SetBinContent(jKstar, iRadius, ratioH->GetDen()->GetBinContent(jKstar, iRadius) * OriVal);
          }
        } else {
          double smearedVal = NumVal / DenVal;
          double ratio      = OriVal / smearedVal;
          C_smeared->SetBinContent(jKstar, iRadius, smearedVal);
          fRatio->SetBinContent(jKstar, iRadius, ratio);
          if (ratioH) {
            ratioH->GetNum()->SetBinContent(jKstar, iRadius, ratioH->GetDen()->GetBinContent(jKstar, iRadius) * smearedVal);
          }
        }
      }
    }
    delete sliceNumRaw;
    // f->Close();
    delete rawDenominator;
  }

  void CorrFit1DSmearing::SetSmearingMap(TH2D& smearing, ESmearInput opt) {
    if (opt == ESmearInput::kXsimYreco) {
      fSmearingMap = (TH2D*) smearing.Clone();
    } else if (opt == ESmearInput::kXrecoYsim) {
      fSmearingMap = DoInversion(&smearing);
    }
  }

  TH2D* CorrFit1DSmearing::DoInversion(TH2D* h) const {
    auto hx = (TH2D*) h->Clone();
    for (int i = 0; i <= h->GetNbinsX(); i++) {
      for (int j = 0; j <= h->GetNbinsY(); j++) {
        hx->SetBinContent(i, j, h->GetBinContent(j, i));
      }
    }
    return hx;
  }

  CorrFit1DSmearing::~CorrFit1DSmearing() {
    // TODO Auto-generated destructor stub
  }


} /* namespace Hal */
