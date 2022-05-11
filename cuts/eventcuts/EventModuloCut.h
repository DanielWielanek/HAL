/*
 * EventModuloCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEVENTMODULOCUT_H_
#define HALEVENTMODULOCUT_H_
#include "EventCut.h"

/**
 * class for rejecting /accpeting events with fixed interwal
 * accept classes only with modulo given by cut min max
 * important note - this class use own counter not event number !
 */

namespace Hal {
  class EventModuloCut : public EventCut {
    Int_t fCount;
    Int_t fModulo;

  public:
    EventModuloCut(Int_t modulo = 1);
    void SetModulo(Int_t modulo) { fModulo = modulo; };
    Bool_t Pass(Event* event);
    virtual ~EventModuloCut();
    virtual Package* Report() const;
    ClassDef(EventModuloCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTMODULOCUT_H_ */
