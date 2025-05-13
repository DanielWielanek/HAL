/*
 * EventSmearTask.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSMOOTHTASK_H_
#define HALSMOOTHTASK_H_

#include "SmearAlgorithm.h"

#include "TrackAna.h"

#include <Rtypes.h>
#include <RtypesCore.h>


/**
 * task for smearing events, later such "smeared" events can be used in analysis
 * as normal events but usually some parameters are modified by this class to
 * simulate detector response
 * TODO rewrite this for complex event purely
 */
namespace Hal {
  class SmearTask : public TrackAna {
  protected:
    /**
     * output format ID
     */
    Int_t fOutputFormatID;
    /**
     * if true then smeared events are saved
     */
    Bool_t fSave;
    /**
     * pointer to smearing algorithm
     */
    EventSmear* fEventAlgorithm;
    /**
     * track smearing algorithm
     */
    TrackSmear* fTrackAlgorithm;
    virtual void ProcessEvent();
    virtual void CheckCutContainerCollections();
    virtual Task::EInitFlag Init();
    Package* Report() const;

  public:
    SmearTask();
    /**
     * if called then smeared events will be saved in output tree
     */
    void SaveEvents();
    /**
     * for adding objects like in Track @see TrackAna::Add
     * here you can also add smering algorithm
     * @param obj
     * @param opt
     */
    using Task::Add;
    virtual void SetTrackSmearAlgo(TrackSmear& smear) { fTrackAlgorithm = (TrackSmear*) smear.MakeCopy(); }
    virtual void SetEventSmearAlgo(EventSmear& smear) { fEventAlgorithm = (EventSmear*) smear.MakeCopy(); }
    virtual void Exec(Option_t* opt);
    virtual ~SmearTask();
    ClassDef(SmearTask, 1)
  };
}  // namespace Hal
#endif /* HALSMOOTHTASK_H_ */
