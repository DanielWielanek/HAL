/*
 * Decay.h
 *
 *  Created on: 21 gru 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_PHYS_DECAY_H_
#define HAL_FEATURES_PHYS_DECAY_H_

#include <TObject.h>
#include <vector>


namespace Hal {
  class McTrack;
  class McEvent;
  /**
   * represents a decay channel
   */
  class DecayChannel : public TObject {
    const Int_t fDaughters;
    std::vector<Int_t> fPdgCodes;
    std::vector<Double_t> fMass;
    Double_t fMassThres = {-1};
    Double_t fBranchRatio;

  public:
    /**
     * two body decay channel
     * @param dau1 daugher1 pdg
     * @param dau2 daughter 2 pdg
     * @param ratio branch ratio
     */
    DecayChannel(Int_t dau1 = 0, Int_t dau2 = 0, Double_t ratio = 0.0);
    /**
     * three body decay channel
     * @param dau1 first daughter PID
     * @param dau2 second daughter PID
     * @param dau3 third daughter PID
     * @param ratio branch ratio
     */
    DecayChannel(Int_t dau1, Int_t dau2, Int_t dau3, Double_t ratio);
    /**
     *
     * @param dau
     * @return daughter PID
     */
    Int_t GetDaughterPdg(Int_t dau) const { return fPdgCodes[dau]; }
    /**
     *
     * @return number of daughters
     */
    Int_t GetDaughtersNo() const { return fPdgCodes.size(); }
    /**
     *
     * @param dau
     * @return daughter mass
     */
    Double_t GetDaughterMass(Int_t dau) const { return fMass[dau]; }
    /**
     *
     * @return branching ratio
     */
    Double_t GetBranchingRatio() const { return fBranchRatio; }
    /**
     *
     * @return sum of decay products
     */
    Double_t GetMassThreshold() const { return fMassThres; }
    /**
     * set branching ratio
     * @param x
     */
    void SetBranchRatio(Double_t x) { fBranchRatio = x; }
    virtual Bool_t Init();
    virtual ~DecayChannel() {};
    ClassDef(DecayChannel, 1)
  };
  /**
   * class that performs decay on mc-track
   */
  class Decay : public TObject {
    std::vector<DecayChannel> fDecayChannels;
    Double_t fGamma       = {0};
    Bool_t fBreightWigher = {kFALSE};
    Double_t fMotherPdg   = {-1};
    Double_t fMotherMass  = {-1};

  protected:
    virtual Double_t GetDecayTime(McTrack& mother, Double_t mass) const;

    virtual void Decay2Body(McTrack& mother, std::vector<McTrack*>& daughters, const DecayChannel& channel) const;
    virtual void Decay3Body(McTrack& mother, std::vector<McTrack*>& daughters, const DecayChannel& channel) const;

  public:
    /**
     *
     * @param motherPdg PID of mother
     */
    Decay(Int_t motherPdg = -1);
    /**
     *
     * @param mother
     * @param daughters daugher vector should have at least 3 elements, NOTE: this vector must have allocated tracks
     * this class doesn't own this vector!
     * @param addToEvent add to mother event daughters (if possible)
     * @return number of daughters
     */
    virtual Int_t DecayParticle(McTrack& mother, std::vector<McTrack*>& daughters, Bool_t addToEvent = kFALSE) const;
    /**
     * switch to breight wigner mass generator
     * @param bw
     */
    void EnableBreightWigner(Bool_t bw = kTRUE) { fBreightWigher = bw; }
    /**
     * set gamma parameter
     * @param gamma
     */
    void SetGamma(Double_t gamma) { fGamma = gamma; };
    /**
     * add decay channel
     * @param decay
     */
    void AddDecayChannel(const DecayChannel& decay) { fDecayChannels.push_back(decay); };
    /**
     * inits decayer
     * @return
     */
    virtual Bool_t Init();
    virtual ~Decay() {};
    ClassDef(Decay, 1)
  };
}  // namespace Hal

#endif /* HAL_FEATURES_PHYS_DECAY_H_ */
