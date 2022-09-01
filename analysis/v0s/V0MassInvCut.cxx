/*
 * V0MassInvCut.cxx
 *
 *  Created on: 26 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "V0MassInvCut.h"

#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "V0Track.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>

namespace Hal {
  V0MassInvCut::V0MassInvCut() : TrackCut(1) { SetUnitName("M_{inv} [GeV/c^2]"); }

  Bool_t V0MassInvCut::Pass(Track* track) {
    if (track->IsV0() == kFALSE) {
      SetValue(-999);
      return ForcedUpdate(kFALSE);
    }
    V0Track* v0 = track->GetV0Info();
    SetValue(v0->GetHypoMass(fM1, fM2));
    return Validate();
  }
  Bool_t V0MassInvCut::Init(Int_t task_id) {
    TDatabasePDG* pid = TDatabasePDG::Instance();
    TParticlePDG* pos = pid->GetParticle(fPid1Hypo);
    TParticlePDG* neg = pid->GetParticle(fPid2Hypo);
    if (pos == nullptr) return kFALSE;
    if (neg == nullptr) return kFALSE;
    fM1 = pos->Mass();
    fM2 = neg->Mass();
    return TrackCut::Init(task_id);
  }
  Package* V0MassInvCut::Report() const {
    Package* pack = TrackCut::Report();
    pack->AddObject(new ParameterInt("Dau1hypo", fPid1Hypo));
    pack->AddObject(new ParameterInt("Dau2hypo", fPid2Hypo));
    return pack;
  }

  //===========================================================================

  V0InvMassLambdaCut::V0InvMassLambdaCut() : TrackCut(1) { SetUnitName("M_{inv #Lambda#rightarrow p + #pi^{-}} [GeV/c^2]"); }

  Bool_t V0InvMassLambdaCut::Pass(Track* track) {
    if (track->IsV0() == kFALSE) {
      SetValue(-999);
      return ForcedUpdate(kFALSE);
    }
    V0Track* v0 = track->GetV0Info();
    SetValue(v0->GetHypoMass(Const::ProtonMass(), Const::PionPlusMass()));
    return Validate();
  }

  //===========================================================================

  V0InvMassAntiLambdaCut::V0InvMassAntiLambdaCut() : TrackCut(1) {
    SetUnitName("M_{inv #Lambda#rightarrow #bar{p} + #pi^{+}} [GeV/c^2]");
  }

  Bool_t V0InvMassAntiLambdaCut::Pass(Track* track) {
    if (track->IsV0() == kFALSE) {
      SetValue(-999);
      return ForcedUpdate(kFALSE);
    }
    V0Track* v0 = track->GetV0Info();
    SetValue(v0->GetHypoMass(Const::PionPlusMass(), Const::ProtonMass()));
    return Validate();
  }

  //===========================================================================

  V0InvMassK0Cut::V0InvMassK0Cut() : TrackCut(1) { SetUnitName("M_{inv K_{0s}#rightarrow #pi^{+} + #pi^{-}} [GeV/c^2]"); }

  Bool_t V0InvMassK0Cut::Pass(Track* track) {
    if (track->IsV0() == kFALSE) {
      SetValue(-999);
      return ForcedUpdate(kFALSE);
    }
    V0Track* v0 = track->GetV0Info();
    SetValue(v0->GetHypoMass(Const::PionPlusMass(), Const::PionPlusMass()));
    return Validate();
  }
}  // namespace Hal
