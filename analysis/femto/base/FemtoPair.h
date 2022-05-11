/*
 * FemtoPair.h
 *
 *  Created on: 29-05-2014
 *      Author: wielanek
 */

#ifndef HALFEMTOPAIRSTRUCT_H_
#define HALFEMTOPAIRSTRUCT_H_

#include "FemtoConst.h"

#include <TMath.h>
#include <TObject.h>

namespace Hal {
  class Track;
  class TwoTrack;

  //#define _kinetic_debug
  /**
   * basic  abstract class that represents femtoscopic pair
   */
  class FemtoMiniPair;
  class FemtoMicroPair;
  class FemtoPicoPair;
  class FemtoPair : public TObject {
  protected:
    /**
     * pointer to first track
     */
    Track* fTrack1;
    /**
     * pointer to second track
     */
    Track* fTrack2;
    /**
     * true momenta of first particle
     */
    Double_t fPx1, fPy1, fPz1, fE1;  // true momenta
    /**
     *  true momenta of second particle
     */
    Double_t fPx2, fPy2, fPz2, fE2;  // true momenta
    /**
     *  fake momenta of first particle
     */
    Double_t fpx1, fpy1, fpz1, fe1;  // fake momenta
    /**
     *  farke momenta of second particle
     */
    Double_t fpx2, fpy2, fpz2, fe2;  // fake momenta
    /**
     * freezout coordinates of first particle
     */
    Double_t fX1, fY1, fZ1, fT1;
    /**
     * freezout coordinates of second particle
     */
    Double_t fX2, fY2, fZ2, fT2;
    /**
     * pdg codes of first and second particle
     */
    Int_t fPdg1, fPdg2;
    /** suared masses used for calculation of energy, used if fM>0 otherwise
     * energy from Track is used*/
    Double_t fM12, fM22;
    /**
     * use fake momenta
     */
    Bool_t fUseImgMomenta;
    /**
     * calculated value
     */
    Double_t fX;
    /**calculate value
     *
     */
    Double_t fY;
    /**
     * calculated value
     */
    Double_t fZ;
    /**
     * "total" calculated value
     */
    Double_t fT;
    /**
     * pair weight
     */
    Double_t fWeight;
    enum eWorkingMode { kMC, kComplexRe, kComplexIm, kNoMC };
    eWorkingMode fMode;
    /**
     * if true then absolute values of momentum differences should be used
     */
    Bool_t fUseAbs;
    void SetTrueMomenta1(Double_t px, Double_t py, Double_t pz);
    void SetTrueMomenta2(Double_t px, Double_t py, Double_t pz);
    void SetMomenta1(Double_t px, Double_t py, Double_t pz);
    void SetMomenta2(Double_t px, Double_t py, Double_t pz);

