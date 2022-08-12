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
namespace Hal {
  V0Track::V0Track() :
    fTrackId(-1),
    fPosId(-1),
    fNegId(-1),
    fS1(0),
    fS2(0),
    fAlpha(0),
    fPtArm(0),
    fDauDist(0),
    fDecLenght(0),
    fCosAngle(0),
    fAssumedPdg(0),
    fAssumedPdgPos(0),
    fAssumedPdgNeg(0) {}

  V0Track::~V0Track() {}

  void V0Track::SetDecayPos(Double_t x, Double_t y, Double_t z) { fDecay.SetXYZ(x, y, z); }

  void V0Track::SetMom(Double_t px, Double_t py, Double_t pz, Double_t m) { fMom.SetXYZM(px, py, pz, m); }

  void V0Track::SetDCA(Double_t x, Double_t y, Double_t z) { fPos.SetXYZ(x, y, z); }

  void V0Track::Recalc(const TVector3& vertex) {
    fMom = fMomPos + fMomNeg;

    Double_t Ptot          = fMom.P();
    TVector3 momV0         = fMom.Vect();
    TVector3 momPos        = fMomPos.Vect();
    TVector3 momNeg        = fMomNeg.Vect();
    Double_t pPosTot       = fMomPos.P();
    Double_t MomPosAlongV0 = momPos * momV0 / Ptot;
    Double_t MomNegALongV0 = momNeg * momV0 / Ptot;

    SetAlphaArm((MomPosAlongV0 - MomNegALongV0) / (MomPosAlongV0 + MomNegALongV0));
    SetPtArm(TMath::Sqrt(pPosTot * pPosTot - MomPosAlongV0 * MomPosAlongV0));
    TVector3 pozV0 = fDecay - vertex;

    Double_t t = -(pozV0 * momV0) / (Ptot * Ptot);
    SetDCA(pozV0.X() + t * momV0.X(), pozV0.Y() + t * momV0.Y(), pozV0.Z() + t * momV0.Z());
    TVector3 dca_rel = fPos - pozV0;
    fCosAngle        = pozV0 * momV0 / (Ptot * pozV0.Mag());
    SetDecLenght(dca_rel.Mag());
  }

  void V0Track::SetPxPyPzMPos(Double_t px, Double_t py, Double_t pz, Double_t m) { fMomPos.SetXYZM(px, py, pz, m); }

  void V0Track::SetPxPyPzMNeg(Double_t px, Double_t py, Double_t pz, Double_t m) { fMomNeg.SetXYZM(px, py, pz, m); }

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
    fMom           = v->fMom;
    fPos           = v->fPos;
    fDecay         = v->fDecay;
    fMomPos        = v->fMomPos;
    fMomNeg        = v->fMomNeg;
  }
}  // namespace Hal
