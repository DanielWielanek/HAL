/*
 * PeakFitterX.h
 *
 *  Created on: 2 sty 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MATH_PEAKFITTERX_H_
#define HAL_FEATURES_MATH_PEAKFITTERX_H_

#include <TGraphErrors.h>
#include <vector>

#include "Object.h"

class TGraphErrors;
class TF1;

namespace Hal {
  /**
   * class that fits data that contains peak and background together
   * user should set data, background function and "peak" region
   * then can call FitAll or FitBackground and FitSignal functions
   * FitBackground fits data outside of peak
   * FitSignal creates "Total function" = signal + background function and fit to all data if full option is used or just to peak
   * if peak option is used data
   *
   */
  class PeakFitterX : public Object {
    TF1* fBackgroundFunction    = {nullptr};
    TF1* fSignalFunction        = {nullptr};
    TF1* fTotalFunction         = {nullptr};
    TGraphErrors* fData         = {nullptr};
    TGraphErrors* fFilteredData = {nullptr};
    Double_t fEx[2]             = {0, -1};
    std::vector<Int_t> fRejectingPointsInt;
    Double_t EvalNumSignal(Double_t* x, Double_t* params) const;
    void MakeFilteredData();

  public:
    PeakFitterX();
    /**
     * set signal function
     * if no function is set then own signal function is created as data - background function
     * @param f
     */
    void SetSignalFunction(TF1* f) { fSignalFunction = f; };
    /**
     * set background function
     * @param f
     */
    void SetBackgroundFunction(TF1* f) { fBackgroundFunction = f; };
    /**
     * set data
     * @param gr
     */
    void SetData(const TGraphErrors& gr);
    /**
     * set data from TH1
     * @param data
     */
    void SetData(const TH1& data);
    /**
     * set peak region, this region is ignored when fitting of background is used
     * @param lo
     * @param hi
     */
    void SetPeakRegion(Double_t lo, Double_t hi);
    /**
     * reject by bins, starts with 1 like in TH1D !
     * this might be used as addition to set peak region
     * @param vals
     */
    void SetRejectingPointsByBin(std::vector<Int_t> vals) { fRejectingPointsInt = vals; };
    /**
     * performs fit of background
     */
    void FitBackground();
    /**
     * performs fit of total function equal to signal + background
     * if peak option is used then total function is fitted only in peak region
     * if full option is used then total function is fitted with all data
     * NOTE: during this fit the background function is fixed, therefore FitBackground should be called first
     * @param opt
     */
    void FitSignal(TString opt = "peak");
    /**
     * calls FitBackgroudn and FitSignal
     * @param opt
     */
    void FillAll(TString opt = "peak") {
      FitBackground();
      FitSignal(opt);
    }
    /**
     *
     * @return signal function
     */
    TF1* GetSignalFunction() const { return fSignalFunction; }
    /**
     *
     * @return background function
     */
    TF1* GetBackgroundFunction() const { return fBackgroundFunction; }
    /**
     *
     * @return signal and background function
     * NOTE: only "signal" parameters can be modified because background part takes
     * values from background function
     */
    TF1* GetTotalFunction() const { return fTotalFunction; }
    /**
     *
     * @return integral of background function in peak region
     */
    Double_t GetB() const;
    /**
     *
     * @return integral of signal function in peak region
     */
    Double_t GetS() const;
    /**
     * draw fitted stuff
     * @param option options:
     * autocolor - set automatic colors
     * legend - draw simple legend
     * zero - start drawing of data from zero
     * tot - draw S+B function
     * sig - draw S function
     * bckg draw B function
     * data - draw data graph
     */
    void Draw(Option_t* option = "autocolor+data+sig+bckg+tot");
    virtual ~PeakFitterX();
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MATH_PEAKFITTERX_H_ */
