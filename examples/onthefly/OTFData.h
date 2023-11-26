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

namespace OTF {
  class RecoTrack : public TObject {
    TLorentzVector fMomentum;
    Int_t fIndexMc;
    Int_t fNHits;
    Int_t fCharge;
    Int_t fDaugthers[2] = {-1, -1};

  public:
    RecoTrack() : fIndexMc(-1), fNHits(0), fCharge(0) {};
    Int_t GetMcIndex() const { return fIndexMc; }
    Int_t GetNHits() const { return fNHits; }
    Int_t GetCharge() const { return fCharge; }
    void SetDaughers(Int_t a, Int_t b) {
      fDaugthers[0] = a;
      fDaugthers[1] = b;
    }
    void GetDaughters(Int_t& a, Int_t& b) {
      a = fDaugthers[0];
      b = fDaugthers[1];
    }
    void SetCharge(Int_t ch) { fCharge = ch; }
    void SetMom(TLorentzVector mom) { fMomentum = mom; }
    void SetNHits(Int_t nHits) { fNHits = nHits; }
    void SetMcIndex(Int_t index) { fIndexMc = index; }
    void SetMom(Double_t px, Double_t py, Double_t pz, Double_t e) { fMomentum.SetXYZT(px, py, pz, e); }
    void Clear(Option_t* /*opt*/ = "") { fDaugthers[0] = fDaugthers[1] = -1; }
    const TLorentzVector& GetMom() const { return fMomentum; }
    virtual ~RecoTrack() {};
    ClassDef(RecoTrack, 1)
  };

  class McTrack : public TObject {
    TLorentzVector fMomentum;
    TLorentzVector fFreezout;
    Int_t fPdgCode;
    Int_t fMotherIdx;

  public:
    McTrack() : fPdgCode(0), fMotherIdx(-1) {};
    void SetMotherId(Int_t momId) { fMotherIdx = momId; }
    void SetPdgCode(Int_t pdgCode) { fPdgCode = pdgCode; }
    void SetFreezout(const TLorentzVector& freezout) { fFreezout = freezout; }
    void SetMomentum(const TLorentzVector& momentum) { fMomentum = momentum; }
    Int_t GetMotherId() const { return fMotherIdx; }
    Int_t GetPdgCode() const { return fPdgCode; }
    const TLorentzVector& GetFreezout() const { return fFreezout; }
    const TLorentzVector& GetMomentum() const { return fMomentum; }
    virtual ~McTrack() {};
    ClassDef(McTrack, 1)
  };

  class RecoEvent : public TNamed {
    TClonesArray* fTracks;
    TVector3 fVertex;

  public:
    RecoEvent() : fTracks(new TClonesArray("OTF::RecoTrack")) {};
    RecoTrack* GetTrack(Int_t index) const { return (RecoTrack*) fTracks->UncheckedAt(index); };
    const TVector3& GetVertex() const { return fVertex; }
    void SetVertex(const TVector3& vertex) { fVertex = vertex; }
    void AddTrack(const RecoTrack& track);
    void Clear(Option_t* /*option*/ = "") { fTracks->Clear(); };
    Int_t GetNTracks() const { return fTracks->GetEntriesFast(); };
    virtual ~RecoEvent() { delete fTracks; };
    ClassDef(RecoEvent, 1)
  };

  class McEvent : public TNamed {
    TClonesArray* fTracks;
    Double_t fB;

  public:
    McEvent() : fTracks(new TClonesArray("OTF::McTrack")), fB(0) {};
    void SetB(Double_t b) { fB = b; }
    void Clear(Option_t* /*option*/ = "") { fTracks->Clear(); };
    void AddTrack(const McTrack& track);
    TClonesArray* GetArray() const { return fTracks; }
    Int_t GetNTracks() const { return fTracks->GetEntriesFast(); };
    Double_t GetB() const { return fB; };
    McTrack* GetTrack(Int_t idx) const { return (McTrack*) fTracks->UncheckedAt(idx); };
    virtual ~McEvent() { delete fTracks; };
    ClassDef(McEvent, 1)
  };


}  // namespace OTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFDATA_H_ */
