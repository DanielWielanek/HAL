/*
 * OTFSource.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFSource.h"

#include "DataManager.h"
#include "OTFData.h"
#include "OTFEventGenerator.h"
#include "OTFIOManager.h"

namespace HalOTF {
  Source::Source(Int_t events) : Hal::Source("root_virtual.root"), fEvents(events) {
    fManager = new HalOTF::IOManager("root_virtual.root", this, fEvents);
  }

  Hal::IOManager* Source::GetIOManager() const { return fManager; }

  Source::~Source() {
    if (fManager) delete fManager;
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
  void Source::GetEvent() {
    fRecoEvent->Clear();
    fMcEvent->Clear();
    for (auto& gen : fGenerators)
      gen->GenerateEvent();
    ++fEvents;
  }

  void Source::RegisterOutputs(HalOTF::IOManager* mngr) {
    fMcEvent   = new OTF::McEvent();
    fRecoEvent = new OTF::RecoEvent();
    mngr->Register("OTF::McEvent.", "HalEvents", fMcEvent, fRegister);
    mngr->Register("OTF::RecoEvent.", "HalEvents", fRecoEvent, fRegister);
    for (auto& gen : fGenerators)
      gen->SetEvents(fMcEvent, fRecoEvent);
  }

}  // namespace HalOTF
