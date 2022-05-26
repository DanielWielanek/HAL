/*
 * MCTrackInterface.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALMCTRACKINTERFACE_H_
#define HALMCTRACKINTERFACE_H_

#include "TrackInterfaceAdvanced.h"
namespace Hal {
  class McTrackInterface : public TrackInterfaceAdvanced {
  public:
    McTrackInterface();
    virtual Double_t GetStartX() const { return 0.; };
    virtual Double_t GetStartY() const { return 0.; };
    virtual Double_t GetStartZ() const { return 0.; };
    virtual Double_t GetStartT() const { return 0.; };
    virtual void SetStart(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/, Double_t /*t*/) {};
    virtual Double_t GetFreezX() const { return 0.; };
    virtual Double_t GetFreezY() const { return 0.; };
    virtual Double_t GetFreezZ() const { return 0.; };
    virtual Double_t GetFreezT() const { return 0.; };
    virtual void SetFreez(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/, Double_t /*t*/) {};
    virtual Int_t GetPdg() const { return 0; };
    virtual void SetPdg(Int_t /*pdg*/) {};
    virtual ~McTrackInterface();
    ClassDef(McTrackInterface, 1)
  };
}  // namespace Hal
#endif /* HALROOT_DATAFORMAT_HALMCTRACKINTERFACE_H_ */
