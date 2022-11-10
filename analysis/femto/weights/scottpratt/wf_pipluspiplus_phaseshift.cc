#include "constants.h"
#include "misc.h"
#include "sf.h"
#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_pipluspiplus_phaseshift::CWaveFunction_pipluspiplus_phaseshift(string parsfilename) {
      int iq, ichannel;
      double q;
      int* I;

      ParsInit(parsfilename);

      m1        = MPI;
      m2        = MPI;
      IDENTICAL = 1;
      q1q2      = 1;
      if (COULOMB == 0) q1q2 = 0;
      nchannels = 2;
      ellmax    = 2;
      InitArrays();
      ell[0] = 0;
      ell[1] = 2;
      InitWaves();
      channelweight[0] = channelweight[1] = 2.0;

      I    = new int[nchannels];
      I[0] = I[1] = 2;
      for (ichannel = 0; ichannel < nchannels; ichannel++) {
        // printf("_____ ell=%d, I=%d _____\n",ell[ichannel],I[ichannel]);
        for (iq = 0; iq < nqmax; iq++) {
          q = qarray[iq];
          WaveFunctionRoutines::getphaseshift_pipi(I[ichannel], ell[ichannel], q, &delta[ichannel][iq], &ddeltadq[ichannel][iq]);
          // delta[ichannel][iq]=ddeltadq[ichannel][iq]=0.0;
          // printf("%6.2f: %g  %g\n",q,(180.0/PI)*delta[ichannel][iq],
          //     (180.0/PI)*ddeltadq[ichannel][iq]);
          if (q1q2 != 0)
            CoulWave::phaseshift_CoulombCorrect(ell[ichannel], q, eta[iq], delta[ichannel][iq], ddeltadq[ichannel][iq]);
          Wepsilon[ichannel][iq] = ddeltadq[ichannel][iq] - GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], delta[ichannel][iq])
                                   + GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], 0.0);
          Wepsilon[ichannel][iq] = 3.0 * Wepsilon[ichannel][iq] / (4.0 * PI * pow(epsilon, 3));
        }
      }
      printf("pipluspiplus wf initialized\n");
    }

    double CWaveFunction_pipluspiplus_phaseshift::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared, x, dpsi2;
      const double ROOT2 = sqrt(2.0);
      double delta_s, delta_d;
      complex<double> psi, hstar, psia, psib;
      double q;
      int ichannel;

      if (iq >= nqmax) {
        psisquared = 1.0;
      } else {
        q    = qarray[iq];
        psia = planewave[iq]->planewave(r, ctheta);
        psib = planewave[iq]->planewave(r, -ctheta);
        psi  = (psia + psib) / ROOT2;
        if (r < epsilon) {
          psisquared = real(psi * conj(psi));
          if (STRONG == 1) {
            for (ichannel = 0; ichannel < nchannels; ichannel++) {
              dpsi2 = (2.0 * ell[ichannel] + 1.0) * 2.0 * PI * Wepsilon[ichannel][iq] * pow(HBARC, 3) / (q * q);
              psisquared += 2.0 * dpsi2;  // factor of two due to symmetrization
            }
          }
        } else {
          if (STRONG == 1) {
            x       = q * r / HBARC;
            delta_s = delta[0][iq];
            delta_d = delta[1][iq];
            // delta_s=delta_d=0.0;

            ichannel = 0;  // This will be the s wave
            hstar    = partwave[ell[ichannel]][iq]->GetPhiIncoming(r) / x;
            psi += ROOT2 * 0.5 * hstar * (Misc::ceiphi(-2.0 * delta_s) - 1.0);

            ichannel = 1;  // This will be the d wave
            hstar    = partwave[ell[ichannel]][iq]->GetPhiIncoming(r) / x;
            psi +=
              ROOT2 * 0.5 * ci * ci * 5.0 * SpherHarmonics::legendre(2, ctheta) * hstar * (Misc::ceiphi(-2.0 * delta_d) - 1.0);
          }
          psisquared = real(psi * conj(psi));
        }
      }

      // printf("psisquared=%g\n",psisquared);
      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }
  }  // namespace ScottPratt
}  // namespace Hal
