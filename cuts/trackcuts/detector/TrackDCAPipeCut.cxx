/*
 * TrackDCAPipeCut.cxx
 *
 *  Created on: 4 cze 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackDCAPipeCut.h"

#include "DataFormatManager.h"
#include "Event.h"
#include "ExpTrack.h"
#include "Helix.h"

namespace Hal {
  TrackDCAPipeCut::TrackDCAPipeCut() : TrackExpCut(3) {
    SetUnitName("DCA_{abs} [cm]", DCA());
    SetUnitName("DCA XY_{abs}[cm]", DCAxy());
    SetUnitName("DCA Z_{abs}[cm]", DCAz());
  }

  Bool_t TrackDCAPipeCut::Pass(Track* track) {
    const Helix& helix = ((ExpTrackHelix*) track)->GetHelix();
    Double_t s         = helix.PathLength(TVector3(0, 0, 0), kFALSE);
    const TVector3 dca = helix.EvalPos(s);
    SetValue(dca.Mag(), DCA());
    SetValue(dca.Pt(), DCAxy());
    SetValue(dca.Z(), DCAz());
    return Validate();
  }

  Bool_t TrackDCAPipeCut::Init(Int_t id_format) {
    if (TrackExpCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(id_format);
    if (ev->InheritsFrom("ExpEventHelix")) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  TrackDCAPipeCut::~TrackDCAPipeCut() {}
}  // namespace Hal
