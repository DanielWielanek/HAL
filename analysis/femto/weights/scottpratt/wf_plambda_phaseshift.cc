#include "constants.h"
#include "misc.h"
#include "sf.h"
#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_plambda_phaseshift::CWaveFunction_plambda_phaseshift(string parsfilename) : CWaveFunction() {
      int iq, ichannel;
      double q;
      ParsInit(parsfilename);

      m1        = MPROTON;
      m2        = MLAMBDA;
      IDENTICAL = 0;
      q1q2      = 0;
      nchannels = 1;
      ellmax    = 0;
      InitArrays();
      printf("Arrays Initialized\n");

      ell[0] = 0;

      InitWaves();
      printf("Partial Waves Initialized\n");

      // Channel weight is (2J+1)/[(2s1+1)*(2s2+1)]
      channelweight[0] = 1.0;
      get_phaseshifts();

      for (ichannel = 0; ichannel < nchannels; ichannel++) {
        for (iq = 0; iq < nqmax; iq++) {
          q = qarray[iq];
          // printf("ichannel=%d, q=%g, delta=%g, ddeltadq=%g\n",
          //     ichannel,q,delta[ichannel][iq]*180.0/PI,
          //     ddeltadq[ichannel][iq]*180.0/PI);
          Wepsilon[ichannel][iq] = ddeltadq[ichannel][iq] - GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], delta[ichannel][iq])
                                   + GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], 0.0);
          Wepsilon[ichannel][iq] = 3.0 * Wepsilon[ichannel][iq] / (4.0 * PI * pow(epsilon, 3));
        }
      }
      // printf("Initialization finished\n");
    }

    double CWaveFunction_plambda_phaseshift::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared, x, dpsi2, q, theta;
      complex<double> psi, hstar0, psi0;
      complex<double> Xlm00;
      int ichannel;

      q = qarray[iq];
      if (iq >= nqmax) {
        printf("iq too large!\n");
        exit(1);
      }
      psi0 = planewave[iq]->planewave(r, ctheta);

      if (STRONG == 1) {
        if (r < epsilon) {
          psisquared = real(psi0 * conj(psi0));
          for (ichannel = 0; ichannel < nchannels; ichannel++) {
            dpsi2 = channelweight[ichannel] * 2.0 * PI * Wepsilon[ichannel][iq] * pow(HBARC, 3) / (q * q);
            psisquared += dpsi2;
          }
        } else {
          theta = acos(ctheta);
          x     = q * r / HBARC;
          // Notation is (2S+1)-L-J
          hstar0 = partwave[0][iq]->GetPhiIncoming(r) / x;
          // XlmLM 9s Y_{LM}*(1/2)*i^L*sqrt(4*PI*(2*L+1))*hstar_L
          Xlm00 = 0.5 * sqrt(4.0 * PI) * SpherHarmonics::Ylm(0, 0, theta, 0.0) * hstar0;

          // First do the case for S=0;
          psi = psi0;
          // this refers to S=0, L=0, J=0 channel
          psi += Xlm00 * (Misc::ceiphi(-2.0 * delta[0][iq]) - 1.0);
          psisquared = real(psi * conj(psi));
        }
      } else
        psisquared = real(psi0 * conj(psi0));
      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }

    void CWaveFunction_plambda_phaseshift::get_phaseshifts() {
      double q, alength, Reff, tandel;
      int iq;
      alength = -1.9;  // Bodmer and UsmaniScattering Length, NPA477, 621 (1988).
      Reff    = 3.4;
      for (iq = 0; iq < nqmax; iq++) {
        q               = qarray[iq];
        tandel          = 1.0 / (-HBARC / (q * alength) + 0.5 * Reff * q / HBARC);
        delta[0][iq]    = atan(tandel);
        ddeltadq[0][iq] = -(HBARC / (q * q * alength) + 0.5 * Reff / HBARC) * tandel * tandel / (1 + tandel * tandel);
      }
    }
  }  // namespace ScottPratt
}  // namespace Hal
