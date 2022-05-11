/*
 * FemtoMiniPair.h
 *
 *  Created on: 9 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOMINIPAIR_H_
#define HALFEMTOMINIPAIR_H_

#include "FemtoPair.h"

#include <TMath.h>

namespace Hal {
  class FemtoPicoPair : public TObject {
  protected:
    /**
     * true momenta of first particle
     */
    Float_t fPx1, fPy1, fPz1, fE1;  // true momenta
    /**
     *  true momenta of second particle
     */
    Float_t fPx2, fPy2, fPz2, fE2;  // true momenta
  public:
    FemtoPicoPair();
    /**
     * assignement operator
     * @param other
     * @return
     */
    FemtoPicoPair& operator=(const FemtoPicoPair& other);
    /**
     *
     * @return true Px of first particle
     */
    inline Float_t TruePx1() const { return fPx1; };
    /**
     *
     * @return true Py of first particle
     */
    inline Float_t TruePy1() const { return fPy1; };
    /**
     *
     * @return true Pz of first particle
     */
    inline Float_t TruePz1() const { return fPz1; };
    /**
     *
     * @return true energy of first particle
     */
    inline Float_t TrueE1() const { return fE1; };
    /**
     *
     * @return true Phi of first particle
     */
    inline Float_t TruePhi1() const { return TMath::ATan2(fPy1, fPx1); }
    /**
     *
     * @return true Px of second particle
     */
    inline Float_t TruePx2() const { return fPx2; };
    /**
     *
     * @return true Py of second particle
     */
    inline Float_t TruePy2() const { return fPy2; };
    /**
     *
     * @return true Pz of second particle
     */
    inline Float_t TruePz2() const { return fPz2; };
    /**
     *
     * @return true energy of second particle
     */
    inline Float_t TrueE2() const { return fE2; };
    /**
     *
     * @return true Phi of second particle
     */
    inline Float_t TruePhi2() const { return TMath::ATan2(fPy2, fPx2); };
    /**
     *
     * @return true pair phi
     */
    inline Float_t GetPhi() const { return TMath::ATan2(fPy1 + fPy2, fPx1 + fPx2); };
    virtual ~FemtoPicoPair();
    ClassDef(FemtoPicoPair, 1)
  };

  class FemtoMicroPair : public FemtoPicoPair {
  protected:
    /**
     *  fake momenta of first particle
     */
    Float_t fpx1, fpy1, fpz1, fe1;  // fake momenta
    /**
     *  farke momenta of second particle
     */
    Float_t fpx2, fpy2, fpz2, fe2;  // fake momenta
    /**
     * pdg codes of first and second particle
     */
    Int_t fPdg1, fPdg2;

  public:
    FemtoMicroPair();
    /**
     *
     * @return fake Px of first particle
     */
    inline Float_t GetPx1() const { return fpx1; };
    /**
     *
     * @return fake Py of first particle
     */
    inline Float_t GetPy1() const { return fpy1; };
    /**
     *
     * @return fake Pz of first particle
     */
    inline Float_t GetPz1() const { return fpz1; };
    /**
     *
     * @return fake energy of first particle
     */
    inline Float_t GetE1() const { return fe1; };
    /**
     *
     * @return fake phi of first particle
     */
    inline Float_t GetPhi1() const { return TMath::ATan2(fpy1, fpx1); };
    /**
     *
     * @return fake Px of second particle
     */
    inline Float_t GetPx2() const { return fpx2; };
    /**
     *
     * @return fake Py of second particle
     */
    inline Float_t GetPy2() const { return fpy2; };
    /**
     *
     * @return fake Pz of second particle
     */
    inline Float_t GetPz2() const { return fpz2; };
    /**
     *
     * @return fake energy of second particle
     */
    inline Float_t GetE2() const { return fe2; };
    /**
     *
     * @return fake phi of second particle
     */
    inline Float_t GetPhi2() const { return TMath::ATan2(fpy2, fpx2); };
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
     * set true momenta for first particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetTrueMomenta1(Float_t px, Float_t py, Float_t pz, Float_t e);
    /**
     * set true momenta for second particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetTrueMomenta2(Float_t px, Float_t py, Float_t pz, Float_t e);

    /**
     * set fake momenta for first particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetMomenta1(Float_t px, Float_t py, Float_t pz, Float_t e);
    /**
     * set fake momenta for second particle
     * @param px Px
     * @param py Py
     * @param pz Pz
     * @param e E
     */
    void SetMomenta2(Float_t px, Float_t py, Float_t pz, Float_t e);
    FemtoMicroPair& operator=(const FemtoPair& other);
    virtual ~FemtoMicroPair();
    ClassDef(FemtoMicroPair, 1)
  };

  /**
   * small class for storing pairs in TTree
   */
  class FemtoMiniPair : public FemtoMicroPair {
    /**
     * freezout coordinates of first particle
     */
    Float_t fX1, fY1, fZ1, fT1;
    /**
     * freezout coordinates of second particle
     */
    Float_t fX2, fY2, fZ2, fT2;

  public:
    FemtoMiniPair();
    /**
     * assignement operator
     * @param other
     * @return
     */
    FemtoMiniPair& operator=(const FemtoPair& other);
    /**
     * print info about this pair
     */
    void PrintInfo() const;
    virtual ~FemtoMiniPair();
    /**
     *
     * @return  X-freezout of first particle
     */
    inline Float_t GetX1() const { return fX1; };
    /**
     *
     * @return  Y-freezout of first particle
     */
    inline Float_t GetY1() const { return fY1; };
    /**
     *
     * @return  Z-freezout of first particle
     */
    inline Float_t GetZ1() const { return fZ1; };
    /**
     *
     * @return  T-freezout of first particle
     */
    inline Float_t GetT1() const { return fT1; };
    /**
     *
     * @return  X-freezout of second particle
     */
    inline Float_t GetX2() const { return fX2; };
    /**
     *
     * @return  Y-freezout of second particle
     */
    inline Float_t GetY2() const { return fY2; };
    /**
     *
     * @return  Z-freezout of second particle
     */
    inline Float_t GetZ2() const { return fZ2; };
    /**
     *
     * @return  T-freezout of second particle
     */
    inline Float_t GetT2() const { return fT2; };

    /**
     * set X-freezout for first particle
     * @param val X-freezout
     */
    inline void SetX1(Float_t val) { fX1 = val; };
    /**
     * set Y-freezout for first particle
     * @param val Y-freezout
     */
    inline void SetY1(Float_t val) { fY1 = val; };
    /**
     * set Z-freezout for first particle
     * @param val XZ-freezout
     */
    inline void SetZ1(Float_t val) { fZ1 = val; };
    /**
     * set T-freezout for first particle
     * @param val T-freezout
     */
    inline void SetT1(Float_t val) { fT1 = val; };
    /**
     * set X-freezout for second particle
     * @param val X-freezout
     */
    inline void SetX2(Float_t val) { fX2 = val; };
    /**
     * set Y-freezout for second particle
     * @param val Y-freezout
     */
    inline void SetY2(Float_t val) { fY2 = val; };
    /**
     * set Z-freezout for second particle
     * @param val Z-freezout
     */
    inline void SetZ2(Float_t val) { fZ2 = val; };
    /**
     * set T-freezout for second particle
     * @param val T-freezout
     */
    inline void SetT2(Float_t val) { fT2 = val; };
    ClassDef(FemtoMiniPair, 1);
  };
}  // namespace Hal
#endif /* HALFEMTOMINIPAIR_H_ */
