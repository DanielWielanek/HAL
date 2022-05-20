/*
 * TrackExpCut.cxx
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackExpCut.h"

#include "DataFormat.h"
#include "DataFormatManager.h"
#include "ExpEvent.h"
#include "ExpTrack.h"
namespace Hal {
  TrackExpCut::TrackExpCut(Int_t size) : TrackCut(size) {}

  Bool_t TrackExpCut::Init(Int_t format_id) {
    if (TrackCut::Init(format_id) == kFALSE) return kFALSE;
    if (FormatInhertis("ExpEvent", format_id) == kFALSE) return kFALSE;
    return kTRUE;
  }

  TrackExpCut::~TrackExpCut() {}

  Bool_t TrackExpCut::TpcAvaiable(Int_t format_id) const {
    const Event* ev = DataFormatManager::Instance()->GetFormat(format_id);
    Bool_t res      = kFALSE;
    if (ev->InheritsFrom("Hal::ExpEvent")) {
      ExpTrack* tr = (ExpTrack*) ev->GetNewTrack();
      if (tr->GetDetTrack(DetectorID::kTPC) != NULL) res = kTRUE;
      delete tr;
    }
    return res;
  }

  Bool_t TrackExpCut::ToFAvaiable(Int_t format_id) const {
    if (FormatInhertis("ExpEvent", format_id) == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(format_id);
    Bool_t res      = kFALSE;
    ExpTrack* tr    = (ExpTrack*) ev->GetNewTrack();
    if (tr->GetDetTrack(DetectorID::kTOF) != NULL) res = kTRUE;
    delete tr;
    return res;
  }
}  // namespace Hal
