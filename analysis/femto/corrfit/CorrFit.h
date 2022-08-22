/*
 * CorrFit.h
 *
 *  Created on: 24 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT_H_
#define HALCORRFIT_H_

#include "Array.h"
#include "FitParam.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <utility>
#include <vector>

class TLegend;
class TLegendEntry;
class TF1;
class TVirtualPad;

/**
 * base class for fitting all correlation functions and groups of correlation
 * functions
 */

namespace Hal {
  class CorrFitGUI;
  class CorrFit : public TObject {
    friend class CorrFitGUI;

  private:
    /**
     * line color
     */
    Color_t fLineColor;
    /**
     * line style
     */
    Style_t fLineStyle;
    /**
     * iine width
     */
    Width_t fLineWidth;
    Int_t fParametersNo;

  public:
    enum EMinFunc {
      kChi, /*!< minimize function fitted to numerator */
      kLog, /*!<loglikehood minimization */
      kChi2 /*!< minimize function fitted to function */
    };
    enum EMinAlgo {
      kMinuitMigrad,
      kMinuitSimplex,
      kMinuitCombined,
      kMinuitScan,
      kMinuitFumili,
      kGLSMultiMinConjungateFR,
      kGLSMultiMinConjugatePR,
      kGLSMultiMinBFGS,
      kGLSMultiMinBFGS2,
      kGLSMultiMinSteppestDescent,
      kGLSMultiFit,
      kGLSSimAn,
      kDefaultAlgo,
      kMinimizerScan,
      kMinimizerSmartScan
    };
    /**specify the way how the CF is calculated **/
    enum ECalcOption {
      kSimple,      /** simple way of calculation of the CF **/
      kExtrapolated /**extrapolate value of the CF by using information from bin
                       edges **/
    };

  protected:
    /**
     * specify how to calculate the value of CF
     */
    ECalcOption fBinCalc;
    EMinFunc fMinFunc;
    EMinAlgo fMinAlgo;
    void AlgoToOptions(EMinAlgo algo, TString& opt1, TString& opt2) const;
    /**
     * number of degrees of freedom
     */
    Int_t fNDF;
    /**
     * minimal number of entries in numerator and denominator in bin
     */
    Double_t fThreshold;
    /**
     * max step
     */
    std::vector<FitParam> fParameters;
    std::vector<std::pair<TF1*, TVirtualPad*>> fDrawFunc;  //!
    /**
     * flat array used to pass arguments int cf eval and recalculate
     */
    mutable Double_t* fTempParamsEval;  // [fParametersNo]
    /**
     * resize arrays used for storing parameters, it should be called only if
     * number of parameters is not the same as in ctor
     */
    void ChangeParametersNo();
    /**
     * used to recalculate parameters
     */
    virtual void ParametersChanged() const {};
    /**
     *
     * @return line color
     */
    Color_t GetLineColor() const { return fLineColor; };
    /**
     *
     * @return line style
     */
    Style_t GetLineStyle() const { return fLineStyle; };
    /**
     *
     * @return line widht
     */
    Width_t GetLineWidth() const { return fLineWidth; };
    /**
     *
     * @param no
     * @return pointer to drawable function (if exists)
     */
    TF1* GetTF1(Int_t no) const { return fDrawFunc[no].first; };
    /**
     *
     * @return pointers to drawable functions (if exists) and their pads
     */
    std::vector<std::pair<TF1*, TVirtualPad*>> GetDrawFunctions() const { return fDrawFunc; };
    /**
     * overwrite fitted parameter (used by GUI)
     * @param par
     * @param val
     */
    void OverwriteParam(Int_t par, Double_t val) { fParameters[par].SetFittedValue(val); }
    /**
     * copy current parameters into given TF1
     * @param f function to set
     * @param copyNumPar copy values of parameters
     * @param copyGraphPar copy graphical settings of this function (line color etc.)
     */
    void CopyParamsToTF1(TF1* f, Bool_t copyNumPar = kTRUE, Bool_t copyGrapPar = kTRUE) const;

  public:
    CorrFit(Int_t parameters_no = 2);
    /**
     * set litms on given parameter
     * @param par parameter number
     * @param min lowest value of parameter
     * @param max hightes value of parameter
     */
    void SetParLimits(Int_t par, Double_t min, Double_t max);
    /**
     * fix parameter
     * @param par parameter no
     * @param val value for fixing
     */
    void FixParameter(Int_t par, Double_t val);
    /**
     * set name of parameter
     * @param par parameter no
     * @param name name of parameter
     */
    void SetParameterName(Int_t par, TString name);
    /**
     * set line color
     * @param color color to set
     */
    void SetLineColor(Color_t color) { fLineColor = color; };
    /**
     * set line style
     * @param style
     */
    void SetLineStyle(Style_t style) { fLineStyle = style; };
    /**
     * set line widht
     * @param width
     */
    void SetLineWidth(Width_t width) { fLineWidth = width; };
    /**
     * set threshold value used for calculation of CF, if number of entries if
     * numerator or denominator for given bin is smaller than threshold value then
     * such bin is not taken into account during calculations of chi2
     * @param thres threshold value
     */
    void SetThreshold(Double_t thres) { fThreshold = thres; };
    /**
     * display fit resutls
     */
    void PrintFitResults() const;
    /**
     *
     * @param par parameter id
     * @return true if parameter is fixed, false otherwise
     */
    Bool_t IsParFixed(Int_t par) const;
    /**
     *
     * @param par
     * @return true if parameter is discrete
     */
    Bool_t IsParDiscrete(Int_t par) const;
    /**
     *
     * @param par parameter id
     * @return lower value of parameter range
     */
    Double_t GetParMin(Int_t par) const;
    /**
     *
     * @param par parameter id
     * @return uppper value of parameter range
     */
    Double_t GetParMax(Int_t par) const;
    /**
     *
     * @param par parameter no
     * @return error from fitting given parameter
     */
    Double_t GetParError(Int_t par) const;
    /**
     *
     * @param par parameter no
     * @return value of fitted parameter
     */
    Double_t GetParameter(Int_t par) const;
    /**
     *
     * @param par
     * @return step for discrete parameter
     */
    const FitParam& GetParamConf(Int_t par) const { return fParameters[par]; }
    /**
     *
     * @return number of degree of freedom fitted function
     */
    Double_t GetNDF() const { return fNDF; };
    /**
     *
     * @return number of parameters
     */
    Int_t GetParametersNo() const { return fParametersNo; };
    /**
     * @param parameter number
     * @return parameter index, -1 if no parameter with given name was found
     */
    Int_t GetParameterIndex(TString name) const;
    /**
     *
     * @param no parameter name
     * @return parameter name
     */
    TString GetParameterName(Int_t no) const { return fParameters[no].GetParName(); };
    /**
     * set minimizer (library and type of minimizaltion algorithm.
     * this might be overwriten by fit  option
     * @param alg
     */
    virtual void SetMinimizer(EMinAlgo alg) { fMinAlgo = alg; };
    /**
     * set minimized function, might be overwriten by fit option
     * @param f
     */
    void SetMinimizedFunc(EMinFunc f) { fMinFunc = f; };
    /**
     * set option of calcuation of the value of the CF for given bin
     * @param f option of calculation : kSimple - assume that
     * CF(x) i equalt to CF(bin_center) kExtrapolated calculate the value of the
     * CF by information from histogram edges
     */
    void SetCalculationOption(ECalcOption f) { fBinCalc = f; }

    virtual ~CorrFit();
    ClassDef(CorrFit, 2)
  };
}  // namespace Hal

#endif /* HALCORRFIT_H_ */
