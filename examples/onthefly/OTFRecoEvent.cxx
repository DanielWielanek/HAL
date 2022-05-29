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

  void RecoEvent::CreateSource() { fSource = new RecoEventInterface(); }

  void RecoEvent::Update() {
    RecoEventInterface* s = (RecoEventInterface*) fSource;
    fTotalTracksNo        = s->GetEvent()->GetNTracks();
    fTracks->Clear();
    fTracks->ExpandCreateFast(fTotalTracksNo);
    OTF::RecoEvent* ev = s->GetEvent();
    TVector3 v         = ev->GetVertex();
    fVertex->SetXYZT(v.X(), v.Y(), v.Z(), 0);

    for (int i = 0; i < fTotalTracksNo; i++) {
      RecoTrack* track   = (RecoTrack*) fTracks->UncheckedAt(i);
      OTF::RecoTrack* mc = (OTF::RecoTrack*) ev->GetTrack(i);
      track->ResetTrack(i, this);
      track->SetMomentum(mc->GetMom().Px(), mc->GetMom().Py(), mc->GetMom().Pz(), mc->GetMom().E());
      track->SetMotherIndex(-1);
      track->SetMatch(mc->GetMcIndex());
    }
  }

  void RecoEvent::Clear(Option_t* opt) {
    if (fSource) fSource->Clear();
  }

  Bool_t RecoEvent::ExistInTree() const {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    if (manager->CheckBranch("OTF::RecoEvent.")) { return kTRUE; }
    return kFALSE;
  }
}  // namespace HalOTF
