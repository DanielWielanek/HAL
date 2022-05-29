/*
 * OTFSource.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "OTFSource.h"

#include "OTFIOManager.h"
#include "Source.h"

namespace HalOTF {
  Source::Source(Int_t events) : Hal::Source("root_virtual.root") {
    fEvents  = events;
    fManager = new HalOTF::IOManager("root_virtual.root", fEvents);
  }

  Hal::IOManager* Source::GetIOManager() const { return fManager; }

  Source::~Source() {
    if (fManager) delete fManager;
  }

}  // namespace HalOTF