  public:
    /**
     * default constructor
     * @param enable_fake true if user want go use MC momenta in complex events
     */
    FemtoPair(Bool_t enable_fake = kFALSE);
    /**
     * should be used to mark that absolute values from kinematics should be used
     */
    void UseAbs() { fUseAbs = kTRUE; };
    /**
     * copy assignement
     * @param other
     * @return
     */
    FemtoPair& operator=(const FemtoMiniPair& other);
    FemtoPair& operator=(const FemtoMicroPair& other);
    FemtoPair& operator=(const FemtoPicoPair& other);
    inline Bool_t IsAbs() const { return fUseAbs; };
    /**
     * set assued mass of particles
     * @param m1
     * @param m2
     */
    void SetMass(Double_t m1, Double_t m2);
    virtual FemtoPair* MakeCopy() const       = 0;
    virtual Femto::EKinematics GetFrame() const = 0;
    Bool_t Init(Int_t task_id);
    /**
     * build femtoscpic pair fro pair of tracks
     * @param tracks
     */
    inline void Build(TwoTrack* tracks) { BuildMomenta(tracks); }
    virtual ~FemtoPair();
    /**
     *
     * @return true Px of first particle
     */
    inline Double_t TruePx1() const { return fPx1; };
    /**
     *
     * @return true Py of first particle
     */
    inline Double_t TruePy1() const { return fPy1; };
    /**
     *
     * @return true Pz of first particle
     */
    inline Double_t TruePz1() const { return fPz1; };
    /**
     *
     * @return true energy of first particle
     */
    inline Double_t TrueE1() const { return fE1; };
    /**
     *
     * @return true Phi of first particle
     */
    inline Double_t TruePhi1() const { return TMath::ATan2(fPy1, fPx1); }
    /**
     *
     * @return true Px of second particle
     */
    inline Double_t TruePx2() const { return fPx2; };
    /**
     *
     * @return true Py of second particle
     */
    inline Double_t TruePy2() const { return fPy2; };
    /**
     *
     * @return true Pz of second particle
     */
    inline Double_t TruePz2() const { return fPz2; };
    /**
     *
     * @return true energy of second particle
     */
    inline Double_t TrueE2() const { return fE2; };
    /**
     *
     * @return true Phi of second particle
     */
    inline Double_t TruePhi2() const { return TMath::ATan2(fPy2, fPx2); };
    /**
     *
     * @return  X-freezout of first particle
     */
    inline Double_t GetX1() const { return fX1; };
    /**
     *
     * @return  Y-freezout of first particle
     */
    inline Double_t GetY1() const { return fY1; };
    /**
     *
     * @return  Z-freezout of first particle
     */
    inline Double_t GetZ1() const { return fZ1; };
    /**
     *
     * @return  T-freezout of first particle
     */
    inline Double_t GetT1() const { return fT1; };
    /**
     *
     * @return  X-freezout of second particle
     */
    inline Double_t GetX2() const { return fX2; };
    /**
     *
     * @return  Y-freezout of second particle
     */
    inline Double_t GetY2() const { return fY2; };
    /**
     *
     * @return  Z-freezout of second particle
     */
    inline Double_t GetZ2() const { return fZ2; };
    /**
     *
     * @return  T-freezout of second particle
     */
    inline Double_t GetT2() const { return fT2; };
    /**
     *
     * @return fake Px of first particle
     */
    inline Double_t GetPx1() const { return fpx1; };
    /**
     *
     * @return fake Py of first particle
     */
    inline Double_t GetPy1() const { return fpy1; };
    /**
     *
     * @return fake Pz of first particle
     */
    inline Double_t GetPz1() const { return fpz1; };
    /**
     *
     * @return fake energy of first particle
     */
    inline Double_t GetE1() const { return fe1; };
    /**
     *
     * @return fake phi of first particle
     */
    inline Double_t GetPhi1() const { return TMath::ATan2(fpy1, fpx1); };
    /**
     *
     * @return fake Px of second particle
     */
    inline Double_t GetPx2() const { return fpx2; };
    /**
     *
     * @return fake Py of second particle
     */
    inline Double_t GetPy2() const { return fpy2; };
    /**
     *
     * @return fake Pz of second particle
     */
    inline Double_t GetPz2() const { return fpz2; };
    /**
     *
     * @return fake energy of second particle
     */
    inline Double_t GetE2() const { return fe2; };
    /**
     *
     * @return fake phi of second particle
     */
    inline Double_t GetPhi2() const { return TMath::ATan2(fpy2, fpx2); };
    /**
     *
     * @return pair weight
     */
    inline Double_t GetWeight() const { return fWeight; };
    /**
     *
     * @return mass of first particle
     */
    inline Double_t GetM1() const { return TMath::Sqrt(fM12); };
    /**
     *
     * @return mass of second particle
     */
    inline Double_t GetM2() const { return TMath::Sqrt(fM22); };
    /**
     *
     * @return first particle's PDG
     */
    inline Int_t GetPdg1() const { return fPdg1; };
    /**
     *
     * @return second particle's PDG
     */
    inline Int_t GetPdg2() const { return fPdg2; };
    /**
     * get out /X component of calculate values
     * @return X component
     */
    inline Double_t GetX() const { return fX; };
    /**
     * get out /Y component of calculate values
     * @return Y component
     */
    inline Double_t GetY() const { return fY; };
    /**
     * get out /Z component of calculate values
     * @return Z component
     */
    inline Double_t GetZ() const { return fZ; };
    /**
     * get out /T component of calculate values
     * @return T component
     */
    inline Double_t GetT() const { return fT; };
    /**
     *
     * @return kt
     */
    Double_t GetKt() const;
    /**
     *
     * @return true kt
     */
    Double_t GetTrueKt() const;
    /**
     *
     * @return pointer to first track
     */
    inline Track* GetTrack1() const { return fTrack1; };
    /**
     *
     * @return pointer to second track
     */
    inline Track* GetTrack2() const { return fTrack2; };
    /**
     * set X-freezout for first particle
     * @param val X-freezout
     */
    inline void SetX1(Double_t val) { fX1 = val; };
    /**
     * set Y-freezout for first particle
     * @param val Y-freezout
     */
    inline void SetY1(Double_t val) { fY1 = val; };
    /**
     * set Z-freezout for first particle
     * @param val XZ-freezout
     */
    inline void SetZ1(Double_t val) { fZ1 = val; };
    /**
     * set T-freezout for first particle
     * @param val T-freezout
     */
    inline void SetT1(Double_t val) { fT1 = val; };
    /**
     * set X-freezout for second particle
     * @param val X-freezout
     */
    inline void SetX2(Double_t val) { fX2 = val; };
    /**
     * set Y-freezout for second particle
     * @param val Y-freezout
     */
    inline void SetY2(Double_t val) { fY2 = val; };
    /**
     * set Z-freezout for second particle
     * @param val Z-freezout
     */
    inline void SetZ2(Double_t val) { fZ2 = val; };
    /**
     * set T-freezout for second particle
     * @param val T-freezout
     */
    inline void SetT2(Double_t val) { fT2 = val; };
    /**
     * set PDG for first particle
     * @param val PDG code
     */
    inline void SetPdg1(Int_t val) { fPdg1 = val; };
    /**
     * set PDG code for second particle
     * @param val PDG code
     */
    inline void SetPdg2(Int_t val) { fPdg2 = val; };
    /**
     * set pair weight
     * @param weight
     */
    inline void SetWeight(Double_t weight) { fWeight = weight; };
    /**
     * set true momenta for first particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetTrueMomenta1(Double_t px, Double_t py, Double_t pz, Double_t e);
    /**
     * set true momenta for second particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetTrueMomenta2(Double_t px, Double_t py, Double_t pz, Double_t e);
    /**
     * print info about this pair
     */
    void PrintInfo() const;
    /**
     * set fake momenta for first particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetMomenta1(Double_t px, Double_t py, Double_t pz, Double_t e);
    /**
     * set fake momenta for second particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetMomenta2(Double_t px, Double_t py, Double_t pz, Double_t e);
    /**
     * set freezout coordinates of first particle
     * @param x
     * @param y
     * @param z
     * @param t
     */
    void SetFreezoutCoord1(Double_t x, Double_t y, Double_t z, Double_t t);
    /**
     * set freezout coordinates of second particle
     * @param x
     * @param y
     * @param z
     * @param t
     */
    void SetFreezoutCoord2(Double_t x, Double_t y, Double_t z, Double_t t);
    /**
     *
     * @return true pair phi
     */
    inline Double_t GetPhi() const { return TMath::ATan2(fPy1 + fPy2, fPx1 + fPx2); };
    /**
     *
     * @return true if fake momenta are copied
     */
    Bool_t IsUseImgMomenta() const { return fUseImgMomenta; }
    /**
     * calculate kinematic variables
     */
    virtual void Compute() = 0;
    /**
     * calculate kinematics for given "rotated "pair
     */
    virtual void Compute_Rotated() = 0;
    /**
     * calculate kinematics for given "hemisphere"  pair
     */
    virtual void Compute_Hemisphere() = 0;
    /**
     * calculate kinematics for given "charged" pair
     */
    virtual void Compute_Charged() { Compute(); };
    /**
     * calculate kinematics for given "mixed" pair
     */
    virtual void Compute_Mixed() { Compute(); };
    /**
     * copy momenta nad freezout cooridnates from pair (PDG is not copied)
     * @param tracks pair to copy data
     */
    void BuildMomenta(TwoTrack* tracks);
    virtual TString GetTags() const = 0;
    ClassDef(FemtoPair, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOPAIR_H_ */
