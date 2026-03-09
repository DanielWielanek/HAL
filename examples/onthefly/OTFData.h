/*
 * OTFData.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFDATA_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFDATA_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TVector3.h>

#include "McTrack.h"

namespace OTF {
  class RecoTrack : public TObject {
    TLorentzVector fMomentum;
    Int_t fIndexMc;
    Int_t fNHitsA, fNHitsB, fNHitsC;
    Int_t fCharge;
    Int_t fDaugthers[2] = {-1, -1};
    Int_t fGeneratorId  = {0};
    Int_t fPidHypo      = {-1};

  public:
    RecoTrack() : fIndexMc(-1), fNHitsA(0), fNHitsB(0), fNHitsC(0), fCharge(0) {};
    Int_t GetMcIndex() const { return fIndexMc; }
    Int_t GetNHitsA() const { return fNHitsA; }
    Int_t GetNHitsB() const { return fNHitsB; }
    Int_t GetNHitsC() const { return fNHitsC; }
    Int_t GetCharge() const { return fCharge; }
    Int_t GetGeneratorId() const { return fGeneratorId; }
    void SetDaughers(Int_t a, Int_t b) {
      fDaugthers[0] = a;
      fDaugthers[1] = b;
    }
    void GetDaughters(Int_t& a, Int_t& b) const {
      a = fDaugthers[0];
      b = fDaugthers[1];
    }
    void SetPidHypo(Int_t pid) { fPidHypo = pid; }
    void SetCharge(Int_t ch) { fCharge = ch; }
    void SetMom(TLorentzVector mom) { fMomentum = mom; }
    void SetNHitsA(Int_t nHits) { fNHitsA = nHits; }
    void SetNHitsB(Int_t nHits) { fNHitsB = nHits; }
    void SetNHitsC(Int_t nHits) { fNHitsC = nHits; }
    void SetMcIndex(Int_t index) { fIndexMc = index; }
    void SetMom(Double_t px, Double_t py, Double_t pz, Double_t e) { fMomentum.SetXYZT(px, py, pz, e); }
    void SetGeneratorId(Int_t type) { fGeneratorId = type; }
    void Clear(Option_t* /*opt*/ = "") { fDaugthers[0] = fDaugthers[1] = -1; }
    Int_t GetPidHypo() const { return fPidHypo; }
    const TLorentzVector& GetMom() const { return fMomentum; }
    virtual ~RecoTrack() {};
    ClassDef(RecoTrack, 1)
  };  // namespace OTF

  class McTrack : public TObject {
    TLorentzVector fMomentum;
    TLorentzVector fFreezeout;
    Int_t fPdgCode;
    Int_t fMotherIdx;
    Int_t fGeneratorId = {0};

  public:
    McTrack() : fPdgCode(0), fMotherIdx(-1) {};
    void SetMotherId(Int_t momId) { fMotherIdx = momId; }
    void SetPdgCode(Int_t pdgCode) { fPdgCode = pdgCode; }
    void SetFreezeout(const TLorentzVector& freezout) { fFreezeout = freezout; }
    void SetMomentum(const TLorentzVector& momentum) { fMomentum = momentum; }
    void SetGeneratorId(Int_t type) { fGeneratorId = type; }
    Int_t GetMotherId() const { return fMotherIdx; }
    Int_t GetPdgCode() const { return fPdgCode; }
    Int_t GetGeneratorId() const { return fGeneratorId; }
    const TLorentzVector& GetFreezeout() const { return fFreezeout; }
    const TLorentzVector& GetMomentum() const { return fMomentum; }
    virtual ~McTrack() {};
    ClassDef(McTrack, 1)
  };

  class RecoEvent : public TNamed {
    TClonesArray* fTracks;
    TVector3 fVertex;
    Double_t fPhi = {0};

  public:
    RecoEvent() : fTracks(new TClonesArray("OTF::RecoTrack")) {};
    RecoTrack* GetTrack(Int_t index) const { return (RecoTrack*) fTracks->UncheckedAt(index); };
    const TVector3& GetVertex() const { return fVertex; }
    void SetPhi(Double_t psi) { fPhi = psi; }
    void SetVertex(const TVector3& vertex) { fVertex = vertex; }
    void AddTrack(const RecoTrack& track);
    void Clear(Option_t* /*option*/ = "") { fTracks->Clear(); };
    Double_t GetPhi() const { return fPhi; }
    Int_t GetNTracks() const { return fTracks->GetEntriesFast(); };
    virtual ~RecoEvent() { delete fTracks; };
    ClassDef(RecoEvent, 1)
  };

  class McEvent : public TNamed {
    TClonesArray* fTracks;
    Double_t fB;
    Double_t fPhi = {0};

  public:
    McEvent() : fTracks(new TClonesArray("OTF::McTrack")), fB(0) {};
    void SetPhi(Double_t psi) { fPhi = psi; }
    void SetB(Double_t b) { fB = b; }
    void Clear(Option_t* /*option*/ = "") { fTracks->Clear(); };
    void AddTrack(const McTrack& track);
    TClonesArray* GetArray() const { return fTracks; }
    Int_t GetNTracks() const { return fTracks->GetEntriesFast(); };
    Double_t GetB() const { return fB; };
    Double_t GetPhi() const { return fPhi; }
    McTrack* GetTrack(Int_t idx) const { return (McTrack*) fTracks->UncheckedAt(idx); };
    virtual ~McEvent() { delete fTracks; };
    ClassDef(McEvent, 1)
  };

  namespace Std {
    /**
     * converts otf-mc track to hal-mc track
     */
    Hal::McTrack OTFMcToHalMc(OTF::McTrack& other);
    /**
     * converts otf mctrack to reco track
     * @param tr
     * @param mass
     * @return
     */
    OTF::RecoTrack HalMcToOTFReco(OTF::McTrack& tr);
    /**
     * converts mctrack to otf-mctrack
     */
    OTF::McTrack HalMcToOTFMc(Hal::McTrack& tr);
  }  // namespace Std

}  // namespace OTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFDATA_H_ */
