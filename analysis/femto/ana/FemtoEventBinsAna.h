/*
 * FemtoEventBinsAna.h
 *
 *  Created on: 4 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_ANA_HALFEMTOEVENTBINSANA_H_
#define HALFEMTO_ANALYSIS_FEMTO_ANA_HALFEMTOEVENTBINSANA_H_

#include "FemtoBasicAna.h"

#include "Array.h"

#include <Rtypes.h>
#include <RtypesCore.h>


namespace Hal {
  class EventBinningCut;

  class FemtoEventBinsAna : public FemtoBasicAna {
  protected:
    /**
     * holds array of event cuts used for binning mixing
     */
    TObjArray* fEventBinningCuts;
    /**
     * Contains number of bins in event cuts used for binning
     */
    Array_1<Int_t>* fEventBinnngsBinsNo;
    Int_t fTotalEventBins;

    virtual void ProcessEvent();
    virtual Package* Report() const;
    virtual Int_t GetEventBin();
    virtual Task::EInitFlag Init();
    virtual void InitMemoryMap();

  public:
    FemtoEventBinsAna();
    virtual ~FemtoEventBinsAna();
    virtual void AddEventBinCut(const EventBinningCut& bin, Option_t* opt);
    FemtoEventBinsAna(const FemtoEventBinsAna& other);
    FemtoEventBinsAna& operator=(const FemtoEventBinsAna& other);
    ClassDef(FemtoEventBinsAna, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_ANALYSIS_FEMTO_ANA_HALFEMTOEVENTBINSANA_H_ */
