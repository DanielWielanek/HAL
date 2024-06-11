/*
 * TrackV0TopoCuts.cxx
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#include "TrackV0TopoCuts.h"

#include "V0Track.h"

namespace Hal {

  TrackV0DaughterDistCut::TrackV0DaughterDistCut() : TrackV0Cut(1) { SetUnitName("DauDist [cm]"); }


  Bool_t TrackV0DaughterDistCut::PassV0(V0Track* tr) {
    SetValue(tr->GetDauDist());
    return Validate();
  }

  //========================================================================================
  TrackV0DecLenCut::TrackV0DecLenCut() : TrackV0Cut(1) { SetUnitName("DL [cm]"); }

  Bool_t TrackV0DecLenCut::PassV0(V0Track* tr) {
    SetValue(tr->GetDecLength());
    return Validate();
  }
  //========================================================================================

  TrackV0CosAngleCut::TrackV0CosAngleCut() : TrackV0Cut(1) { SetUnitName("Cos Angle [rad]"); }

  Bool_t TrackV0CosAngleCut::PassV0(V0Track* tr) {
    SetValue(tr->GetCosAngle());
    return Validate();
  }

  //========================================================================================

} /* namespace Hal */
