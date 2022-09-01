/*
 * Track.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACK_H_
#define HALTRACK_H_

#include "V0Track.h"

#include "Link.h"

#include <TLorentzVector.h>

#include <vector>

namespace Hal {
  class Event;

  /**
   * basic abstract class for storing information about tracks
   */
  class Track : public TObject {
    friend class Event;

  protected:
    /**
     * pointer to parent event
     */
    Event* fEvent;  //! don't stream it will be build on the fly
    TLorentzVector fP;
    Int_t fID;
    Int_t fStatus;
    Int_t fHiddenInfo;
    Int_t fMotherID;
    Int_t fThisID;
    UInt_t fType;
    Float_t fCharge;
    enum ETrackBits {
      kPrimary     = 0,
      kV0          = 1,
      kXi          = 2,
      kGlobal      = 3,
      kMother      = 4,  // set if mother is known (and its secondary)
      kV0Daughters = 5,  // set if v0 and daughters are known
      kXiDaughters = 6   // set if xi and daughters are known
    };

  public:
    Track();
    /**
     * copy constructor
     * @param track
     */
    Track(const Track& track);
    /**
     * assignement operator
     * @param track
     * @return
     */
    Track& operator=(const Track& track) = default;
    /**
     * set position id of this object (position in clones array equal to the position of original object
     * @param id
     */
    inline void SetThisID(Int_t id) { fThisID = id; };
    /**
     * return position of this object in array
     * @return
     */
    inline Int_t GetThisID() const { return fThisID; };
    /**
     * set ThisID and ID simultaneously
     * @param id
     */

    inline void SetIDs(Int_t id) { fThisID = fID = id; }
    /**
     * return position of hiddend data
     * @return
     */
    inline Int_t GetHiddenInfoIndex() const { return fHiddenInfo; };
    /**
     *
     * @return track Px
     */
    inline Double_t GetPx() const { return fP.Px(); };
    /**
     *
     * @return track Py
     */
    inline Double_t GetPy() const { return fP.Py(); };
    /**
     *
     * @return track Pz
     */
    inline Double_t GetPz() const { return fP.Pz(); };
    /**
     *
     * @return track energy
     */
    inline Double_t GetE() const { return fP.E(); };
    /**
     * @return 4-momentum
     */
    inline const TLorentzVector& GetMomentum() const { return fP; };
    /**
     * boost this particle
     * @param vx
     * @param vy
     * @param vz
     */
    virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
    /**
     * print information about track
     */
    virtual void Print(Option_t* option = "") const;
    /**
     * set charge of particle
     * @param charge
     */
    inline void SetCharge(Double_t charge) { fCharge = charge; };
    /**
     * set particle id
     * @param id
     */
    inline void SetID(Int_t id) { fID = id; };
    /**
     * set status of particle
     * @param status
     */
    inline void SetStatus(Int_t status) { fStatus = status; };
    /**
     * set momentum
     * @param px
     * @param py
     * @param pz
     * @param e
     */
    inline void SetMomentum(Double_t px, Double_t py, Double_t pz, Double_t e) { fP.SetPxPyPzE(px, py, pz, e); }
    /**
     *
     * @param v0 if true mark particle as V0, otherwise mark as normal particle
     * @param daughters  - true if daughters ID's are known
     */
    void EnableV0(Bool_t v0, Bool_t daughters = kTRUE);
    /**
     * set particle mother index
     * @param index
     */
    inline void SetMotherIndex(Int_t index) {
      CLRBIT(fType, kPrimary);
      if (index >= 0) SETBIT(fType, kMother);
      fMotherID = index;
    };
    /**
     *
     * @return mass of track
     */
    inline Double_t GetMass() const { return fP.M(); };
    /**
     *
     * @return charge of track
     */
    inline Double_t GetCharge() const { return fCharge; };
    /**
     *
     * @return id of hidden info
     */
    inline Int_t GetHiddenInfoId() const { return fHiddenInfo; };
    /**
     *
     * @return track id
     */
    virtual Int_t GetID() const { return fID; };
    /**
     *
     * @return track status
     */
    inline Int_t GetStatus() const { return fStatus; };
    /**
     *
     * @return mother index, -1 if particle has no mother
     */
    inline Int_t GetMotherIndex() const { return fMotherID; };
    /**
     *
     * @return true if track is primary, false otherwise
     */
    inline Bool_t IsPrimary() const { return TESTBIT(fType, kPrimary); };
    /**
     *
     * @return true if particle is secondary and mother ID is known
     */
    inline Bool_t IsGoodSecondary() const { return TESTBIT(fType, kMother); };
    /**
     *
     * @return true if particle is V0 and daughters ID's are known
     */
    inline Bool_t IsGoodV0() const { return TESTBIT(fType, kV0Daughters); };
    /**
     *
     * @return true if track is global
     */
    inline Bool_t IsGlobal() const { return TESTBIT(fType, kGlobal); };
    /**
     *
     * @return pointer to standard hidden info
     */
    V0Track* GetV0Info() const;
    /**
     * mark tras as global if global is true
     * @param global
     */
    inline void SetGlobal(Bool_t global = kTRUE) {
      if (global)
        SETBIT(fType, kGlobal);
      else
        CLRBIT(fType, kGlobal);
    };
    /**
     * clear type flags
     */
    inline void ClearType() { fType = 0; };
    /**
     * set energy in TLorentzMomentum
     * @param mass
     */
    void SetMass(Double_t mass);
    /**
     * mark particle as primary
     */
    void SetPrimary();
    /**
     * mark this particle as secondary
     * @param parent true if parent iD is known
     */
    void SetSecondary(Bool_t parent);
    /**
     *
     * @return pointer to original track
     */
    TObject* GetTrackPointer() const;
    /**
     * translate links of this track into new links
     * @param map
     */
    void TranslateLinks(Int_t* map);
    /**
     * translate links of this track into new links
     * @param vec
     */
    void TranslateLinksVec(const std::vector<int>& vec);
    /**
     *
     * @return true if particle has 3 dependencies (probably V0)
     */
    inline Bool_t IsV0() const { return TESTBIT(fType, kV0); };
    /**
     * copy basic data from other track
     * does not copy event or "thisID"
     * @param other
     */
    virtual void CopyData(Track* other);
    /**
     * copy all data from other track
     * @param other
     */
    virtual void CopyAllData(Track* other);
    /**
     * clear flags and links
     * @param
     */
    virtual void Clear(Option_t* opt = "");
    /**
     * clean variables before filling with values overwriten in ComplexTrack
     * @param thisID
     * @param event
     */
    virtual void ResetTrack(Int_t thisID = -1, Event* event = nullptr);
    /**
     *
     * @return pointer to event that own this track
     */
    Event* GetEvent() const { return fEvent; };
    /**
     *
     * @return indexes of needed other tracks /children, parent
     */
    virtual std::vector<int> GetLinks() const;
    /**
     *
     * @param vec indexes of needed other tracks
     */
    virtual void SetLinks(std::vector<int>& vec);
    /**
     * set event for this track
     * @param event
     */
    void SetEvent(Event* event) { fEvent = event; };
    /**
     * return track property by ID, this should be positve number larger than 200 and smaller than 10000
     * @param fieldID
     * @return
     */
    virtual Float_t GetFieldVal(Int_t fieldID) const;
    /**
     * return name of the track property by ID
     * @param fieldID
     * @return
     */
    virtual TString GetFieldName(Int_t fieldID) const;
    virtual ~Track();
    ClassDef(Track, 1)
  };
}  // namespace Hal

#endif /* HALTRACK_H_ */
