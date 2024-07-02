/*
 * NicaUnigenSource.h
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAUNIGENEVENTINTERFACE_H_
#define NICAUNIGENEVENTINTERFACE_H_

#include "UnigenEvent.h"

#include <TObject.h>

#include "EventInterfaceAdvanced.h"
#include "Pointer.h"

/**
 * event interface for UnigenData
 */


namespace Hal {
  class TrackInterface;
}
namespace HalUni {
  class UnigenEventInterface : public Hal::EventInterfaceAdvanced {
    friend class UnigenEvent;
    Hal::ObjectDoublePointer* fEventPointer = {nullptr};
    UEvent* GetUEvent() const { return (UEvent*) fEventPointer->GetPointer(); };

  protected:
    virtual void ConnectToTreeInternal(eMode mode);

  public:
    UnigenEventInterface();
    virtual Int_t GetTotalTrackNo() const { return GetUEvent()->GetNpa(); }
    virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
    virtual void Register(Bool_t write);
    virtual void Compress(Int_t* map, Int_t map_size);
    virtual void CopyData(Hal::EventInterface* s);
    virtual void CopyAndCompress(Hal::EventInterface* s, Int_t* map, Int_t map_size);
    virtual void FillTrackInterface(Hal::TrackInterface* track, Int_t index);
    TObject* GetRawEventPointer() const { return GetUEvent(); };
    virtual TObject* GetRawTrackPointer(Int_t index) const { return GetUEvent()->GetParticle(index); };
    virtual void Clear(Option_t* /*opt*/ = "") { GetUEvent()->Clear(); };
    /** GETTER SETTER SECTION*/
    virtual void SetB(Double_t b) { GetUEvent()->SetB(b); };
    virtual void SetVertex(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) {};
    virtual void SetPhi(Double_t phi, Double_t /*phi_error*/) { GetUEvent()->SetPhi(phi); };
    virtual Double_t GetB() const { return GetUEvent()->GetB(); };
    virtual Double_t GetPhi() const { return GetUEvent()->GetPhi(); };
    virtual ~UnigenEventInterface();
    ClassDef(UnigenEventInterface, 1)
  };
}  // namespace HalUni
#endif /* NICAUNIGENEVENTINTERFACE_H_ */
