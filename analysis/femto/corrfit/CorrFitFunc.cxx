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
#include "CorrFitGUI.h"
#include "CorrFitHDFunc.h"
#include "CorrFitPainter.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"
#include "Minimizer.h"
#include "Painter.h"
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
#include <fstream>
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
    ROOT::Math::Minimizer *min, *min2 = nullptr;
    CheckOrder();
    fNDF = CountNDF();
    min  = GetMinimizer1(fMinAlgo);
    min2 = GetMinimizer2(fMinAlgo);
    ROOT::Math::Functor f;
    switch (fMinFunc) {
      case kChi: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTFD, GetParametersNo()); break;
      case kChi2: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTF, GetParametersNo()); break;
      case kLog: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorLogTFD, GetParametersNo()); break;
      default: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTFD, GetParametersNo()); break;
    }
    min->SetFunction(f);
    if (min2) min2->SetFunction(f);
    min->Minimize();

    if (min2) {
      if (Hal::Cout::GetVerboseMode() <= Hal::EInfo::kInfo || fTrace) std::cout << "Switching to second minimizer " << std::endl;
      const double* parameters_guess = min->X();
      const double* errors_guess     = min->Errors();
      PrepareSecondMiminizer(min, parameters_guess, errors_guess);
      SetParsOfMinimizer(min2);
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
    auto testParam = [&](Int_t par, Int_t& step, Double_t& min, Double_t& max) {
      auto& parval = fParameters[par];
      min          = Std::Discretize(parval.GetNPoints() - 1, parval.GetMapMin(), parval.GetMapMax(), min, '-');
      max          = Std::Discretize(parval.GetNPoints() - 1, parval.GetMapMin(), parval.GetMapMax(), max, '+');
      if (min < parval.GetMapMin()) min = parval.GetMapMin();
      if (max > parval.GetMapMax()) max = parval.GetMapMax();
      double step_size = parval.GetDParam();
      min -= step_size * 0.5;
      max += step_size * 0.5;
      step = std::round((max - min) / parval.GetDParam());
    };


    if (IsParDiscrete(par1)) { testParam(par1, par1_steps, par1_min, par1_max); }
    if (IsParDiscrete(par2)) { testParam(par2, par2_steps, par2_min, par2_max); }

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
    if (fPainter) delete fPainter;
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
      if (fParameters[i].IsFixed()) {
        fParameters[i].SetFittedValue(fParameters[i].GetMin());
      } else
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

  void CorrFitFunc::PrepareHalMinimizer() const {
    Minimizer* min = Minimizer::Instance();
    min->SetTrace(fTrace);
    min->Reset();
    min->SetNDF(fNDF);
    if (fMinAlgo == EMinAlgo::kHalAnt) {
      min->SetMinimizerType("ant");
    } else {
      min->SetMinimizerType("scan");
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
    if (fDiscretteMinimzerConf.GetNParams() != 0) {
      MinimizerStepConf conf(fDiscretteMinimzerConf, fFitOrder);
      min->SetParamConf(conf, false);
    } else {
      HalCoutDebug("Wrong discrete conf ,using default");
    }
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

  void CorrFitFunc::PrepareRootMinimizer(ROOT::Math::Minimizer* min) const {
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

  void CorrFitFunc::PreFit(TObject* histo, Double_t bins) {
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
    NumericalPreMinimization(bins);
  }

  void CorrFitFunc::NumericalPreMinimization(Double_t bins) {
    ROOT::Math::Minimizer* min = nullptr;
    CheckOrder();
    min = Minimizer::Instance();
    static_cast<Minimizer*>(min)->SetMinimizerType("scan");
    fNDF = CountNDF();
    PrepareHalMinimizer();
    SetParsOfMinimizer(min);
    auto minx = static_cast<Minimizer*>(min);
    minx->SetNDF(fNDF);

    ROOT::Math::Functor f;
    switch (fMinFunc) {
      case kChi: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTFD, GetParametersNo()); break;
      case kChi2: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTF, GetParametersNo()); break;
      case kLog: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorLogTFD, GetParametersNo()); break;
      default: f = ROOT::Math::Functor(this, &CorrFitFunc::FunctorChiTFD, GetParametersNo()); break;
    }
    min->SetFunction(f);

    min->Minimize();
    const double* parameters = min->X();

    if (bins == 0) {
      for (int i = 0; i < GetParametersNo(); i++) {
        if (!fParameters[i].IsFixed()) {
          std::cout << "Estimated minimum of parameter " << fParameters[i].GetName() << " is " << Form("%4.4f", parameters[i]);
        }
      }
    } else if (bins > 0) {
      double scale = bins;
      for (int i = 0; i < GetParametersNo(); i++) {
        if (!fParameters[i].IsFixed()) {
          FitParam p = minx->GetParConf(i);
          p.Init();
          double dx = p.GetDParam();
          SetParLimits(i, parameters[i] - scale * dx, parameters[i] + scale * dx);
          std::cout << "PRESCALED " << p.GetParName() << " " << (parameters[i] - scale * dx) << " "
                    << (parameters[i] + scale * dx) << std::endl;
        }
      }
    } else if (bins < 0) {
      for (int i = 0; i < GetParametersNo(); i++) {
        if (!fParameters[i].IsFixed()) {
          FitParam p = minx->GetParConf(i);
          p.Init();
          double dx = p.GetDParam();
          SetParLimits(i, parameters[i] - dx, parameters[i] + dx);
          MinimizerStepConf conf;
          double npoints = dx / bins + 1;
          conf.ConfigureParameter(p.GetParName(), npoints, parameters[i] - dx, parameters[i] + dx);
          minx->SetParamConf(conf, false);
        }
      }
    }
    //=== COPY FROM NUMERICAL FUNCTION TO "FITTED" FUNCTION
    for (int i = 0; i < GetParametersNo(); i++) {
      int to = fFitOrder[i];
      fParameters[to].SetFittedValue(parameters[i]);
      // fParameters[to].SetError(errors[i]);
      fTempParamsEval[to] = parameters[i];
    }

    ParametersChanged();
    fChi[0] = GetChiTF(parameters);
  }

  void CorrFitFunc::SetMinimizerConf(TString xmlFile) { fDiscretteMinimzerConf.LoadFromXML(xmlFile); }

  void CorrFitFunc::MakeDummyXMLConfig(TString xmlFile) {
    std::ofstream file(xmlFile);
    file << "<minimizer>" << std::endl;
    for (int i = 0; i < GetParametersNo(); i++) {
      file << "<param name=\"" << GetParameterName(i) << "\" min=\"" << GetParMin(i) << "\" max=\"" << GetParMax(i)
           << "\" step=\"0.01\"></param>" << std::endl;
    }
    file << "</minimizer>" << std::endl;
    file.close();
  }

  void CorrFitFunc::Draw(Option_t* option) {
    bool painter_set = true;
    if (!fPainter) {
      MakePainter(option);
      painter_set = false;
      if (fCF) {  // we have our CF, and it was painted, let's link painter
                  /* HalCoutDebug("LINKING TO CF");
                   auto div     = (Hal::DividedHisto1D*) fCF;
                   auto painter = (Hal::Painter*) div->GetSpecial("painter");
                   if (painter) {
                     painter->AddPainter(fPainter);
                     HalCoutDebug("LINKING TO CF");
                   } else
                     Hal::Cout::PrintInfo(
                       Form("%s %i, CF is present but there is not painter, call cf->Draw() before func->Draw()", __FILE__, __LINE__),
                       EInfo::kWarning);*/
      }
    }
    fPainter->SetOption(option);
    fPainter->Paint();
  }

  void CorrFitFunc::Repaint() {
    if (!fPainter) {
      Draw("skip");
      return;
    }
    fPainter->Paint();

#ifdef __APPLE__
    for (int ispe = 0; ispe < 2; ispe++) {
      if (gSystem->ProcessEvents()) break;
    }
#endif
  }

  void CorrFitFunc::SetParsOfMinimizer(ROOT::Math::Minimizer* min) const {
    min->SetMaxFunctionCalls(fMaxIterations);
    min->SetMaxIterations(fMaxIterations);
    min->SetTolerance(fTolerance);
  }

  Int_t CorrFitFunc::CountNDF() const {
    Int_t freeParameters = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!fParameters[i].IsFixed()) freeParameters++;
    }
    return fActiveBins - freeParameters;
  }

  ROOT::Math::Minimizer* CorrFitFunc::GetMinimizer1(EMinAlgo algo) const {
    auto algos = AlgoToOptions(fMinAlgo);
    if (algos.size() < 2) algos.push_back("");
    TString pat1               = algos[0];
    TString pat2               = algos[1];
    ROOT::Math::Minimizer* min = nullptr;
    if (pat1.EqualTo("HalMinimizer")) {
      min = Minimizer::Instance();
      static_cast<Minimizer*>(min)->SetMinimizerType(pat2);
      static_cast<Minimizer*>(min)->SetNDF(fNDF);
      PrepareHalMinimizer();
    } else {
      min = ROOT::Math::Factory::CreateMinimizer(pat1.Data(), pat2.Data());
      PrepareRootMinimizer(min);
    }
    SetParsOfMinimizer(min);
    return min;
  }

  ROOT::Math::Minimizer* CorrFitFunc::GetMinimizer2(EMinAlgo algo) const {
    auto algos = AlgoToOptions(fMinAlgo);
    if (algos.size() <= 3) return nullptr;
    TString pat1               = algos[2];
    TString pat2               = algos[3];
    ROOT::Math::Minimizer* min = nullptr;
    if (pat1.EqualTo("HalMinimizer")) {
      Hal::Cout::PrintInfo("You should not use hal minimizer as second minimizer", EInfo::kWarning);
      min = Minimizer::Instance();
      static_cast<Minimizer*>(min)->SetMinimizerType(pat2);
      static_cast<Minimizer*>(min)->SetNDF(fNDF);
      PrepareHalMinimizer();
    } else {
      min = ROOT::Math::Factory::CreateMinimizer(pat1.Data(), pat2.Data());
      PrepareRootMinimizer(min);
    }
    SetParsOfMinimizer(min);
    return min;
  }

  void CorrFitFunc::PrepareSecondMiminizer(ROOT::Math::Minimizer* min,
                                           const double* parameters_guess,
                                           const double* errors_guess) const {
    for (int i = 0; i < GetParametersNo(); i++) {
      auto Param = fParameters[fFitOrder[i]];
      if (Param.IsFixed()) {
        min->SetFixedVariable(i, Param.GetParName().Data(), Param.GetStartVal());
      } else {
        Double_t param  = parameters_guess[i];
        Double_t minpar = param - 3.0 * errors_guess[i];
        Double_t maxpar = param + 3.0 * errors_guess[i];
        minpar          = TMath::Max(Param.GetMin(), minpar);
        maxpar          = TMath::Min(Param.GetMax(), maxpar);
        Double_t step   = (maxpar - minpar) * 0.1;
        min->SetLimitedVariable(i, Param.GetParName().Data(), parameters_guess[i], step, minpar, maxpar);
        if (Hal::Cout::GetVerboseMode() >= Hal::EInfo::kInfo)
          std::cout << "Set limits " << Param.GetParName() << "\t"
                    << Form(" %4.4f+/-%4.4f", parameters_guess[i], 3.0 * errors_guess[i]) << std::endl;
      }
    }
  }

  Hal::CorrFitGUI* CorrFitFunc::StartGui(Int_t prec) { return new CorrFitGUI(this, prec); }

}  // namespace Hal
