/*
 * FemtoYlmMath.h
 *
 *  Created on: 25 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOYLMMATH_H_
#define HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOYLMMATH_H_

#include <complex>

#include <TObject.h>

namespace Hal {

  class FemtoYlmMath : public TObject {
  private:
    std::complex<double> Ceiphi(double phi) const;
    double fPrefactors[36];
    int fPrefshift[6];
    int fPlmshift[6];
    double fFactorials[24];
    mutable std::complex<double>* fYlms;  //!
  public:
    FemtoYlmMath();
    virtual ~FemtoYlmMath();
    FemtoYlmMath(const FemtoYlmMath& aYlm);
    FemtoYlmMath& operator=(const FemtoYlmMath& aYlm);
    double Legendre(int ell, int emm, double ctheta) const;
    void LegendreUpToYlm(int lmax, double ctheta, double* lbuf) const;
    std::complex<double> Ylm(int ell, int m, double theta, double phi) const;
    std::complex<double> Ylm(int ell, int m, double x, double y, double z) const;
    std::complex<double>* YlmUpToL(int lmax, double x, double y, double z);
    std::complex<double>* YlmUpToL(int lmax, double ctheta, double phi);
    double ReYlm(int ell, int m, double theta, double phi) const;
    double ReYlm(int ell, int m, double x, double y, double z) const;
    double ImYlm(int ell, int m, double theta, double phi) const;
    double ImYlm(int ell, int m, double x, double y, double z) const;
    double DeltaJ(double aJot1, double aJot2, double aJot) const;
    double ClebschGordan(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const;
    double WignerSymbol(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const;
    void InitializeYlms();
    ClassDef(FemtoYlmMath, 1)
  };
};  // namespace Hal


#endif /* HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOYLMMATH_H_ */
