/*
 * CorrFit.h
 *
 *  Created on: 08-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFITFUNC_H_
#define HALCORRFITFUNC_H_

#include "CorrFit.h"

#include "Array.h"
#include "FemtoConst.h"
#include "MinimizerStepConf.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TMath.h>
#include <TString.h>


class TF1;
class TH1;


/**
 * abstract class for fitting correlation function
 */
// #define CF_FIT_TRACKING
namespace ROOT {
  namespace Math {
    class Minimizer;
  }
}  // namespace ROOT
namespace Hal {
  class CorrFitGUI;
  class CorrFitMask;
  class ChiSqMap2D;
  class CorrFitHDFunc;
  class CorrFitPainter;
  class CorrFitFunc : public CorrFit {
    friend class Femto1DCF;
    friend class Femto3DCF;
    friend class FemtoSHCF;
    friend class CorrFitPainter;

    void PrepareSecondMiminizer(ROOT::Math::Minimizer* min, const double* pars, const double* errors) const;
    void SetParsOfMinimizer(ROOT::Math::Minimizer* min) const;
    Int_t CountNDF() const;
    ROOT::Math::Minimizer* GetMinimizer1(EMinAlgo algo) const;
    ROOT::Math::Minimizer* GetMinimizer2(EMinAlgo algo) const;

  protected:
    /**
     * called for each calculation of chi2 or loglikehood minimalization - used
     * for recalculation CF is parameters are changed
     */
    virtual void RecalculateFunction() const {};
    /**
     * recalculate CF's with taking into acount limited bin resolution
     */
    virtual void RecalculateSmoothFunction() const = 0;
    /**
     * kinematics of pair
     */
    Femto::EKinematics fKinematics;
    /**
     * number of dimensions
     */
    const Int_t fDim;
    /**
     * last calcualted chi square value
     */
    Double_t fChi[2];
    /**
     * max numer of iterations
     */
    Int_t fMaxIterations;
    /**
     * norm index
     */
    Int_t fNormParIndex = {0};
    /**
     * desired precission
     */
    Double_t fTolerance;
    /**
     * number of bins used in minimization
     */
    Double_t fActiveBins;
    /**
     * range of fitted functions
     */
    Array_1<Double_t> fRange;
    /**
     * trace fitting procedure
     */
    Bool_t fTrace;
    /**
     * user provided fitting map
     */
    Bool_t fOwnRangeMap;
    /**
     * pointer to correlation function
     */
    TObject* fCF;
    /**
     * copy of denominator
     */
    TH1* fDenominatorHistogram;
    /**
     * numerator histogram copy
     */
    TH1* fNumeratorHistogram;
    /**
     * copy of correlation function
     */
    TH1* fCorrelationFunctionHistogram;
    /**
     * fitting mask
     */
    CorrFitMask* fMask;
    /**
     * for drawing CF
     */
    CorrFitPainter* fPainter = {nullptr};  //!
    MinimizerStepConf fDiscretteMinimzerConf;
    /**
     * extrapolated function
     */
    mutable CorrFitHDFunc* fHDMaps;
    /**
     * prepares hal minimizer (numer of steps etc. should be ovewriten if fit
     * base on discreate map, for other functions the configuration of minimizer
     * should be done manualy before each fit, by default the calculation matrix
     * check 10 points for each non-fixed parameter from min to max
     */
    virtual void PrepareHalMinimizer() const;
    /**
     * prepares root minimizer
     */
    virtual void PrepareRootMinimizer(ROOT::Math::Minimizer* minizer) const;
    /**
     * estimate number of active bins, NDF, also calculate mask in needed
     */
    virtual void EstimateActiveBins() = 0;
    /**
     * set parameters, their names or ranges
     * @param f function with is set
     */
    void SetupFunction(TF1* f) const;
    /**
     * main function that performs numerical minimization of given function
     */
    void NumericalMinimization();
    /**
     * function that estimates parameters by using simple scan minimizer, the scan
     * is performed by Hal::Minimizer
     * @param bins
     * if bin = 0 - just display minimas
     * if bin > 0 - set par limits as value +/- bin_width*bin
     * if bin < 0 - set par limits as value +/- bin_widht also configure Hal::Minimizer to value +/- bin width with step
     * bin_width/bin
     */
    void NumericalPreMinimization(Double_t bins);
    /**
     * main function that performs some estitamtions for numerical calculations
     * but doesn't perform minimalization itself
     */
    void DummyNumericalFunction();
    /**
     * uset do full recalcuate function and smoth function
     * @param histo
     */
    void ParametersChanged() const {  // TODO optimize
      RecalculateFunction();
      RecalculateSmoothFunction();
    }
    /**
     * fit this function to given CF
     * @param histo - object that will be fitted
     * @see Femto1DCF#Fit or @see Femto3DCF#Fit
     */
    virtual void Fit(TObject* histo);
    /**
     * make dummy fit - allows to draw this function but doesn't try to fit it
     */
    virtual void FitDummy(TObject* histo);
    /**
     * check ranges set by user and other stuff
     */
    virtual void Check();
    /**
     * clear fit, and functions when old cf is used
     * (cleaning histograms is not required)
     */
    virtual void Prepare();
    /**
     * prepare histograms when new fit is called
     */
    virtual void PrepareRaw() = 0;
    /**
     * makes parent for this objec
     */
    virtual void MakePainter(TString options) = 0;
    /**
     * fit funcion by steps, usefull only for 1D fits
     * @param step - step of fitting
     */
    void FitStep(Int_t /*step*/) {};
    /**
     * functor for fitting
     * @param params
     * @return
     */
    double FunctorChiTFD(const double* params);
    /**
     * functor for fitting
     * @param params
     * @return
     */
    double FunctorChiTF(const double* params);
    /**
     * functor for fitting

     * @param params
     * @return
     */

