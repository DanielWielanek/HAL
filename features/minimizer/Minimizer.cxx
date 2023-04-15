/*
 * HalMinimizer.cxx
 *
 *  Created on: 7 lis 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Minimizer.h"

#include "Array.h"
#include "Cout.h"
#include "Std.h"
#include <Math/IFunctionfwd.h>
#include <RtypesCore.h>
#include <TMath.h>
#include <TMatrixD.h>
#include <TString.h>
#include <fstream>
#include <string>

namespace Hal {
  Minimizer* Minimizer::fgInstance = NULL;
  Minimizer::Minimizer() :
    fFunc(nullptr),
    fMinimizeType(kScan),
    fNCalls(0),
    fDiscreteFit(kFALSE),
    fTrace(kFALSE),
    fGlobMin(1.),
    fNDF(0),
    fQuantumFits(nullptr),
    fSmoothFits(nullptr),
    fErrors(nullptr),
    fTempParams(nullptr) {}

  // set param methods

  bool Minimizer::SetFixedVariable(unsigned int ivar, const std::string& name, double val) {
    if (fParameters.size() <= ivar) { fParameters.resize(ivar + 1); }
    fParameters[ivar].SetParName(name);
    fParameters[ivar].SetRange(val, val);
    fParameters[ivar].SetIsFixed(kTRUE);
    return true;
  }

  bool Minimizer::SetVariableLimits(unsigned int ivar, double lower, double upper) {
    if (lower > upper) {
      double temp = upper;
      upper       = lower;
      lower       = temp;
    }
    if (fParameters.size() <= ivar) { fParameters.resize(ivar + 1); }
    fParameters[ivar].SetRange(lower, upper);
    fParameters[ivar].SetIsFixed(kFALSE);
    return true;
  }

  bool
  Minimizer::SetLimitedVariable(unsigned int ivar, const std::string& name, double val, double step, double lower, double upper) {
    if (fParameters.size() <= ivar) { fParameters.resize(ivar + 1); }
    fParameters[ivar].SetParName(name);
    fParameters[ivar].SetRange(lower, upper);
    fParameters[ivar].SetStartVal(val);
    fParameters[ivar].SetMapRange(lower, upper, TMath::Nint((upper - lower) / step + 1));
    fParameters[ivar].SetIsFixed(kFALSE);
    return false;
  }

  // recalc methods

  void Minimizer::InitFit() {
    fNonConstMap.clear();
    fAllowedValues.clear();
    fStateVector.resize(GetNParams());
    fStateVectorMin.resize(GetNParams());
    for (auto& x : fStateVector)
      x = 0;
    for (auto& x : fStateVectorMin)
      x = 0;
    fNCalls   = 0;
    fFreePars = 0;
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      if (!fParameters[i].IsFixed()) {
        fNonConstMap.push_back(i);
      } else {
        fFreePars++;
      }
    }
    auto lambda = [](Int_t a, Int_t b) -> bool { return a < b; };
    std::sort(fNonConstMap.begin(), fNonConstMap.end(), lambda);
    Cout::PrintInfo("BEFORE INIT ", EInfo::kLowWarning);
    std::cout << Cout::GetColor(kBlue);
    Cout::Database({"ParName", "MinMap", "MaxMap", "Points", "Min", "Max"});
    std::cout << Cout::GetDisableColor();
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      if (fParameters[i].IsFixed()) { std::cout << Cout::GetColor(kOrange); }
      Cout::Database({fParameters[i].GetParName().Data(),
                      Form("%4.4f", fParameters[i].GetMapMin()),
                      Form("%4.4f", fParameters[i].GetMapMax()),
                      Form("%d", fParameters[i].GetNPoints()),
                      Form("%4.4f", fParameters[i].GetMin()),
                      Form("%4.4f", fParameters[i].GetMax())});
      if (fParameters[i].IsFixed()) std::cout << Cout::GetDisableColor();
    }

    // round minimum
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      fParameters[i].SetIsDiscrete(kTRUE);  // all parameters must be discrete
      fParameters[i].Init();
      // fParameters[i].Print();
    }
    Cout::PrintInfo("AFTER INIT ", EInfo::kLowWarning);
    std::cout << Cout::GetColor(kBlue);
    Cout::Database({"ParName", "MinMap", "MaxMap", "Points", "Min", "Max"});
    std::cout << Cout::GetDisableColor();
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      if (fParameters[i].IsFixed()) { std::cout << Cout::GetColor(kOrange); }
      Cout::Database({fParameters[i].GetParName().Data(),
                      Form("%4.4f", fParameters[i].GetMapMin()),
                      Form("%4.4f", fParameters[i].GetMapMax()),
                      Form("%d", fParameters[i].GetNPoints()),
                      Form("%4.4f", fParameters[i].GetMin()),
                      Form("%4.4f", fParameters[i].GetMax())});
      if (fParameters[i].IsFixed()) std::cout << Cout::GetDisableColor();
    }
    if (fQuantumFits) {
      delete[] fQuantumFits;
      delete[] fSmoothFits;
      delete[] fErrors;
      delete[] fTempParams;
    }
    fQuantumFits = new double[GetNParams()];
    fSmoothFits  = new double[GetNParams()];
    fErrors      = new double[GetNParams()];
    fParamsMin.resize(GetNParams());
    fTempParams = new double[GetNParams()];
    for (int i = 0; i < GetNParams(); i++) {
      if (fParameters[i].IsFixed()) { fTempParams[i] = fParameters[i].GetStartVal(); }
    }
    fGlobMin = DBL_MAX;
    for (int i = 0; i < GetNParams(); i++) {
      fAllowedValues.push_back(fParameters[i].GetValuesArray());
    }
  }

  void Minimizer::Reset() { fParameters.clear(); }


  // minimize methods


  bool Minimizer::Minimize() {
    if (GetNParams() == 0) return false;
    InitFit();
    switch (fMinimizeType) {
      case eMinimizeType::kAnt: {
        /*    LoopOverParameter(0);
            for (unsigned int i = 0; i < GetNParams(); i++) {
              fTempParams[i] = fParamsMin[i];
              fSmoothFits[i] = fParamsMin[i];
            }
            std::cout << "*************************" << std::endl;
            EstimateErrors();
            FinishFit();*/
        MinimizeAnt();
      } break;
      case eMinimizeType::kScan: {
        MinimizeAnt();
        // MinimizeScan();
      } break;
    }
    FinishFit();
    return true;
  }

  const double* Minimizer::X() const {
    if (!fDiscreteFit) return fSmoothFits;
    return fQuantumFits;
  }

  void Minimizer::SetFunction(const ROOT::Math::IMultiGenFunction& func) {
    fFunc = const_cast<ROOT::Math::IMultiGenFunction*>(&func);
  }

  void Minimizer::SetParamConf(const MinimizerStepConf& conf) { conf.SetParameters(fParameters); }

  Minimizer::~Minimizer() {
    if (fQuantumFits) {
      delete[] fQuantumFits;
      delete[] fSmoothFits;
      delete[] fErrors;
      delete[] fTempParams;
    }
  }

  Bool_t Minimizer::LoopOverParameter(Int_t param) {
    /** all parameters set **/
    if ((UInt_t) param == fNonConstMap.size()) {
      Double_t chi = GetChi2();
      if (chi < fGlobMin) {
        fGlobMin        = chi;
        fStateVectorMin = fStateVector;
      }
      return kTRUE;
    }
    /** make loop over other parameters **/
    Int_t pairID = fNonConstMap[param];
    for (unsigned int i = 0; i < fAllowedValues[pairID].size(); i++) {
      fStateVector[pairID] = i;
      Bool_t val           = LoopOverParameter(param + 1);
      if (val == kFALSE) return kFALSE;
    }
    return kTRUE;
  }

  void Minimizer::EstimateErrors() {
    for (int i = 0; i < GetNParams(); i++) {
      if (IsFixed(i)) {
        fQuantumFits[i] = fParamsMin[i];
        fSmoothFits[i]  = fParamsMin[i];
        fErrors[i]      = 0;
      } else {
        Double_t par, err, parQ;
        EstimateError(i, par, parQ, err);
        fQuantumFits[i] = parQ;
        fSmoothFits[i]  = par;
        fErrors[i]      = err;
      }
    }
  }

  void Minimizer::EstimateError(Int_t par, Double_t& min, Double_t& quantumMin, Double_t& error) {

    Double_t x2   = fParamsMin[par];
    Double_t step = fParameters[par].GetDParam();
    Double_t x1   = x2 - step;
    Double_t x3   = x2 + step;
    Double_t y2   = fGlobMin / fNDF;
    Double_t y3, y1 = 0;
    if (fParameters[par].IsFixed()) {
      min        = x2;
      quantumMin = x2;
      error      = 0;
      return;
    }

    for (int i = 0; i < GetNParams(); i++)
      fTempParams[i] = fParamsMin[i];
    if (x1 >= fParameters[par].GetMapMin()) {
      fTempParams[par] = x1;
      y1               = (*fFunc)(fTempParams) / fNDF;
    }
    if (x3 > fParameters[par].GetMapMax()) {
      y3 = y1;
    } else {
      fTempParams[par] = x3;
      y3               = (*fFunc)(fTempParams) / fNDF;
    }
    if (x1 < fParameters[par].GetMapMin()) { y1 = y3; }
    fTempParams[par] = fParamsMin[par];
    Double_t a, b, c;
    Hal::Std::FitParabola(x1, x2, x3, y1, y2, y3, a, b, c);
    Double_t X1, X2;
    Hal::Std::SolveParabola(a, b, c - 0.9 * fGlobMin / fNDF, X1, X2);
    Double_t min_Fit = -b / (2.0 * a);


    //    std::cout << " pAR " << x1 << " " << x2 << " " << x3 << " " << y1 << " " << y2 << " " << y3 << " " << a << std::endl;

    quantumMin = fParamsMin[par];
    min        = min_Fit;
    error      = TMath::Max(TMath::Abs(min_Fit - X1), TMath::Abs(min_Fit - X2));
    // hessian-like method
    error = TMath::Sqrt(1.0 / a) * y2;
    return;
    /*
      // SolveEquation(x1, x2, x3, y1, y2, y3, par);
      Double_t C       = fDeriveratives[par][0];
      Double_t B       = fDeriveratives[par][1];
      Double_t A       = fDeriveratives[par][2];
      Double_t min_x   = -B / (2.0 * A);
      Double_t min_chi = min_x * B + min_x * min_x * A + C;
      min              = min_x;
      C                = C - min_chi * 1.1;
      // ten minimizer daje bledy o polowe mniejsze dlaczego?

       * przykladowo w pewnej analizie robilem skok o 1, mama chi2 znajduje minimum
       * na 4 ale jak to sie fituje parabola (jak robi to minimizer) to minimum jest
       * na 4.15 w efekcie najmniejsza wartoscia z paraboli jest 4.15 z dwoma
       * miejscami zerowymi np. 4 i 4.3 chi2 liczy odleglosc do min. w mapie i daje
       * 4 ten kod liczy odleglosc do min. w ficie parabolicznym i daje 4.15 w
       * efekcie rozrzut w przypadku 1 to 0.3 a w drugim to 0.15

      Double_t delta = TMath::Sqrt(B * B - 4. * A * C);
      Double_t xs    = (-B + delta) / (2. * A);
      error          = TMath::Abs(xs - min_x);*/
  }

  Bool_t Minimizer::IsFixed(Int_t i) const { return fParameters[i].IsFixed(); }

  Minimizer* Minimizer::Instance() {
    if (fgInstance == NULL) fgInstance = new Minimizer();
    //	fgInstance->Reset();
    return fgInstance;
  }

  void Minimizer::FinishFit() {
    for (unsigned int i = 0; i < GetNParams(); i++) {
      fParamsMin[i] = fAllowedValues[i][fStateVectorMin[i]];
    }
    EstimateErrors();
    delete[] fTempParams;
    fTempParams = nullptr;
  }

  void Minimizer::SetMinimizerType(TString opt) {
    opt.ToLower();
    if (opt == "scan") {
      fMinimizeType = kScan;
    } else if (opt == "ant") {
      fMinimizeType = kAnt;
    }
  }

  void Minimizer::MinimizeAnt() {
    std::vector<std::vector<int>> directionIndex;
    for (int i = 0; i < GetNFree(); i++) {
      std::vector<int> poz;
      for (int j = 0; j < GetNFree(); j++) {
        poz.push_back(0);
      }
      poz[i] = 1;
      directionIndex.push_back(poz);
    }
    for (int i = 0; i < GetNFree(); i++) {
      std::vector<int> poz;
      for (int j = 0; j < GetNFree(); j++) {
        poz.push_back(0);
      }
      poz[i] = -1;
      directionIndex.push_back(poz);
    }
    int direction = 0;
    int freePars  = GetNFree();
    std::vector<int> tmpVec(freePars, 0);
    Bool_t cont = kTRUE;

    auto applyVector = [&]() {
      for (unsigned int p = 0; p < freePars; p++) {
        int constToPar = fNonConstMap[p];
        tmpVec[constToPar] += directionIndex[direction][p];
        if (tmpVec[constToPar] < 0 || tmpVec[constToPar] > fParameters[constToPar].GetNPoints()) { return kFALSE; }
      }
      /*
      std::cout << "VEC ";
      for (auto i : directionIndex[direction]) {
        std::cout << i << " ";
      }
      std::cout << std::endl;
      std::cout << "STATE ";
      for (auto i : fStateVector) {
        std::cout << i << " ";
      }
      std::cout << std::endl;*/
      for (unsigned int p = 0; p < freePars; p++) {
        fStateVector[fNonConstMap[p]] = tmpVec[p];
      }

      return kTRUE;
    };
    double old_chi = 1E+9;
    int bad        = 0;
    while (cont) {
      cont = applyVector();
      if (cont) {
        double chi = GetChi2();
        if (chi > old_chi) {  // larger chi, change direction
          direction = (direction + 1) % freePars;
          bad++;
        }
        if (chi < fGlobMin) {  // smaller chi
          fGlobMin        = chi;
          fStateVectorMin = fStateVector;
          bad             = 0;
          old_chi         = chi;
        }
      } else {  // out of bounds, whatever increase bad attemps counter
        direction = (direction + 1) % freePars;
        bad++;
      }
      fNCalls++;
      if (bad == freePars) {  // to many bad attempts
        // std::cout << "Cannot find better minimum" << std::endl;
        break;
      }
      if (fNCalls > 1E+6) break;  // to many call
    }
  }

  void Minimizer::MinimizeScan() {
    // set start parameters
    for (auto& x : fStateVector) {
      x = 0;
    }
    SetTempParams();
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      fTempParams[i] = fParameters[i].GetMin();
    }
    LoopOverParameter(0);
    for (int i = 0; i < GetNParams(); i++) {
      fTempParams[i] = fParamsMin[i];
      fSmoothFits[i] = fParamsMin[i];
    }
    std::cout << "*************************" << std::endl;
  }

  void Minimizer::MinimizeNelderMead() {
    InitFit();
    // meat
    Hal::Array_2<Double_t> vertices;
    vertices.MakeBigger(fFreePars + 1, fFreePars);
    Hal::Array_1<Double_t> chiMap;
    chiMap.MakeBigger(fFreePars + 1);
    Hal::Array_1<Double_t> centroid, futurePar, worstPar;
    centroid.MakeBigger(fFreePars);
    futurePar.MakeBigger(fFreePars);
    worstPar.MakeBigger(fFreePars);
    for (int i = 0; i < fFreePars; i++) {
      for (int j = 0; j < fFreePars; j++) {
        vertices[i][j] = fParameters[fNonConstMap[i]].GetMin();
      }
    }
    for (int j = 0; j < fFreePars; j++) {
      vertices[fFreePars][j] = fParameters[fNonConstMap[0]].GetMax();
    }

    auto copyPars = [&](const Hal::Array_1<Double_t>& val) {
      for (int i = 0; i < fFreePars; i++) {
        //    fTempParams[fNonConstMap[i]] = fParameters[i].FindClosest(val[i]);
      }
    };

    // calcualte first vertices
    for (int i = 0; i < fFreePars + 1; i++) {
      for (int j = 0; j < fFreePars; j++) {
        copyPars(vertices[i]);
      }
      chiMap[i] = (*fFunc)(fTempParams);
    }

    Bool_t found = kTRUE;
    while (found) {
      int largest_chi, lowest_chi;
      double max_chi = chiMap.FindMax(largest_chi);
      double min_chi = chiMap.FindMin(lowest_chi);
      // find worse vertice
      double almost_max_chi = 0;
      for (int i = 0; i < fFreePars + 1; i++) {
        if (chiMap[i] > almost_max_chi && chiMap[i] != max_chi) { almost_max_chi = chiMap[i]; }
      }
      worstPar = vertices[largest_chi];

      // double calc centroid

      for (int i = 0; i < fFreePars; i++) {
        centroid[i] = 0;
        for (int j = 0; j < fFreePars + 1; j++) {
          if (j == largest_chi) continue;
          centroid += vertices[i];
        }
        centroid = centroid / double(fFreePars);
      }
      // do reflection
      futurePar = centroid + (centroid - worstPar);
      copyPars(futurePar);
      double chi_r = (*fFunc)(fTempParams);

      if (chi_r < almost_max_chi && chi_r > min_chi) {
        vertices[largest_chi] = futurePar;
        continue;
      }
      if (chi_r <= min_chi) {  // expand
      }

      // double second_worst_chi = 0; //ntot used warning
      for (int i = 0; i < fFreePars; i++) {
        centroid[i] = 0;
        for (int j = 0; j < fFreePars + 1; j++) {
          if (j == largest_chi) continue;
          centroid += vertices[i];
          centroid += vertices[i];
        }
        centroid = centroid / double(fFreePars);
      }
    }

    futurePar = centroid + (centroid - worstPar);
    copyPars(futurePar);
    // double chi_r = (*fFunc)(fTempParams); not used warning
    // TO DO complete this https://codesachin.wordpress.com/2016/01/16/nelder-mead-optimization/

    EstimateErrors();
    FinishFit();
  }

  void Minimizer::ChangeStateVector(std::vector<Int_t>& vec) {
    std::vector<Int_t> temp = vec;
    for (unsigned int i = 0; i < vec.size(); i++) {
      vec[i + 1] = temp[i];
    }
    vec[0] = temp[temp.size() - 1];
  }

  bool Minimizer::SetParLimits(unsigned int ivar, double min, double max) {
    if (min > max) {
      double temp = max;
      max         = min;
      min         = temp;
    }
    fParameters[ivar].SetMin(min);
    fParameters[ivar].SetMax(max);
    return true;
  }

  void Minimizer::SetTempParams() {
    for (unsigned int i = 0; i < GetNParams(); i++) {
      fTempParams[i] = fAllowedValues[i][fStateVector[i]];
    }
  }

  Double_t Minimizer::GetChi2() {
    SetTempParams();
    return (*fFunc)(fTempParams);
  }

}  // namespace Hal
