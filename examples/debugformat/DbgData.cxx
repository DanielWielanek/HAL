/*
 * OTFData.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DbgData.h"

void Dbg::RecoEvent::AddTrack(const RecoTrack& track) {
  Int_t pos       = fTracks->GetEntriesFast();
  RecoTrack* last = (RecoTrack*) fTracks->ConstructedAt(pos);
  *last           = track;
}

void Dbg::McEvent::AddTrack(const McTrack& track) {
  Int_t pos     = fTracks->GetEntriesFast();
  McTrack* last = (McTrack*) fTracks->ConstructedAt(pos);
  *last         = track;
}
