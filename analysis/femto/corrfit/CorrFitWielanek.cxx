/*
 * CorrFitWielanek.cxx
 *
 *  Created on: 28 sty 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitWielanek.h"
#include "CorrFit1DCF.h"
#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "Std.h"
#include "Splines.h"

#include <TAxis.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>


namespace Hal {
  CorrFitWielanek::CorrFitWielanek() : fExtrapolationsSteps(40), fFSIMap(NULL), fSource(NULL) {}

  void CorrFitWielanek::LoadMapFromFile(TString name, Int_t /*kt_bin*/) {
    TFile* file = new TFile(name);
    TH2D* map   = (TH2D*) file->Get("map");
    fFSIMap     = (TH2D*) map->Clone();
    fFSIMap->SetDirectory(0);
    file->Close();
  }

  void CorrFitWielanek::Check() {
    CorrFit1DCF::Check();
    if (fMaps.size() == 0) { CreateMap(); }
    CorrFitKisiel::Check();
  }

  void CorrFitWielanek::CreateMap() {
    if (fSource == NULL) { Cout::PrintInfo("No source emission funciont", EInfo::kCriticalError); }
    if (fMaps.size() > 0) return;
    Double_t r_min = fParameters[Radius()].GetMin();
    Double_t r_max = fParameters[Radius()].GetMax();
    Double_t d_r   = r_max - r_min;
    d_r            = d_r / ((Double_t) fExtrapolationsSteps * 2.0);
    r_min          = r_min - d_r;
    r_max          = r_max + d_r;
    TH2D* MAP      = new TH2D("new_map",
                         "new_map",
                         fFSIMap->GetNbinsX(),
                         fFSIMap->GetXaxis()->GetBinLowEdge(1),
                         fFSIMap->GetXaxis()->GetBinUpEdge(fFSIMap->GetNbinsX()),
                         fExtrapolationsSteps + 1,
                         r_min,
                         r_max);
    Int_t R_Map    = fFSIMap->GetNbinsY();
    TAxis* R_Ax    = fFSIMap->GetYaxis();
    Double_t* prob = new Double_t[R_Map];
    for (int j = 1; j <= MAP->GetNbinsY(); j++) {  // loop over R
      Double_t R    = MAP->GetYaxis()->GetBinCenter(j);
      Double_t infs = 0;
      fSource->FixParameter(0, R);
      if (fSource->GetNpar() == 2) { fSource->FixParameter(1, 1); }
      Double_t sum = 0;
      for (int a = 0; a < R_Map; a++) {  // calculate density probability for given R
        // prob[a] = fSource->Eval(R_Ax->GetBinCenter(a+1));
        prob[a] = fSource->Integral(R_Ax->GetBinLowEdge(a + 1), R_Ax->GetBinUpEdge(a + 1));
        sum += prob[a];
      }

      infs += fSource->Integral(-1000, R_Ax->GetBinLowEdge(1));
      infs += fSource->Integral(R_Ax->GetBinUpEdge(R_Ax->GetNbins()), 1000);
      sum += infs;
      for (int a = 0; a < R_Map; a++) {  // calculate density probability for given R
        prob[a] = prob[a] / sum;
      }
      for (int i = 1; i <= MAP->GetNbinsX(); i++) {
        Double_t val = 0;
        for (int k = 0; k < R_Map; k++) {
          val += prob[k] * fFSIMap->GetBinContent(i, k + 1);
        }
        MAP->SetBinContent(i, j, val + infs);
      }
    }

    CorrFitMapKstarRstar* map = new CorrFitMapKstarRstar(*MAP, Femto::EKinematics::kPRF);
    map->Recalc();
    fMaps.push_back(map);
    delete[] prob;
    delete MAP;
  }

  void CorrFitWielanek::SetSourceEmissionFunction(TF1* func) {
    if (fSource) {
      Cout::PrintInfo("cannot set more source emission functions", EInfo::kWarning);
      return;
    }
    fSource = func;
  }

  void CorrFitWielanek::SetSourceEmissionFunstion(Option_t* opt) {
    if (fSource) {
      Cout::PrintInfo("cannot set more source emission functions", EInfo::kLowWarning);
      return;
    } else {
      TString option = opt;
      if (option == "gaus") {
        fSource = new TF1("gaus", "[1]*x*x*exp(-x*x/[0]/[0]/4)", 2);
      } else {
        Cout::PrintInfo(Form("Unkonw option in %s", this->ClassName()), EInfo::kLowWarning);
      }
    }
  }

  void CorrFitWielanek::SetExtrapolationsSteps(Int_t /*step*/) {}

  CorrFitWielanek::~CorrFitWielanek() {
    if (fFSIMap) delete fFSIMap;
    if (fSource) delete fSource;
  }
}  // namespace Hal
