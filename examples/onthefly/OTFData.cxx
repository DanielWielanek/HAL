/*
 * OTFData.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "OTFData.h"

#include "McTrack.h"

#include <TDatabasePDG.h>

void OTF::RecoEvent::AddTrack(const RecoTrack& track) {
  Int_t pos       = fTracks->GetEntriesFast();
  RecoTrack* last = (RecoTrack*) fTracks->ConstructedAt(pos);
  *last           = track;
}

void OTF::McEvent::AddTrack(const McTrack& track) {
  Int_t pos     = fTracks->GetEntriesFast();
  McTrack* last = (McTrack*) fTracks->ConstructedAt(pos);
  *last         = track;
}

namespace OTF {
  namespace Std {
    Hal::McTrack OTFMcToHalMc(OTF::McTrack& other) {
      Hal::McTrack mommy;
      mommy.SetMomentum(other.GetMomentum().X(), other.GetMomentum().Y(), other.GetMomentum().Z(), other.GetMomentum().T());
      mommy.SetFreezeoutPosition(
        other.GetFreezeout().X(), other.GetFreezeout().Y(), other.GetFreezeout().Z(), other.GetFreezeout().T());
      mommy.SetPdg(other.GetPdgCode());
      return mommy;
    }

    OTF::RecoTrack HalMcToOTFReco(OTF::McTrack& tr) {
      OTF::RecoTrack rtr;
      double px  = tr.GetMomentum().Px();
      double py  = tr.GetMomentum().Py();
      double pz  = tr.GetMomentum().Pz();
      Double_t e = tr.GetMomentum().E();
      rtr.SetMom(px, py, pz, e);
      rtr.SetNHitsA(2);
      rtr.SetNHitsB(2);
      rtr.SetNHitsC(2);
      rtr.SetPidHypo(tr.GetPdgCode());
      rtr.SetGeneratorId(tr.GetGeneratorId());
      auto pid = TDatabasePDG::Instance()->GetParticle(tr.GetPdgCode());
      if (pid) {
        rtr.SetCharge(pid->Charge() / 3.0);
      } else {
        rtr.SetCharge(0);
      }
      return rtr;
    }

    OTF::McTrack HalMcToOTFMc(Hal::McTrack& tr) {
      OTF::McTrack trx;
      trx.SetMomentum(tr.GetMomentum());
      trx.SetFreezeout(tr.GetFreezeoutPosition());
      trx.SetPdgCode(tr.GetPdg());
      trx.SetMotherId(tr.GetMotherIndex());
      return trx;
    }
  }  // namespace Std
}  // namespace OTF
