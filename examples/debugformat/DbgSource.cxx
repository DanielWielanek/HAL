/*
 * DbgSource.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DbgSource.h"

#include "DbgIOManager.h"
#include "Source.h"

namespace HalDbg {
  Source::Source(Int_t events) : Hal::Source("root_virtual.root") {
    fEvents  = events;
    fManager = new HalDbg::IOManager("root_virtual.root", fEvents);
  }

  Hal::IOManager* Source::GetIOManager() const { return fManager; }

  Source::~Source() {
    if (fManager) delete fManager;
  }

}  // namespace HalDbg
