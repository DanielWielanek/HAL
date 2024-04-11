/*
 * CorrHisto.h
 *
 *  Created on: 11 kwi 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_CORRELATIONHISTO_H_
#define HAL_FEATURES_HIST_CORRELATIONHISTO_H_

#include <RtypesCore.h>
#include <TString.h>
#include <initializer_list>
#include <vector>

#include "HistogramManager.h"
#include "Object.h"

class TH2D;

namespace Hal {

  class CorrelationHisto : public Object {
    std::vector<HistogramAxisConf> fAxesConf;
    std::vector<std::vector<double>> fTestValues;
    Int_t fNHistograms = {0};
    Int_t fNParams     = {0};
    Int_t fNTest       = {0};
    TH2D** fHistograms = {nullptr};  //[fNHistograms]
    Bool_t fAutoFill   = {kFALSE};

    void Initialize();
    void AutoInitialize();

  public:
    /**
     * default ctor
     * @param nParams number of parameters to monitor
     * @param nTest number of test samples
     */
    CorrelationHisto(Int_t nParams = 1, Int_t nTest = 1000);
    void ConfigParam(Int_t par, Int_t bins, Double_t min, Double_t max, TString name);
    /**
     *
     * @param vals - config of histograms for 2 parameters :{{binsA, minA, minB},{binsB,minB, maxB}}
     */
    CorrelationHisto(std::initializer_list<std::initializer_list<Double_t>> vals, Int_t nTest = 1000);
    void Fill(std::vector<Double_t> params);
    void FillUnchecked(std::vector<Double_t> params);
    void Draw(Option_t* opt = "");
    virtual ~CorrelationHisto();
    ClassDef(CorrelationHisto, 1)
  };

}  // namespace Hal

#endif /* HAL_FEATURES_HIST_CORRELATIONHISTO_H_ */
