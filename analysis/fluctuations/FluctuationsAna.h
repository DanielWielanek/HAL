/*
 * FluctuationsAna.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFLUCTUATIONSANA_H_
#define HALFLUCTUATIONSANA_H_

#include <TH1.h>

#include "Array.h"
#include "HistogramManager.h"
#include "TrackAna.h"

namespace Hal {
  class FluctuationsAna : public TrackAna {
    Array_1<Int_t> fArray;
    Int_t fBins;
    Int_t fTackCollectionsPerEvent;
    Double_t fMin;
    Double_t fMax;
    HistogramManager_2_1D<TH1D>* fHistograms;

  protected:
    virtual void CheckCutContainerCollections();
    virtual void ProcessEvent();
    Task::EInitFlag Init();

  public:
    FluctuationsAna();
    /**
     * set histogram axis
     * @param bins number of bins
     * @param min min
     * @param max max
     */
    void SetHistogramAxis(Int_t bins, Double_t min, Double_t max);
    /**
     * set histogram axis -  recommended method - in this case range is made in
     * following way that bin centers corresponds from 0, 1.. nbisn
     * @param bins number of bins
     * @param min min
     * @param max max
     */
    void SetHistogramAxis(Int_t bins);
    virtual Package* Report() const;
    virtual ~FluctuationsAna();
    ClassDef(FluctuationsAna, 1)
  };
}  // namespace Hal

#endif /* HALFLUCTUATIONSANA_H_ */
