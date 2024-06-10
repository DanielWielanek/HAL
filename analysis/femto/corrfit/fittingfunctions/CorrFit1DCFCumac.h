/*
 * CorrFIt1DCFCumac.h
 *
 *  Created on: 13 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT1DCFCUMAC_H_
#define HALCORRFIT1DCFCUMAC_H_

#include "CorrFit1DCF.h"

namespace Hal {
  class CorrFit1DCFCumac : public CorrFit1DCF {
  protected:
    const Double_t fPis;
    const Double_t fHc;
    mutable Double_t fZ;
    virtual Double_t Ff1(Double_t x) const;
    virtual Double_t Integr() const;
    Double_t Sign(Double_t a, Double_t b) const;
    /**
     *
     * @param a1 integration limit
     * @param b1 integration limit
     * @param h1 initial step of integration
     * @param reps1 relative precission of int.
     * @param aeps1 absolute prec. of integration
     * @param x argument of integrand
     * @param aih value of integral with step of integration
     * @param aiabs the valuue of integrand for module of the integrand
     * @return value of integral
     */
    Double_t
    Simps(Double_t a1, Double_t b1, Double_t h1, Double_t reps1, Double_t aeps1, Double_t x, Double_t aih, Double_t aiabs) const;
    Double_t F1(Double_t Z) const;
    Double_t F2(Double_t Z) const;
    CorrFit1DCFCumac(Int_t params);

  public:
    CorrFit1DCFCumac();
    virtual ~CorrFit1DCFCumac();
    ClassDef(CorrFit1DCFCumac, 1)
  };

  /**
   * C++ version of R.Lednicky's fortran code rlamlam2n.f
   */

  class CorrFIt1DCFCumacLamLam : public CorrFit1DCFCumac {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFIt1DCFCumacLamLam();
    Double_t GetValue(Double_t x, Double_t* p) { return CalculateCF(&x, p); };
    Int_t ScatteringLengthID() const { return 3; };
    Int_t EffectiveRadiusID() const { return 4; };
    Int_t LambdaPolarizationID() const { return 5; };
    Int_t ResidualAmplitudeID() const { return 6; };
    Int_t ResidualGaussWidhtID() const { return 7; };
    virtual ~CorrFIt1DCFCumacLamLam();
    ClassDef(CorrFIt1DCFCumacLamLam, 1)
  };

  /**
   * C++ version of Lednicky's code rplam1.F
   */
  class CorrFit1DCFCumacPLam : public CorrFit1DCFCumac {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCFCumacPLam();
    Double_t Get(Double_t q, Double_t r);
    Int_t SingletScatteringLengthID() const { return 3; }
    Int_t SingletEffectiveRadiusID() const { return 4; }
    Int_t TripletScatteringLenghtID() const { return 5; };
    Int_t TripletEffectiveRadiusID() const { return 6; };
    Int_t LambdaPolarizationID() const { return 7; };
    virtual ~CorrFit1DCFCumacPLam();
    ClassDef(CorrFit1DCFCumacPLam, 1)
  };

  class CorrFit1DCFCumacK0K0 : public CorrFit1DCFCumac {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCFCumacK0K0();
    Double_t GetValue(Double_t x, Double_t* p) { return CalculateCF(&x, p); };
    Int_t Mf0ID() const { return 3; };
    Int_t Gamma_f0KKID() const { return 4; }
    Int_t Ma0ID() const { return 5; };
    Int_t Gamma_f0pipiID() const { return 6; };
    Int_t Gamma_a0PiEtaID() const { return 7; }
    Int_t Gamma_a0KKID() const { return 8; }
    /**
     * assymetry = (1-e^2)/2 where e is the K0-K0-bar abundance assymetry
     * @return
     */
    Int_t AssymetryID() const { return 9; }

    /**
     *
     * @param opt 0 - 5
     */
    void SetDefParams(Int_t opt);
    void UseLCMS() { fKinematics = Femto::EKinematics::kLCMS; }
    virtual ~CorrFit1DCFCumacK0K0() {};
    ClassDef(CorrFit1DCFCumacK0K0, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT1DCFCUMAC_H_ */
