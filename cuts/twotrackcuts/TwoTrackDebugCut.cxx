/*
 * TwoTrackDebugCut.cxx
 *
 *  Created on: 17-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackDebugCut.h"

#include "McTrack.h"
#include "DataFormatManager.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackDebugCut::TwoTrackDebugCut() : TwoTrackCut(2) {
    SetUnitName("mass_first", 0);
    SetUnitName("mass_second", 1);
  }

  Bool_t TwoTrackDebugCut::Pass(TwoTrack* pair) {
    Track* track1 = pair->GetTrack1();
    Track* track2 = pair->GetTrack2();

    const TLorentzVector& p1 = track1->GetMomentum();
    const TLorentzVector& p2 = track2->GetMomentum();
    const TLorentzVector& x1 = ((McTrack*) track1)->GetFreezeoutPosition();
    const TLorentzVector& x2 = ((McTrack*) track2)->GetFreezeoutPosition();


    Double_t dpx, dpy, dpz, de;
    Double_t dx, dy, dz, dt;
    dpx = p1.X() - p2.X();
    dpy = p1.Y() - p2.Y();
    dpz = p1.Z() - p2.Z();
    de  = p1.T() - p2.T();

    dx = x1.X() - x2.X();
    dy = x1.Y() - x2.Y();
    dz = x1.Z() - x2.Z();
    dt = x1.T() - x2.T();

    Double_t w = 1.0 + TMath::Cos((dpx * dx + dpy * dy + dpz * dz - dt * de) / 0.197327);
    Double_t q = TMath::Sqrt(TMath::Abs(dpx * dpx + dpy * dpy + dpz * dpz - de * de));

    SetValue(w, 0);
    SetValue(q, 1);
    return kTRUE;
  }

  TwoTrackDebugCut::~TwoTrackDebugCut() {}
}  // namespace Hal
