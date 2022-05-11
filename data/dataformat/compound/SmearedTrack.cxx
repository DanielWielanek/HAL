/*
 * TrackSmeared.cxx
 *
 *  Created on: 20 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "SmearedTrack.h"

namespace Hal {
  SmearedTrack::SmearedTrack() : McTrack(), fTrack(NULL) { fTrueMomenta = new TLorentzVector(); }

  SmearedTrack::~SmearedTrack() {}

  SmearedTrack::SmearedTrack(Track* track) : SmearedTrack() { fTrack = track; }

  void SmearedTrack::CopyData(Track* other) {
    McTrack::CopyData(other);
    if (((SmearedTrack*) other)->fTrack) { fTrack = ((SmearedTrack*) other)->fTrack; }
    *fTrueMomenta = *((SmearedTrack*) other)->fTrueMomenta;
  }
}  // namespace Hal
