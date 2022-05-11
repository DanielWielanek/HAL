/*
 * TrackMtCut.cxx
 *
 *  Created on: 20-05-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackMtCut.h"


#include <TMath.h>
#include <TString.h>

#include "Cut.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"

namespace Hal {

  TrackMtCut::TrackMtCut() : TrackCut(1), fMassFixed(kFALSE), fMass2(0), fPdg(NULL) { SetUnitName("m_{T} [GeV/c^{2}]"); }

  Package* TrackMtCut::Report() const {
    Package* pack = TrackCut::Report();
    TString mass_flag;
    TString mass_val;
    if (fMassFixed) {
      mass_flag = "fixed mass";
      mass_val  = Form("%4.3f", TMath::Sqrt(fMass2));
    } else {
      mass_flag = "free mass";
      mass_val  = " NULL";
    }
    pack->AddObject(new ParameterString("Mass status", mass_flag));
    pack->AddObject(new ParameterString("Mass val", mass_val));
    return pack;
  }

  TrackMtCut::TrackMtCut(const TrackMtCut& cut) : TrackCut(cut) {
    this->fMass2     = cut.fMass2;
    this->fMassFixed = cut.fMassFixed;
    this->fPdg       = cut.fPdg;
  }

  TrackMtCut::~TrackMtCut() {
    // TODO Auto-generated destructor stub
  }

  Bool_t TrackMtCut::Pass(Track* track) {
    Double_t px  = track->GetPx();
    Double_t py  = track->GetPy();
    Double_t pt2 = px * px + py * py;
    if (fMassFixed) {
      Double_t mt = TMath::Sqrt(pt2 + fMass2);
      SetValue(mt);
      return Validate();
    } else {
      Double_t m  = track->GetMass();
      Double_t mt = TMath::Sqrt(m * m + pt2);
      SetValue(mt);
      return Validate();
    }
  }

  void TrackMtCut::FixMass(Double_t mass) {
    fMassFixed = kTRUE;
    fMass2     = mass * mass;
  }

  Bool_t TrackMtCut::Init(const Int_t task_id) {
    Bool_t stat = TrackCut::Init(task_id);
    fPdg        = TDatabasePDG::Instance();
    return stat;
  }
}
