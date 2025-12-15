/*
 * TrackBasicMcCut.cxx
 *
 *  Created on: 19 maj 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackBasicMcCut.h"

#include "DataFormatManager.h"
#include "McTrack.h"

namespace Hal {


  TrackBasicMcCut::TrackBasicMcCut() : TrackMcCut(4) {
    SetUnitName("status", Status);
    SetUnitName("pdg", Pdg);
    SetUnitName("p_{T} [GeV/c]", Pt);
    SetUnitName("#eta", Eta);
    SetMinMax(0, 1000, Status);
    SetMinMax(211, 211, Pdg);
    SetMinMax(0, 1, Pt);
    SetMinMax(-1, 1, Eta);
  }

  Bool_t TrackBasicMcCut::Pass(Track* track) {
    McTrack* mc_track = (McTrack*) track;
    SetValue(mc_track->GetStatus(), Status);
    SetValue(mc_track->GetPdg(), Pdg);
    SetValue(mc_track->GetMomentum().Pt(), Pt);
    Double_t p  = mc_track->GetMomentum().P();
    Double_t pz = mc_track->GetMomentum().Pz();
    if (p == pz) {
      if (p >= 0)
        SetValue(kMaxInt, Eta);
      else
        SetValue(-kMaxInt, Eta);
    } else {
      SetValue(mc_track->GetMomentum().Eta(), Eta);
    }
    return Validate();
  }

  void TrackBasicMcCut::SetStatusCut(Int_t stat) { SetMinAndMax(stat, Status); }

  void TrackBasicMcCut::SetPdgCut(Int_t pdg) { SetMinAndMax(pdg, Pdg); }

  void TrackBasicMcCut::SetPtCut(Double_t min, Double_t max) { SetMinMax(min, max, Pt); }

  void TrackBasicMcCut::SetEtaCut(Double_t min, Double_t max) { SetMinMax(min, max, Eta); }

  TrackBasicMcCut::~TrackBasicMcCut() {}
}  // namespace Hal
