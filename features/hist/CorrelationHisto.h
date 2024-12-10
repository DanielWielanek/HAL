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

class TH1;

namespace Hal {
  /**
   * class for plotting correlations betwen pais of parameters
   */
  class CorrelationHisto : public Object {
    std::vector<HistogramAxisConf> fAxesConf;
    std::vector<std::vector<double>> fTestValues;
    Int_t fNHistograms = {0};
    Int_t fNParams     = {0};
    Int_t fNTest       = {0};
    std::vector<TH1*> fHistograms;
    Bool_t fAutoFill = {kFALSE};

    void Initialize();
    void AutoInitialize();

  public:
    /**
     * default ctor
     * @param nParams number of parameters to monitor
     * @param nTest number of test samples
     */
    CorrelationHisto(Int_t nParams = 1, Int_t nTest = 1000);
    /**
     * configure single parameters
     * @param par paramenter number
     * @param bins number of bins in histograms
     * @param min minimal value of histogram
     * @param max maximal value of histogram
     * @param name name of parameter
     */
    void ConfigParam(Int_t par, Int_t bins, Double_t min, Double_t max, TString name);
    /**
     *
     * @param vals - config of histograms e.g., for 2 parameters :{{binsA, minA, minB},{binsB,minB, maxB}}
     * @param names = config of histograms e.g, for 2 parameters {"a","b}
     */
    CorrelationHisto(std::initializer_list<std::initializer_list<Double_t>> vals, std::initializer_list<TString> names = {});
    /**
     * fill histograms and automatically set the optimized limits of histograms, after fill -nsamples times
     * memeory of histogram is allocated and FillUnchecked can be used
     * @param params
     */
    void Fill(std::vector<Double_t> params);
    /**
     * fill histograms without set optimized limites
     * @param params
     */
    void FillUnchecked(std::vector<Double_t> params);
    /**
     *
     * @param opt additional draw options avaiable logz logx and logy
     * and gridx grid, note - because 1d histograms are kind of projections using logy will not be used here, use logz instead
     * you can also pass other options directly to histogram
     */
    void Draw(Option_t* opt = "");
    /**use to initialize histogram
     * user must call Config params before calling this method
     * later fill unchecked can be used
     **/
    void Init() { Initialize(); }
    /**
     * return correlations between two parameters
     * @param parA
     * @param parB
     * @return
     */
    TH1* GetCorrelationHisto(Int_t parA, Int_t parB) const;
    /**
     *
     * @return number of histograms
     */
    Int_t GetEntries() const { return fNHistograms; }
    virtual Bool_t CanBeTableElement() const;
    virtual Long64_t Merge(TCollection* collection);
    virtual TString GetExtractType() const;
    virtual TString HTMLExtract(Int_t, TString) const;
    virtual void Add(const Hal::Object* pack);
    /**
     *
     * @return raw array of histograms
     */
    std::vector<TH1*> GetHistograms() const { return fHistograms; }
    virtual ~CorrelationHisto();
    ClassDef(CorrelationHisto, 2)
  };

}  // namespace Hal

#endif /* HAL_FEATURES_HIST_CORRELATIONHISTO_H_ */
