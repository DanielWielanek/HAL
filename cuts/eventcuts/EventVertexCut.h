/*
 * EventVertexCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEVENTVERTEXCUT_H_
#define HALEVENTVERTEXCUT_H_
#include "EventCut.h"

namespace Hal {
  class EventVertexCut : public EventCut {
  public:
    EventVertexCut();
    static Int_t Rt() { return 0; }
    static Int_t Z() { return 1; };
    virtual Bool_t Init(Int_t format_id);
    virtual Bool_t Pass(Event* event);
    virtual ~EventVertexCut();
    ClassDef(EventVertexCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTVERTEXCUT_H_ */
