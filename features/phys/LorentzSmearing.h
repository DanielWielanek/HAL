/*
 * LorentzSmearing.h
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_PHYS_LORENTZSMEARING_H_
#define HAL_FEATURES_PHYS_LORENTZSMEARING_H_

#include <RtypesCore.h>
#include <TH1.h>
#include <vector>

class TLorentzVector;

class TH2D;


namespace Hal {
  /**
   * smear TLorentzVector
   */
  class LorentzSmearing : public TObject {
  protected:
    Int_t fPid     = {81};
    Double_t fMass = {0};
    Bool_t fKeepE  = {kFALSE};

  public:
    /**
     * basic constructor
     * @param pid
     */
    LorentzSmearing(Int_t pid = 211);
    LorentzSmearing(const LorentzSmearing& other);
    /**
     * keep orignal energy of TLorentzVector
     * @param flag
     */
    void KeepEnergy(Bool_t flag) { fKeepE = flag; };
    /**
     * smear momenta
     * @param smeared smeared momenta
     * @param unsmeared
     */
    virtual void Smear(TLorentzVector& smeared, const TLorentzVector& unsmeared) const = 0;
    /**
     * initialize
     * @return
     */
    virtual Bool_t Init();
    Double_t GetMass() const { return fMass; }
    Double_t GetPid() const { return fPid; }
    void SetPid(Int_t pid);
    virtual LorentzSmearing* MakeCopy() const = 0;
    virtual ~LorentzSmearing() {};
    ClassDef(LorentzSmearing, 0)
  };
  /**
   * smear momentum according to TF1
   */
  class LorentzSmearingTF1 : public LorentzSmearing {
    TF1* fFuncP     = {nullptr};
    TF1* fFuncPhi   = {nullptr};
    TF1* fFuncTheta = {nullptr};

  public:
    LorentzSmearingTF1(Int_t pid = 211) : LorentzSmearing(pid) {};
    LorentzSmearingTF1(const LorentzSmearingTF1& other);
    /**
     * set sigma of momentum ,phi ,theta as function of reconstructed momentum
     * @param p
     * @param phi
     * @param theta
     */
    void SetResolution(const TF1& p, const TF1& phi, const TF1& theta);
    virtual void Smear(TLorentzVector& smeared, const TLorentzVector& unsmeared) const;
    virtual Bool_t Init();
    virtual LorentzSmearing* MakeCopy() const { return new LorentzSmearingTF1(*this); };
    virtual ~LorentzSmearingTF1();
    ClassDef(LorentzSmearingTF1, 0)
  };
  /**
   * smear momentum according to TH2
   */
  class LorentzSmearingTH2 : public LorentzSmearing {
    std::vector<TH1D*> fResoP;
    std::vector<TH1D*> fResoPhi;
    std::vector<TH1D*> fResoTheta;
    TH2D* fResoP2d     = {nullptr};
    TH2D* fResoPhi2d   = {nullptr};
    TH2D* fResoTheta2d = {nullptr};

  public:
    LorentzSmearingTH2(Int_t pid = 211) : LorentzSmearing(pid) {};
    LorentzSmearingTH2(const LorentzSmearingTH2& other);
    /**
     * set resolutions NOTE - on X axis there is p_reco, all histograms should have the same ranges and number of bins on X-axis
     * @param p p_reco vs dp (sim-reco)
     * @param phi phi_reco (sim-reco)
     * @param theta theta reco (sim-reco)
     */
    void SetResolution(const TH2D& p, const TH2D& phi, const TH2D& theta);
    virtual void Smear(TLorentzVector& smeared, const TLorentzVector& unsmeared) const;
    virtual Bool_t Init();
    virtual LorentzSmearing* MakeCopy() const { return new LorentzSmearingTH2(*this); };
    virtual ~LorentzSmearingTH2();
    ClassDef(LorentzSmearingTH2, 0)
  };

  class LorentzSmearingPercent : public LorentzSmearing {
    Double_t fPerc = {0};

  public:
    LorentzSmearingPercent(Int_t pid = 211) : LorentzSmearing(pid) {};
    LorentzSmearingPercent(const LorentzSmearingPercent& other);
    /**
     * set resolutions in percent
     */
    void SetResolution(Double_t perc) { fPerc = perc / 100.0; };
    virtual void Smear(TLorentzVector& smeared, const TLorentzVector& unsmeared) const;
    virtual Bool_t Init();
    virtual LorentzSmearing* MakeCopy() const { return new LorentzSmearingPercent(*this); };
    virtual ~LorentzSmearingPercent() {};
    ClassDef(LorentzSmearingPercent, 0)
  };


} /* namespace Hal */

#endif /* HAL_FEATURES_PHYS_LORENTZSMEARING_H_ */
