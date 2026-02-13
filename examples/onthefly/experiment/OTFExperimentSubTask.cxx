/*
 * OTFExperimentSubTask.cxx
 *
 *  Created on: 11 lut 2026
 *      Author: daniel
 */

#include "OTFExperimentSubTask.h"

#include <algorithm>
#include <iterator>

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Event.h"
#include "OTFRecoEvent.h"
#include "Package.h"

namespace HalOTF {

  ExperimentSubTask::ExperimentSubTask(std::vector<int> ids) {
    ids.erase(std::remove_if(ids.begin(), ids.end(), [](int x) { return x < 0; }), ids.end());
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
    fGeneratorIds = ids;
  }

  Hal::Package* ExperimentSubTask::Report() const {
    Hal::Package* pack = new Hal::Package(this);
    return pack;
  }

  void ExperimentSubTask::Exec(Hal::ComplexEvent* event) {
    for (auto idx : fGeneratorIds) {
      for (int i = 0; i < event->GetTotalTrackNo(); i++) {
        auto z_track      = (Hal::ComplexTrack*) event->GetTrack(i);
        auto reco_track   = (HalOTF::RecoTrack*) z_track->GetRealTrack();
        Int_t generatorId = reco_track->GetGeneratorId();
        if (generatorId == idx) { ProcessTrack(z_track); }
      }
    }
  }
} /* namespace HalOTF */
