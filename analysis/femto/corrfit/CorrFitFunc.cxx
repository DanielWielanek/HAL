/*
 * CorrFit.cxx
 *
 *  Created on: 08-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitFunc.h"

#include "ChiSqMap2D.h"
#include "CorrFitHDFunc.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"
#include "Minimizer.h"
#include "Splines.h"
#include "Std.h"
#include "StdString.h"

#include <Math/Factory.h>
#include <Math/Functor.h>
#include <Math/Minimizer.h>
#include <TAxis.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TRegexp.h>
#include <TVirtualPad.h>
#include <iostream>
#include <string>
#include <vector>


namespace Hal {
  CorrFitFunc::CorrFitFunc(Int_t nparams, Int_t dim) :
    CorrFit(nparams),
    fDim(dim),
    fMaxIterations(10000),
    fTolerance(0.001),
    fActiveBins(0),
    fTrace(kFALSE),
    fOwnRangeMap(kFALSE),
    fCF(nullptr),
    fDenominatorHistogram(nullptr),
    fNumeratorHistogram(nullptr),
    fCorrelationFunctionHistogram(nullptr),
    fMask(nullptr),
    fHDMaps(nullptr) {
    fChi[0] = fChi[1] = 0.0;
    fKinematics       = Femto::EKinematics::kLCMS;
    fRange.MakeBigger(fDim * 2);
    for (int i = 0; i < fDim * 2; i++) {
      if (i % 2 == 0)
        fRange[i] = 0;
      else
        fRange[i] = 1;
    }
  }

  Double_t CorrFitFunc::GetChiSquare(Option_t* opt) const {
    TString option = opt;
    if (option.EqualTo("fitted")) {
      return fChi[0];
    } else if (option.EqualTo("fitting")) {
      return fChi[1];
    }
    return 0;
  }

  Double_t CorrFitFunc::GetChiNDF(Option_t* opt) const { return GetChiSquare(opt) / GetNDF(); }

  void CorrFitFunc::SetRange(Double_t min, Double_t max) {
    if (fDim == 1) {
      fRange[0] = min;
      fRange[1] = max;
    }
    if (fDim == 3) {
      fRange[0] = fRange[2] = fRange[4] = min;
      fRange[1] = fRange[3] = fRange[5] = max;
    }
  }

  void CorrFitFunc::Draw(Option_t* draw_option) {
    TString drawOpts = draw_option;
    if (fDrawFunc.size() > 0) {
      std::cout << "CorrFitFunc::Draw Canot set CorrFitDrawOptions when function was drawn options were set" << std::endl;
    }
    if (!fDrawOptionSet && drawOpts.Length() > 0) {
      fDrawOptions   = CorrFitDrawOptions(draw_option);
      fDrawOptionSet = kTRUE;
    }
    Paint(kFALSE, kTRUE);
  }

  void CorrFitFunc::SetDrawOption(const CorrFitDrawOptions& options) {
    if (fDrawOptionSet || fDrawFunc.size() > 0) {
      std::cout << "Canot set CorrFitDrawOptions when function was set or draw options were set" << std::endl;
      return;
    }
    fDrawOptions   = options;
    fDrawOptionSet = kTRUE;
  }

  void CorrFitFunc::SetupFunction(TF1* f) const {
    for (int i = 0; i < GetParametersNo(); i++) {
      f->SetParName(i, fParameters[i].GetName());
      if (fParameters[i].IsFixed()) {
        f->FixParameter(i, fParameters[i].GetStartVal());
      } else {
        f->SetParLimits(i, fParameters[i].GetMin(), fParameters[i].GetMax());
      }
    }
    f->SetRange(fRange.Get(0), fRange.Get(2), fRange.Get(4), fRange.Get(1), fRange.Get(3), fRange.Get(5));
  }

  void CorrFitFunc::Prepare() {
    fNDF    = 0;
    fChi[0] = 0;
    fChi[1] = 0;
  }

  void CorrFitFunc::Check() {
    for (int i = 0; i < fDim; i++) {
      if (fRange[2 * i] == fRange[2 * i + 1]) {  // ranges on given axis are the same, fixing
        switch (i) {
          case 0:  // one dim
            fRange[0] = fCorrelationFunctionHistogram->GetXaxis()->GetXmin();
            fRange[1] = fCorrelationFunctionHistogram->GetXaxis()->GetXmax();
            break;
          case 1:  // two dims
            fRange[2] = fCorrelationFunctionHistogram->GetYaxis()->GetXmin();
            fRange[3] = fCorrelationFunctionHistogram->GetYaxis()->GetXmax();
            break;
          case 2:  // three dims
            fRange[4] = fCorrelationFunctionHistogram->GetZaxis()->GetXmin();
            fRange[5] = fCorrelationFunctionHistogram->GetZaxis()->GetXmax();
            break;
        }
      }
    }
  }

  void CorrFitFunc::CalcError(const Double_t Num,
                              const Double_t NumErr,
                              const Double_t Den,
                              const Double_t DenErr,
                              Double_t& cf,
                              Double_t& cfe) const {
    cf          = Num / Den;
    Double_t D2 = Den * Den;
    cfe         = TMath::Sqrt((NumErr * NumErr + Num * Num / D2 * DenErr) / D2);
  }

  void CorrFitFunc::NumericalMinimization() {
    TString opt1, opt2;
    AlgoToOptions(fMinAlgo, opt1, opt2);
    ROOT::Math::Minimizer *min, *min2 = nullptr;
    auto setPars = [&](ROOT::Math::Minimizer* minn) {
      minn->SetMaxFunctionCalls(fMaxIterations);
      minn->SetMaxIterations(fMaxIterations);
      minn->SetTolerance(fTolerance);
    };
    Bool_t custom_minimizer = kFALSE;
    CheckOrder();
    if (fMinAlgo == kDefaultAlgo) {
      min = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Scan");
    } else if (fMinAlgo == kMinimizerScan || fMinAlgo == kMinimizerSmartScan) {
      min = Minimizer::Instance();
      static_cast<Minimizer*>(min)->SetMinimizerType(opt2);
      PrepareMinimizer();
      custom_minimizer = kTRUE;
    } else {
      min = ROOT::Math::Factory::CreateMinimizer(opt1.Data(), opt2.Data());
    }
    setPars(min);
    Double_t free_parameters = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!fParameters[i].IsFixed()) free_parameters++;
    }
    fNDF = fActiveBins - free_parameters;
    if (custom_minimizer) {
      static_cast<Minimizer*>(min)->SetNDF(fNDF);
    } else
      PrepareRootMinimizer(min);
    ROOT::Math::Functor f;
    switch (fMinFunc) {
      case kChi: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTFD, GetParametersNo()); break;
      case kChi2: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTF, GetParametersNo()); break;
      case kLog: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorLogTFD, GetParametersNo()); break;
      default: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTFD, GetParametersNo()); break;
    }
    min->SetFunction(f);

    min->Minimize();
    if (fMinAlgo == kDefaultAlgo) {  // now call midgrad
      if (fTrace) std::cout << "Switching to migrad" << std::endl;
      min2 = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
      min2->SetFunction(f);
      const double* parameters_guess = min->X();
      const double* errors_guess     = min->Errors();
      for (int i = 0; i < GetParametersNo(); i++) {
        auto Param = fParameters[fFitOrder[i]];
        if (Param.IsFixed()) {
          min2->SetFixedVariable(i, Param.GetParName().Data(), Param.GetStartVal());
        } else {
          Double_t param  = parameters_guess[i];
          Double_t minpar = param - 3.0 * errors_guess[i];
          Double_t maxpar = param + 3.0 * errors_guess[i];
          minpar          = TMath::Max(Param.GetMin(), minpar);
          maxpar          = TMath::Min(Param.GetMax(), maxpar);
          Double_t step   = (maxpar - minpar) * 0.1;
          min2->SetLimitedVariable(i, Param.GetParName().Data(), parameters_guess[i], step, minpar, maxpar);
          if (fTrace)
            std::cout << "Set limits " << Param.GetParName() << "\t"
                      << Form(" %4.4f+/-%4.4f", parameters_guess[i], 3.0 * errors_guess[i]) << std::endl;
        }
      }
      setPars(min2);
      min2->Minimize();
    }

    if (min2 == nullptr) min2 = min;

    const double* parameters = min2->X();
    const double* errors     = min2->Errors();
    /* MINOS ERROR ESTIMATION
     if(true){
     for(int ivar=0;ivar<GetParametersNo();ivar++){
     double errLow, errUp;
     min2->GetMinosError(ivar, errLow, errUp);
     std::cout<<ivar<<"OE \t"<<errors[ivar]<<std::endl;
     std::cout<<ivar<<"NE\t"<<TMath::Max(errLow,errUp)<<std::endl;
     }

     }
     */
    //=== COPY FROM NUMERICAL FUNCTION TO "FITTED" FUNCTION
    for (int i = 0; i < GetParametersNo(); i++) {
      int to = fFitOrder[i];
      fParameters[to].SetFittedValue(parameters[i]);
      fParameters[to].SetError(errors[i]);
      fTempParamsEval[to] = parameters[i];
    }

    ParametersChanged();
    fChi[0] = GetChiTF(parameters);
  }

  ChiSqMap2D*
  CorrFitFunc::GetChiSquareMap(Int_t par1, Int_t par1_steps, Int_t par2, Int_t par2_steps, Bool_t scale, EMinFunc algo) const {
    return GetChiSquareMap(par1,
                           par1_steps,
                           fParameters[par1].GetMin(),
                           fParameters[par1].GetMax(),
                           par2,
                           par2_steps,
                           fParameters[par1].GetMin(),
                           fParameters[par2].GetMax(),
                           scale,
                           algo);
  }

  ChiSqMap2D* CorrFitFunc::GetChiSquareMap(Int_t par1,
                                           Int_t par1_steps,
                                           Double_t par1_min,
                                           Double_t par1_max,
                                           Int_t par2,
                                           Int_t par2_steps,
                                           Double_t par2_min,
                                           Double_t par2_max,
                                           Bool_t scale,
                                           EMinFunc algo) const {
    // TF1 *func = GetFittingFunction("TF");
    if (IsParDiscrete(par1)) {
      par1_min = Hal::Std::Discretize(
        fParameters[par1].GetNPoints(), fParameters[par1].GetMapMin(), fParameters[par1].GetMapMax(), par1_min, '-');
      par1_max = Hal::Std::Discretize(
        fParameters[par1].GetNPoints(), fParameters[par1].GetMapMin(), fParameters[par1].GetMapMax(), par1_max, '+');
      if (par1_min < fParameters[par1].GetMapMin()) { par1_min = fParameters[par1].GetMapMin(); }
      if (par1_max > fParameters[par1].GetMapMax()) { par1_max = fParameters[par1].GetMapMax(); }
      par1_steps = (par1_max - par1_min) / fParameters[par1].GetDParam() + 1.;
    }
    if (IsParDiscrete(par2)) {
      par2_min = Hal::Std::Discretize(
        fParameters[par2].GetNPoints(), fParameters[par2].GetMapMin(), fParameters[par2].GetMapMax(), par2_min, '-');
      par1_max = Hal::Std::Discretize(
        fParameters[par2].GetNPoints(), fParameters[par2].GetMapMin(), fParameters[par2].GetMapMax(), par2_max, '+');
      if (par2_min < fParameters[par2].GetMapMin()) { par2_min = fParameters[par2].GetMapMin(); }
      if (par2_max > fParameters[par2].GetMapMax()) { par2_max = fParameters[par2].GetMapMax(); }
      par2_steps = (par2_max - par2_min) / fParameters[par2].GetDParam() + 1.;
    }

    Double_t* params = new Double_t[GetParametersNo()];
    TString var;
    switch (algo) {
      case kChi: var = "#chi^{2}_{den}"; break;
      case kLog: var = "loglike"; break;
      case kChi2: var = "#chi^{2}"; break;
    }
    TString title = var + "map";
    if (scale) { title = var + " map (scaled)"; }
    ChiSqMap2D* map = new ChiSqMap2D("chi_map", par1_steps, par1_min, par1_max, par2_steps, par2_min, par2_max);
    map->SetParNames(fParameters[par1].GetParName(), fParameters[par2].GetParName());
    Double_t bins, chi;
    for (int i = 0; i < GetParametersNo(); i++) {
      fTempParamsEval[i] = GetParameter(i);
    }
    for (double i = 1; i <= par1_steps; i++) {
      Double_t par1_val = map->GetHist()->GetXaxis()->GetBinCenter(i);
      for (double j = 1; j <= par2_steps; j++) {
        Double_t par2_val     = map->GetHist()->GetYaxis()->GetBinCenter(j);
        fTempParamsEval[par1] = par1_val;
        fTempParamsEval[par2] = par2_val;
        ParametersChanged();
        switch (algo) {
          case kChi: {
            chi = GetChiTFD(params);
          } break;
          case kChi2: {
            chi = GetChiTF(params);
          } break;
          case kLog: {
            chi = GetLogTFD(params);
          } break;
          default: chi = 0; break;
        }
        bins = fActiveBins - GetFreeParamsNo();
        if (scale) {
          // std::cout<<"SC "<<chi/bins<<std::endl;
          map->GetHist()->SetBinContent(i, j, chi / bins);
        } else {
          // std::cout<<"NSC "<<chi<<std::endl;
          map->GetHist()->SetBinContent(i, j, chi);
        }
      }
    }
    map->SetXFit(GetParameter(par1), GetParError(par1), GetParError(par1));
    map->SetYFit(GetParameter(par2), GetParError(par2), GetParError(par2));
    fTempParamsEval[par1] = GetParameter(par1);
    fTempParamsEval[par2] = GetParameter(par2);
    /*prevent root from drawing CF with current parameters*/
    ParametersChanged();
    delete[] params;
    return map;
  }

  CorrFitFunc::~CorrFitFunc() {
    if (fMask) delete fMask;
    if (fHDMaps) delete fHDMaps;
    fDenominatorHistogram = nullptr;
    if (fCorrelationFunctionHistogram) delete fCorrelationFunctionHistogram;
    fCorrelationFunctionHistogram = nullptr;
    if (fLegend) delete fLegend;
    for (auto& i : fDrawHistograms)
      delete i;
  }

  Int_t CorrFitFunc::GetFreeParamsNo() const {
    Int_t free = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!IsParFixed(i)) free++;
    }
    return free;
  }

  double CorrFitFunc::FunctorChiTFD(const double* params) {
    for (int i = 0; i < GetParametersNo(); i++)
      fTempParamsEval[i] = params[i];
    ParametersChanged();
    Double_t f = GetChiTFD(params);
    if (fTrace) {
      std::cout << "*** <ChiTFD> ***" << f << " / " << fActiveBins << std::endl;
      for (int i = 0; i < GetParametersNo(); i++) {
        std::cout << "\t" << GetParameterName(i) << Form("%4.5f", params[i]) << std::endl;
      }
    }
    return f;
  }

  double CorrFitFunc::FunctorChiTF(const double* params) {
    for (int i = 0; i < GetParametersNo(); i++)
      fTempParamsEval[i] = params[i];
    ParametersChanged();
    Double_t f = GetChiTF(params);
    if (fTrace) {
      std::cout << "*** <ChiTF> ***" << f << " / " << fActiveBins << std::endl;
      for (int i = 0; i < GetParametersNo(); i++) {
        std::cout << "\t" << GetParameterName(i) << Form("%4.5f", params[i]) << std::endl;
      }
    }
    return f;
  }

  double CorrFitFunc::FunctorLogTFD(const double* params) {
    for (int i = 0; i < GetParametersNo(); i++)
      fTempParamsEval[i] = params[i];
    ParametersChanged();
    Double_t f = GetLogTFD(params);
    if (fTrace) {
      std::cout << "*** <LogTFD> ***" << f << " / " << fActiveBins << std::endl;
      for (int i = 0; i < GetParametersNo(); i++) {
        std::cout << "\t" << GetParameterName(i) << Form("%4.5f", params[i]) << std::endl;
      }
    }
    return f;
  }

  void CorrFitFunc::DummyNumericalFunction() {
    // calculate NDF
    for (int i = 0; i < GetParametersNo(); i++) {
      fParameters[i].SetError(0);
      fParameters[i].SetFittedValue(fParameters[i].GetStartVal());
      fTempParamsEval[i] = fParameters[i].GetFittedValue();
    }
  }

  Double_t CorrFitFunc::GetRangeMin(Int_t flag) const {
    if (flag < fDim) return fRange.Get(flag * 2);
    return 0;
  }

  Double_t CorrFitFunc::GetRangeMax(Int_t flag) const {
    if (flag < fDim) return fRange.Get(flag * 2 + 1);
    return 0;
  }

  void CorrFitFunc::PrepareMinimizer() {
    Minimizer* min = Minimizer::Instance();
    min->SetTrace(fTrace);
    min->Reset();
    if (fMinAlgo == EMinAlgo::kMinimizerScan) {
      min->SetMinimizerType("scan");
    } else {
      min->SetMinimizerType("ant");
    }
    for (int i = 0; i < GetParametersNo(); i++) {
      auto Param       = fParameters[fFitOrder[i]];
      std::string name = Param.GetParName().Data();
      if (Param.IsFixed()) {
        min->SetFixedVariable(i, name, Param.GetStartVal());
      } else {
        Double_t lower = Param.GetMin();
        Double_t upper = Param.GetMax();
        min->SetLimitedVariable(i, name, 0.5 * (lower + upper), (upper - lower), lower, upper);
      }
    }
    MinimizerStepConf conf(fDiscretteMinimzerConf, fFitOrder);
    min->SetParamConf(conf);
  }

  void CorrFitFunc::Fit(TObject* histo) {
    if (histo == fCF) {
      Prepare();
    } else {
      fCF = histo;
      PrepareRaw();
    }
    // checking ranges &other stuff
    Check();
    // set par names depending on frame
    // init params
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      fParameters[i].Init();
    }
    EstimateActiveBins();
    if (fMinAlgo == kDefaultAlgo) fMinAlgo = kMinuitScan;
    NumericalMinimization();
  }

  void CorrFitFunc::FitDummy(TObject* histo) {
    if (fCF != histo) {
      fCF = histo;
      PrepareRaw();
    } else {
      Prepare();
    }
    // checking ranges &other stuff
    Check();
    // set par names depending on frame
    // init params
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      fParameters[i].Init();
    }
    EstimateActiveBins();
    if (fMinAlgo == kDefaultAlgo) fMinAlgo = kMinuitScan;
    DummyNumericalFunction();
  }

  void CorrFitFunc::UpdateLegend() {
    if (fDrawOptions.DrawLegend() == kFALSE) return;
    TString chi_label = "";
    if (fDrawOptions.Chi2()) {
      if (fLegend) fChi[0] = GetChiTF(fTempParamsEval);  // legend present, we have to recalcuate chi2
      TString chi_s, chindf_s, ndf_s;
      Double_t chi    = GetChiSquare();
      Double_t chindf = GetChiNDF();
      Double_t ndf    = GetNDF();
      if (chi <= 1000)
        chi_s = Form("%4.3f", chi);
      else
        chi_s = Hal::Std::RoundToString(chi, 2, "prefix");
      if (chindf <= 1000) {
        chindf_s = Form("%4.3f", chindf);
      } else {
        chindf_s = Hal::Std::RoundToString(chindf, 2, "prefix");
      }
      if (ndf <= 1000) {
        ndf_s = Form("%i", (int) ndf);
      } else {
        ndf_s = Hal::Std::RoundToString(ndf, 2, "prefix");
      }
      chi_label = Form("#chi^{2}/NDF %s (%s/%s)", chindf_s.Data(), chi_s.Data(), ndf_s.Data());
    }
    std::vector<TString> label;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (IsParFixed(i)) {
        label.push_back(Form("%s %4.3f (fixed)", fParameters[i].GetParName().Data(), fParameters[i].GetFittedValue()));
      } else {
        if (TMath::IsNaN(fParameters[i].GetError())) {
          label.push_back(Form("%s %4.3f#color[2]{#pm%4.3f}",
                               fParameters[i].GetParName().Data(),
                               fParameters[i].GetFittedValue(),
                               fParameters[i].GetError()));
        } else if (fParameters[i].GetError() < 0) {
          label.push_back(Form("%s %4.3f#color[16]{#pm%4.3f}",
                               fParameters[i].GetParName().Data(),
                               fParameters[i].GetFittedValue(),
                               fParameters[i].GetError()));
        } else {
          label.push_back(Form(
            "%s %4.3f#pm%4.3f", fParameters[i].GetParName().Data(), fParameters[i].GetFittedValue(), fParameters[i].GetError()));
        }
      }
    }
    if (chi_label.Length() > 0) label.push_back(chi_label);
    if (fLegend == nullptr) {
      if (fDrawOptions.LegendPos()) {
        fLegend = new TLegend(
          fDrawOptions.GetLegendPos(0), fDrawOptions.GetLegendPos(1), fDrawOptions.GetLegendPos(2), fDrawOptions.GetLegendPos(3));
      } else {
        fLegend = new TLegend(0.7, 0.7, 0.95, 0.95);
      }
      fLegend->SetHeader(GetName());
      for (auto str : label) {
        fLegendEntries.push_back(fLegend->AddEntry((TObject*) 0x0, str, ""));
      }
    } else {
      for (unsigned int i = 0; i < label.size(); i++) {
        TLegendEntry* ent = fLegendEntries[i];
        ent->SetLabel(label[i]);
        ent->SetTextColor(kGreen);
      }
    }
  }

  void CorrFitFunc::PrepareRootMinimizer(ROOT::Math::Minimizer* min) {
    for (int i = 0; i < GetParametersNo(); i++) {
      auto Param = fParameters[fFitOrder[i]];
      if (Param.IsFixed()) {
        if (TMath::IsNaN(Param.GetStartVal())) {
          Cout::Text(Form(" Par No. %i Is Nan fixed parameter", fFitOrder[i]), "M", kRed);
        }
        min->SetFixedVariable(i, Param.GetParName().Data(), Param.GetStartVal());
      } else {
        if (TMath::IsNaN(Param.GetStartVal())) { Cout::Text(Form(" Par No. %i Is Nan parameter", fFitOrder[i]), "M", kRed); }
        Double_t step = TMath::Max(Param.GetDParam(), (Param.GetMax() - Param.GetMin()) / 100.0);
        min->SetLimitedVariable(i, Param.GetParName().Data(), Param.GetMin(), step, Param.GetMin(), Param.GetMax());
      }
    }
  }

}  // namespace Hal
