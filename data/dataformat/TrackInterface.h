/*
 * TrackInterface.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKINTERFACE_H_
#define HALTRACKINTERFACE_H_

#include <TObject.h>
/**
 * basic class for representig tracks from tree as common tracks
 */
namespace Hal {
  class TrackInterface : public TObject {
  protected:
    const Int_t fRawObjNo;
    TObject** fRawObject;  //[fRawObjNo]

  public:
    TrackInterface(Int_t rawObj = 1);
    /**
     *
     * @param charge set charge
     */
    virtual void SetCharge(Double_t /*charge*/) {};
    /**
     *
     * @param id particle id
     */
    virtual void SetID(Int_t /*id*/) {};
    /**
     * set four-momentum
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    virtual void SetPxPyPzE(Double_t /*px*/, Double_t /*py*/, Double_t /*pz*/, Double_t /*e*/) {};
    /**
     *
     * @param index mother index
     */
    virtual void SetMotherIndex(Int_t /*index*/) {};
    /**
     *
     * @param stat particle status
     */
    virtual void SetStatus(Int_t /*stat*/) {};
    /**
     *
     * @param prim true if particle should be primary false otherwise
     */
    virtual void SetPrimary(Int_t /*prim*/) {};
    /**
     *
     * @return Px
     */
    virtual Double_t GetPx() const { return 0.; };
    /**
     *
     * @return Py
     */
    virtual Double_t GetPy() const { return 0.; };
    /**
     *
     * @return Pz
     */
    virtual Double_t GetPz() const { return 0.; };
    /**
     *
     * @return energy
     */
    virtual Double_t GetE() const { return 0.; };
    /**
     *
     * @return particle charge
     */
    virtual Double_t GetCharge() const { return 0.; };
    /**
     *
     * @return particle ID
     */
    virtual Int_t GetID() const { return 0; };
    /**
     *
     * @return mother index
     */
    virtual Int_t GetMotherIndex() const { return 0; };
    /**
     *
     * @return particle status
     */
    virtual Int_t GetStatus() const { return 0; };
    /**
     * return daughter Ids
     * @param i
     * @return
     */
    virtual Int_t GetDaughterIndex(Int_t /*i*/) { return 0; };
    /**
     *
     * @return true if this is primary particle
     */
    virtual Bool_t IsPrimary() const { return kTRUE; };
    /**
     * set pointer to oryginal track
     * @param source
     * @param no - number of obj in array of objects
     */
    void SetRawTrack(TObject* source, Int_t obj = 0) { fRawObject[obj] = source; };
    /**
     *
     *@param no - number of objects in array of objects
     * @return pointer to oryginal track
     */
    TObject* GetSource(Int_t obj = 0) const { return fRawObject[obj]; };
    virtual ~TrackInterface();
    ClassDef(TrackInterface, 1)
  };
}  // namespace Hal
#endif /* HALROOT_DATAFORMAT_HALTRACKINTERFACE_H_ */
