/*
 * TwoTrackDCACut.cxx
 *
 *  Created on: 10 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackDCACut.h"

#include "ExpTrack.h"
#include "Helix.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackDCACut::TwoTrackDCACut() : TwoTrackCut(3) {
    SetUnitName("DCA 1-2 [cm]", 0);
    SetUnitName("DCAxy 1-2 [cm]", 0);
    SetUnitName("DCAz 1-2 [cm]", 0);
  }

  Bool_t TwoTrackDCACut::Pass(TwoTrack* pair) {
    ExpTrack* tr1    = (ExpTrack*) pair->GetTrack1();
    ExpTrack* tr2    = (ExpTrack*) pair->GetTrack2();
    TVector3* point1 = tr1->GetDCA();
    TVector3* point2 = tr2->GetDCA();
    TVector3 dif     = *point1 - *point2;
    SetValue(DCA(), dif.Mag());
    SetValue(DCAxy(), dif.Pt());
    SetValue(DCAz(), dif.Z());
    return Validate();
  }

  TwoTrackDCACut::~TwoTrackDCACut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
