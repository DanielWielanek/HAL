/*
 * V0Track.cxx
 *
 *  Created on: 26 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "V0Track.h"

#include "ExpEvent.h"
#include "Track.h"

namespace Hal {
  V0Track::V0Track() :
    fTrackId(-1),
    fPosId(-1),
    fNegId(-1),
    fAssumedPdg(0),
    fAssumedPdgPos(0),
    fAssumedPdgNeg(0),
    fS1(0),
    fS2(0),
    fAlpha(0),
    fPtArm(0),
    fDauDist(0),
    fDecLenght(0),
    fCosAngle(0) {}

  V0Track::~V0Track() {}

  Double_t V0Track::GetHypoMass(Double_t m_dau1, Double_t m_dau2) const {
    Double_t tPx = fMomPos.X() + fMomNeg.X();
    Double_t tPy = fMomPos.Y() + fMomNeg.Y();
    Double_t tPz = fMomPos.Z() + fMomNeg.Z();
    Double_t e1  = TMath::Sqrt(fMomPos.Mag2() + m_dau1 * m_dau1);
    Double_t e2  = TMath::Sqrt(fMomNeg.Mag2() + m_dau2 * m_dau2);
    Double_t tE  = e1 + e2;
    return TMath::Sqrt(tE * tE - tPx * tPx - tPy * tPy - tPz * tPz);
  }

  void V0Track::SetDecayPos(Double_t x, Double_t y, Double_t z) { fDecay.SetXYZ(x, y, z); }

  TVector3 V0Track::Recalc(const Track& track) {
    const TVector3 Mom(track.GetPx(), track.GetPy(), track.GetPz());

    Double_t Ptot          = Mom.Mag();
    Double_t pPosTot       = fMomPos.Mag();
    Double_t MomPosAlongV0 = fMomPos * Mom / Ptot;
    Double_t MomNegALongV0 = fMomNeg * Mom / Ptot;

    SetAlphaArm((MomPosAlongV0 - MomNegALongV0) / (MomPosAlongV0 + MomNegALongV0));
    SetPtArm(TMath::Sqrt(pPosTot * pPosTot - MomPosAlongV0 * MomPosAlongV0));
    TVector3 vertex = track.GetEvent()->GetVertex()->Vect();
    TVector3 pozV0  = fDecay - vertex;

    Double_t t = -(pozV0 * Mom) / (Ptot * Ptot);
    TVector3 dca;
    dca.SetXYZ(pozV0.X() + t * Mom.X(), pozV0.Y() + t * Mom.Y(), pozV0.Z() + t * Mom.Z());
    TVector3 dca_rel = dca - pozV0;
    fCosAngle        = pozV0 * Mom / (Ptot * pozV0.Mag());
    SetDecLenght(dca_rel.Mag());
    return dca;
  }

  void V0Track::SetMomPos(Double_t px, Double_t py, Double_t pz) { fMomPos.SetXYZ(px, py, pz); }

  void V0Track::SetMomNeg(Double_t px, Double_t py, Double_t pz) { fMomNeg.SetXYZ(px, py, pz); }

  void V0Track::CopyData(V0Track* v) {
    fTrackId       = v->fTrackId;
    fPosId         = v->fPosId;
    fNegId         = v->fNegId;
    fS1            = v->fS1;
    fS2            = v->fS2;
    fAlpha         = v->fAlpha;
    fPtArm         = v->fPtArm;
    fDauDist       = v->fDauDist;
    fDecLenght     = v->fDecLenght;
    fCosAngle      = v->fCosAngle;
    fAssumedPdg    = v->fAssumedPdg;
    fAssumedPdgPos = v->fAssumedPdgPos;
    fAssumedPdgNeg = v->fAssumedPdgNeg;
    fDecay         = v->fDecay;
    fMomPos        = v->fMomPos;
    fMomNeg        = v->fMomNeg;
  }
}  // namespace Hal
