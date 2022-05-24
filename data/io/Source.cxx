/*
 * Source.cxx
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Source.h"

#include "RootIOManager.h"

#include <TString.h>

namespace Hal {
  Source::Source(TString name) : fFileName(name) {}

  Source::~Source() {}

  RootSource::RootSource(TString name) : Source(name), fManager(nullptr) {
    if (fFileName.Length() != 0) { fManager = new RootIOManager(fFileName); }
  }

  void RootSource::AddFriend(TString friendName) {
    if (fManager != nullptr) { fManager->AddFriend(friendName); }
  }

  RootSource::~RootSource() {
    if (fManager) delete fManager;
  }
  IOManager* RootSource::GetIOManager() const {
    { return fManager; };
  }
}  // namespace Hal
