/*
 * TrackTPCToFCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKTPCTOFCUT_H_
#define HALTRACKTPCTOFCUT_H_

#include "TrackToFMass2Cut.h"
#include "TrackTpcCut.h"

namespace Hal {
  class TrackTpcToFCut : public TrackExpCut {
  protected:
    TrackToFMass2Cut* fToF;
    TrackTpcCut* fTpc;
    Double_t fThreshold;
    Bool_t fTofOpt;

  public:
    TrackTpcToFCut(TrackTpcCut* tpc = NULL, TrackToFMass2Cut* tof = NULL);
    TrackTpcToFCut(const TrackTpcToFCut& other);
    /**
     * Set mode to "good". In this mode are accepted only particles that have good
     * enough "good sigma" and bad enough "bad sigma". Eg. if this cut looks for
     * pions is not eounugh to be pion but particle should not be electron or kaon
     */
    void SetModeGood() { fTpc->SetModeGood(); };
    /**
     * Set mode to not bad. In this mode all particles that have sigma between
     * lower/upper value of given type are accepted.
     */
    void SetModeNotBad() { fTpc->SetModeNotBad(); };
    /**
     *
     * set sigma cut
     * @param min minimal sigma
     * @param max maximal sigma
     * @param type parameter index - for pion "pi" for kaon "K"  for proton
     * "p"  for electron "e"
     */
    void SetSigma(Double_t min, Double_t max, TString type) { fTpc->SetSigma(min, max, type); };
    /**
     * set charge cut
     * @param i accepted charge
     */
    void SetCharge(Int_t i) { fTpc->SetCharge(i); };
    /**
     * set TPC hits
     * @param min minimal number of hits in TPC
     * @param max maximal number of hits in TPC
     */
    void SetNHits(Int_t min, Int_t max) { fTpc->SetNHits(min, max); };
    /**
     * set type of particles considered as "good particles"
     * @param flag "pi/K/p/e" for pions/kaons/protons/electrons respectively
     */
    void SetActiveSigma(TString flag) { fTpc->SetActiveSigma(flag); };
    void SetDeDx(Double_t min, Double_t max);
    void SetM2(Double_t min, Double_t max) { fToF->SetMinMax(min, max); }
    /**
     *
     * @return parameter index for pion sigma
     */
    static inline Int_t PionSigma() { return TrackTpcCut::PionSigma(); };
    /**
     *
     * @return  parameter index for kaon sigma
     */
    static inline Int_t KaonSigma() { return TrackTpcCut::KaonSigma(); };
    /**
     *
     * @return parameter index for proton sigma
     */
    static inline Int_t ProtonSigma() { return TrackTpcCut::ProtonSigma(); };
    /**
     *
     * @return parameter index for electron sigma
     */
    static inline Int_t ElectronSigma() { return TrackTpcCut::ElectronSigma(); };
    /**
     *
     * @return parameter index for number of tpc hits
     */
    static inline Int_t TpcHits() { return TrackTpcCut::TpcHits(); };
    /**
     *
     * @return parameter index for charge
     */
    static inline Int_t Charge() { return TrackTpcCut::Charge(); };
    /**
     *
     * @return parameter index of DeDx
     */
    static inline Int_t DeDx() { return TrackTpcCut::DeDx(); };
    static inline Int_t M2() { return 7; };
    void SetToFPolyLineDown(Double_t a, Double_t b, Double_t c) { fToF->SetPolyLineDown(a, b, c); };
    void SetToFPolyLIneUp(Double_t a, Double_t b, Double_t c) { fToF->SetPolyLineUp(a, b, c); };
    void GetPolyUp(Double_t& a, Double_t& b, Double_t& c) const { fToF->GetPolyUp(a, b, c); };
    void GetPolyDown(Double_t& a, Double_t& b, Double_t& c) const { fToF->GetPolyDown(a, b, c); };
    void SetThreshold(Double_t threshold) { fThreshold = threshold; };
    void SetUpPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t x3, Double_t y3) {
      fToF->SetUpPoints(x1, y1, x2, y2, x3, y3);
    };
    void SetDownPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t x3, Double_t y3) {
      fToF->SetDownPoints(x1, y1, x2, y2, x3, y3);
    };
    /**
     * use ToF below threshold if ToF data avaiable
     */
    void UseToF() { fTofOpt = kTRUE; };
    Bool_t Init(Int_t format_id);
    virtual Bool_t Pass(Track* tr);
    virtual Package* Report() const;
    virtual ~TrackTpcToFCut();
    ClassDef(TrackTpcToFCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKTPCTOFCUT_H_ */
