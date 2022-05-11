/*
 * ExpEventInterface.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEXPEVENTINTERFACE_H_
#define HALEXPEVENTINTERFACE_H_
#include "EventInterfaceAdvanced.h"

#include <TLorentzVector.h>
#include <TVector3.h>

namespace Hal {
  class ExpEventInterface : public EventInterfaceAdvanced {
  public:
    ExpEventInterface();
    /** GETTERS AND SETTERS **/
    virtual void SetRunInfoId(Int_t /*i*/) {};
    virtual Int_t GetRunInfoId() const { return 0; };
    virtual void SetMagneticField(TVector3 /*mag*/) const {};
    virtual TVector3 GetMagneticField() const { return TVector3(); };
    virtual TLorentzVector GetVertexVector() const { return TLorentzVector(); };
    virtual ~ExpEventInterface();
    ClassDef(ExpEventInterface, 1)
  };
}  // namespace Hal

#endif /* HALEXPEVENTINTERFACE_H_ */
