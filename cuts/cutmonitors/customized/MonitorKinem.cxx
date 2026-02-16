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

  MonitorEtaPt::MonitorEtaPt() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kEta, Hal::DataFieldID::Track::EBasic::kPt) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }


  MonitorYPt::MonitorYPt() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kRapidity, Hal::DataFieldID::Track::EBasic::kPt) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

  MonitorEtaPtReco::MonitorEtaPtReco() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kEta + Hal::DataFieldID::ReStep,
                        Hal::DataFieldID::Track::EBasic::kPt + Hal::DataFieldID::ReStep) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }


  MonitorYPtReco::MonitorYPtReco() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kRapidity + Hal::DataFieldID::ReStep,
                        Hal::DataFieldID::Track::EBasic::kPt + Hal::DataFieldID::ReStep) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

  MonitorEtaPtMc::MonitorEtaPtMc() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kEta + Hal::DataFieldID::ImStep,
                        Hal::DataFieldID::Track::EBasic::kPt + Hal::DataFieldID::ImStep) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }


  MonitorYPtMc::MonitorYPtMc() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kRapidity + Hal::DataFieldID::ImStep,
                        Hal::DataFieldID::Track::EBasic::kPt + Hal::DataFieldID::ImStep) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

} /* namespace Hal */
