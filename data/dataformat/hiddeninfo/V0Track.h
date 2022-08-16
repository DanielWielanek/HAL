/*
 * V0Track.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALV0TRACK_H_
#define HALV0TRACK_H_

#include <TLorentzVector.h>
#include <TObject.h>
#include <TVector3.h>

/**
 * representation of V0 track
 */
namespace Hal {
  class V0Track : public TObject {
    Int_t fTrackId;
    Int_t fPosId;
    Int_t fNegId;
    Int_t fAssumedPdg;
    Int_t fAssumedPdgPos;
    Int_t fAssumedPdgNeg;
    Double_t fS1, fS2;
    Double_t fAlpha;
    Double_t fPtArm;
    Double_t fDauDist;
    Double_t fDecLenght;
    Double_t fCosAngle;
    TLorentzVector fMom;
    TVector3 fPos;
    TVector3 fDecay;
    TLorentzVector fMomPos;
    TLorentzVector fMomNeg;

  public:
    V0Track();
    V0Track(const V0Track& other) = default;
    /**
     * copy content
     * @param v
     */
    virtual void CopyData(V0Track* v);
    /**
     * set id of track in event
     * @param id
     */
    void SetTrackId(Int_t id) { fTrackId = id; }
    /**
     * get id of track in event
     * @return
     */
    inline Int_t GetTrackId() const { return fTrackId; }
    /**
     *
     * @return assumed PDG of this particle
     */
    inline Int_t GetPdg() const { return fAssumedPdg; };
    /**
     *
     * @return assumed dpg of postive daughter
     */
    inline Int_t GetPdgPosDau() const { return fAssumedPdgPos; };
    /**
     *
     * @return assumed pdg of negative daughter
     */
    inline Int_t GetPdgNegDau() const { return fAssumedPdgNeg; };
    /**
     *
     * @return index of negative daughter in track array
     */
    Int_t GetNegId() const { return fNegId; }
    /**
     *
     * @return index of positive daughter in track array
     */
    Int_t GetPosId() const { return fPosId; }
    /**
     *
     * @return armenteros alpha
     */
    Double_t GetAlphaArm() const { return fAlpha; }
    /**
     *
     * @return armenteros pt
     */
    Double_t GetPtArm() const { return fPtArm; }
    /**
     *
     * @return minimal distance between daughers
     */
    Double_t GetDauDist() const { return fDauDist; }
    /**
     *
     * @return decay length
     */
    Double_t GetDecLength() const { return fDecLenght; };
    /**
     *
     * @return cosine angle
     */
    Double_t GetCosAngle() const { return fCosAngle; }
    /**
     * set helix lenghts at decay vertex
     * @param s1
     * @param s2
     */
    void SetS(Double_t s1, Double_t s2) {
      fS1 = s1;
      fS2 = s2;
    };
    /**
     * set pdg of daughters
     * @param pos
     * @param neg
     */
    void SetPdgDaughters(Int_t pos, Int_t neg) {
      fAssumedPdgNeg = neg;
      fAssumedPdgPos = pos;
    }
    /**
     * set assumed PDG of this particle
     * @param pid
     */
    void SetPdg(Int_t pid) { fAssumedPdg = pid; };
    /**
     * set index of negative particle
     * @param negId
     */
    void SetNegId(Int_t negId) { fNegId = negId; }
    /**
     * set index of positve particle
     * @param posId
     */
    void SetPosId(Int_t posId) { fPosId = posId; }
    /**
     * set decay length
     * @param L
     */
    void SetDecLenght(Double_t L) { fDecLenght = L; };
    /**
     *
     * @return DCA to primary vertex
     */
    const TVector3& GetDCA() const { return fPos; };
    /**
     *
     * @return momentum of positive daughter
     */
    const TLorentzVector& GetMomPos() const { return fMomPos; };
    /**
     *
     * @return momentum of negative daugher
     */
    const TLorentzVector& GetMomNeg() const { return fMomNeg; }
    /**
     *
     * @return momentum of V0
     */
    const TLorentzVector& GetMom() const { return fMom; }
    /**
     *
     * @return decay vertex
     */
    const TVector3& GetDecay() const { return fDecay; }
    /**
     * set momentum of positive daugther
     * @param positiveDaughter
     */
    void SetMomentumPos(const TLorentzVector& positiveDaughter) { fMomPos = positiveDaughter; }
    /**
     * set momenum of postive daugher
     * @param px
     * @param py
     * @param pz
     * @param m mass of positive daughter
     */
    void SetPxPyPzMPos(Double_t px, Double_t py, Double_t pz, Double_t m);
    /**
     * set momentum of positive daugther
     * @param positiveDaughter
     */
    void SetMomentumNeg(const TLorentzVector& negativeDaughter) { fMomNeg = negativeDaughter; }
    /**
     * set momentum of negative daughter
     * @param px
     * @param py
     * @param pz
     * @param m mass of negative daughter
     */
    void SetPxPyPzMNeg(Double_t px, Double_t py, Double_t pz, Double_t m);
    /**
     * set decay position
     * @param x
     * @param y
     * @param z
     */
    void SetDecayPos(Double_t x, Double_t y, Double_t z);
    /**
     * set decay position
     * @param pos
     */
    void SetDecayPos(const TVector3& pos) { fDecay = pos; }
    /**
     * set momentum of V0
     * @param px
     * @param py
     * @param pz
     * @param m mass of V0
     */
    void SetMom(Double_t px, Double_t py, Double_t pz, Double_t m);
    /**
     * set DCA  to primary vertex
     * @param x
     * @param y
     * @param z
     */
    void SetDCA(Double_t x, Double_t y, Double_t z);
    /**
     * set dca of v0
     * @param dca
     */
    void SetDCA(const TVector3& dca) { fPos = dca; }
    /**
     * set armenteros alpha
     * @param alpha
     */
    void SetAlphaArm(Double_t alpha) { fAlpha = alpha; }
    /**
     * set armenteros pT
     * @param ptArm
     */
    void SetPtArm(Double_t ptArm) { fPtArm = ptArm; }
    /**
     * set distance between daughters at decay vertex
     * @param dauDist
     */
    void SetDauDist(Double_t dauDist) { fDauDist = dauDist; }
    /**
     * recalculates variables:
     * - momentum of V0
     * armenteros alpha & pt
     * DCA and decay lenght
     *
     * @param event_vertex event primary vertex
     */
    void Recalc(const TVector3& event_vertex);
    virtual ~V0Track();
    ClassDef(V0Track, 1)
  };
}  // namespace Hal
#endif /* HALV0TRACK_H_ */
