/*
 * FluctuationsAna.h
 *
 *  Created on: 31 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCHARGEDFLUCTUATIONSANA_H_
#define HALCHARGEDFLUCTUATIONSANA_H_
/**
 * basic class for calculation of fluctiuations of any number of charged particles
 * by default draw number of particles selected by odd cuts vs number of particles selected by event cuts vs event parameter
 */

#include <FairTask.h>
#include <TH3.h>
#include <TString.h>

#include "Cut.h"
#include "HistogramManager.h"
#include "TrackAna.h"

namespace Hal {
  class TrackCut;
  class EventCut;

  class ChargedFluctuationsAna : public TrackAna {
    Int_t fBins;
    Double_t fMin, fMax;
    Int_t fEventBins, fEventPar, fTrackColsHalf;
    Double_t fEventMin, fEventMax;
    std::vector<Int_t> fCounts;
    std::vector<TString> fEventLabels;
    std::vector<TString> fTrackLabels;
    TString HistoName(Int_t id, TString pattern) const;
    TString HistoTitle(Int_t id, TString pattern) const;
    TString CleanOpt(Option_t* opt, Int_t col) const;
    HistogramManager_2_3D<TH3D>* fHistogram;

  protected:
    virtual void CheckCutContainerCollections();
    virtual void ProcessEvent();
    Task::EInitFlag Init();

  public:
    ChargedFluctuationsAna();
    void SetEventLabels(const std::initializer_list<TString>& init = {"Events"});
    void SetTrackLabels(const std::initializer_list<TString>& init = {"Pions"});
    /**
     * set histogram axis
     * @param bins number of bins
     * @param min min
     * @param max max
     * @param flag flag that is equal to cut collection id - 0 for pions, 2 for
     * kaons, 4 for protons, 6 for charged particles (see PionPlusID etc.)
     */
    void SetTrackHistogramAxis(Int_t bins, Double_t min, Double_t max);
    /**
     * set third axis of a histogram with fluctuations
     */
    void SetEventHistogramAxis(Int_t bins, Double_t min, Double_t max);
    virtual void AddCut(const Cut& cut, Option_t* opt = "");
    /**
     * save way to add pair of track cuts
     */
    void AddCut(const TrackCut& pos, const TrackCut& neg, Option_t* opt = "");

    /**
     * event property around second/third axis
     */
    void SetEventprop(Int_t evProp);
    virtual Package* Report() const;
    virtual ~ChargedFluctuationsAna();
    ClassDef(ChargedFluctuationsAna, 1)
  };
}  // namespace Hal

#endif /* NICACHARGEDFLUCTUATIONSANA_H_ */
