/*
 * DetectedEvent.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEXPEVENT_H_
#define HALEXPEVENT_H_
#include "Event.h"

#include <TString.h>

namespace Hal {
  class DetectorEvent;

  /**
   * basic class for representation of detected event
   */
  class ExpEvent : public Event {
  protected:
    Int_t fRunInfoId;
    Int_t fEventTrigger;
    Int_t fNTofTracks;
    TVector3* fMagField;
    TLorentzVector* fVertexError;
    virtual void ShallowCopyEvent(Event* event);
    ExpEvent(TString track_class, TString v0_class = "Hal::V0Track");

  public:
    ExpEvent();
    ExpEvent(const ExpEvent& other);
    /**
     *
     * @return number of tracks with at least one tof hit
     */
    inline Int_t GetNTofTracks() const { return fNTofTracks; };
    /**
     *
     * @return run id
     */
    inline Int_t GetRunId() const { return fRunInfoId; }
    /**
     *
     * @return DetectorEvent
     */
    virtual DetectorEvent* GetDetEvent() const { return NULL; };
    virtual Float_t GetFieldVal(Int_t fieldID) const;
    virtual TString GetFieldName(Int_t fieldID) const;
    TLorentzVector* GetVertexError() const { return fVertexError; };
    TVector3* GetMagField() const { return fMagField; };
    virtual ~ExpEvent();
    ClassDef(ExpEvent, 1)
  };

  class ExpEventHelix : public ExpEvent {
  protected:
    ExpEventHelix(TString track_class, TString v0_class = "V0Track");

  public:
    ExpEventHelix();
    ExpEventHelix(const ExpEventHelix& other);
    virtual ~ExpEventHelix();
    ClassDef(ExpEventHelix, 1)
  };
}  // namespace Hal
#endif /* HALEXPEVENT_H_ */
