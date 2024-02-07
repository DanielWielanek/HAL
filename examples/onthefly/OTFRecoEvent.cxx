/*
 * OTFRecoEvent.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFRecoEvent.h"
#include "OTFData.h"
#include "OTFRecoEventInterface.h"

#include "DataManager.h"

#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TVector3.h>


namespace HalOTF {
  RecoEvent::RecoEvent() : Hal::ExpEvent("HalOTF::RecoTrack") {}

  Hal::EventInterface* RecoEvent::CreateInterface() const { return new RecoEventInterface(); }

  void RecoEvent::Update(Hal::EventInterface* interface) {
    Clear();
    RecoEventInterface* s = (RecoEventInterface*) interface;
    fTotalTracksNo        = s->GetEvent()->GetNTracks();
    fMultiplicity         = fTotalTracksNo;
    fTracks->ExpandCreateFast(fTotalTracksNo);
    OTF::RecoEvent* ev = s->GetEvent();
    TVector3 v         = ev->GetVertex();
    fVertex->SetXYZT(v.X(), v.Y(), v.Z(), 0);
    fPhi = ev->GetPhi();

    for (int i = 0; i < fTotalTracksNo; i++) {
      RecoTrack* track   = (RecoTrack*) fTracks->UncheckedAt(i);
      OTF::RecoTrack* mc = (OTF::RecoTrack*) ev->GetTrack(i);
      track->ResetTrack(i, this);
      track->SetMomentum(mc->GetMom().Px(), mc->GetMom().Py(), mc->GetMom().Pz(), mc->GetMom().E());
      track->SetMotherIndex(-1);
      track->SetMatch(mc->GetMcIndex());
      Int_t dau1, dau2;
      mc->GetDaughters(dau1, dau2);
      if (dau1 != -1) {
        track->EnableV0(1, 1);
        track->GetV0Info()->SetPosId(dau1);
        track->GetV0Info()->SetNegId(dau2);
      }
    }
  }

  Bool_t RecoEvent::ExistInTree() const {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    if (manager->CheckBranch("OTF::RecoEvent.")) { return kTRUE; }
    return kFALSE;
  }
}  // namespace HalOTF
