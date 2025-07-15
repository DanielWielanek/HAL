/*
 * ProfileAna2D.h
 *
 *  Created on: 9 gru 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_PROFILEANA2D_H_
#define HAL_FEATURES_HIST_PROFILEANA2D_H_

#include <Rtypes.h>
#include <RtypesCore.h>

#include "Object.h"
#include "Splines.h"
#include "Std.h"

class TF1;

class TH2;
class TGraphErrors;

namespace Hal {
  /**
   * fit a polynomial of given degree that tries to describe
   * a av and rms of function along Y axis as a function of X parameter
   * by default used to fit gaussians functions + polynomial
   * user should overwrite AnalizeCustom to fit more advanced functions
   * options currently supported:
   * In default configuration a gaussian function for slice is used and polynomial od given degree is used do describe sigma/mean
   * of such gaussian. The nubmer of points defines the degreee of polynomia, user can also specify the numbers used to fit the
   * poly (number of those points also defines the polynomial).
   * When UseFixed is called then fitting procedure use points defined in constructor (or choose N-points in similar distances) to
   * calculate the parameters of fit. When used without "fix" option - the algo only uses those parameters are starting parameters
   * to fit a free polynomial.
   * Option UseRMSGauss calculates RMS and sigma from projections and fit them to global data. UseFreeGaus use gaussian func to
   fit projections.

   */
  class ProfileAna2D : public Object {
  public:
    /**
     * data for slice after filtering outliers
     * x - x(Y) value
     * y - number of entriees
     * ye - error
     */
    struct SliceData {
      int fBin = {0};
      std::vector<Double_t> fX;
      std::vector<Double_t> fY;
      std::vector<Double_t> fYe;
    };
    /**
     * analysis type
     */
    static TString UseFixed() { return "fix"; }
    static TString UseRMSGaus() { return "rmsgauss"; };
    static TString UseFreeGaus() { return "reegauss"; }
    enum EDataId {
      kSigmaPlus       = 0,
      kSigmaMinus      = 1,
      kMean            = 2,
      kEntries         = 3,
      kSum             = 4,
      kMaxX            = 5,
      kMaxY            = 6,
      kSigmaPlusError  = 7,
      kSigmaMinusError = 8,
      kMeanError       = 9
    };
    Int_t fMaxStatPars = {10};

  private:
    Bool_t fAnalyzed           = {kFALSE};
    Double_t fXMin             = {0};
    Double_t fXMax             = {0};
    Double_t fYMin             = {0};
    Double_t fYMax             = {0};
    Double_t fEntriesThreshold = {0};
    Double_t fBinsThreshold    = {0};
    Double_t fOutLiersCut      = {-1};
    Color_t fColor             = {kRed};
    Color_t fColor2            = {kGray};
    TH2* fHisto                = {nullptr};
    /**
     * ids of parameters
     */
    Int_t fSigmaIdLow  = {EDataId::kSigmaMinus};
    Int_t fSigmaIdHigh = {EDataId::kSigmaPlus};
    Int_t fMeanId      = {EDataId::kMean};


  protected:
    Int_t fNParam   = {0};
    TF1* fAverage   = {nullptr};
    TF1* fSigmaLow  = {nullptr};
    TF1* fSigmaHigh = {nullptr};

    Bool_t fFreeSigma = {kTRUE};

    /** estimated values - called "raw"
     */
    std::vector<Double_t> fAverages;
    std::vector<Double_t> fRmsLow, fRmsHigh;
    std::vector<Double_t> fValues;
    /**
     * points to fit
     */
    std::vector<Double_t> fPoints;
    /**
     * points to fit sigmas
     */
    std::vector<Double_t> fPointsSigma;
    /**
     * data after filtering noise
     */
    std::vector<SliceData> fData;
    /**
     * pattern to fill global dependency (x vs y)
     */
    TString fFittingPattern;
    /**
     * pattern to fill global sigma dependency, sigma is calculated relative to mean
     */
    TString fFittingSigmaPattern;
    /**
     * pattern to fit projection
     */
    TString fFittingProjPattern;
    Bool_t fCustomFit  = {kFALSE};
    Double_t fTotBinsX = {0}, fTotMinX = {0}, fTotMaxX = {0};
    /** fitted params */
    std::vector<Double_t> fParamsSigmaLow;
    std::vector<Double_t> fParamsSigmaHigh;
    std::vector<Double_t> fParamsAverage;

    TString fAnalyzeOption;
    void AnalyzeProj();
    void AnalyzePolyGaus();
    void AnalyzePolyGausFitSlice();
    void InitializeAnalysis();
    void AutoEstimParams(const std::unique_ptr<TGraphErrors>& av,
                         const std::unique_ptr<TGraphErrors>& sigMinus,
                         const std::unique_ptr<TGraphErrors>& sigPlus);
    virtual void AnalizeCustom();
    Int_t GetSigmaLowId() const { return fSigmaIdLow; }
    Int_t GetSigmaHighId() const { return fSigmaIdHigh; }
    Int_t GetMeanId() const { return fMeanId; }
    Double_t EvalRawMean(Double_t* x, Double_t* p) const;
    Double_t EvalRawRMSPlus(Double_t* x, Double_t* p) const;
    Double_t EvalRawRMSMinus(Double_t* x, Double_t* p) const;

    Double_t EvalSigmaPlus(Double_t* x, Double_t* p) const;
    Double_t EvalSigmaMinus(Double_t* x, Double_t* p) const;
    Double_t EvaluateMean(Double_t* x, Double_t* p) const;

    Double_t EvalNSigmaPlus(Double_t* x, Double_t* p) const;
    Double_t EvalNSigmaMinus(Double_t* x, Double_t* p) const;
    Double_t GetYMin() const { return fYMin; }
    Double_t GetYMax() const { return fYMax; }
    Double_t GetXMin() const { return fXMin; }
    Double_t GetXMax() const { return fXMax; }
    // Double_t Eval2DimGauss(Double_t* x, Double_t* p) const;
    /**
     * tries to estimate parameters for slice fit
     * @param data
     * @return
     */
    virtual std::vector<Double_t> ReCalculateFree(const SliceData& data) const;
    /**
     * estimates parameters of global fit
     * @param gr
     * @return
     */
    virtual std::vector<Double_t> InitEstimPoly(const std::unique_ptr<TGraphErrors>& gr) const;
    /**
     * estimates parameters of global fit
     * @param gr
     * @param points - points at witch paramters will be estimated, if empty or size = 0 then automatic division is done according
     * to n-points
     * @return
     */
    virtual std::vector<Double_t>
    InitEstimFunc(const std::unique_ptr<TGraphErrors>& gr, TF1* f, const std::vector<Double_t>& points) const;
    void
    GetRow(std::vector<Double_t>& valuesX, std::vector<Double_t>& valuesY, std::vector<Double_t>& errorY, const Int_t row) const;
    void RemoveOutliers(std::vector<Double_t>& valuesX,
                        std::vector<Double_t>& valuesY,
                        std::vector<Double_t>& errorY,
                        Double_t av,
                        Double_t rms) const;
    /**
     * calculates parameter for given slice
     * @param valuesX
     * @param valuesY
     * @return vector of values - number according to EDataId
     */
    std::vector<Double_t> Calculate(const std::vector<Double_t>& valuesX, const std::vector<Double_t>& valuesY) const;
    /**
     * calculates parameter for given slice
     * @param valuesX
     * @param valuesY
     * @return vector of values - number according to EDataId
     */
    virtual std::vector<Double_t> ReCalculateGauss(const SliceData& data) const;
    virtual std::vector<Double_t> ReCalculateFreeGauss(const SliceData& data) const;


  public:
    ProfileAna2D();
    /**
     *
     * @param h histogram to analyze
     * @param poly degree of polynomial
     */
    ProfileAna2D(TH2& h, Int_t poly);
    /**
     *
     * @param h histogram to analyze
     * @param points to interpolate
     */
    ProfileAna2D(TH2& h, std::vector<Double_t> points);
    /**
     * set custom range of fitting data along X axis, if not set then entire histogram will be used
     * @param min
     * @param max
     */
    void SetCustomXRange(Double_t min, Double_t max) {
      fXMin = min;
      fXMax = max;
    }
    /**
     * fitted function to average or sigma
     * @return
     */
    TF1* GetPolyAv() const { return fAverage; }
    /**
     * analyse parameters
     * @param kCustomProjAv - initilize fit from averages, kCustomProjMax - initialize fit from maximum values
     */
    void Analyze(TString opt = "poly");
    /**
     * set minimal number of entries along Y-axis to take point in account during fittiong polynomial
     * @param threshold
     */
    void SetSliceThreshold(Double_t threshold) { fEntriesThreshold = threshold; };
    /**
     * ignore entries in Y axis beyond av+/-RMS*sigma
     * @param sigma
     */
    void SetOutliersCut(Double_t sigma) { fOutLiersCut = sigma; };
    /**
     * returns true or false dependin if point pass sigma cut, note - works only for symmetric sigmas
     * @param x
     * @param y
     * @param sigmaLow
     * @param sigmaHi
     * @return
     */
    Bool_t Check(Double_t x, Double_t y, Double_t sigmaLow, Double_t sigmaHi) const;
    /**
     * draw if colz then histo is drawn, otherwise only average function + sigma functions are drawn
     * use sig2 or sig3 to draw +/- 2/3 sigma respectively (default value is +/- 1 sigma
     * @param opt
     */
    void Draw(Option_t* opt = "");
    /**
     * set color of lines used to draw
     * @param colfited - color of fitted parameters
     * @param colraw - color of RMS/averages/max used for fitting
     */
    void SetLineColors(Color_t colfited, Color_t colraw) {
      fColor  = colfited;
      fColor2 = colraw;
    }
    /**
     * set main fitting function used to describe av(x) or sigma(x) dependency
     * @param fitting_pattern
     */
    void SetFittingFunction(TString fitting_pattern) {
      fFittingPattern = fitting_pattern;
      fCustomFit      = kTRUE;
    }
    /**
     * set function used to fit slice o data
     * @param fitting_pattern
     */
    void SetFittingProjFunction(TString fitting_pattern) {
      fFittingProjPattern = fitting_pattern;
      fCustomFit          = kTRUE;
    }
    /**
     * set custom fitting pattern - in such case sigma is not fitted as "free function" but as relative to mean
     * @param fitting_patter
     */
    void SetFittingSigmaFunction(TString fitting_patter, std::vector<Double_t> estimPoints) {
      fFittingSigmaPattern = fitting_patter;
      fFreeSigma           = kFALSE;
      fPointsSigma         = estimPoints;
    }
    /**
     * draw slice with fit
     * @param val
     * @param opt drawing option hist - draw normalized histogram (to max =1)
     * func - draw gaussian function normalized to 1
     * av - draw mean (Tline)
     * sig1 - draw +/- sigma (TLine)
     * sig2 - draw +/- 2 sigma
     * sig3 - draw +/- 2 sigma
     */
    void DrawSlice(Double_t val, TString opt = "hist+func+av+sig1+stat");
    void SetSigmaId(Int_t id) { SetSigmaId(id, id); }
    void SetSigmaId(Int_t idLow, Int_t idHigh);
    void SetAverageId(Int_t id) { fMeanId = id; };
    void SetNBinsThreshold(Int_t n) { fBinsThreshold = n; }
    std::vector<Double_t> GetMeanParams() const { return fParamsAverage; }
    std::vector<Double_t> GetSigmaPlusParams() const { return fParamsSigmaHigh; }
    std::vector<Double_t> GetSigmaMinusParams() const { return fParamsSigmaLow; }
    virtual ~ProfileAna2D();
    ClassDef(ProfileAna2D, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_PROFILEANA2D_H_ */
