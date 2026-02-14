/*
 * DetectorHits.cxx
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#include <RtypesCore.h>
#include <TString.h>
#include <utility>
#include <vector>

#include "Cut.h"
#include "DataFormatManager.h"
#include "OTFDetectorHitsCut.h"
#include "OTFRecoEvent.h"
#include "StdTypes.h"


namespace HalOTF {
  DetectorHitsCut::DetectorHitsCut(Int_t par) : Hal::TrackExpCut(par) {}

  std::vector<std::pair<TString, Double_t>> DetectorHitsCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> res;
    std::pair<TString, Double_t> data;
    data.first  = "Undected";
    data.second = 0;
    res.push_back(data);
    data.first  = "Modified";
    data.second = 1;
    res.push_back(data);
    data.first  = "Unmodified";
    data.second = 2;
    res.push_back(data);
    return res;
  }

  Bool_t DetectorHitsCut::Init(Int_t taskId) {
    const Hal::Event* ev = Hal::DataFormatManager::Instance()->GetFormat(taskId, Hal::EFormatDepth::kNonBuffered);
    if (dynamic_cast<const HalOTF::RecoEvent*>(ev)) { return kTRUE; }
    return kTRUE;
  }

  DetectorHitsACut::DetectorHitsACut() : DetectorHitsCut(1) { SetUnitName("NHits_{A} [AU]"); }

  Bool_t DetectorHitsACut::Pass(Hal::Track* track) {
    auto tr = (HalOTF::RecoTrack*) track;
    SetValue(tr->GetNHitsA());
    return Validate();
  }

  DetectorHitsBCut::DetectorHitsBCut() : DetectorHitsCut(1) { SetUnitName("NHits_{B} [AU]"); }

  Bool_t DetectorHitsBCut::Pass(Hal::Track* track) {
    auto tr = (HalOTF::RecoTrack*) track;
    SetValue(tr->GetNHitsB());
    return Validate();
  }

  DetectorHitsCCut::DetectorHitsCCut() : DetectorHitsCut(1) { SetUnitName("NHits_{A} [AU]"); }

  Bool_t DetectorHitsCCut::Pass(Hal::Track* track) {
    auto tr = (HalOTF::RecoTrack*) track;
    SetValue(tr->GetNHitsC());
    return Validate();
  }

  DetectorHitsABCSumCut::DetectorHitsABCSumCut() : DetectorHitsCut(1) { SetUnitName("NHits_{ABC} [AU]"); }

  std::vector<std::pair<TString, Double_t>> DetectorHitsABCSumCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> x;
    return x;
  }

  Bool_t DetectorHitsABCSumCut::Pass(Hal::Track* track) {
    auto tr = (HalOTF::RecoTrack*) track;
    SetValue(tr->GetNHitsA() + tr->GetNHitsB() + tr->GetNHitsC());
    return Validate();
  }

} /* namespace HalOTF */
