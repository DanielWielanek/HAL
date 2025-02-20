/*
 * MultiplicityAna.h
 *
 *  Created on: 4 gru 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FLUCTUATIONS_MULTIPLICITYANA_H_
#define HAL_ANALYSIS_FLUCTUATIONS_MULTIPLICITYANA_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>
#include <TString.h>
#include <vector>

#include "HistogramManager.h"
#include "TrackAna.h"

namespace Hal {
  class CorrelationHisto;
} /* namespace Hal */

namespace Hal {
  /**
   * plots number of tracks that passed event and track cuts
   * can be used e.g. to define the distribution of given particles as a function of centrality
   */

  class MultiplicityAna : public TrackAna {
  protected:
    std::vector<TString> fEventNames;
    std::vector<TString> fTrackNames;
    std::vector<Double_t> fCounts;
    std::vector<std::vector<int>> fMatchIndex;
    HistogramManager_2_1D<TH1D>* fData = {nullptr};
    std::vector<CorrelationHisto*> fCovariances;

    Int_t fMultiplicityBins   = {100};
    Double_t fMultiplicityMin = {0};
    Double_t fMultiplicityMax = {100};
    Double_t fCovMax          = {0};
    Double_t fCovMin          = {0};
    Int_t fCovBins            = {0};
    Bool_t fUseCov            = {kFALSE};

    virtual void LinkCollections();
    virtual void ProcessEvent();
    virtual Hal::Package* Report() const;
    virtual void ProcessTrack();

  public:
    MultiplicityAna();
    void SetCovariancesMax(Int_t bins, Double_t min, Double_t max) {
      fCovBins = bins;
      fCovMax  = max;
      fCovMin  = min;
    }
    void SetMcPidNames(TString opt = "all");
    void SetMultiplicityAxis(Int_t bin, Double_t min, Double_t max) {
      fMultiplicityBins = bin;
      fMultiplicityMin  = min;
      fMultiplicityMax  = max;
      fUseCov           = kTRUE;
    }
    void SetEventCollectionNames(std::vector<TString> names) { fEventNames = names; };
    void SetTrackCollectionNames(std::vector<TString> names) { fTrackNames = names; };
    virtual Hal::Task::EInitFlag Init();
    virtual ~MultiplicityAna();
    ClassDef(MultiplicityAna, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FLUCTUATIONS_MULTIPLICITYANA_H_ */
