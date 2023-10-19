/*
 * FemtoYlmMathMath.cxx
 *
 *  Created on: 25 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoYlmMath.h"


#include <TMath.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace Hal {

  FemtoYlmMath::FemtoYlmMath() { InitializeYlms(); }

  FemtoYlmMath::~FemtoYlmMath() {}

  FemtoYlmMath::FemtoYlmMath(const FemtoYlmMath& aYlm) : TObject(aYlm) { InitializeYlms(); }

  FemtoYlmMath& FemtoYlmMath::operator=(const FemtoYlmMath& aYlm) {
    if (this == &aYlm) return *this;

    InitializeYlms();

    return *this;
  }

  std::complex<double> FemtoYlmMath::Ceiphi(double phi) const { return std::complex<double>(cos(phi), sin(phi)); }

  double FemtoYlmMath::Legendre(int ell, int em, double ctheta) const {
    // Calculate a single Legendre value
    // *** Warning - NOT optimal - calculated all Plms up to L !!!
    double lbuf[36];
    FemtoYlmMath::LegendreUpToYlm(ell, ctheta, lbuf);

    return lbuf[fPlmshift[ell] - abs(em)];
  }
  double FemtoYlmMath::DeltaJ(double aJot1, double aJot2, double aJot) const {
    if ((aJot1 + aJot2 - aJot) < 0) { return 0; }
    if ((aJot1 - aJot2 + aJot) < 0) { return 0; }
    if ((-aJot1 + aJot2 + aJot) < 0) { return 0; }
    if ((aJot1 + aJot2 + aJot + 1) < 0) { return 0; }
    double res = TMath::Sqrt(1.0 * fFactorials[lrint(aJot1 + aJot2 - aJot)] * fFactorials[lrint(aJot1 - aJot2 + aJot)]
                             * fFactorials[lrint(-aJot1 + aJot2 + aJot)] / fFactorials[lrint(aJot1 + aJot2 + aJot + 1)]);
    return res;
  }

  double FemtoYlmMath::ClebschGordan(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const {
    int mint, maxt;
    double cgc = 0.0;
    int titer;
    double coef;

    maxt = lrint(aJot1 + aJot2 - aJot);
    mint = 0;
    if (lrint(aJot1 - aEm1) < maxt) maxt = lrint(aJot1 - aEm1);
    if (lrint(aJot2 + aEm2) < maxt) maxt = lrint(aJot2 + aEm2);
    if (lrint(-(aJot - aJot2 + aEm1)) > mint) mint = lrint(-(aJot - aJot2 + aEm1));
    if (lrint(-(aJot - aJot1 - aEm2)) > mint) mint = lrint(-(aJot - aJot1 - aEm2));

    for (titer = mint; titer <= maxt; titer++) {
      coef = TMath::Power(-1, titer);
      coef *= TMath::Sqrt((2 * aJot + 1) * fFactorials[lrint(aJot1 + aEm1)] * fFactorials[lrint(aJot1 - aEm1)]
                          * fFactorials[lrint(aJot2 + aEm2)] * fFactorials[lrint(aJot2 - aEm2)] * fFactorials[lrint(aJot + aEm)]
                          * fFactorials[lrint(aJot - aEm)]);
      coef /= (fFactorials[titer] * fFactorials[lrint(aJot1 + aJot2 - aJot - titer)] * fFactorials[lrint(aJot1 - aEm1 - titer)]
               * fFactorials[lrint(aJot2 + aEm2 - titer)] * fFactorials[lrint(aJot - aJot2 + aEm1 + titer)]
               * fFactorials[lrint(aJot - aJot1 - aEm2 + titer)]);

      cgc += coef;
    }

    cgc *= DeltaJ(aJot1, aJot2, aJot);

    return cgc;
  }

  double FemtoYlmMath::WignerSymbol(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const {
    if (lrint(aEm1 + aEm2 + aEm) != 0.0) return 0.0;
    double cge = ClebschGordan(aJot1, aEm1, aJot2, aEm2, aJot, -aEm);
    if (lrint(abs(aJot1 - aJot2 - aEm)) % 2) cge *= -1.0;
    cge /= sqrt(2 * aJot + 1);

    if (cge == -0.0) cge = 0.0;

    return cge;
  }

  std::complex<double> FemtoYlmMath::Ylm(int ell, int m, double theta, double phi) const {
    // Calculate Ylm spherical input
    double ctheta;
    std::complex<double> answer;
    std::complex<double> ci(0.0, 1.0);
    ctheta = cos(theta);
    answer = (fPrefactors[fPrefshift[ell] + m] * Legendre(ell, m, ctheta)) * Ceiphi(m * phi);

    return answer;
  }

  std::complex<double> FemtoYlmMath::Ylm(int ell, int m, double x, double y, double z) const {
    // Calculate Ylm cartesian input
    std::complex<double> answer;
    double ctheta, phi;
    double r = sqrt(x * x + y * y + z * z);
    if (r < 1e-10 || fabs(z) < 1e-10)
      ctheta = 0.0;
    else
      ctheta = z / r;
    phi    = atan2(y, x);
    answer = (fPrefactors[fPrefshift[ell] + m] * Legendre(ell, m, ctheta)) * Ceiphi(m * phi);

    return answer;
  }

  double FemtoYlmMath::ReYlm(int ell, int m, double theta, double phi) const {
    return real(FemtoYlmMath::Ylm(ell, m, theta, phi));
  }

  double FemtoYlmMath::ImYlm(int ell, int m, double theta, double phi) const {
    return imag(FemtoYlmMath::Ylm(ell, m, theta, phi));
  }

  double FemtoYlmMath::ReYlm(int ell, int m, double x, double y, double z) const {
    return real(FemtoYlmMath::Ylm(ell, m, x, y, z));
  }

  double FemtoYlmMath::ImYlm(int ell, int m, double x, double y, double z) const {
    return imag(FemtoYlmMath::Ylm(ell, m, x, y, z));
  }

  void FemtoYlmMath::InitializeYlms() {
    // Calculate prefactors for fast Ylm calculation

    double oneoversqrtpi = 1.0 / TMath::Sqrt(TMath::Pi());


    // l=0 prefactors
    fPrefactors[0] = 0.5 * oneoversqrtpi;

    // l=1 prefactors
    fPrefactors[1] = 0.5 * sqrt(3.0 / 2.0) * oneoversqrtpi;
    fPrefactors[2] = 0.5 * sqrt(3.0) * oneoversqrtpi;
    fPrefactors[3] = -fPrefactors[1];

    // l=2 prefactors
    fPrefactors[4] = 0.25 * sqrt(15.0 / 2.0) * oneoversqrtpi;
    fPrefactors[5] = 0.5 * sqrt(15.0 / 2.0) * oneoversqrtpi;
    fPrefactors[6] = 0.25 * sqrt(5.0) * oneoversqrtpi;
    fPrefactors[7] = -fPrefactors[5];
    fPrefactors[8] = fPrefactors[4];

    // l=3 prefactors
    fPrefactors[9]  = 0.125 * sqrt(35.0) * oneoversqrtpi;
    fPrefactors[10] = 0.25 * sqrt(105.0 / 2.0) * oneoversqrtpi;
    fPrefactors[11] = 0.125 * sqrt(21.0) * oneoversqrtpi;
    fPrefactors[12] = 0.25 * sqrt(7.0) * oneoversqrtpi;
    fPrefactors[13] = -fPrefactors[11];
    fPrefactors[14] = fPrefactors[10];
    fPrefactors[15] = -fPrefactors[9];

    // l=4 prefactors
    fPrefactors[16] = 3.0 / 16.0 * sqrt(35.0 / 2.0) * oneoversqrtpi;
    fPrefactors[17] = 3.0 / 8.0 * sqrt(35.0) * oneoversqrtpi;
    fPrefactors[18] = 3.0 / 8.0 * sqrt(5.0 / 2.0) * oneoversqrtpi;
    fPrefactors[19] = 3.0 / 8.0 * sqrt(5.0) * oneoversqrtpi;
    fPrefactors[20] = 3.0 / 16.0 * oneoversqrtpi;
    fPrefactors[21] = -fPrefactors[19];
    fPrefactors[22] = fPrefactors[18];
    fPrefactors[23] = -fPrefactors[17];
    fPrefactors[24] = fPrefactors[16];

    // l=5 prefactors
    fPrefactors[25] = 3.0 / 32.0 * sqrt(77.0) * oneoversqrtpi;
    fPrefactors[26] = 3.0 / 16.0 * sqrt(385.0 / 2.0) * oneoversqrtpi;
    fPrefactors[27] = 1.0 / 32.0 * sqrt(385.0) * oneoversqrtpi;
    fPrefactors[28] = 1.0 / 8.0 * sqrt(1155.0 / 2.0) * oneoversqrtpi;
    fPrefactors[29] = 1.0 / 16.0 * sqrt(165.0 / 2.0) * oneoversqrtpi;
    fPrefactors[30] = 1.0 / 16.0 * sqrt(11.0) * oneoversqrtpi;
    fPrefactors[31] = -fPrefactors[29];
    fPrefactors[32] = fPrefactors[28];
    fPrefactors[33] = -fPrefactors[27];
    fPrefactors[34] = fPrefactors[26];
    fPrefactors[35] = -fPrefactors[25];

    fPrefshift[0] = 0;
    fPrefshift[1] = 2;
    fPrefshift[2] = 6;
    fPrefshift[3] = 12;
    fPrefshift[4] = 20;
    fPrefshift[5] = 30;

    fPlmshift[0] = 0;
    fPlmshift[1] = 2;
    fPlmshift[2] = 5;
    fPlmshift[3] = 9;
    fPlmshift[4] = 14;
    fPlmshift[5] = 20;
    fYlms        = new std::complex<double>[36];

    Double_t fac   = 1;
    fFactorials[0] = 1;
    for (int iter = 1; iter < 24; iter++) {
      fac *= iter;
      fFactorials[iter] = fac;
    }
  }

  void FemtoYlmMath::LegendreUpToYlm(int lmax, double ctheta, double* lbuf) const {
    // Calculate a set of legendre polynomials up to a given l
    // with spherical input
    double sins[6];
    double coss[6];
    sins[0] = 0.0;
    coss[0] = 1.0;
    sins[1] = sqrt(1 - ctheta * ctheta);
    coss[1] = ctheta;
    for (int iter = 2; iter < 6; iter++) {
      sins[iter] = sins[iter - 1] * sins[1];
      coss[iter] = coss[iter - 1] * coss[1];
    }

    // Legendre polynomials l=0
    lbuf[0] = 1.0;

    // Legendre polynomials l=1
    if (lmax > 0) {
      lbuf[1] = sins[1];
      lbuf[2] = coss[1];
    }

    // Legendre polynomials l=2
    if (lmax > 1) {
      lbuf[3] = sins[2];
      lbuf[4] = sins[1] * coss[1];
      lbuf[5] = 3 * coss[2] - 1;
    }

    // Legendre polynomials l=3
    if (lmax > 2) {
      lbuf[6] = sins[3];
      lbuf[7] = sins[2] * coss[1];
      lbuf[8] = (5 * coss[2] - 1) * sins[1];
      lbuf[9] = 5 * coss[3] - 3 * coss[1];
    }

    // Legendre polynomials l=4
    if (lmax > 3) {
      lbuf[10] = sins[4];
      lbuf[11] = sins[3] * coss[1];
      lbuf[12] = (7 * coss[2] - 1) * sins[2];
      lbuf[13] = (7 * coss[3] - 3 * coss[1]) * sins[1];
      lbuf[14] = 35 * coss[4] - 30 * coss[2] + 3;
    }

    // Legendre polynomials l=5
    if (lmax > 4) {
      lbuf[15] = sins[5];
      lbuf[16] = sins[4] * coss[1];
      lbuf[17] = (9 * coss[2] - 1) * sins[3];
      lbuf[18] = (3 * coss[3] - 1 * coss[1]) * sins[2];
      lbuf[19] = (21 * coss[4] - 14 * coss[2] + 1) * sins[1];
      lbuf[20] = 63 * coss[5] - 70 * coss[3] + 15 * coss[1];
    }
  }

  std::complex<double>* FemtoYlmMath::YlmUpToL(int lmax, double x, double y, double z) {
    // Calculate a set of Ylms up to a given l
    // with cartesian input
    double ctheta, phi;

    double r = sqrt(x * x + y * y + z * z);
    if (r < 1e-10 || fabs(z) < 1e-10)
      ctheta = 0.0;
    else
      ctheta = z / r;
    phi = atan2(y, x);

    return YlmUpToL(lmax, ctheta, phi);
  }

  std::complex<double>* FemtoYlmMath::YlmUpToL(int lmax, double ctheta, double phi) {
    // Calculate a set of Ylms up to a given l
    // with spherical input
    int lcur = 0;
    double lpol;

    double coss[6];
    double sins[6];

    double lbuf[21];  // was 36 but not used
    LegendreUpToYlm(lmax, ctheta, lbuf);

    for (int iter = 1; iter <= lmax; iter++) {
      coss[iter - 1] = cos(iter * phi);
      sins[iter - 1] = sin(iter * phi);
    }
    fYlms[lcur++] = fPrefactors[0] * lbuf[0] * std::complex<double>(1, 0);

    for (int il = 1; il <= lmax; il++) {
      // First im = 0
      fYlms[lcur + il] = fPrefactors[fPrefshift[il]] * lbuf[fPlmshift[il]] * std::complex<double>(1.0, 0.0);
      // Im != 0
      for (int im = 1; im <= il; im++) {
        lpol                  = lbuf[fPlmshift[il] - im];
        fYlms[lcur + il - im] = fPrefactors[fPrefshift[il] - im] * lpol * std::complex<double>(coss[im - 1], -sins[im - 1]);
        fYlms[lcur + il + im] = fPrefactors[fPrefshift[il] + im] * lpol * std::complex<double>(coss[im - 1], sins[im - 1]);
      }
      lcur += 2 * il + 1;
    }
    return fYlms;
  }
}  // namespace Hal
