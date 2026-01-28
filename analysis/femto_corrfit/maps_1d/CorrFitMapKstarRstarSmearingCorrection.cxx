/*
 * CorrFit1DMomemtumSmearingCorrection.cxx
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#include "CorrFitMapKstarRstarSmearingCorrection.h"

#include <RtypesCore.h>
#include <TH1.h>
#include <TH2.h>

#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "SmearAlgo.h"
#include "StdHist.h"
#include "StdTypes.h"

namespace Hal {

  void CorrFitMapKstarRstarSmearingCorrection::SetCorrelationFunction(const Hal::Femto1DCF& cf) {
    fCF = (Hal::Femto1DCF*) cf.Clone();
  }

  void CorrFitMapKstarRstarSmearingCorrection::SetCorrFitMap(const Hal::CorrFitMapKstarRstar& map) {
    fInputMap = (Hal::CorrFitMapKstarRstar*) map.Clone();
  }

  void CorrFitMapKstarRstarSmearingCorrection::Calculate() {


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

    Hal::SmearAlgoMatrix Algo;
    Algo.SetSmearMatrix(*fSmearingMap);

    for (int i = 1; i <= CF_unsmeared->GetNbinsY(); i++) {
      TH1D* sliceNum = Hal::Std::GetProjection1D(CF_unsmeared, i, i, "bins+x");
      TH1D* sliceDen = Hal::Std::GetProjection1D(CF_unsmeared, i, i, "bins+x");
      TH1D* sliceOri = Hal::Std::GetProjection1D(CF_unsmeared, i, i, "bins+x");
      for (int j = 1; j <= sliceNum->GetNbinsX(); j++) {
        sliceNum->SetBinContent(j, sliceNum->GetBinContent(j) * rawDenominator->GetBinContent(j));
        sliceDen->SetBinContent(j, rawDenominator->GetBinContent(j));
      }
      auto smearedN = Algo.GetSmeared(*sliceNum);
      auto smearedD = Algo.GetSmeared(*sliceDen);
      for (int j = 1; j <= CF_unsmeared->GetNbinsX(); j++) {
        double DenVal = smearedD->GetBinContent(j);
        double NumVal = smearedN->GetBinContent(j);
        double OriVal = sliceOri->GetBinContent(j);
        if (DenVal == 0) {
          C_smeared->SetBinContent(j, i, OriVal);
          fRatio->SetBinContent(j, i, 1);
        } else {
          double smearedVal = NumVal / DenVal;
          double ratio      = OriVal / smearedVal;
          C_smeared->SetBinContent(j, i, smearedVal);
          fRatio->SetBinContent(j, i, ratio);
        }
      }
      delete sliceNum;
      delete sliceDen;
      delete sliceOri;
      delete smearedN;
      delete smearedD;
    }
    delete rawDenominator;
  }

  void CorrFitMapKstarRstarSmearingCorrection::SetSmearingMap(TH2D& smearing, ESmearInput opt) {
    if (opt == ESmearInput::kXsimYreco) {
      fSmearingMap = (TH2D*) smearing.Clone();
    } else if (opt == ESmearInput::kXrecoYsim) {
      fSmearingMap = DoInversion(&smearing);
    }
  }

  TH2D* CorrFitMapKstarRstarSmearingCorrection::DoInversion(TH2D* h) const {
    auto hx = (TH2D*) h->Clone();
    for (int i = 0; i <= h->GetNbinsX(); i++) {
      for (int j = 0; j <= h->GetNbinsY(); j++) {
        hx->SetBinContent(i, j, h->GetBinContent(j, i));
      }
    }
    return hx;
  }

  CorrFitMapKstarRstarSmearingCorrection::~CorrFitMapKstarRstarSmearingCorrection() {
    // TODO Auto-generated destructor stub
  }


} /* namespace Hal */
