/*
 * NicaUnigenTrackInterface.h
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAUNIGENTRACKINTERFACE_H_
#define NICAUNIGENTRACKINTERFACE_H_

#include "McTrackInterface.h"
#include "UParticle.h"
/**
 * interface to UParticle
 */
namespace HalUni {
  class UnigenTrackInterface : public Hal::McTrackInterface {
    TObject* fRawObject;

  public:
    UnigenTrackInterface();
    void SetID(Int_t id) { ((UParticle*) fRawObject)->SetUniqueID(id); };
    void SetPxPyPzE(Double_t px, Double_t py, Double_t pz, Double_t e) { ((UParticle*) fRawObject)->SetMomentum(px, py, pz, e); };
    void SetStatus(Int_t stat) { ((UParticle*) fRawObject)->SetStatus(stat); };
    void SetPrimary(Int_t /*prim*/) {};
    Double_t GetPx() const { return ((UParticle*) fRawObject)->Px(); };
    Double_t GetPy() const { return ((UParticle*) fRawObject)->Py(); };
    Double_t GetPz() const { return ((UParticle*) fRawObject)->Pz(); };
    Double_t GetE() const { return ((UParticle*) fRawObject)->E(); };
    Int_t GetMotherIndex() const { return ((UParticle*) fRawObject)->GetMate(); };
    Int_t GetStatus() const { return ((UParticle*) fRawObject)->GetStatus(); };
    Bool_t IsPrimary() const { return kTRUE; };
    virtual ~UnigenTrackInterface();
    ClassDef(UnigenTrackInterface, 1)
  };
}  // namespace HalUni

#endif /* NICAROOT_DATAFORMAT_FORMATS_NICAUNIGENTRACKINTERFACE_H_ */
