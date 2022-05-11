/*
 * DetectedTrack.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEXPTRACK_H_
#define HALEXPTRACK_H_

#include "ToFTrack.h"
#include "TpcTrack.h"

#include "Helix.h"
#include "Track.h"

#include <TObject.h>
#include <TVector3.h>

/**
 * basic representation of experimental track
 */
namespace Hal {
  class ExpTrack : public Track {
  public:
  private:
    Int_t fNHits;
    Float_t fChi2;
    Float_t fTrackLenght;
    TVector3 fDCA;

  public:
    ExpTrack();
    /**
     * copy ctor
     * @param other
     */
    ExpTrack(const ExpTrack& other) = default;
    /**
     * Assignment operator
     * @param track
     * @return
     */
    ExpTrack& operator=(const ExpTrack& track) = default;
    /**
     *
     * @return number of hits in all detectors
     */
    inline Int_t GetNHits() const { return fNHits; };
    /**
     *
     * @return chi2 of fitting in all detectors
     */
    inline Float_t GetChi2() const { return fChi2; }
    /**
     *
     * @return track lenghth
     */
    inline Float_t GetTrackLenght() const { return fTrackLenght; }
    /**
     *
     * @return DCA vector
     */
    inline const TVector3& GetDCA() const { return fDCA; };
    /**
     * set DCA
     * @param x
     * @param y
     * @param z
     */
    inline void SetDCA(Double_t x, Double_t y, Double_t z) { fDCA.SetXYZ(x, y, z); };
    /**
     *
     * @param nHits set total number of htis
     */
    void SetNHits(Int_t nHits) { fNHits = nHits; }
    /**
     *
     * @param chi2 set chi2 of fit in all detectors
     */
    void SetChi2(Float_t chi2) { fChi2 = chi2; }
    /**
     * set track lenght
     * @param lenght
     */
    void SetTrackLenght(Float_t lenght) { fTrackLenght = lenght; }
    /**
     *
     * @param detID detector id
     * @return
     */
    virtual DetectorTrack* GetDetTrack(const UInt_t /*detID*/) const { return nullptr; };
    virtual void CopyData(Track* other);
    /**
     *
     * @return true if track is global
     */
    Bool_t IsGlobal() const { return TESTBIT(fType, kGlobal); };
    /**
     * set status of track
     * @param global set true if track is global
     */
    void SetGlobal(Bool_t global = kTRUE);
    /**
     * return track property by ID, this should be positve number above 100 (for ID outside of framework)
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
    virtual ~ExpTrack();
    ClassDef(ExpTrack, 1)
  };

  class ExpTrackHelix : public ExpTrack {
    Helix fHelix;

  protected:
    void UpdateHelix();

  public:
    ExpTrackHelix();
    /**
     * copy ctor
     * @param other
     */
    ExpTrackHelix(const ExpTrackHelix& other) = default;
    /**
     * Assignment operator
     * @param track
     * @return
     */
    ExpTrackHelix& operator=(const ExpTrackHelix& track) = default;
    virtual void CopyData(Track* other);
    /**
     *
     * @return helix  - warning assumed fiedl at const Z!
     */
    inline const Helix& GetHelix() const { return fHelix; };
    virtual ~ExpTrackHelix();
    ClassDef(ExpTrackHelix, 1)
  };
}  // namespace Hal
#endif /* HALEXPTRACK_H_ */
