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

class TF1;

class TH2;
class TGraphErrors;

namespace Hal {
  /**
   * fit a polynomial of given degree that tries to describe
   * a av and rms of function along Y axis as a function of X parameter
   *
   */
  class ProfileAna2D : public Object {
    Double_t fRmsCut        = {-1};
    Double_t fRowEntriesCut = {0};
    Int_t fNParam           = {0};
    Double_t fXMin          = {0};
    Double_t fXMax          = {0};
    TF1* fAverage           = {nullptr};
    TF1* fSigma             = {nullptr};
    TH2* fHisto             = {nullptr};
    Bool_t fUseMax          = {kFALSE};
    Bool_t fAnalyzed        = {kFALSE};
    Color_t fColor          = {kRed};
    void AnalyzeProj();
    void AnalyzeCustom();
    void Calculate(const std::vector<Double_t>& valuesX,
                   const std::vector<Double_t>& valuesY,
                   Double_t& mean,
                   Double_t& sigma,
                   Double_t& maxi) const;
    void PrepareRow(std::vector<Double_t>& valuesX,
                    std::vector<Double_t>& valuesY,
                    Double_t& mean,
                    Double_t& sigma,
                    Double_t& maxi,
                    const Int_t row) const;


    Double_t EvaluateSigma(Double_t* x, Double_t* p) const;
    Double_t EvaluateAverage(Double_t* x, Double_t* p) const;
    Double_t EvalDrawAv(Double_t* x, Double_t* p) const;
    Double_t EvalDrawAvShift(Double_t* x, Double_t* p) const;
    Double_t Eval2DimGauss(Double_t* x, Double_t* p) const;
    std::vector<Double_t> fPolySigma;
    std::vector<Double_t> fPolyAverage;
    std::vector<Double_t> InitEstim(TGraphErrors* gr) const;

  public:
    /**
     * analysis type
     */
    enum EAnaType {
      kCustomProjAv,  /// calculates RMS and average along Y axis, fit such functions with used function
      kCustomProjMax  /// calculates RMS and maximum along Y axis, fit such functions with used function
    };
    ProfileAna2D();
    /**
     *
     * @param h histogram to analyze
     * @param poly degree of polynomial
     */
    ProfileAna2D(TH2& h, Int_t poly);
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
    TF1* GetPolySigma() const { return fSigma; }
    /**
     * analyse parameters
     * @param kCustomProjAv - initilize fit from averages, kCustomProjMax - initialize fit from maximum values
     */
    void Analyze(EAnaType type = EAnaType::kCustomProjAv);
    /**
     * set minimal number of entries along Y-axis to take point in account during fittiong polynomial
     * @param threshold
     */
    void SetSliceThreshold(Double_t threshold) { fRowEntriesCut = threshold; };
    /**
     * ignore entries in Y axis beyond av+/-RMS*sigma
     * @param sigma
     */
    void SetThresholdRMSAna(Double_t sigma) { fRmsCut = sigma; };
    /**
     * returns true or false dependin if point pass sigma cut
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
     * @param col
     */
    void SetLineColor(Color_t col) { fColor = col; }
    virtual ~ProfileAna2D();
    ClassDef(ProfileAna2D, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_PROFILEANA2D_H_ */
