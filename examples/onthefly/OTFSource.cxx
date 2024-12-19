/*
 * OTFSource.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFSource.h"

#include "Cout.h"
#include "DataManager.h"
#include "OTFData.h"
#include "OTFEventGenerator.h"
#include "OTFIOManager.h"
#include "VirtualIOManager.h"
#include "VirtualSource.h"

namespace HalOTF {
  Source::Source(Int_t events) : Hal::VirtualSource(new HalOTF::IOManager(this, events), events) {}

  Source::~Source() {
    if (fMcEvent) delete fMcEvent;
    if (fRecoEvent) delete fRecoEvent;
  }

  Bool_t Source::Init() {
    if (fGenerators.size() == 0) return kFALSE;
    for (auto& gen : fGenerators) {
      if (!gen->Init()) return kFALSE;
    }
    return kTRUE;
  }

  void Source::GetEvent(Int_t /*i*/, Int_t /*flag*/) {
    fRecoEvent->Clear();
    fMcEvent->Clear();
    for (auto& gen : fGenerators)
      gen->GenerateEvent();
    ++fEvents;
  }

  void Source::RegisterInputs() {
    fMcEvent   = new OTF::McEvent();
    fRecoEvent = new OTF::RecoEvent();
    fManager->Register("OTF::McEvent.", "HalEvents", fMcEvent, fRegister);
    fManager->Register("OTF::RecoEvent.", "HalEvents", fRecoEvent, fRegister);
    for (auto& gen : fGenerators)
      gen->SetEvents(fMcEvent, fRecoEvent);
  }

}  // namespace HalOTF
