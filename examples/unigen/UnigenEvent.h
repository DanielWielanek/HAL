/*
 * NicaUnigen.h
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAUNIGENEVENT_H_
#define NICAUNIGENEVENT_H_

#include "UnigenTrack.h"

#include "McEvent.h"
#include "UEvent.h"
#include "UParticle.h"
/**
 * class used for representation of unigen event in "fake" format
 */
namespace HalUni {
  class UnigenEvent : public Hal::McEvent {
  public:
    /**
     * default constructor
     */
    UnigenEvent();
    UnigenEvent(const UnigenEvent& other);
    Hal::EventInterface* CreateInterface() const;
    virtual void Update(Hal::EventInterface* interface);
    virtual void Clear(Option_t* opt = " ");
    virtual Bool_t ExistInTree() const;
    virtual TString GetFormatName() const;
    virtual ~UnigenEvent();
    ClassDef(UnigenEvent, 1)
  };
}  // namespace HalUni
#endif /* NICAUNIGENEVENT_H_ */
