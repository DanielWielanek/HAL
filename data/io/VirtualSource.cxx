/*
 * VirtualRootSource.cxx
 *
 *  Created on: 19 gru 2024
 *      Author: daniel
 */

#include "VirtualSource.h"

#include <TFile.h>
#include <TTree.h>

#include "Cout.h"
#include "InputDataInfo.h"
#include "Std.h"
#include "VirtualIOManager.h"

namespace Hal {

  VirtualSource::VirtualSource(Int_t nevents) : Hal::Source("/dev/null"), fEvents(nevents) {
    fManager = new VirtualIOManager(this, nevents);
  }

  VirtualSource::VirtualSource(IOManager* mngr, Int_t nevents) : Source(mngr), fEvents(nevents) {}

  Bool_t VirtualSource::Init() { return kTRUE; }

  void VirtualSource::GetEvent(Int_t /*i*/, Int_t /*flag*/) { ++fEvents; }

} /* namespace Hal */
