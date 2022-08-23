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
  Source::Source(TString name) { fFileName.push_back(name); }

  Source::~Source() {}

  RootSource::RootSource(TString name) : Source(name), fManager(nullptr) {
    if (fFileName[0].Length() != 0) { fManager = new RootIOManager(fFileName[0]); }
  }

  void RootSource::AddFile(TString friendName) {
    if (fManager != nullptr) { fManager->AddFile(friendName); }
  }
  void RootSource::AddFriend(TString friendName, Int_t level) {
    if (fManager != nullptr) { fManager->AddFriend(friendName, level); }
  }
  RootSource::~RootSource() {
    if (fManager) delete fManager;
  }

  IOManager* RootSource::GetIOManager() const {
    { return fManager; };
  }
}  // namespace Hal
