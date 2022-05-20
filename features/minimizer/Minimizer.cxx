/*
 * HalMinimizer.cxx
 *
 *  Created on: 7 lis 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Minimizer.h"

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
    return true;
  }


  bool Minimizer::SetVariableLimits(unsigned int ivar, double lower, double upper) {
    if (lower > upper) {
      double temp = upper;
      upper       = lower;
      lower       = upper;
    }
    if (fParameters.size() <= ivar) { fParameters.resize(ivar + 1); }
    fParameters[ivar].SetRange(lower, upper);
    return true;
  }

  bool
  Minimizer::SetLimitedVariable(unsigned int ivar, const std::string& name, double val, double step, double lower, double upper) {
    if (fParameters.size() <= ivar) { fParameters.resize(ivar + 1); }
    fParameters[ivar].SetParName(name);
    fParameters[ivar].SetRange(lower, upper);
    fParameters[ivar].SetMapRange(lower, upper, (upper - lower) / step + 1);

    std::cout << "***" << name << " " << val << " " << step << " " << lower << " " << upper << "x" << (upper - lower) / step + 1
              << std::endl;
    std::cout << "MAP STEP " << fParameters[ivar].GetNPoints() << " " << step << std::endl;
    return false;
  }

  // recalc methods


  void Minimizer::InitFit() {
    fNonConstMap.clear();
    fNCalls = 0;
    for (Int_t i = 0; i < fParameters.size(); i++) {
      if (!fParameters[i].IsFixed()) { fNonConstMap.push_back(i); }
    }
    auto lambda = [](Int_t a, Int_t b) -> bool { return a < b; };
    std::sort(fNonConstMap.begin(), fNonConstMap.end(), lambda);

    Cout::Database(6, "ParName", "MinMap", "MaxMap", "Steps", "Min", "Max");
    for (int i = 0; i < fParameters.size(); i++) {
      if (fParameters[i].IsFixed()) {
        Cout::Database(2, fParameters[i].GetParName().Data(), Form("%4.4f", fParameters[i].GetStartVal()));
      } else {
        Cout::Database(6,
                       fParameters[i].GetParName().Data(),
                       Form("%4.4f", fParameters[i].GetMapMin()),
                       Form("%4.4f", fParameters[i].GetMapMax()),
                       Form("%i", fParameters[i].GetNPoints()),
                       Form("%4.4f", fParameters[i].GetMin()),
                       Form("%4.4f", fParameters[i].GetMax()));
      }
    }

    // round minimum
    for (int i = 0; i < fParameters.size(); i++) {
      fParameters[i].SetIsDiscrete(kTRUE);  // all parameters must be discrete
      fParameters[i].Init();
      fParameters[i].Print();
    }
    Cout::Database(6, "ParName", "MinMap", "MaxMap", "Steps", "Min", "Max");
    for (int i = 0; i < fParameters.size(); i++) {
      if (fParameters[i].IsFixed()) {
        Cout::Database(2, fParameters[i].GetParName().Data(), Form("%4.4f", fParameters[i].GetStartVal()));
      } else {
        Cout::Database(6,
                       fParameters[i].GetParName().Data(),
                       Form("%4.4f", fParameters[i].GetMapMin()),
                       Form("%4.4f", fParameters[i].GetMapMax()),
                       Form("%d", fParameters[i].GetNPoints()),
                       Form("%4.4f", fParameters[i].GetMin()),
                       Form("%4.4f", fParameters[i].GetMax()));
      }
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
    fGlobMin    = DBL_MAX;
  }

  void Minimizer::Reset() { fParameters.clear(); }


  // minimize methods


  bool Minimizer::Minimize() {
    if (GetNParams() == 0) return false;
    fGlobMin = DBL_MAX;
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
      } break;
      case eMinimizeType::kScan: {
        Cout::DebugInfo(0);
        MinimizeScan();
      } break;
    }


    return true;
  }

  const double* Minimizer::X() const {
    if (!fDiscreteFit) return fSmoothFits;
    return fQuantumFits;
  }

  void Minimizer::SetFunction(const ROOT::Math::IMultiGenFunction& func) {
    fFunc = const_cast<ROOT::Math::IMultiGenFunction*>(&func);
  }

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
      Double_t chi = (*fFunc)(fTempParams);
      if (chi < fGlobMin) {
        fGlobMin = chi;
        for (unsigned int i = 0; i < fParameters.size(); i++)
          fParamsMin[i] = fTempParams[i];
      }
      return kTRUE;
    }
    /** make loop over other parameters **/
    Int_t pairID = fNonConstMap[param];
    for (int i = 0; i < fParameters[pairID].GetValues().size(); i++) {
      fTempParams[pairID] = fParameters[pairID].GetValues()[i];
      Bool_t val          = LoopOverParameter(param + 1);
      if (val == kFALSE) return kFALSE;
    }
    return kTRUE;
  }

  void Minimizer::EstimateErrors() {
    for (unsigned int i = 0; i < GetNParams(); i++) {
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

    for (unsigned int i = 0; i < GetNParams(); i++)
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


    std::cout << " pAR " << x1 << " " << x2 << " " << x3 << " " << y1 << " " << y2 << " " << y3 << " " << a << std::endl;

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
      /**
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
    InitFit();
    std::vector<Int_t> MinimizingVector;
    MinimizingVector.resize(GetNFree() * 2);
    Int_t half_vector = MinimizingVector.size() / 2;
    for (int i = 0; i < half_vector; i++) {
      MinimizingVector[i] = -fNonConstMap[i] - 1;
    }
    for (int i = 0; i < half_vector; i++) {
      MinimizingVector[i + half_vector] = fNonConstMap[i] + 1;
    }


    Bool_t not_found = kTRUE;


    Int_t bad_direction = 0;
    std::ofstream debug;
    debug.open("debug.txt");
    Int_t stepNo = 0;

    // init first param
    Int_t vectorF        = MinimizingVector[MinimizingVector.size() - 1];
    Int_t par_to_changeF = TMath::Abs(vectorF);
    Int_t par_noF        = par_to_changeF - 1;
    fTempParams[par_noF] = fTempParams[par_noF] - TMath::Sign(1, vectorF) * fParameters[par_noF].GetDParam();

    std::vector<Double_t> paramsMin;
    Int_t similarMin = 0;
    fGlobMin         = DBL_MAX;
    do {
      fNCalls++;

      if (similarMin == GetNFree() + 1) {
        if (fTrace) std::cout << "FOUND SIMILAR minima" << std::endl;
        break;
      }
      if (stepNo > 1000) break;
      // we tried all direction time to give up
      if (bad_direction == GetNFree() * 2) break;
      // find new direction
      Int_t vector        = MinimizingVector[MinimizingVector.size() - 1];
      Int_t par_to_change = TMath::Abs(vector);
      Int_t par_no        = fNonConstMap[par_to_change - 1];
      std::cout << "\tstate vec " << vector << " " << par_no << " " << fParameters[par_no].GetDParam() << std::endl;
      fTempParams[par_no] = fTempParams[par_no] + TMath::Sign(1, vector) * fParameters[par_no].GetDParam();


      std::cout << "par\t";
      for (int i = 0; i < fParameters.size(); i++) {
        std::cout << fTempParams[i] << " ";
        debug << fTempParams[i] << " ";
      }
      std::cout << std::endl;
      debug << std::endl;

      // check if we are on the egde
      if (fTempParams[par_no] > fParameters[par_no].GetMax()) {
        std::cout << "GO Backward" << std::endl;
        fTempParams[par_no] = fTempParams[par_no] - TMath::Sign(1, vector) * fParameters[par_no].GetDParam();
        bad_direction++;
        ChangeStateVector(MinimizingVector);
        continue;
      }
      if (fTempParams[par_no] < fParameters[par_no].GetMin()) {
        std::cout << "GO Forward" << std::endl;
        fTempParams[par_no] = fTempParams[par_no] + TMath::Sign(1, vector) * fParameters[par_no].GetDParam();
        bad_direction++;
        ChangeStateVector(MinimizingVector);
        continue;
      }


      Double_t chi = (*fFunc)(fTempParams);
      if (fTrace) {
        std::cout << "par\t";
        for (int i = 0; i < fParameters.size(); i++) {
          std::cout << fTempParams[i] << " ";
          debug << fTempParams[i] << " ";
        }
        std::cout << "\n chi2 = " << chi << std::endl;
      }
      // larger chi - change direction !
      if (chi > fGlobMin) {  // larger chi than in prev iteration
        bad_direction++;
        std::cout << "Chi larger" << std::endl;
        ChangeStateVector(MinimizingVector);
        fTempParams[par_no] = fTempParams[par_no] - TMath::Sign(1, vector) * fParameters[par_no].GetDParam();
        continue;
      } else {  // smaller chi continue looking for min
        if (chi == fGlobMin) {
          similarMin++;
          std::cout << "similiar" << std::endl;
        } else {
          std::cout << "smaller" << std::endl;
          for (int i = 0; i < paramsMin.size(); i++)
            paramsMin[i] = fTempParams[i];
          similarMin    = 0;
          bad_direction = 0;
          fGlobMin      = chi;
        }
      }
    } while (not_found);

    for (int i = 0; i < GetNParams(); i++)
      fTempParams[i] = paramsMin[i];
    // fParams[last_par_no] = fParams[last_par_no] + TMath::Sign(1, -last_vector) * fStep[last_par_no];
    for (int i = 0; i < GetNParams(); i++) {
      std::cout << fTempParams[i] << " ";
      debug << fTempParams[i] << " ";
    }
    std::vector<Double_t> chiParamsLow;
    std::vector<Double_t> chiParamsHigh;
    chiParamsLow.resize(GetNFree());
    chiParamsHigh.resize(GetNFree());
    // no calculate hessian

    Int_t nElem = 2 * TMath::Power(GetNFree(), 2) + 1;
    std::vector<std::vector<Double_t>> state_vector;
    std::vector<Double_t> chi_vector;
    std::vector<Double_t> parchi_vector;
    chi_vector.resize(nElem);
    parchi_vector.resize(nElem);
    state_vector.resize(nElem);
    for (int i = 0; i < nElem; i++) {
      state_vector[i].resize(GetNFree());
      Int_t base = i;
      for (int j = 0; j < GetNFree(); j++) {
        Int_t o = base % 3;
        base    = base / 3;
        switch (o) {
          case 0: state_vector[i][j] = 0; break;
          case 1: state_vector[i][j] = 1; break;
          case 2: state_vector[i][j] = -1; break;
        }
        if (base <= 0) break;
      }
    }
    std::cout << "TEST VEC" << std::endl;
    for (int i = 0; i < nElem; i++) {
      for (int j = 0; j < GetNFree(); j++) {
        std::cout << state_vector[i][j] << " ";
      }
      std::cout << std::endl;
    }

    Double_t lMin = DBL_MAX;
    for (int i = 0; i < nElem; i++) {
      for (int j = 0; j < GetNFree(); j++) {
        Int_t poz        = fNonConstMap[j];
        fTempParams[poz] = fTempParams[poz] + state_vector[i][j] * fParameters[poz].GetDParam();
        if (i != 0) {
          Double_t chi  = (*fFunc)(fTempParams);
          chi_vector[i] = chi / fNDF;
          if (lMin > chi) lMin = chi;
        } else {
          chi_vector[i] = fGlobMin / fNDF;
        }
      }
      for (int k = 0; k < paramsMin.size(); k++) {
        fTempParams[k] = paramsMin[k];
      }
    }
    if (lMin <= fGlobMin) { std::cout << "FOUND BAD MIN" << std::endl; }
    /*
      std::cout << "****** chi vecs " << std::endl;
      for (int i = 0; i < nElem; i++) {
        std::cout << "chi " << chi_vector[i] << "\t vec \t";
        for (int j = 0; j < state_vector[i].size(); j++) {
          std::cout << state_vector[i][j] << " ";
        }
        std::cout << std::endl;
      }
    */

    // estimate approx stat. error
    auto lenght = [](const std::vector<Double_t>& v) -> Double_t {
      Double_t L = 0;
      for (int i = 0; i < v.size(); i++) {
        L += v[i] * v[i];
      }
      return TMath::Abs(L);
    };
    for (int i = 0; i < GetNParams(); i++) {
      fQuantumFits[i] = fSmoothFits[i] = fTempParams[i];
      fErrors[i]                       = 0;
    }

    const Double_t scalled_chi2 = fGlobMin / fNDF;
    for (int i = 0; i < GetNFree(); i++) {
      Double_t chi_low = 0, chi_high = 0;
      Int_t poz = fNonConstMap[i];
      Double_t x1, x2, x3, y1, y2, y3, a, b, c;
      x1 = fTempParams[poz] - fParameters[poz].GetDParam();
      x2 = fTempParams[poz];
      x3 = fTempParams[poz] + fParameters[poz].GetDParam();
      for (int j = 0; j < state_vector.size(); j++) {
        Double_t L = lenght(state_vector[j]);
        if (L != 1) continue;
        if (state_vector[j][i] == 1) { chi_high = chi_vector[j]; }
        if (state_vector[j][i] == -1) { chi_low = chi_vector[j]; }
      }
      y1 = chi_low;
      y2 = scalled_chi2;
      y3 = chi_high;
      Hal::Std::FitParabola(x1, x2, x3, y1, y2, y3, a, b, c);
      fSmoothFits[poz]  = -b / (2.0 * a);
      fQuantumFits[poz] = fTempParams[poz];
      fErrors[poz]      = TMath::Sqrt(1.0 / a) * scalled_chi2;
      std::cout << " pAR " << x1 << " " << x2 << " " << x3 << " " << y1 << " " << y2 << " " << y3 << " " << a << std::endl;


      std::cout << "Par " << i << "\t" << fTempParams[poz] << " +\\- " << TMath::Sqrt(1. / a) * scalled_chi2 << " " << a
                << std::endl;
    }

    FinishFit();
  }

  void Minimizer::MinimizeScan() {
    // set start parameters
    InitFit();
    for (int i = 0; i < fParameters.size(); i++) {
      fTempParams[i] = fParameters[i].GetMin();
    }
    // calculate parameters array
    fGlobMin = DBL_MAX;

    LoopOverParameter(0);
    for (unsigned int i = 0; i < GetNParams(); i++) {
      fTempParams[i] = fParamsMin[i];
      fSmoothFits[i] = fParamsMin[i];
    }
    std::cout << "*************************" << std::endl;
    EstimateErrors();
    FinishFit();
  }

  void Minimizer::ChangeStateVector(std::vector<Int_t>& vec) {
    std::vector<Int_t> temp = vec;
    for (int i = 0; i < vec.size(); i++) {
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
}  // namespace Hal
