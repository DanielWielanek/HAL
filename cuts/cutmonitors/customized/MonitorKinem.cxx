/*
 * MonitorTrackKinem.cxx
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#include "MonitorKinem.h"

#include <RtypesCore.h>
#include <TH2.h>
#include <TLorentzVector.h>

#include "ComplexTrack.h"
#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "ExpTrack.h"
#include "Std.h"

namespace Hal {

  MonitorEtaPt::MonitorEtaPt() {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
    fFieldId1 = Hal::DataFieldID::Track::EBasic::kRapidity;
    fFieldId2 = Hal::DataFieldID::Track::EBasic::kPt;
  }


  MonitorYPt::MonitorYPt() {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
    fFieldId1 = Hal::DataFieldID::Track::EBasic::kEta;
    fFieldId2 = Hal::DataFieldID::Track::EBasic::kPt;
  }

} /* namespace Hal */
