/*
 * Source.cxx
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Source.h"

#include "InputDataInfo.h"
#include "RootIOManager.h"

#include <TString.h>

namespace Hal {

  Source::~Source() {
    if (fManager) delete fManager;
  }

  void Source::AddFile(TString file) { fManager->AddFile(file); };

  void Source::AddFriend(TString friendName, Int_t level) { fManager->AddFriend(friendName, level); };

  TString Source::GetSourceName() const { return fManager->GetSourceName(); };

  IOManager* Source::GetIOManager() const { return fManager; };

  RootSource::RootSource(TString name) { fManager = new RootIOManager(name); }

}  // namespace Hal
