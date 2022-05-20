/*
 * TrackDCACut.cxx
 *
 *  Created on: 27 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackDCACut.h"

#include "DataFormatManager.h"
#include "ExpEvent.h"
#include "ExpTrack.h"

namespace Hal {
  TrackDCACut::TrackDCACut() : TrackCut(3) {
    SetUnitName("DCA [cm]", DCA());
    SetUnitName("DCA XY[cm]", DCAxy());
    SetUnitName("DCA Z[cm]", DCAz());
  }

  Bool_t TrackDCACut::Pass(Track* track) {
    const TVector3& dca = ((ExpTrack*) track)->GetDCA();
    SetValue(dca.Mag(), DCA());
    SetValue(dca.Pt(), DCAxy());
    SetValue(dca.Z(), DCAz());
    return Validate();
  }

  Bool_t TrackDCACut::Init(Int_t id_format) {
    if (TrackCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(id_format);
    if (ev->InheritsFrom("Hal::ExpEvent")) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  TrackDCACut::~TrackDCACut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
