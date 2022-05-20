/*
 * TrackCut.cpp
 *
 *  Created on: 21-06-2013
 *      Author: Daniel Wielanek
 */

#include "TrackCut.h"

#include <TString.h>

#include "Std.h"
#include "TrackComplexCut.h"
#include "TrackVirtualCut.h"

namespace Hal {
  TrackCut::TrackCut(const Int_t i) : Cut(i, ECutUpdate::kTrack, "N_{tracks}") {
    // fUnitName = " ";
  }

  TrackCut::~TrackCut() {
    // TODO Auto-generated destructor stub
  }

  Package* TrackCut::Report() const {
    Package* pack = Cut::Report();
    return pack;
  }

  TrackComplexCut* TrackCut::MakeComplexCut(Option_t* opt) const {
    if (this->InheritsFrom("Hal::TrackComplexCut")) return NULL;
    TString option = opt;
    if (option == "re" || option == "RE" || option == "Re") {
      return new TrackComplexCut(this, new TrackVirtualCut());
    } else if (option == "im" || option == "IM" || option == "Im") {
      return new TrackComplexCut(new TrackVirtualCut(), this);
    } else {
      return new TrackComplexCut(this, this);
    }
  }
}  // namespace Hal
