/*
 * FemtoWeightGeneratorKisiel.h
 *
 *  Created on: 30 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_WEIGHTS_HALFEMTOWEIGHTGENERATORKISIEL_H_
#define HALFEMTO_ANALYSIS_FEMTO_WEIGHTS_HALFEMTOWEIGHTGENERATORKISIEL_H_

#include "FemtoWeightGenerator.h"

/**
 * code for FSI based on A. Kisiel TPI software - TODO TEST THIS CODE!
 */
namespace Hal {
  class FemtoWeightGeneratorKisiel : public FemtoWeightGenerator {
    struct _dcomplex {
      long double re;
      long double im;
    };
    typedef struct _dcomplex dcomplex;

    dcomplex fD0s;
    dcomplex fF0s;
    dcomplex fD0t;
    dcomplex fF0t;
    // double gamov[2000];
    long double fPionac;
    long double fOneoveracsq;
    long double fTwopioverac;
    long double fCoulqscpart;
    int fTwospin;
    int fWritegrps;
    Int_t fPcount;
    long double fEuler;
    long double fF0;
    long double fD0;
    Double_t fRStarOutS, fRStarSideS, fRStarLongS, fRStarS;
    dcomplex conj(const dcomplex& arg) const;
    dcomplex mult(const dcomplex& arga, long double argb) const;
    dcomplex mult(const dcomplex& arga, const dcomplex& argb) const;
    long double modl2(dcomplex arg) const { return arg.re * arg.re + arg.im * arg.im; };
    long double modl(dcomplex arg) const { return hypot(arg.re, arg.im); };
    dcomplex invr(dcomplex arg) const {
      dcomplex res;

      res.re = arg.re / modl2(arg);
      res.im = -arg.im / modl2(arg);

      return res;
    };
    /**
     * Calculates the confluent hypergeometric function F from single orientation of cos(theta*) for non-symmetrized wave-function
     * (non-identical particles)
     * @param ffp
     * @param sign
     */
    void GetFFsingle(dcomplex& ffp, int sign = 1) const;
    /**
     * Calculates the confluent hypergeometric function for two orientations of cos(theta*) for symmetrized wave-function
     * (identical particles)
     * @param ffp
     * @param ffm
     */
    void GetFFdouble(dcomplex& ffp, dcomplex& ffm) const;
    /**
     * calculate G~ function
     * @param eta
     * @param rho
     * @param hfun
     * @return
     */
    dcomplex GetG(long double eta, long double rho, long double hfun) const;
    long double Chiim(long double eta) const { return Gamow(1.0 / (eta * fPionac)) / (2.0 * eta); }
    /**
     * calculate H function for strong interaction
     * @param eta
     * @return
     */
    long double GetH(long double eta) const;
    void Getfc(long double kstar, long double eta, long double hfun, dcomplex& fcs, dcomplex& fct) const;  // TODO
    void Bfunpfun(long double eta, long double rho, long double& bret, long double& pret) const;
    double Funeh(double xarg, double rad, double alfa) const { return exp(-sqrt(xarg * xarg / (rad * rad) + alfa * alfa)); }
    double Funex(double xarg, double rad) const { return exp(-xarg / rad); }
    void InitializeGamow();
    void PairKinematics(FemtoPair* pair);
    double GetCoulomb();
    double Gamow(double arg) const;
    double GetQuantumCoulombStrong();
    double GetCoulombStrong();
    double GetQuantumStrong();
    double GetStrong();
    double GetQuantum();
    /**
     * calculate weight for identical bosons
     * @return
     */
    double GetQuantumCoulomb();

  public:
    FemtoWeightGeneratorKisiel();
    FemtoWeightGeneratorKisiel(const FemtoWeightGeneratorKisiel& other) = default;
    Double_t GenerateWeight(FemtoPair* pair);
    virtual FemtoWeightGenerator* MakeCopy() const { return new FemtoWeightGeneratorKisiel(*this); };
    FemtoWeightGeneratorKisiel& operator=(const FemtoWeightGeneratorKisiel& other) = default;
    virtual Bool_t IsPairSupported(Femto::EPairType type) const;
    virtual Package* Report() const;
    virtual ~FemtoWeightGeneratorKisiel();
    ClassDef(FemtoWeightGeneratorKisiel, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_ANALYSIS_FEMTO_WEIGHTS_HALFEMTOWEIGHTGENERATORKISIEL_H_ */
