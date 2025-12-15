/*
 * TrackFlagCuts.cxx
 *
 *  Created on: 2 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackFlagCuts.h"

#include "Track.h"
namespace Hal {
  Bool_t TrackPrimaryFlagCut::Pass(Track* track) {
    if (track->IsPrimary()) {
      SetValue(Primary);
    } else {
      SetValue(NonPrimary);
    }
    return Validate();
  }

  Bool_t TrackV0FlagCut::Pass(Track* track) {
    if (track->IsGoodV0()) {
      SetValue(GoodV0);
    } else if (track->IsV0()) {
      SetValue(BadV0);
    } else {
      SetValue(NonV0);
    }
    return Validate();
  }

  Bool_t TrackGoodSecondaryFlagCut::Pass(Track* track) {
    if (track->IsGoodSecondary()) {
      SetValue(HasMother);
    } else {
      SetValue(IsOrphan);
    }
    return Validate();
  }

  Bool_t TrackGlobalFlagCut::Pass(Track* track) {
    if (track->IsGlobal()) {
      SetValue(GlobalTrack);
    } else {
      SetValue(NonGlobalTrack);
    }
    return Validate();
  }

  Bool_t TrackEmbeddedFlagCut::Pass(Track* track) {
    if (track->IsEmbedded())
      SetValue(ValID::EmbeddedTrack);
    else
      SetValue(ValID::NonEmbeddedTrack);
    return Validate();
  }

  Bool_t TrackBackgroundFlagCut::Pass(Track* track) {
    if (track->IsBackground())
      SetValue(ValID::BackgroundTrack);
    else
      SetValue(ValID::NonBackgroundTrack);
    return Validate();
  }

  std::vector<std::pair<TString, Double_t>> TrackPrimaryFlagCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> res;
    res.push_back(std::pair<TString, Double_t>("NonPrimary", ValID::NonPrimary));
    res.push_back(std::pair<TString, Double_t>("Primary", ValID::Primary));
    return res;
  }


  std::vector<std::pair<TString, Double_t>> TrackV0FlagCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> res;
    res.push_back(std::pair<TString, Double_t>("NonV0", ValID::NonV0));
    res.push_back(std::pair<TString, Double_t>("BadV0", ValID::BadV0));
    res.push_back(std::pair<TString, Double_t>("GoodV0", ValID::GoodV0));
    return res;
  }


  std::vector<std::pair<TString, Double_t>> TrackGoodSecondaryFlagCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> res;
    res.push_back(std::pair<TString, Double_t>("HasMother", ValID::HasMother));
    res.push_back(std::pair<TString, Double_t>("Orphan", ValID::IsOrphan));
    return res;
  }


  std::vector<std::pair<TString, Double_t>> TrackGlobalFlagCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> res;
    res.push_back(std::pair<TString, Double_t>("GlobalTrack", ValID::GlobalTrack));
    res.push_back(std::pair<TString, Double_t>("NonGlobalTrack", ValID::NonGlobalTrack));
    return res;
  }

  std::vector<std::pair<TString, Double_t>> TrackEmbeddedFlagCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> res;
    res.push_back(std::pair<TString, Double_t>("EmbeddedTrack", ValID::EmbeddedTrack));
    res.push_back(std::pair<TString, Double_t>("NonEmbeddedTrack", ValID::NonEmbeddedTrack));
    return res;
  }

  std::vector<std::pair<TString, Double_t>> TrackBackgroundFlagCut::GetBinLabels(Int_t par) const {
    std::vector<std::pair<TString, Double_t>> res;
    res.push_back(std::pair<TString, Double_t>("BackgroundTrack", ValID::BackgroundTrack));
    res.push_back(std::pair<TString, Double_t>("NonBackgroundTrack", ValID::NonBackgroundTrack));
    return res;
  }

}  // namespace Hal
