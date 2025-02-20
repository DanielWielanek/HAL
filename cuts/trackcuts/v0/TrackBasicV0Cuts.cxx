/*
 * TrackBasicV0Cuts.cxx
 *
 *  Created on: 11 cze 2024
 *      Author: daniel
 */

#include "TrackBasicV0Cuts.h"

#include "TrackDCACut.h"
#include "TrackV0MinvCut.h"
#include "TrackV0TopoCuts.h"


namespace Hal {

  TrackBasicV0Cuts::TrackBasicV0Cuts() :
    fDCAxyzXY("Hal::TrackDCACut", 1, 100, 0, 10),
    fDCAxyzZ("Hal::TrackDCACut", 2, 100, -20, 20),
    fDCA("Hal::TrackDCACut", 0, 100, 0, 10),
    fMinv("Hal::TrackV0MinvCut", 0, 1000, 0, 2),
    fDecLen("Hal::TrackV0DecLenCut", 0, 100, 0, 10),
    fDauDist("Hal::TrackV0DaughterDistCut", 0, 100, 0, 10) {
    AddCut(Hal::TrackDCACut());
    AddCut(Hal::TrackV0DaughterDistCut());
    AddCut(Hal::TrackV0MinvCut());
    AddCut(Hal::TrackV0DecLenCut());
  }

  void TrackBasicV0Cuts::AddAllCutMonitorRequests(Option_t* opt) {
    TString option = opt;
    if (Hal::Std::FindParam(option, "all")) {
      AddCutMonitorRequest(fDCAxyzXY, fDCAxyzZ);
      AddCutMonitorRequest(fDCA);
      AddCutMonitorRequest(fMinv);
      AddCutMonitorRequest(fDecLen);
      AddCutMonitorRequest(fDauDist);
      return;
    }
    if (Hal::Std::FindParam(option, "minv")) AddCutMonitorRequest(fMinv);
    if (Hal::Std::FindParam(option, "dca")) {
      AddCutMonitorRequest(fDCA);
      AddCutMonitorRequest(fDCAxyzXY, fDCAxyzZ);
    }
    if (Hal::Std::FindParam(option, "daudist")) AddCutMonitorRequest(fDauDist);
    if (Hal::Std::FindParam(option, "declen")) AddCutMonitorRequest(fDecLen);
  }

  TrackBasicV0Cuts::~TrackBasicV0Cuts() {}

} /* namespace Hal */
