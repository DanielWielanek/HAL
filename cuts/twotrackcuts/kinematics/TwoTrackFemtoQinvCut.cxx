/*
 * TwoTrackQinvCut.cxx
 *
 *  Created on: 5 lip 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackFemtoQinvCut.h"


#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>

#include "Cut.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "TwoTrack.h"

namespace Hal {
  TwoTrackFemtoQinvCut::TwoTrackFemtoQinvCut(Double_t mass) : TwoTrackCut(1), fMass2(mass) {
    SetUnitName("q_{inv} [GeV/c]");
    fMass2 = fMass2 * fMass2;
  }

  Bool_t TwoTrackFemtoQinvCut::Pass(TwoTrack* pair) {
    const TLorentzVector& tr1 = pair->GetTrack1()->GetMomentum();
    const TLorentzVector& tr2 = pair->GetTrack2()->GetMomentum();
    Double_t px, py, pz;
    switch (pair->GetPairType()) {
      case TwoTrack::kHemishpere:
        px = tr1.Px() + tr2.Px();
        py = tr1.Py() + tr2.Py();
        pz = tr1.Pz() + tr2.Pz();
        break;
      case TwoTrack::kRotated:
        px = tr1.Px() + tr2.Px();
        py = tr1.Py() + tr2.Py();
        pz = tr1.Pz() - tr2.Pz();
        break;
      default:
        px = tr1.Px() - tr2.Px();
        py = tr1.Py() - tr2.Py();
        pz = tr1.Pz() - tr2.Pz();
        break;
    }
    Double_t p1 = tr1.P();
    Double_t p2 = tr2.P();
    Double_t e1 = TMath::Sqrt(p1 * p1 + fMass2);
    Double_t e2 = TMath::Sqrt(p2 * p2 + fMass2);

    Double_t e  = e1 - e2;
    Double_t q2 = TMath::Sqrt(TMath::Abs((e * e - px * px - py * py - pz * pz)));
    SetValue(q2);
    return Validate();
  }

  TwoTrackFemtoQinvCut::~TwoTrackFemtoQinvCut() {}

  Package* TwoTrackFemtoQinvCut::Report() const {
    Package* pack = TwoTrackCut::Report();
    pack->AddObject(new ParameterDouble("Assumed mass", TMath::Sqrt(fMass2)));
    return pack;
  }
}  // namespace Hal
