#include "constants.h"
#include "misc.h"
#include "sf.h"
#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_pipluspiminus_phaseshift::CWaveFunction_pipluspiminus_phaseshift(string parsfilename) {
      int iq, ichannel, *I;
      double q;

      ParsInit(parsfilename);

      m1        = MPI;
      m2        = MPI;
      IDENTICAL = 0;
      q1q2      = -1;
      if (COULOMB == 0) q1q2 = 0;
      nchannels = 5;

      ellmax = 2;
      InitArrays();
      ell[0] = 0;
      ell[1] = 2;
      ell[2] = 1;
      ell[3] = 0;
      ell[4] = 2;

      InitWaves();
      channelweight[0] = (2.0 / 3.0);
      channelweight[1] = (2.0 / 3.0) * 5.0;
      channelweight[2] = 3.0;
      channelweight[3] = (1.0 / 3.0);
      channelweight[4] = (1.0 / 3.0) * 5.0;
      I                = new int[nchannels];
      I[0] = I[1] = 0;
      I[2]        = 1;
      I[3] = I[4] = 2;

      for (ichannel = 0; ichannel < nchannels; ichannel++) {
        for (iq = 0; iq < nqmax; iq++) {
          q = qarray[iq];
          WaveFunctionRoutines::getphaseshift_pipi(I[ichannel], ell[ichannel], q, &delta[ichannel][iq], &ddeltadq[ichannel][iq]);
          // delta[ichannel][iq]=ddeltadq[ichannel][iq]=0.0;
          // printf("%6.2f : %g %g\n",q,(180.0/PI)*delta[ichannel][iq],
          //     (180.0/PI)*ddeltadq[ichannel][iq]);
          if (q1q2 != 0)
            CoulWave::phaseshift_CoulombCorrect(ell[ichannel], q, eta[iq], delta[ichannel][iq], ddeltadq[ichannel][iq]);
          Wepsilon[ichannel][iq] = ddeltadq[ichannel][iq] - GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], delta[ichannel][iq])
                                   + GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], 0.0);
          Wepsilon[ichannel][iq] = 3.0 * Wepsilon[ichannel][iq] / (4.0 * PI * pow(epsilon, 3));
        }
      }
      /*  for(iq=0;iq<nqmax;iq++){
          q=qarray[iq];
          printf("q=%g, delta=%g, ddeltadq=%g, IW(epsilon=1.0)=%g\n",
          q,delta[2][iq],ddeltadq[2][iq],
          GetIW(1,1.0,q,0,0,delta[2][iq])-GetIW(1,1.0,q,0,0,0));
          }*/
      // printf("pipluspiminus_phaseshift wf initialized\n");
    }

    double CWaveFunction_pipluspiminus_phaseshift::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared, x, dpsi2, q, ROOT2 = sqrt(2.0);
      double delta_s, delta_p, delta_d;
      complex<double> psi, hstar0, hstar1, hstar2, psi0, psia, psib;
      int ichannel;

      if (iq >= nqmax) {
        psisquared = 1.0;
      } else {
        q = qarray[iq];
        x = q * r / HBARC;

        if (r < epsilon) {
          psi0       = planewave[iq]->planewave(r, ctheta);
          psisquared = real(psi0 * conj(psi0));
          if (STRONG == 1) {
            for (ichannel = 0; ichannel < nchannels; ichannel++) {
              dpsi2 = channelweight[ichannel] * 2.0 * PI * Wepsilon[ichannel][iq] * pow(HBARC, 3) / (q * q);
              psisquared += dpsi2;
            }
          }
        } else {
          psia = planewave[iq]->planewave(r, ctheta);
          psib = planewave[iq]->planewave(r, -ctheta);

          hstar0 = partwave[0][iq]->GetPhiIncoming(r) / x;
          hstar1 = partwave[1][iq]->GetPhiIncoming(r) / x;
          hstar2 = partwave[2][iq]->GetPhiIncoming(r) / x;

          // For I=0
          psi = (psia + psib) / ROOT2;
          if (STRONG == 1) {
            delta_s = delta[0][iq];
            // delta_d=delta[1][iq];
            delta_d  = 0.0;
            ichannel = 0;  // This will be the s wave
            psi += ROOT2 * 0.5 * hstar0 * (Misc::ceiphi(-2.0 * delta_s) - 1.0);
            ichannel = 1;  // This will be the d wave
            psi +=
              ROOT2 * 0.5 * ci * ci * 5.0 * SpherHarmonics::legendre(2, ctheta) * hstar2 * (Misc::ceiphi(-2.0 * delta_d) - 1.0);
          }
          psisquared = (1.0 / 3.0) * real(psi * conj(psi));

          // For I=1
          psi = (psia - psib) / ROOT2;
          if (STRONG == 1) {
            // delta_p=delta[2][iq];
            delta_p  = 0.0;
            ichannel = 2;  // This will be the p wave
            psi += ROOT2 * 0.5 * ci * 3.0 * SpherHarmonics::legendre(1, ctheta) * hstar1 * (Misc::ceiphi(-2.0 * delta_p) - 1.0);
          }
          psisquared += 0.5 * real(psi * conj(psi));
          // psisquared+=0.5*real(dpsi*conj(dpsi));

          // For I=2
          psi = (psia + psib) / ROOT2;
          if (STRONG == 1) {
            delta_s = delta[3][iq];
            // delta_d=delta[4][iq];
            delta_d  = 0.0;
            ichannel = 3;  // This will be the s wave
            psi += ROOT2 * 0.5 * hstar0 * (Misc::ceiphi(-2.0 * delta_s) - 1.0);
            ichannel = 4;  // This will be the d wave
            psi +=
              ROOT2 * 0.5 * ci * ci * 5.0 * SpherHarmonics::legendre(2, ctheta) * hstar2 * (Misc::ceiphi(-2.0 * delta_d) - 1.0);
          }
          psisquared += (1.0 / 6.0) * real(psi * conj(psi));
        }
      }
      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }
  }  // namespace ScottPratt
}  // namespace Hal
