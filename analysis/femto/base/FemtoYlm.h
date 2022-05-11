/*
 * FemtoYlm.h
 *
 *  Created on: 28 kwi 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *
 *		Derived from AliROOT AliFemtoYlm
 */
#ifndef HALFEMTOYLM_H_
#define HALFEMTOYLM_H_

#include <TObject.h>
#include <complex>

namespace Hal {
  class FemtoYlm : public TObject {
  private:
    static std::complex<double> Ceiphi(double phi);
    static FemtoYlm* fgInstance;
    static double* fgPrefactors;
    static int* fgPrefshift;
    static int* fgPlmshift;
    static std::complex<double>* fgYlms;  //!
  public:
    FemtoYlm();
    virtual ~FemtoYlm();
    static FemtoYlm* Instance();
    FemtoYlm(const FemtoYlm& aYlm);
    FemtoYlm& operator=(const FemtoYlm& aYlm);
    static double Legendre(int ell, int emm, double ctheta);
    static void LegendreUpToYlm(int lmax, double ctheta, double* lbuf);
    static std::complex<double> Ylm(int ell, int m, double theta, double phi);
    static std::complex<double> Ylm(int ell, int m, double x, double y, double z);
    static std::complex<double>* YlmUpToL(int lmax, double x, double y, double z);
    static std::complex<double>* YlmUpToL(int lmax, double ctheta, double phi);
    static double ReYlm(int ell, int m, double theta, double phi);
    static double ReYlm(int ell, int m, double x, double y, double z);
    static double ImYlm(int ell, int m, double theta, double phi);
    static double ImYlm(int ell, int m, double x, double y, double z);
    static void InitializeYlms();
    ClassDef(FemtoYlm, 1)
  };
}  // namespace Hal

#endif /* HALROOT_ANALYSIS_FEMTO_BASE_HALFEMTOYLM_H_ */
