/*
 * OTFReader.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFReader.h"
#include "OTFComplexEvent.h"
#include "OTFMcEvent.h"
#include "OTFRecoEvent.h"

#include "Cout.h"
#include "DataManager.h"
#include "Event.h"
#include "McTrack.h"
#include "OTFData.h"
#include "Std.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TString.h>

namespace HalOTF {
  Reader::Reader() : fMcEvent(nullptr), fRecoEvent(nullptr) {}

  Hal::Task::EInitFlag Reader::Init() {
    fMcEvent              = new OTF::McEvent();
    fRecoEvent            = new OTF::RecoEvent();
    Hal::DataManager* mng = Hal::DataManager::Instance();
    if (mng->GetObject("OTF::McEvent.")) {  // branch exists
      fMcEvent   = (OTF::McEvent*) mng->GetObject("OTF::McEvent.");
      fRecoEvent = (OTF::RecoEvent*) mng->GetObject("OTF::RecoEvent.");
    }
    if (!fMcEvent) {
      Hal::Cout::PrintInfo(Form("%s %d lack of mc-event", __FILE__, __LINE__), Hal::EInfo::kError);
      return Hal::Task::EInitFlag::kERROR;
    }
    if (!fRecoEvent) {
      Hal::Cout::PrintInfo(Form("%s %d lack of reco-event", __FILE__, __LINE__), Hal::EInfo::kError);
      return Hal::Task::EInitFlag::kERROR;
    }

    switch (fTranslate) {
      case ETranslate::kMc: {
        if (mng->GetObject("HalOTF::McEvent.")) {
          return Hal::Task::EInitFlag::kERROR;
        } else {
          fHalMcEvent = new HalOTF::McEvent();
          mng->Register("HalEvent.", "HalEvents", fHalMcEvent, kFALSE);
          fTranslateInterface = fHalMcEvent->CreateInterface();
        }
      } break;
      case ETranslate::kReco: {
        if (mng->GetObject("HalOTF::RecoEvent.")) {
          return Hal::Task::EInitFlag::kERROR;
        } else {
          fHalRecoEvent = new HalOTF::RecoEvent();
          mng->Register("HalEvent.", "HalEvents", fHalRecoEvent, kFALSE);
          fTranslateInterface = fHalRecoEvent->CreateInterface();
        }
      } break;
      case ETranslate::kComplex: {
        if (mng->GetObject("HalOTF::ComplexEvent.")) {
          return Hal::Task::EInitFlag::kERROR;
        } else {
          fHalComplexEvent = new HalOTF::ComplexEvent();
          mng->Register("HalEvent.", "HalEvents", fHalComplexEvent, kFALSE);
          fTranslateInterface = fHalComplexEvent->CreateInterface();
        }
      } break;
      default: break;
    }
    if (fTranslateInterface) fTranslateInterface->ConnectToTree(Hal::EventInterface::eMode::kRead);
    return Hal::Task::EInitFlag::kSUCCESS;
  }

  void Reader::Exec(Option_t* /*opt*/) {
    if (fTranslate != ETranslate::kNone) {
      switch (fTranslate) {
        case ETranslate::kMc: {
          fHalMcEvent->Update(fTranslateInterface);
        } break;
        case ETranslate::kReco: {
          fHalRecoEvent->Update(fTranslateInterface);
        } break;
        case ETranslate::kComplex: {
          fHalComplexEvent->Update(fTranslateInterface);
        } break;
        default: break;
      }
    }
  }

  Reader::~Reader() {}

  void Reader::Translate(TString opt) {
    Int_t flag = 0;
    if (Hal::Std::FindParam(opt, "sim")) { flag = 1; }
    if (Hal::Std::FindParam(opt, "reco")) { flag += 2; }
    switch (flag) {
      case 1: fTranslate = ETranslate::kMc; break;
      case 2: fTranslate = ETranslate::kReco; break;
      case 3: fTranslate = ETranslate::kComplex; break;
    }
  }

}  // namespace HalOTF
