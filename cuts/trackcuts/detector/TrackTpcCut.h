/*
 * TrackTpcBasicCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKTPCCUT_H_
#define HALTRACKTPCCUT_H_
#include "TrackExpCut.h"
/**
 * basic abstract class for TPC tracks cut , supports two modes:
 * in mode "not bad" are accepted particles with nsigma of "good" particle
 * between upper and lower bounds in mode 'good" are additionaly rejected
 * particles with low sigma for "bad particles" even if have good enough "good
 * sigma". This class should not be used directly because ToFTrack doesn't
 * inherit directly from Track. Therefore user should create own version of
 * Pass method.
 * This Pass(Track*) method should explicite specify class of track that is
 * passed to cut. Then by using SetValue set values and use VerifyTpc to return
 * value (instead of Validate()).
 */
namespace Hal {
  class TrackTpcCut : public TrackExpCut {
    Int_t fParticleType;
    enum TrackTpcMode { kNotBad, kGood };
    TrackTpcMode fMode;

  protected:
    Bool_t Verify();

  public:
    enum ParID { TpcHits = 0, Charge = 1, SigmaPion = 2, SigmaKaon = 3, SigmaProton = 4, SigmaElectron = 5, DeDx = 6 };
    TrackTpcCut();
    TrackTpcCut(const TrackTpcCut& other);
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t task_id);
    /**
     * Set mode to "good". In this mode are accepted only particles that have good
     * enough "good sigma" and bad enough "bad sigma". Eg. if this cut looks for
     * pions is not eounugh to be pion but particle should not be electron or kaon
     */
    void SetModeGood() { fMode = kGood; };
    /**
     * Set mode to not bad. In this mode all particles that have sigma between
     * lower/upper value of given type are accepted.
     */
    void SetModeNotBad() { fMode = kNotBad; };
    /**
     *
     * set sigma cut
     * @param min minimal sigma
     * @param max maximal sigma
     * @param type parameter index - for pion "pi" for kaon "K"  for proton
     * "p"  for electron "e"
     */
    void SetSigma(Double_t min, Double_t max, TString sigma);
    /**
     * set charge cut
     * @param i accepted charge
     */
    void SetCharge(Int_t i);
    /**
     * set TPC hits
     * @param min minimal number of hits in TPC
     * @param max maximal number of hits in TPC
     */
    void SetNHits(Int_t min, Int_t max);
    /**
     * set type of particles considered as "good particles"
     * @param flag pi/K/p/e for pion/kaon/proton/electron respectively
     */
    void SetActiveSigma(TString flag);
    void SetDeDx(Double_t min, Double_t max);
    /**
     *
     * @return active sigma Id
     */
    Int_t GetActiveSigma() const { return fParticleType; };
    Package* Report() const;
    virtual ~TrackTpcCut();
    ClassDef(TrackTpcCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKTPCCUT_H_ */
