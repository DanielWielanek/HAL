/*
 * ToFBasicCut.cxx
 *
 *  Created on: 26 wrz 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackBasicToFCut.h"

#include "Cut.h"
#include "DataFormat.h"
#include "ExpTrack.h"
#include "ToFTrack.h"
namespace Hal {
  const Int_t TrackBasicToFCut::fgMass2 = 0;
  const Int_t TrackBasicToFCut::fgBeta  = 1;
  const Int_t TrackBasicToFCut::fgFlag  = 2;
  TrackBasicToFCut::TrackBasicToFCut() : TrackExpCut(3) {
    SetUnitName("m_{ToF}^{2} [GeV^{2}/c^{4}]", 0);
    SetUnitName("#beta_{ToF} [c]", 1);
    SetUnitName("flag_{ToF} [AU]", 2);
    SetMinMax(0, 1E+9, 0);
    SetMinMax(0, 1E+9, 1);
    SetMinMax(0, 1E+9, 2);
  }

  Bool_t TrackBasicToFCut::Init(Int_t formad_id) {
    Bool_t stat = TrackExpCut::Init(formad_id);
    if (stat == kFALSE) return kFALSE;
    if (ToFAvaiable(formad_id)) return kTRUE;
    return kFALSE;
  }

  Bool_t TrackBasicToFCut::Pass(Track* tr) {
    ToFTrack* tof = (ToFTrack*) ((ExpTrack*) tr)->GetDetTrack(DetectorID::kTOF);
    SetValue(tof->GetBeta(), fgBeta);
    SetValue(tof->GetMass2(), fgMass2);
    SetValue(tof->GetFlag(), fgFlag);
    return Validate();
  }

  TrackBasicToFCut::~TrackBasicToFCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
