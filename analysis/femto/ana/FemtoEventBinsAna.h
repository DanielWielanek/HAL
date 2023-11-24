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
    std::vector<EventBinningCut*> fEventBinningCuts;
    Int_t fEventBins;
    Int_t fFakeEventBinID = {0};

    virtual void ProcessEvent();
    virtual Package* Report() const;
    virtual Int_t GetEventBin();
    virtual Bool_t InitArray();
    virtual Task::EInitFlag Init();
    virtual void InitMemoryMap();
    virtual void ProcessFemtoPair();
    virtual void ProcessFemtoPair_Perfect();
    virtual void ProcessFemtoPair_Rotated();
    virtual void ProcessFemtoPair_Hemisphere();
    virtual void ProcessFemtoPair_Mixed();
    virtual void ProcessFemtoPair_Charged();

  public:
    FemtoEventBinsAna();
    virtual ~FemtoEventBinsAna();
    virtual void SetEventBinCut(const EventBinningCut& bin, Option_t* opt = "");
    FemtoEventBinsAna(const FemtoEventBinsAna& other);
    FemtoEventBinsAna& operator=(const FemtoEventBinsAna& other);
    ClassDef(FemtoEventBinsAna, 1)

      virtual void AddCut(const Hal::Cut& cut, Option_t* opt = "");
  };
}  // namespace Hal


#endif /* HALFEMTO_ANALYSIS_FEMTO_ANA_HALFEMTOEVENTBINSANA_H_ */