    double FunctorLogTFD(const double* params);
    //{ParametersChanged(params);return
    // GetLogTFD(params);};//fgMinimizer->GetLogTFD(params);}
    virtual double GetChiTFD(const double* params) const = 0;
    virtual double GetLogTFD(const double* params) const = 0;
    virtual double GetChiTF(const double* params) const  = 0;
    /**
     * "recompute errors in numerator by adding influence of errors in denominator
     * used when fitting with TFD is called
     * @param num numerator
     * @param den denominator
     */
    virtual void SetErrors(TH1* num, const TH1* den) const = 0;
    /**
     * calculate error of correlation function
     * @param Num numerator value
     * @param NumErr numeratora error
     * @param Den denominator
     * @param DenErr denominator error
     * @param cf calculated correlaction function
     * @param cfe calculated error of CF
     */
    void CalcError(const Double_t Num,
                   const Double_t NumErr,
                   const Double_t Den,
                   const Double_t DenErr,
                   Double_t& cf,
                   Double_t& cfe) const;

  public:
    /**
     * default constructor
     * @param nparams number of parameters
     * @param dim dimension of fitted function
     */
    CorrFitFunc(Int_t nparams = 1, Int_t dim = 1);
    /**
     * set tolerance of minimization
     * @param tol
     */
    void SetTolerance(Double_t tol) { fTolerance = tol; };
    /**
     * set maximum number of iterations during minimizations
     * @param max
     */
    void SetMaxIteration(Int_t max) { fMaxIterations = max; };
    /**
     * set range of CF's
     * @param min lower range limit
     * @param max upper range limit
     */
    void SetRange(Double_t min, Double_t max);
    /**
     * enable tracing of fit
     */
    void TraceFitting() { fTrace = kTRUE; };
    /**
     * provide user fitting mask, such mask overwrite SetRange method
     * @param
     */
    virtual void SetFittingMask(const CorrFitMask& map) = 0;
    /**
     * set minimizer configuration (used only for fit with discrette parametrization and Hal::Minimizer
     * @param conf configuration
     */
    void SetMinimizerConf(const MinimizerStepConf& conf) { fDiscretteMinimzerConf = conf; };
    /**
     * set minimizer configuration (used only for fit with discrette parametrization and Hal::Minimizer
     * @param xmlFile xml file with configuration
     */
    void SetMinimizerConf(TString xmlFile);
    /**
     * makes dummy config for Hal::Minimizer configuration
     * @param name of xml file
     */
    void MakeDummyXMLConfig(TString xmlFile);
    /**
     *
     * @return numbers of free parameters
     */
    Int_t GetFreeParamsNo() const;
    /**
     *
     * @param opt "fitted" return chi square  of fitted function if "fitting"
     * return chi square of function used for fittig
     * @return
     */
    Double_t GetChiSquare(Option_t* opt = "fitted") const;
    /**
     *
     * @param opt "fitted" or "fitting" to return value for fitted function or
     * fitting function
     * @return  chi square of fittend function divided by number of degreees of
     * freedom
     */
    Double_t GetChiNDF(Option_t* opt = "fitted") const;
    /**
     * get lower fit range
     * @param flag - axis ID
     * @return
     */
    Double_t GetRangeMin(Int_t flag = 0) const;
    /**
     *
     * @param flag
     * @return
     */
    Double_t GetRangeMax(Int_t flag = 0) const;
    /**
     * return chi square map for two parameters, other parameters are from fit,
     * this function draw map acros range of parameters
     * @param par1 first parameter no
     * @param par1_steps number of bins in first axis in  map
     * @param par2 second parameter no
     * @param par2_steps  /number of bins on second axis map
     * @param scale if true then all bin contents are divided by NDF
     * @return chi square map
     */
    ChiSqMap2D*
    GetChiSquareMap(Int_t par1, Int_t par1_steps, Int_t par2, Int_t par2_steps, Bool_t scale = kTRUE, EMinFunc = kChi2) const;
    /**
     * draw chi square map where user can define range of such map
     * @param par1 - first parameters no
     * @param par1_steps - number of bins on first axis in returend map
     * @param par1_min lowest value of first parameter used for calculation map
     * @param par1_max highest value of first parameter used for calculation map
     * @param par2 second parameter no
     * @param par2_steps number of bins on second axis of the map
     * @param par2_min lowest value of second parameter used for calculation map
     * @param par2_max highest value of second parameter used for calculation map
     * @param scale if true then all bin contents are divided by NDF
     * @return map of chisquare
     */
    ChiSqMap2D* GetChiSquareMap(Int_t par1,
                                Int_t par1_steps,
                                Double_t par1_min,
                                Double_t par1_max,
                                Int_t par2,
                                Int_t par2_steps,
                                Double_t par2_min,
                                Double_t par2_max,
                                Bool_t scale = kTRUE,
                                EMinFunc     = kChi2) const;
    TF1* GetFittedFunction() const;
    CorrFitHDFunc* GetHD() const { return fHDMaps; };
    /**
     * make an estimation of fited parameters by using full scan
     * @param histo
     * @param bins - the new parameters range if one fit inside the minimal bin, if 2 fit inside two neighbours bins in <1 do not
     * change fit parameters - just print estimated values
     */
    virtual void PreFit(TObject* histo, Double_t bins = 1);
    /**
     *
     * @return normalization parameter
     */
    Double_t GetNorm() const { return GetParameter(NormID()); };
    /**
     *
     * @return param number that correspond to norm
     */
    inline Int_t NormID() const { return fNormParIndex; };
    /**
     *
     * @return normalization error
     */
    Double_t GetNormError() const { return GetParError(fNormParIndex); };
    /**
     * set norm limits
     * @param min
     * @param max
     */
    void SetNormLimits(Double_t min, Double_t max) { SetParLimits(NormID(), min, max); }
    virtual void Draw(Option_t* option = "");
    virtual void Repaint();
    /**
     * start GUI
     * @param prec precission of sliders for parameter manipulation
     * @return
     */
    Hal::CorrFitGUI* StartGui(Int_t prec = -1);
    CorrFitPainter* GetPainter() const { return fPainter; }
    virtual ~CorrFitFunc();
    ClassDef(CorrFitFunc, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITFUNC_H_ */
