/*
 * MCEvent.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALMCEVENT_H_
#define HALMCEVENT_H_

#include "Event.h"
/**
 * class for MC events
 */
namespace Hal {
  class McEvent : public Event {
  protected:
    /**
     * impact parameter
     */
    Double_t fB;
    virtual void ShallowCopyEvent(Event* event);
    virtual void ShallowCopyTracks(Event* event);
    McEvent(TString track_classname, TString v0_class = "Hal::V0Track");

  public:
    McEvent();
    McEvent(const McEvent& other);
    virtual void Update();
    inline Double_t GetImpactParameter() const { return fB; };
    virtual EFormatType GetFormatType() const { return EFormatType::kSim; };
    inline McEvent* GetMC() const { return const_cast<McEvent*>(this); };
    virtual Float_t GetFieldVal(Int_t fieldID) const;
    virtual TString GetFieldName(Int_t fieldID) const;
    virtual ~McEvent();
    ClassDef(McEvent, 1)
  };
}  // namespace Hal

#endif /* HALMCEVENT_H_ */
