/*
 * TrackV0MinvCut.cxx
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#include "TrackV0MinvCut.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>

#include "Cout.h"
#include "V0Track.h"

namespace Hal {

  Bool_t TrackV0MinvCut::PassV0(V0Track* tr) {
    SetValue(tr->GetHypoMass(fM1, fM2));
    return Validate();
  }

  TrackV0MinvCut::TrackV0MinvCut() : TrackV0Cut(1) { SetUnitName("M_{inv} [GeV/c]", 0); }

  void TrackV0MinvCut::SetDaughersPid(Int_t pos, Int_t neg) {
    fPid1 = pos;
    fPid2 = neg;
  }

  Bool_t TrackV0MinvCut::Init(Int_t taskid) {
    TDatabasePDG* pid = TDatabasePDG::Instance();
    auto p1           = pid->GetParticle(fPid1);
    auto p2           = pid->GetParticle(fPid2);
    if (!p1 || !p2) {
      Hal::Cout::PrintInfo(Form("Cannot find particle with PID(%i,%i) in %s", fPid1, fPid2, ClassName()), EInfo::kError);
      return kFALSE;
    }
    fM1 = p1->Mass();
    fM2 = p2->Mass();

    return TrackV0Cut::Init(taskid);
  }
  //===========================================================================

  TrackV0InvMassLambdaCut::TrackV0InvMassLambdaCut() : TrackV0Cut(1) {
    SetUnitName("M_{inv #Lambda#rightarrow p + #pi^{-}} [GeV/c^{2}]");
  }

  Bool_t TrackV0InvMassLambdaCut::PassV0(V0Track* tr) {
    SetValue(tr->GetLambdaMass());
    return Validate();
  }

  //===========================================================================

  TrackV0InvMassAntiLambdaCut::TrackV0InvMassAntiLambdaCut() : TrackV0Cut(1) {
    SetUnitName("M_{inv #Lambda#rightarrow #bar{p} + #pi^{+}} [GeV/c^{2}]");
  }

  Bool_t TrackV0InvMassAntiLambdaCut::PassV0(V0Track* tr) {
    SetValue(tr->GetAntiLambdaMass());
    return Validate();
  }

  //===========================================================================

  TrackV0InvMassK0Cut::TrackV0InvMassK0Cut() : TrackV0Cut(1) {
    SetUnitName("M_{inv K_{0s}#rightarrow #pi^{+} + #pi^{-}} [GeV/c^{2}]");
  }


  Bool_t TrackV0InvMassK0Cut::PassV0(V0Track* tr) {
    SetValue(tr->GetK0Mass());
    return Validate();
  }

} /* namespace Hal */
