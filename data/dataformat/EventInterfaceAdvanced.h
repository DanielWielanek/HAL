/*
 * AdvancedEventInterface.h
 *
 *  Created on: 4 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_DATAFORMAT_HALEVENTINTERFACEADVANCED_H_
#define HAL_DATAFORMAT_HALEVENTINTERFACEADVANCED_H_

#include "EventInterface.h"
namespace Hal {
  class TrackInterface;

  class EventInterfaceAdvanced : public EventInterface {
  protected:
    Bool_t fCanDeleteEvent;
    TrackInterface* fTrInterface;
    virtual void ConnectToTreeInternal(EventInterface::eMode mode);
    EventInterfaceAdvanced(TrackInterface* interface);

  public:
    EventInterfaceAdvanced();
    /**
     * boost this event
     * @param vx x-velocity
     * @param vy y-velocity
     * @param vz z-velocit
     */
    virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
    /**
     * clear this object
     * @param opt
     */
    virtual void Clear(Option_t* /*opt*/ = "") {};
    /**
     * compress this event
     * @param map
     * @param map_size
     */
    virtual void Compress(Int_t* map, Int_t map_size) = 0;
    /**
     * copy source from one event into this event
     * @param s event to copy into this
     */
    virtual void CopyData(EventInterface* s) = 0;

    /**
     * make copy and compress event "s" into this object
     * @param s event to copy
     * @param map map of copying
     * @param map_size size of map
     */
    virtual void CopyAndCompress(EventInterface* s, Int_t* map, Int_t map_size) = 0;
    /**
     * Fill track interface - set track data in original format into track
     * interface. If information about track is stored in single object this is
     * equivalednt of TrackInterface->SetRawTrack(). However not  in all cases
     * it's possible to get all informations about track from single object.
     * @param track track interface to fill
     * @param index index of track
     */
    virtual void FillTrackInterface(TrackInterface* track, Int_t index) = 0;
    /**
     *
     * @return total number of tracks
     */
    virtual Int_t GetTotalTrackNo() const = 0;
    /**
     *
     * @return new allocated interface to tracks in this tree
     */
    TrackInterface* GetTrackInterface() const { return fTrInterface; };
    /**
     * return pointer to track in source
     * @param index
     * @return
     */
    virtual TObject* GetRawTrackPointer(Int_t index) const = 0;
    /**
     * pointer to event/ event header
     * @return  pointer to event/ event header
     */
    virtual TObject* GetRawEventPointer() const = 0;
    /** GETTER SETTER SECTION **/
    /**
     * set vertex position
     * @param x
     * @param y
     * @param z
     */
    virtual void SetVertex(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) {};
    /**
     * set event reaction plane angle and error
     * @param phi
     * @param phi_error
     */
    virtual void SetPhi(Double_t /*phi*/, Double_t /*phi_error*/ = 0) {};
    /**
     *
     * @return event reaction plane
     */
    virtual Double_t GetPhi() const { return 0.; };
    /**
     *
     * @return reaction plane angle uncrtainty
     */
    virtual Double_t GetPhiError() const { return 0.; };
    /**
     *
     * @return vertex position
     */
    virtual TLorentzVector GetVertex() const { return TLorentzVector(0., 0., 0., 0.); };
    virtual ~EventInterfaceAdvanced();
    ClassDef(EventInterfaceAdvanced, 1)
  };
}  // namespace Hal
#endif /* HAL_DATAFORMAT_HALEVENTINTERFACEADVANCED_H_ */
