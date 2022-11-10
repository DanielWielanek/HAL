#include "constants.h"
#include "misc.h"
#include "wavefunction.h"

namespace Hal {
  namespace ScottPratt {
    CWaveFunction_lambdalambdaparspin_phaseshift::CWaveFunction_lambdalambdaparspin_phaseshift(string parsfilename) {
      ParsInit(parsfilename);

      m1        = 1115.7;
      m2        = m1;
      IDENTICAL = 0;
      q1q2      = 0;
      nchannels = 0;
      ellmax    = 0;
      InitArrays();

      InitWaves();
    }

    double CWaveFunction_lambdalambdaparspin_phaseshift::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared;
      complex<double> psiantisymm, psia, psib;
      const double ROOT2 = sqrt(2.0);

      if (iq >= nqmax) {
        psisquared = 1.0;
      } else {
        psia        = planewave[iq]->planewave(r, ctheta);
        psib        = planewave[iq]->planewave(r, -ctheta);
        psiantisymm = (psia - psib) / ROOT2;
        psisquared  = real(psiantisymm * conj(psiantisymm));
      }
      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }

    void CWaveFunction_lambdalambdaparspin_phaseshift::GetPhaseshifts() {
      double q, q0;
      double tandelta, alength, dtandeltadq;
      double MH0, M, EH0, GammaH0, lambda = 500.0;
      int iq;

      printf("Enter the energy of the H0 above the 2Lambda threshold in MeV : ");
      scanf("%lf", &EH0);
      printf("Enter the width of the H0 in MeV : ");
      scanf("%lf", &GammaH0);

      // Scattering length and effective range parameters
      printf("Enter scattering length in fm (Do not include effect of H0): ");
      scanf("%lf", &alength);
      lambda = 500.0;  // Arbitrary choice, returns delta to zero at large q
      // reff=2*HBARC*HBARC/(lambda*lambda*a); // Effective range (not used)

      MH0 = m1 + m2 + EH0;
      q0  = sqrt(0.25 * MH0 * MH0 - m1 * m1);
      printf("Resonance occurs at q=%g, mesh goes q=%g\n", q0, qarray[nqmax - 1]);

      for (iq = 0; iq < nqmax; iq++) {
        q           = qarray[iq];
        M           = 2.0 * sqrt(q * q + m1 * m1);
        tandelta    = (alength * q / HBARC) / (1.0 + q * q / (lambda * lambda));
        dtandeltadq = (tandelta / q) * (1.0 - q * q / (lambda * lambda)) / (1.0 + q * q / (lambda * lambda));
        tandelta += 0.5 * (q / q0) * GammaH0 / (MH0 - M);
        dtandeltadq += (tandelta / q) * (1 + (4.0 * q * q / M) / (MH0 - M));
        delta[0][iq]    = atan(tandelta);
        ddeltadq[0][iq] = dtandeltadq * pow(cos(delta[0][iq]), 2);
      }
    }
  }  // namespace ScottPratt
}  // namespace Hal
