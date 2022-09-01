/*
 * V0CandidateHelix.cpp
 *
 *  Created on: 26 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "V0CandidateHelix.h"

#include "ExpEvent.h"
#include "ExpTrack.h"
#include "Helix.h"

namespace Hal {
  V0CandidateHelix::V0CandidateHelix() {}

  void V0CandidateHelix::MakeV0(Double_t mPos, Double_t mNeg, Int_t pidThis, Int_t pidPos, Int_t pidNeg) {
    fV0Track.SetPosId(GetTrack1()->GetThisID());
    fV0Track.SetNegId(GetTrack2()->GetThisID());
    const Helix& helix1 = static_cast<ExpTrackHelix*>(GetTrack1())->GetHelix();
    const Helix& helix2 = static_cast<ExpTrackHelix*>(GetTrack2())->GetHelix();
    fV0Track.SetPdg(pidThis);
    fV0Track.SetPdgDaughters(pidPos, pidNeg);
    Double_t s1, s2;
    helix1.PathLengths(helix2, s1, s2);
    fV0Track.SetS(s1, s2);
    TVector3 pPos, pNeg, poz1, poz2;
    helix1.EvaluateFull(s1, pPos, poz1);
    helix2.EvaluateFull(s2, pNeg, poz2);

    fV0Track.SetMomPos(pPos.Px(), pPos.Py(), pPos.Pz());
    fV0Track.SetMomNeg(pNeg.Px(), pNeg.Py(), pNeg.Pz());

    TVector3 pozV0 = (poz1 + poz2) * 0.5;
    fV0Track.SetDauDist((poz1 - poz2).Mag());
    fV0Track.SetDecayPos(pozV0.X(), pozV0.Y(), pozV0.Z());
    TVector3 vertex = static_cast<ExpEvent*>(GetTrack1()->GetEvent())->GetVertex()->Vect();
    fV0Track.Recalc(vertex);
  }

  V0CandidateHelix::~V0CandidateHelix() {}
}  // namespace Hal
