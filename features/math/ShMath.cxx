/*
 * SHMath.cxx
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#include "ShMath.h"

#include <RtypesCore.h>
#include <TString.h>
#include <iostream>

#include "Cout.h"
#include "StdTypes.h"

namespace Hal {

  namespace Sh {

    YlmMath::YlmMath() { InitializeYlms(); }

    YlmMath::~YlmMath() {
      if (fYlms) delete fYlms;
    }

    YlmMath::YlmMath(const YlmMath& aYlm) : TObject(aYlm) { InitializeYlms(); }

    YlmMath& YlmMath::operator=(const YlmMath& aYlm) {
      if (this == &aYlm) return *this;
      InitializeYlms();
      return *this;
    }

    std::complex<double> YlmMath::Ceiphi(double phi) const { return std::complex<double>(cos(phi), sin(phi)); }

    double YlmMath::Legendre(int ell, int em, double ctheta) const {
      // Calculate a single Legendre value
      // *** Warning - NOT optimal - calculated all Plms up to L !!!
      double lbuf[66];
      YlmMath::LegendreUpToYlm(ell, ctheta, lbuf);

      return lbuf[fPlmshift[ell] - abs(em)];
    }
    double YlmMath::DeltaJ(double aJot1, double aJot2, double aJot) const {
      if ((aJot1 + aJot2 - aJot) < 0) { return 0; }
      if ((aJot1 - aJot2 + aJot) < 0) { return 0; }
      if ((-aJot1 + aJot2 + aJot) < 0) { return 0; }
      if ((aJot1 + aJot2 + aJot + 1) < 0) { return 0; }
      double res = TMath::Sqrt(1.0 * fFactorials[lrint(aJot1 + aJot2 - aJot)] * fFactorials[lrint(aJot1 - aJot2 + aJot)]
                               * fFactorials[lrint(-aJot1 + aJot2 + aJot)] / fFactorials[lrint(aJot1 + aJot2 + aJot + 1)]);
      return res;
    }

    double YlmMath::ClebschGordan(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const {
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

    double YlmMath::WignerSymbol(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const {
      if (lrint(aEm1 + aEm2 + aEm) != 0.0) return 0.0;
      double cge = ClebschGordan(aJot1, aEm1, aJot2, aEm2, aJot, -aEm);
      if (lrint(abs(aJot1 - aJot2 - aEm)) % 2) cge *= -1.0;
      cge /= sqrt(2 * aJot + 1);

      if (cge == -0.0) cge = 0.0;

      return cge;
    }

    std::complex<double> YlmMath::Ylm(int ell, int m, double theta, double phi) const {
      // Calculate Ylm spherical input
      double ctheta;
      std::complex<double> answer;
      std::complex<double> ci(0.0, 1.0);
      ctheta = cos(theta);
      answer = (fPrefactors[fPrefshift[ell] + m] * Legendre(ell, m, ctheta)) * Ceiphi(m * phi);

      return answer;
    }

    std::complex<double> YlmMath::Ylm(int ell, int m, double x, double y, double z) const {
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

    double YlmMath::ReYlm(int ell, int m, double theta, double phi) const { return real(Ylm(ell, m, theta, phi)); }

    double YlmMath::ImYlm(int ell, int m, double theta, double phi) const { return imag(Ylm(ell, m, theta, phi)); }

    double YlmMath::ReYlm(int ell, int m, double x, double y, double z) const { return real(Ylm(ell, m, x, y, z)); }

    double YlmMath::ImYlm(int ell, int m, double x, double y, double z) const { return imag(Ylm(ell, m, x, y, z)); }

    void YlmMath::InitializeYlms() {
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
      fPrefactors[30] = 1.0 / 16.0 * sqrt(11.0) * oneoversqrtpi;  //
      fPrefactors[31] = -fPrefactors[29];
      fPrefactors[32] = fPrefactors[28];
      fPrefactors[33] = -fPrefactors[27];
      fPrefactors[34] = fPrefactors[26];
      fPrefactors[35] = -fPrefactors[25];


      auto prefactor = [](double a, double b, double c, double d = 1) { return a / b * TMath::Sqrt(c / d / TMath::Pi()); };

      // l=6 prefactors (m = -6 ... +6) --- GPT + WIki
      fPrefactors[36] = prefactor(1, 64, 3003, 1);
      fPrefactors[37] = prefactor(3, 32, 1001, 1);
      fPrefactors[38] = prefactor(3, 32, 91, 2);
      fPrefactors[39] = prefactor(1, 32, 1365, 1);
      fPrefactors[40] = prefactor(1, 64, 1365, 1);
      fPrefactors[41] = prefactor(1, 16, 273, 2);
      fPrefactors[42] = prefactor(1, 32, 13, 1);  ///
      fPrefactors[43] = prefactor(-1, 16, 273, 2);
      fPrefactors[44] = prefactor(1, 64, 1365, 1);
      fPrefactors[45] = prefactor(-1, 32, 1365, 1);
      fPrefactors[46] = prefactor(3, 32, 91, 2);
      fPrefactors[47] = prefactor(-3, 32, 1001, 1);
      fPrefactors[48] = prefactor(1, 64, 3003, 1);

      // l=7 prefactors (m = -7 ... +7)
      fPrefactors[49] = prefactor(3, 64, 715, 2);
      fPrefactors[50] = prefactor(3, 64, 5005, 1);
      fPrefactors[51] = prefactor(3, 64, 385, 2);
      fPrefactors[52] = prefactor(3, 32, 385, 2);
      fPrefactors[53] = prefactor(3, 64, 35, 2);
      fPrefactors[54] = prefactor(3, 64, 35, 1);
      fPrefactors[55] = prefactor(1, 64, 105, 2);
      fPrefactors[56] = prefactor(1, 32, 15, 1);  ///
      fPrefactors[57] = prefactor(-1, 64, 105, 2);
      fPrefactors[58] = prefactor(3, 64, 35, 1);
      fPrefactors[59] = prefactor(-3, 64, 35, 2);
      fPrefactors[60] = prefactor(3, 32, 385, 2);
      fPrefactors[61] = prefactor(-3, 64, 385, 2);
      fPrefactors[62] = prefactor(3, 64, 5005, 1);
      fPrefactors[63] = prefactor(-3, 64, 715, 2);

      // l=8 prefactors (m = -8 ... +8)
      fPrefactors[64] = prefactor(3, 256, 12155, 2);
      fPrefactors[65] = prefactor(3, 64, 12155, 2);
      fPrefactors[66] = prefactor(1, 128, 7293, 1);
      fPrefactors[67] = prefactor(3, 64, 17017, 2);
      fPrefactors[68] = prefactor(3, 128, 1309, 2);
      fPrefactors[69] = prefactor(1, 64, 19635, 2);
      fPrefactors[70] = prefactor(3, 128, 595, 1);
      fPrefactors[71] = prefactor(3, 64, 17, 2);
      fPrefactors[72] = prefactor(1, 256, 17, 1);  //
      fPrefactors[73] = prefactor(-3, 64, 17, 2);
      fPrefactors[74] = prefactor(3, 128, 595, 1);
      fPrefactors[75] = prefactor(-1, 64, 19635, 2);
      fPrefactors[76] = prefactor(3, 128, 1309, 2);
      fPrefactors[77] = prefactor(-3, 64, 17017, 2);
      fPrefactors[78] = prefactor(1, 128, 7293, 1);
      fPrefactors[79] = prefactor(-3, 64, 12155, 2);
      fPrefactors[80] = prefactor(3, 256, 12155, 2);

      // l=9 prefactors (m = -9 ... +9)
      fPrefactors[81] = prefactor(1, 512, 230945, 1);
      fPrefactors[82] = prefactor(3, 256, 230945, 2);
      fPrefactors[83] = prefactor(3, 512, 13585, 1);
      fPrefactors[84] = prefactor(1, 128, 40755, 1);
      fPrefactors[85] = prefactor(3, 256, 2717, 1);
      fPrefactors[86] = prefactor(3, 128, 95095, 2);
      fPrefactors[87] = prefactor(1, 256, 21945, 1);
      fPrefactors[88] = prefactor(3, 128, 1045, 1);
      fPrefactors[89] = prefactor(3, 256, 95, 2);
      fPrefactors[90] = prefactor(1, 256, 19, 1);  //
      fPrefactors[91] = prefactor(-3, 256, 95, 2);
      fPrefactors[92] = prefactor(3, 128, 1045, 1);
      fPrefactors[93] = prefactor(-1, 256, 21945, 1);
      fPrefactors[94] = prefactor(3, 128, 95095, 2);
      fPrefactors[95] = prefactor(-3, 256, 2717, 1);
      fPrefactors[96] = prefactor(1, 128, 40755, 1);
      fPrefactors[97] = prefactor(-3, 512, 13585, 1);
      fPrefactors[98] = prefactor(3, 256, 230945, 2);
      fPrefactors[99] = prefactor(-1, 512, 230945, 1);

      // l=10 prefactors (m = -10 ... +10)
      fPrefactors[100] = prefactor(1, 1024, 969969, 1);
      fPrefactors[101] = prefactor(1, 512, 4849845, 1);
      fPrefactors[102] = prefactor(1, 512, 255255, 2);
      fPrefactors[103] = prefactor(3, 512, 85085, 1);
      fPrefactors[104] = prefactor(3, 1024, 5005, 1);
      fPrefactors[105] = prefactor(3, 256, 1001, 1);
      fPrefactors[106] = prefactor(3, 256, 5005, 2);
      fPrefactors[107] = prefactor(3, 256, 5005, 1);
      fPrefactors[108] = prefactor(3, 512, 385, 2);
      fPrefactors[109] = prefactor(1, 256, 1155, 2);
      fPrefactors[110] = prefactor(1, 512, 21, 1);  //
      fPrefactors[111] = prefactor(-1, 256, 1155, 2);
      fPrefactors[112] = prefactor(3, 512, 385, 2);
      fPrefactors[113] = prefactor(-3, 256, 5005, 1);
      fPrefactors[114] = prefactor(3, 256, 5005, 2);
      fPrefactors[115] = prefactor(-3, 256, 1001, 1);
      fPrefactors[116] = prefactor(3, 1024, 5005, 1);
      fPrefactors[117] = prefactor(-3, 512, 85085, 1);
      fPrefactors[118] = prefactor(1, 512, 255255, 2);
      fPrefactors[119] = prefactor(-1, 512, 4849845, 1);
      fPrefactors[120] = prefactor(1, 1024, 969969, 1);


      for (int l = 0; l <= 10; l++) {
        fPrefshift[l] = l * (l + 1);
        fPlmshift[l]  = l * (l + 3) / 2;
      }

      fYlms = new std::complex<double>[121];

      Double_t fac   = 1;
      fFactorials[0] = 1;
      for (int iter = 1; iter < 24; iter++) {
        fac *= iter;
        fFactorials[iter] = fac;
      }
    }

    void YlmMath::LegendreUpToYlm(int lmax, double ctheta, double* lbuf) const {
      // Calculate a set of legendre polynomials up to a given l
      // with spherical input
      double sins[11];
      double coss[11];
      sins[0] = 0.0;
      coss[0] = 1.0;
      sins[1] = sqrt(1. - ctheta * ctheta);
      coss[1] = ctheta;
      for (int iter = 2; iter < 11; iter++) {
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
      if (lmax > 5)  // switch to recursive
        for (int l = 5; l <= lmax; ++l) {
          const int shift = fPlmshift[l];

          // P_l^l = (2l - 1) sin(theta) P_{l-1}^{l-1}
          lbuf[shift - l] = (2.0 * l - 1.0) * sins[1] * lbuf[fPlmshift[l - 1] - (l - 1)];

          // P_l^{l-1} = (2l - 1) cos(theta) P_{l-1}^{l-1}
          lbuf[shift - (l - 1)] = (2.0 * l - 1.0) * ctheta * lbuf[fPlmshift[l - 1] - (l - 1)];

          // P_l^m = [(2l - 1) cos(theta) P_{l-1}^m
          //          - (l + m - 1) P_{l-2}^m] / (l - m)
          for (int m = 0; m <= l - 2; ++m) {
            lbuf[shift - m] =
              ((2.0 * l - 1.0) * ctheta * lbuf[fPlmshift[l - 1] - m] - (l + m - 1.0) * lbuf[fPlmshift[l - 2] - m]) / (l - m);
          }
        }
    }

    std::complex<double>* YlmMath::YlmUpToL(int lmax, double x, double y, double z) {
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

    std::complex<double>* YlmMath::YlmUpToL(int lmax, double ctheta, double phi) {
      // Calculate a set of Ylms up to a given l
      // with spherical input
      int lcur = 0;
      double lpol;

      double coss[11];
      double sins[11];

      double lbuf[66];  // was 36 but not used
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

    double YlmMath::GetScale(int l, int m) const { return fPrefactors[fPrefshift[l] + m]; }

  }  // namespace Sh


} /* namespace Hal */
