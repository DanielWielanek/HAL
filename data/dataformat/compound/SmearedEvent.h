/*
 * EventSmeared.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSMEAREDEVENT_H_
#define HALSMEAREDEVENT_H_

#include "ComplexEvent.h"

/**
 * class for reading events with smeared parameters
 */
namespace Hal {
  class SmearedEvent : public ComplexEvent {
    Bool_t fSmearing;
    Bool_t fRealMC;

  public:
    /**
     * default c-tor
     */
    SmearedEvent();
    SmearedEvent(const SmearedEvent& other);
    /**
     * create non-allocated event that can be passed to analysis
     * @param event oryginal format
     */
    SmearedEvent(Event* event);
    void ActivateSmearing() { fSmearing = kTRUE; };
    virtual void Update(EventInterface* interface);
    void Clear(Option_t* opt = " ");
    TString GetFormatName() const;
    virtual ~SmearedEvent();
    ClassDef(SmearedEvent, 1)
  };
}  // namespace Hal

#endif /* HALSMEAREDEVENT_H_ */
