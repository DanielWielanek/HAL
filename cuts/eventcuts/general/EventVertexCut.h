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
  class Package;
  class EventVertexCut : public EventCut {

  public:
    EventVertexCut();
    static Int_t Rt() { return 0; }
    static Int_t Z() { return 1; };
    Bool_t Init(Int_t format_id);
    Bool_t Pass(Event* event);
    virtual ~EventVertexCut() {};
    ClassDef(EventVertexCut, 1)
  };

  class EventVertexXYZCut : public EventCut {
    TVector3 fShift;

  public:
    EventVertexXYZCut();
    static Int_t Rt() { return 0; }
    static Int_t X() { return 1; };
    static Int_t Y() { return 2; };
    static Int_t Z() { return 3; };
    void SetShift(const TVector3& vec);
    Bool_t Pass(Event* event);
    Package* Report() const;
    virtual ~EventVertexXYZCut() {};
    ClassDef(EventVertexXYZCut, 1)
  };


  class EventVertexZCut : public EventCut {
  public:
    EventVertexZCut();
    Bool_t Pass(Event* event);
    virtual ~EventVertexZCut() {};
    ClassDef(EventVertexZCut, 1);
  };
}  // namespace Hal
#endif /* HALEVENTVERTEXCUT_H_ */
