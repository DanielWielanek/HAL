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

#include "Const.h"
#include "HiddenInfo.h"

/**
 * representation of V0 track
 */
namespace Hal {
  class Track;
  class V0Track : public HiddenInfo {
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
    TVector3 fDecay;
    TVector3 fMomPos;
    TVector3 fMomNeg;

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
     * @return mass with assumption that this is lambda particle that decay into proton and pion-
     */
    Double_t GetLambdaMass() const { return GetHypoMass(Const::ProtonMass(), Const::PionPlusMass()); }
    /**
     *
     * @return mass with assumption that this is ant-lambda particle that decay into antiproton and pion+
     */
    Double_t GetAntiLambdaMass() const { return GetHypoMass(Const::PionPlusMass(), Const::ProtonMass()); }
    /**
     *
     * @return mass with assumption that this is lambda particle that decay into pion+ and pion-
     */
    Double_t GetK0Mass() const { return GetHypoMass(Const::PionPlusMass(), Const::PionPlusMass()); }
    /**
     *  get mass based on daughter mass hypothesis
     * @return
     */
    Double_t GetHypoMass(Double_t m_dau1, Double_t m_dau2) const;
    /**
     *
     * @return daughters path's lengths, first length corresponds to positive daughter
     */
    std::pair<Double_t, Double_t> GetS() const { return std::pair<Double_t, Double_t>(fS1, fS2); }
    /**
     *
     * @return momentum of positive daughter
     */
    const TVector3& GetMomPos() const { return fMomPos; };
    /**
     *
     * @return momentum of negative daugher
     */
    const TVector3& GetMomNeg() const { return fMomNeg; }
    /**
     *
     * @return decay vertex
     */
    const TVector3& GetDecay() const { return fDecay; }
    /**
     * set momentum of positive daugther
     * @param positiveDaughter
     */
    void SetMomentumPos(const TVector3& positiveDaughter) { fMomPos = positiveDaughter; }
    /**
     * set momenum of postive daugher
     * @param px
     * @param py
     * @param pz
     * @param m mass of positive daughter
     */
    void SetMomPos(Double_t px, Double_t py, Double_t pz);
    /**
     * set momentum of positive daugther
     * @param positiveDaughter
     */
    void SetMomentumNeg(const TVector3& negativeDaughter) { fMomNeg = negativeDaughter; }
    /**
     * set momentum of negative daughter
     * @param px
     * @param py
     * @param pz
     * @param m mass of negative daughter
     */
    void SetMomNeg(Double_t px, Double_t py, Double_t pz);
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
     * rotates particle around z-axis
     * @param angle
     */
    virtual void RotateZ(Double_t angle);
    /**
     * recalculates variables:
     * - momentum of V0
     * Armenteros alpha & pt
     * DCA and decay length
     *
     * @param pointer to track that own this hidden info
     * @return dca of V0
     */
    TVector3 Recalc(const Track& track);
    virtual ~V0Track();
    ClassDef(V0Track, 1)
  };
}  // namespace Hal
#endif /* HALV0TRACK_H_ */
