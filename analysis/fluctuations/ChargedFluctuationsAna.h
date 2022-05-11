/*
 * FluctuationsAna.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCHARGEDFLUCTUATIONSANA_H_
#define HALCHARGEDFLUCTUATIONSANA_H_
/**
 * basic class for calculation of fluctiuations of any variables
 */


#include <FairTask.h>
#include <TH2.h>
#include <TString.h>

#include "Cut.h"
#include "HistogramManager.h"
#include "TrackAna.h"

namespace Hal {
  class ChargedFluctuationsAna : public TrackAna {
    Int_t fBins[4];
    Double_t fMin[4], fMax[4];
    Int_t fNPos[4];
    Int_t fNNeg[4];
    enum eCollectionFlag { kPion = 0, kKaon = 1, kProton = 2, kCharge = 3 };
    TString HistoName(Int_t id, TString pattern) const;
    TString HistoTitle(Int_t id, TString pattern) const;
    HistogramManager_1_2D<TH2D>* fKaon;
    HistogramManager_1_2D<TH2D>* fPion;
    HistogramManager_1_2D<TH2D>* fProton;
    HistogramManager_1_2D<TH2D>* fCharged;

  protected:
    virtual void CheckCutContainerCollections();
    virtual void ProcessEvent();
    Task::EInitFlag Init();

  public:
    ChargedFluctuationsAna();
    /**
     * set histogram axis
     * @param bins number of bins
     * @param min min
     * @param max max
     * @param flag flag that is equal to cut collection id - 0 for pions, 2 for
     * kaons, 4 for protons, 6 for charged particles (see PionPlusID etc.)
     */
    void SetHistogramAxis(Int_t bins, Double_t min, Double_t max, Int_t flag);
    /**
     * set histogram axis -  recommended method - in this case range is made in
     * following way that bin centers corresponds from 0, 1.. nbisn
     * @param bins number of bins
     * @param min min
     * @param max max
     * @param flag flag that is equal to cut collection id - 0 for pions, 2 for
     * kaons, 4 for protons, 6 for charged particles (see PionPlusID etc.)
     */
    void SetHistogramAxis(Int_t bins, Int_t flag);
    virtual void AddCut(const Cut& cut, Option_t* opt = "");
    static Int_t PionPlusID() { return 0; };
    static Int_t PionMinusID() { return 1; };
    static Int_t KaonMinusID() { return 3; };
    static Int_t KaonPlusID() { return 2; };
    static Int_t ProtonID() { return 4; };
    static Int_t AntiProtonID() { return 5; };
    static Int_t PositiveChargeID() { return 6; };
    static Int_t NegativeChargeID() { return 7; };
    virtual Package* Report() const;
    virtual ~ChargedFluctuationsAna();
    ClassDef(ChargedFluctuationsAna, 1)
  };
}  // namespace Hal
#endif /* HALCHARGEDFLUCTUATIONSANA_H_ */
