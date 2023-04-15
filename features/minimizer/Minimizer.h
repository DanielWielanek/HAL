/*
 * HalMinimizer.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALMINIMIZER_H_
#define HALMINIMIZER_H_
#include "FitParam.h"

#include "Array.h"
#include "MinimizerStepConf.h"

#include "Math/Minimizer.h"

#include <vector>
/**
 * class for minizing function with psedudodiscrete parameter
 * This class makes loop over discrete grid. Therefore user don't have to
 * provide continuous function. If discrete fit is used then fitted parameters
 * correspod to minimum of function at given point on grid.  If discrete fit is
 * disabled then minimum is found by using parabolic interpolation around
 * discrete minimum.
 */
/*
class HalMinimizerParameter : public TObject {
private:
  Double_t fMapMin;
  Double_t fMapMax;
  Double_t fMin;
  Double_t fMax;
  Double_t fStartParam;
  Int_t fMapSteps;
  std::vector<Double_t> fValues;
  TString fParName;

public:
  HalMinimizerParameter() : fMapMin(0), fMapMax(0), fMin(0), fMax(0), fStartParam(0), fMapSteps(0) {};
  Double_t GetMapMin() const { return fMapMin; };
  Double_t GetMapMax() const { return fMapMax; }

  Int_t GetMapSteps() const { return fMapSteps; };
  Int_t GetNProbes() const { return fValues.size(); };
  Double_t GetStepSize() const {
    Double_t n = GetNProbes() - 1;
    return (fMin - fMax) / n;
  };
  Bool_t IsConst() const {
    if (GetNProbes() < 1) return kTRUE;
    return kFALSE;
  }
  virtual ~HalMinimizerParameter() {};
};
*/
namespace Hal {
  class Minimizer : public ROOT::Math::Minimizer {

  protected:
    enum eMinimizeType { kScan, kAnt };

  private:
    static Minimizer* fgInstance;
    std::vector<FitParam> fParameters;  //!
    std::vector<Double_t> fParamsMin;   // found minimum
    std::vector<Int_t> fNonConstMap;
    std::vector<std::vector<Double_t>> fAllowedValues;
    std::vector<Int_t> fStateVector;
    std::vector<Int_t> fStateVectorMin;

    ROOT::Math::IMultiGenFunction* fFunc;
    eMinimizeType fMinimizeType;
    Int_t fNCalls;
    Int_t fFreePars = {0};
    Bool_t fDiscreteFit;
    Bool_t fTrace;
    Double_t fGlobMin;
    Double_t fNDF;
    double* fQuantumFits;  //[fNo]
    double* fSmoothFits;   //[fNo]
    double* fErrors;       //[fNo]
    double* fTempParams;   //[fNo]
    void InitFit();
    void FinishFit();
    void EstimateErrors();
    void MinimizeAnt();
    void MinimizeScan();
    void MinimizeNelderMead();
    void ChangeStateVector(std::vector<Int_t>& vec);
    void SetTempParams();
    void EstimateError(Int_t par, Double_t& min, Double_t& quantumMin, Double_t& error);
    Bool_t LoopOverParameter(Int_t param);
    Bool_t IsFixed(Int_t i) const;
    Double_t GetChi2();
    Int_t GetNParams() const { return fParameters.size(); };
    Int_t GetNFree() const { return fNonConstMap.size(); };
    Int_t GetNFixed() const { return fParameters.size() - fNonConstMap.size(); };

  public:
    Minimizer();
    void SetNDF(Double_t ndf) { fNDF = ndf; }
    void SetTrace(Bool_t trace) { fTrace = trace; };
    /**
     * use discrete fit
     */
    void UseDiscreteFit() { fDiscreteFit = kTRUE; };
    /**
     * improve fit by interpolation between grid points
     */
    void DisableDiscreteFit() { fDiscreteFit = kFALSE; };
    /**
     *
     * @return instance
     */
    static Minimizer* Instance();
    virtual bool SetVariable(unsigned int /* ivar*/, const std::string& /*name*/, double /*val*/, double /*step*/) {
      return false;
    };

    virtual bool SetParLimits(unsigned int ivar, double min, double max);
    /**
     * set fixed variabble
     * @param ivar
     * @param name
     * @param val
     * @return
     */
    virtual bool SetFixedVariable(unsigned int ivar, const std::string& name, double val);
    /**
     * set limited variable, to set number of steps call @see SetStepsNo
     * @param ivar parameter number
     * @param name parameter's name
     * @param val initial value of parameter (not used)
     * @param points - number of points on map
     * @param lower lower limite of value
     * @param upper upper limit of variable
     * @return
     */
    virtual bool
    SetLimitedVariable(unsigned int ivar, const std::string& name, double val, double points, double lower, double upper);
    /**
     * not used please use SetStepsNo
     * @param ivar
     * @param lower
     * @param upper
     * @return
     */
    virtual bool SetVariableLimits(unsigned int ivar, double lower, double upper);
    virtual bool Minimize();
    virtual void Reset();
    virtual void SetParamConf(const MinimizerStepConf& conf);
    virtual double MinValue() const { return 0; };
    virtual const double* X() const;
    virtual const double* Errors() const { return fErrors; }
    virtual double Edm() const { return -1; }
    virtual const double* MinGradient() const { return NULL; }
    virtual unsigned int NCalls() const { return 0; };
    virtual unsigned int NIterations() const { return NCalls(); }
    virtual unsigned int NDim() const { return fParameters.size(); };
    void SetFunction(const ROOT::Math::IMultiGenFunction& func);
    void SetMinimizerType(TString opt);
    virtual ~Minimizer();
    ClassDef(Minimizer, 1)
  };
}  // namespace Hal

#endif /* HALMINIMIZER_H_ */
