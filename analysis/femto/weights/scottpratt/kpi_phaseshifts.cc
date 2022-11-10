#include "misc.h"
#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    void WaveFunctionRoutines::getphaseshift_kpi(int twoI, int ell, double q, double* delta, double* ddeltadq) {
      double a, reff;
      const double R = (1000.0 / 4.3), GammaR = 52.9, MR = 895.7, M1 = 139.57, M2 = 493.677;
      double dEdq, E, E1, E2, Gamma, qR, dGammadq, tandelta, dtandeltadq, denom;
      // phaseshifts from P. Estabrooks etal, NPB133, p. 490 (1978).

      if (ell == 0) {
        if (twoI == 1) {
          a    = 0.00239;
          reff = -0.00176;
        } else {
          a    = -0.001;
          reff = -0.00176;  // yes, they are the same values of reff - this is not a typo
        }

        denom       = ((1.0 / a) + 0.5 * reff * q * q);
        tandelta    = q / denom;
        dtandeltadq = (tandelta / q) - tandelta * reff * q / denom;
        *delta      = atan(tandelta);
        *ddeltadq   = dtandeltadq / (1.0 + tandelta * tandelta);
      } else if (ell == 1) {
        if (twoI == 1) {
          E1          = sqrt(M1 * M1 + q * q);
          E2          = sqrt(M2 * M2 + q * q);
          E           = E1 + E2;
          dEdq        = (q / E1) + (q / E2);
          qR          = sqrt(Misc::triangle(MR, M1, M2));
          Gamma       = GammaR * pow(q / qR, 3) * ((1.0 + pow(qR / R, 2)) / (1.0 + pow(q / R, 2)));
          dGammadq    = (3.0 * Gamma / q) - Gamma * 2.0 * (q / (R * R)) / (1.0 + pow(q / R, 2));
          tandelta    = MR * Gamma / (MR * MR - E * E);
          dtandeltadq = (tandelta * dGammadq / Gamma) + 2.0 * E * tandelta * dEdq / (MR * MR - E * E);
          *delta      = atan(tandelta);
          *ddeltadq   = dtandeltadq / (1.0 + tandelta * tandelta);
        } else {
          *delta    = 0.0;
          *ddeltadq = 0.0;
        }
      } else {
        *delta    = 0.0;
        *ddeltadq = 0.0;
      }
    }
  }  // namespace ScottPratt
}  // namespace Hal
