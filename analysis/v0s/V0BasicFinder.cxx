/*
 * V0BasicFinder.cxx
 *
 *  Created on: 27 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "V0BasicFinder.h"


#include <FairTask.h>
#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TObjArray.h>
#include <TParticlePDG.h>
#include <TString.h>

#include "Cout.h"
#include "CutContainer.h"
#include "CutMonitor.h"
#include "DataManager.h"
#include "MemoryMapManager.h"
#include "Package.h"
#include "Parameter.h"
#include "TrackCut.h"
#include "TwoTrack.h"
#include "TwoTrackCut.h"
#include "V0Candidate.h"
#include "V0Track.h"

#include <iostream>

namespace Hal {
  V0BasicFinder::V0BasicFinder() :
    TrackAna(),
    fPosPdg(2212),
    fNegPdg(-211),
    fV0Pdg(3312),
    fPosMass(0),
    fNegMass(0),
    fV0Mass(0),
    fFirstV0Finder(kTRUE),
    fWrite(kFALSE),
    fV0Candiate(nullptr),
    fV0Tracks(nullptr) {
    fTiers = ECutUpdate::kTwoTrack;
  }

  Task::EInitFlag V0BasicFinder::Init() {
    DataManager* mngr = DataManager::Instance();
    if (mngr->CheckBranch("V0Tracks") != 0) {
      fFirstV0Finder = kFALSE;
      fV0Tracks      = (TClonesArray*) mngr->GetObject("V0Tracks");
    } else {
      fV0Tracks = new TClonesArray("V0Track");
      mngr->Register("V0Tracks", "V0", fV0Tracks, fWrite);
    }

    Task::EInitFlag stat = TrackAna::Init();
    if (fV0Candiate == nullptr) {
      Cout::PrintInfo("No VO candidate class!", EInfo::kLessError);
      return Task::EInitFlag::kFATAL;
    }
    TDatabasePDG* db       = TDatabasePDG::Instance();
    TParticlePDG* particle = db->GetParticle(fV0Pdg);
    if (particle == nullptr) return Task::EInitFlag::kFATAL;
    fV0Mass  = particle->Mass();
    particle = db->GetParticle(fPosPdg);
    if (particle == nullptr) return Task::EInitFlag::kFATAL;
    fPosMass = particle->Mass();
    particle = db->GetParticle(fNegPdg);
    if (particle == nullptr) return Task::EInitFlag::kFATAL;
    fNegMass = particle->Mass();
    return stat;
  }

  Package* V0BasicFinder::Report() const {
    Package* pack = TrackAna::Report();
    pack->AddObject(new ParameterInt("Pos_dau PDG", fPosPdg));
    pack->AddObject(new ParameterInt("Neg dau PDG", fNegPdg));
    pack->AddObject(new ParameterInt("V_{0} PDG", fV0Pdg));
    return pack;
  }

  void V0BasicFinder::ProcessEvent() {
    TrackAna::ProcessEvent();
    for (int i = 0; i < fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 0); i++) {
      fV0Candiate->AddFirstTrack(fMemoryMap->GetTemporaryTrack(fCurrentEventCollectionID, 0, i));
      for (int j = 0; j < fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 1); j++) {
        fV0Candiate->AddSecondTrack(fMemoryMap->GetTemporaryTrack(fCurrentEventCollectionID, 1, j));
        fV0Candiate->MakeV0(fPosMass, fNegMass, fV0Pdg, fPosPdg, fNegPdg);
        //		if(TMath::Abs(fV0Candiate->GetV0Track().GetMom().M()-1.115)<0.05)
        if (fCutContainer->PassSignalPair(fV0Candiate, 0)) {
          V0Track* v0 = (V0Track*) fV0Tracks->ConstructedAt(fV0Tracks->GetEntriesFast());
          *v0         = fV0Candiate->GetV0Track();
        }
      }
    }
  }

  void V0BasicFinder::Exec(Option_t* option) {
    if (fFirstV0Finder) fV0Tracks->Clear();
    TrackAna::Exec(option);
  }

  V0BasicFinder::~V0BasicFinder() {}

  void V0BasicFinder::AddNegDauCut(const TrackCut& dau_neg, Option_t* opt) {
    TString option = opt;
    TString optz   = "";
    if (Hal::Std::FindParam(option, "fast")) { optz = "fast"; }
    if (Hal::Std::FindParam(option, "re")) { optz = optz + "+re"; }
    if (Hal::Std::FindParam(option, "im")) { optz = optz + "+im"; }
    TrackAna::AddCut(dau_neg, optz + "{1}");
  }

  void V0BasicFinder::AddPosDauCut(const TrackCut& pos_cut, Option_t* opt) {
    TString option = opt;
    TString optz   = "";
    if (Hal::Std::FindParam(option, "fast")) { optz = "fast"; }
    if (Hal::Std::FindParam(option, "re")) { optz = optz + "+re"; }
    if (Hal::Std::FindParam(option, "im")) { optz = optz + "+im"; }
    TrackAna::AddCut(pos_cut, optz + "{0}");
  }

  void V0BasicFinder::AddCandicateCut(const TwoTrackCut& obj, Option_t* opt) { TrackAna::AddCut(obj, opt); }

  void V0BasicFinder::AddCandicateCutMon(const CutMonitor& mon, Option_t* opt) { TrackAna::AddCutMonitor(mon, opt); }

  void V0BasicFinder::SetPDG(Int_t dau1, Int_t dau2, Int_t v0) {
    fPosPdg = dau1;
    fNegPdg = dau2;
    fV0Pdg  = v0;
  }

  void V0BasicFinder::SetV0Container(const V0Candidate& vo) { fV0Candiate = (V0Candidate*) vo.Clone("v0 candidate"); }

  void V0BasicFinder::AddNegDauCutMon(const CutMonitor& mon, Option_t* /*opt*/) { TrackAna::AddCutMonitor(mon, "{1}"); }

  void V0BasicFinder::AddPosDauCutMon(const CutMonitor& mon, Option_t* /*opt*/) { TrackAna::AddCutMonitor(mon, "{0}"); }

  void V0BasicFinder::LinkCollections() {
    Int_t eventTrig = fCutContainer->GetEventCollectionsNo();
    for (int iEv = 0; iEv < eventTrig; iEv++) {
      fCutContainer->LinkCollections(ECutUpdate::kEvent, iEv, ECutUpdate::kTrack, 0);
      fCutContainer->LinkCollections(ECutUpdate::kEvent, iEv, ECutUpdate::kTrack, 1);
      fCutContainer->LinkCollections(ECutUpdate::kTrack, 0, ECutUpdate::kTwoTrack, 0);
      fCutContainer->LinkCollections(ECutUpdate::kTrack, 1, ECutUpdate::kTwoTrack, 0);
    }
  }
}  // namespace Hal
