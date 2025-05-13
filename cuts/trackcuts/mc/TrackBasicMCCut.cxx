/*
 * TrackBasicMCCut.cxx
 *
 *  Created on: 19 maj 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackBasicMCCut.h"

#include "DataFormatManager.h"
#include "McTrack.h"

namespace Hal {

  const Int_t TrackBasicMCCut::fgStatusId = 0;
  const Int_t TrackBasicMCCut::fgPdgId    = 1;
  const Int_t TrackBasicMCCut::fgPtId     = 2;
  const Int_t TrackBasicMCCut::fgEtaId    = 3;
  TrackBasicMCCut::TrackBasicMCCut() : TrackMCCut(4) {
    SetUnitName("status", Status());
    SetUnitName("pdg", Pdg());
    SetUnitName("p_{T} [GeV/c]", Pt());
    SetUnitName("#eta", Eta());
    SetMinMax(0, 1000, Status());
    SetMinMax(211, 211, Pdg());
    SetMinMax(0, 1, Pt());
    SetMinMax(-1, 1, Eta());
  }

  Bool_t TrackBasicMCCut::Pass(Track* track) {
    McTrack* mc_track = (McTrack*) track;
    SetValue(mc_track->GetStatus(), Status());
    SetValue(mc_track->GetPdg(), Pdg());
    SetValue(mc_track->GetMomentum().Pt(), Pt());
    Double_t p  = mc_track->GetMomentum().P();
    Double_t pz = mc_track->GetMomentum().Pz();
    if (p == pz) {
      if (p >= 0)
        SetValue(kMaxInt, Eta());
      else
        SetValue(-kMaxInt, Eta());
    } else {
      SetValue(mc_track->GetMomentum().Eta(), Eta());
    }
    return Validate();
  }

  void TrackBasicMCCut::SetStatusCut(Int_t stat) { SetMinAndMax(stat, Status()); }

  void TrackBasicMCCut::SetPdgCut(Int_t pdg) { SetMinAndMax(pdg, Pdg()); }

  void TrackBasicMCCut::SetPtCut(Double_t min, Double_t max) { SetMinMax(min, max, Pt()); }

  void TrackBasicMCCut::SetEtaCut(Double_t min, Double_t max) { SetMinMax(min, max, Eta()); }

  TrackBasicMCCut::~TrackBasicMCCut() {}
}  // namespace Hal
