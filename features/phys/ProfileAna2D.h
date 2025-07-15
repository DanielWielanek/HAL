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

  protected:
    Double_t fOutLiersCut      = {-1};
    Double_t fEntriesThreshold = {0};
    Double_t fBinsThreshold    = {0};
    Int_t fNParam              = {0};
    Double_t fXMin             = {0};
    Double_t fXMax             = {0};
    Double_t fYMin             = {0};
    Double_t fYMax             = {0};
    TF1* fAverage              = {nullptr};
    TF1* fSigmaLow             = {nullptr};
    TF1* fSigmaHigh            = {nullptr};
    TH2* fHisto                = {nullptr};
    Bool_t fUseMax             = {kFALSE};
    Bool_t fAnalyzed           = {kFALSE};
    Color_t fColor             = {kRed};
    Color_t fColor2            = {kGray};
    std::vector<Double_t> fAverages;
    std::vector<Double_t> fRmsLow, fRmsHigh;
    std::vector<Double_t> fValues;
    std::vector<Double_t> fPoints;
    std::vector<SliceData> fData;
    TString fFittingPattern;
    TString fFittingProjPattern;
    Bool_t fCustomFit  = {kFALSE};
    Double_t fTotBinsX = {0}, fTotMinX = {0}, fTotMaxX = {0};
    void AnalyzeProj();
    void AnalyzePolyGaus();
    void AnalyzePolyGausFitSlice();
    void InitializeAnalysis();
    virtual void AnalizeCustom();
    Double_t EvalRawMean(Double_t* x, Double_t* p) const;
    Double_t EvalRawRMSPlus(Double_t* x, Double_t* p) const;
    Double_t EvalRawRMSMinus(Double_t* x, Double_t* p) const;
    Double_t EvalSigmaPlus(Double_t* x, Double_t* p) const;
    Double_t EvalSigmaMinus(Double_t* x, Double_t* p) const;
    Double_t EvalShiftedPlus(Double_t* x, Double_t* p) const;
    Double_t EvalShiftedMinus(Double_t* x, Double_t* p) const;
    Double_t EvaluateMean(Double_t* x, Double_t* p) const;
    // Double_t Eval2DimGauss(Double_t* x, Double_t* p) const;
    std::vector<Double_t> fPolySigmaLow;
    std::vector<Double_t> fPolySigmaHigh;
    std::vector<Double_t> fPolyAverage;
    /**
     * estimates parameters of global fit
     * @param gr
     * @return
     */
    virtual std::vector<Double_t> InitEstim(const std::unique_ptr<TGraphErrors>& gr) const;
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
    Int_t fSigmaIdLow  = {EDataId::kSigmaMinus};
    Int_t fSigmaIdHigh = {EDataId::kSigmaPlus};
    Int_t fAverageId   = {EDataId::kMean};
    TString fAnalyzeOption;

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
     * fitted function to sigma
     * @return
     */
    TF1* GetPolySigmaLow() const { return fSigmaLow; }
    /**
     * fitted function to sigma
     * @return
     */
    TF1* GetPolySigmaHigh() const { return fSigmaHigh; }
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
    void SetCutsomFittingFunction(TString fitting_pattern) {
      fFittingPattern = fitting_pattern;
      fCustomFit      = kTRUE;
    }
    /**
     * set function used to fit slice o data
     * @param fitting_pattern
     */
    void SetCutsomFittingProjFunction(TString fitting_pattern) {
      fFittingProjPattern = fitting_pattern;
      fCustomFit          = kTRUE;
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
    void DrawSlice(Double_t val, TString opt = "hist+func+av+sig1");
    void SetSigmaId(Int_t id) { SetSigmaId(id, id); }
    void SetSigmaId(Int_t idLow, Int_t idHigh);
    void SetAverageId(Int_t id) { fAverageId = id; };
    void SetNBinsThreshold(Int_t n) { fBinsThreshold = n; }
    virtual ~ProfileAna2D();
    ClassDef(ProfileAna2D, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_PROFILEANA2D_H_ */
