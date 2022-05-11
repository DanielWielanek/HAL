/*
 * SpectraAna.h
 *
 *  Created on: 21-07-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALSPECTRAANA_H_
#define HALSPECTRAANA_H_

#include "Array.h"
#include "HistogramManager.h"
#include "TrackAna.h"
#include <TClonesArray.h>
#include <vector>
/**
 * basic class for making spectra analysis
 */
namespace Hal {
  class SpectraAna : public TrackAna {
    HistogramManager_1_2D<TH2D>* fPtEta;
    HistogramManager_1_2D<TH2D>* fPtY;
    HistogramManager_1_2D<TH2D>* fMtEta;
    HistogramManager_1_2D<TH2D>* fMtY;
    Int_t fPtBins, fMtBins, fEtaBins, fYBins;
    Double_t fPtMin, fPtMax;
    Double_t fMtMin, fMtMax;
    Double_t fEtaMin, fEtaMax;
    Double_t fYMin, fYMax;
    TClonesArray* fEventCollectionsNames;
    TClonesArray* fTrackCollectionsNames;
    std::vector<Double_t> fMass;
    Bool_t fUseMass;
    void CheckNames();

  protected:
    virtual void ProcessTrack();
    virtual Task::EInitFlag Init();
    Package* Report() const;

  public:
    /**
     * default constructor
     */
    SpectraAna();
    /**
     * configure pt axis
     * @param bins
     * @param min
     * @param max
     */
    void SetPtAxis(Int_t bins, Double_t min, Double_t max);
    /**
     * configure mt axis
     * @param bins
     * @param min
     * @param max
     */
    void SetMtAxis(Int_t bins, Double_t min, Double_t max);
    /**
     * configure rapdity axis
     * @param bins
     * @param min
     * @param max
     */
    void SetYAxis(Int_t bins, Double_t min, Double_t max);
    /**
     * configure pseudorapidity axis
     * @param bins
     * @param min
     * @param max
     */
    void SetEtaAxis(Int_t bins, Double_t min, Double_t max);
    /**
     * set name for event collection (later used for naming histograms)
     * @param name
     * @param event_collection
     */
    void SetEventCollectionName(TString name, Int_t event_collection = 0);
    /**
     * set name fot track collection (used for naming histograms)
     * @param name
     * @param track_collection
     */
    void SetTrackCollectionName(TString name, Int_t track_collection = 0);
    /**
     * set mass for given track (if energy is not available from data), if user
     * set mass for one track collection - then should set mass for all track
     * collections
     * @param mass
     * @param track_collection
     */
    void SetMass(Double_t mass, Int_t track_collection);
    /** like for TrackAna + additionaly option
     * @param opt if "auto" then track collections are configured in automatic
     * mode, if user want to make analysis as function of event collections all
     * event collections must be added before calling this options
     */
    void SetOption(Option_t* opt);
    virtual ~SpectraAna();
    ClassDef(SpectraAna, 1)
  };
}  // namespace Hal
#endif /* HALSPECTRAANA_H_ */
