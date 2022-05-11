/*
 * CorrFitSmear1DCF.cxx
 *
 *  Created on: 4 sty 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitSmear1DCF.h"

#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "HalStdHist.h"
#include "Splines.h"

#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>


namespace Hal {
  CorrFitSmear1DCF::CorrFitSmear1DCF() : CorrFitKisiel(), fSmeared(kFALSE), fSmearingMap(NULL) {}

  void CorrFitSmear1DCF::Check() {
    CorrFit1DCF::Check();
    if (fSmearingMap == NULL) {
      Cout::Text("No smearing map", "M", kRed);
      return;
    }
    CorrFitKisiel::Check();
    // neeed to recalcuate smeared map

    if (!fSmeared) {
      TH2D* cf_map2d = new TH2D("newMap",
                                "newMap",
                                fNumeratorHistogram->GetNbinsX(),
                                fNumeratorHistogram->GetXaxis()->GetXmin(),
                                fNumeratorHistogram->GetXaxis()->GetXmax(),
                                fMaps[0]->GetNRbins(),
                                fMaps[0]->GetRmin(),
                                fMaps[0]->GetRmax());
      for (int i = 0; i <= cf_map2d->GetNbinsX() + 1; i++) {
        Double_t x = cf_map2d->GetXaxis()->GetBinCenter(i);
        for (int j = 0; j <= cf_map2d->GetNbinsY() + 1; j++) {
          Double_t y = cf_map2d->GetYaxis()->GetBinCenter(j);
          cf_map2d->SetBinContent(i, j, fMaps[0]->Eval(x, y));
          cf_map2d->SetBinError(i, j, fMaps[0]->EvalNumError(x, y));
        }
      }
      if (fNumeratorHistogram->GetNbinsX() != fSmearingMap->GetNbinsX()
            || fNumeratorHistogram->GetXaxis()->GetXmin() != fSmearingMap->GetXaxis()->GetXmin(),
          fNumeratorHistogram->GetXaxis()->GetXmax() != fSmearingMap->GetXaxis()->GetXmax()) {
        RescaleSmearingMap();
      }

      TAxis* x            = cf_map2d->GetXaxis();
      TH1D* smeared_den   = (TH1D*) fDenominatorHistogram->Clone("temp");
      TH1D* unsmeared_den = HalStd::SmearHistogram(smeared_den, fSmearingMap, "rev");

      for (int j = 0; j <= fMaps[0]->GetNRbins() + 1; j++) {
        TH1D* unsmeared_num = (TH1D*) unsmeared_den->Clone();
        /* we have unsmeared denominator, let's make numerator by
         multiplyingy by 2map
         */
        for (int i = 0; i <= x->GetNbins() + 1; i++) {
          Double_t cf = cf_map2d->GetBinContent(i, j);
          if (cf < 0) cf = 0.0;
          Double_t n = unsmeared_den->GetBinContent(i);
          unsmeared_num->SetBinContent(i, cf * n);
          unsmeared_num->SetBinError(i, TMath::Sqrt(n));
        }
        /*
         * now we have to make smeared numerator
         */
        TH1D* smeared_num = HalStd::SmearHistogram(unsmeared_num, fSmearingMap, "");
        delete unsmeared_num;
        smeared_num->Divide(smeared_den);
        /*
         * we have smreared cf, lets update map
         */
        for (int i = 0; i <= x->GetNbins() + 1; i++) {
          cf_map2d->SetBinContent(i, j, smeared_num->GetBinContent(i));
        }
        delete smeared_num;
      }
      delete smeared_den;
      delete unsmeared_den;
      Femto::EKinematics kin = fMaps[0]->GetFrame();
      delete fMaps[0];
      CorrFitMapKstarRstar* mapNew = new CorrFitMapKstarRstar(*cf_map2d, kin);
      fMaps[0]                     = mapNew;
      delete cf_map2d;
      fSmeared = kTRUE;
    }
  }

  void CorrFitSmear1DCF::LoadSmearingMap(TH2D* map) {
    if (fSmearingMap) delete fSmearingMap;
    fSmearingMap = (TH2D*) map->Clone();
  }

  CorrFitSmear1DCF::~CorrFitSmear1DCF() {
    if (fSmearingMap) { delete fSmearingMap; }
  }

  void CorrFitSmear1DCF::RescaleSmearingMap() {
    TH2D* smear_new  = new TH2D("scaled_smear",
                               "scaled_smear",
                               fNumeratorHistogram->GetNbinsX(),
                               fNumeratorHistogram->GetXaxis()->GetXmin(),
                               fNumeratorHistogram->GetXaxis()->GetXmax(),
                               fNumeratorHistogram->GetNbinsX(),
                               fNumeratorHistogram->GetXaxis()->GetXmin(),
                               fNumeratorHistogram->GetXaxis()->GetXmax());
    Spline2D* spline = new Spline2D(fSmearingMap);
    for (int i = 0; i <= smear_new->GetNbinsX() + 1; i++) {
      Double_t x = smear_new->GetXaxis()->GetBinCenter(i);
      for (int j = 0; j <= smear_new->GetNbinsX() + 1; j++) {
        Double_t y = smear_new->GetYaxis()->GetBinCenter(j);
        if (spline->Eval(x, y) >= 0) {
          smear_new->SetBinContent(i, j, spline->Eval(x, y));
          smear_new->SetBinError(i, j, spline->GetError(x, y));
        }
      }
    }
    delete fSmearingMap;
    delete spline;
    fSmearingMap = smear_new;
  }
}  // namespace Hal
