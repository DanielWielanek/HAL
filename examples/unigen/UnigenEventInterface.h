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

/**
 * event interface for UnigenData
 */


namespace Hal {
  class TrackInterface;
}
namespace HalUni {
  class UnigenEventInterface : public Hal::EventInterfaceAdvanced {
    friend class UnigenEvent;
    UEvent* fEvent;

  protected:
    virtual void ConnectToTree();

  public:
    UnigenEventInterface();
    virtual Int_t GetTotalTrackNo() const { return fEvent->GetNpa(); }
    virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
    virtual void Register(Bool_t write);
    virtual void Compress(Int_t* map, Int_t map_size);
    virtual void CopyData(Hal::EventInterface* s);
    virtual void CopyAndCompress(Hal::EventInterface* s, Int_t* map, Int_t map_size);
    virtual void FillTrackInterface(Hal::TrackInterface* track, Int_t index);
    TObject* GetRawEventPointer() const { return fEvent; };
    virtual TObject* GetRawTrackPointer(Int_t index) const { return fEvent->GetParticle(index); };
    virtual Hal::TrackInterface* GetTrackInterface() const;
    virtual void Clear(Option_t* opt = "") { fEvent->Clear(); };
    /** GETTER SETTER SECTION*/
    virtual void SetB(Double_t b) { fEvent->SetB(b); };
    virtual void SetVertex(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) {};
    virtual void SetPhi(Double_t phi, Double_t /*phi_error*/) { fEvent->SetPhi(phi); };
    virtual Double_t GetB() const { return fEvent->GetB(); };
    virtual Double_t GetPhi() const { return fEvent->GetPhi(); };
    virtual ~UnigenEventInterface();
    ClassDef(UnigenEventInterface, 1)
  };
}  // namespace HalUni
#endif /* NICAUNIGENEVENTINTERFACE_H_ */
