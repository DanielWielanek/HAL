#include "constants.h"
#include "misc.h"
#include "sf.h"
#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_pn_phaseshift::CWaveFunction_pn_phaseshift(string parsfilename) : CWaveFunction() {
      int iq, ichannel;
      double q;
      ParsInit(parsfilename);
      m1        = MPROTON;
      m2        = MNEUTRON;
      IDENTICAL = 0;
      q1q2      = 0;
      COULOMB   = 0;
      nchannels = 6;

      ellmax = 1;
      InitArrays();
      printf("Arrays Initialized\n");

      ell[0] = ell[1] = 0;
      ell[2] = ell[3] = ell[4] = ell[5] = 1;

      InitWaves();
      printf("Partial Waves Initialized\n");

      // Channel weight is (2J+1)/[(2s1+1)*(2s2+1)]
      channelweight[0] = 0.25;
      channelweight[1] = 0.75;
      channelweight[2] = 0.25;
      channelweight[3] = 0.75;
      channelweight[4] = 0.75;
      channelweight[5] = 1.25;
      read_phaseshifts();
      EffectiveRange(0, -16.75, 2.7);
      EffectiveRange(1, 5.36, 1.81);
      for (iq = 0; iq < nqmax; iq++)
        delta[1][iq] += PI;
      for (ichannel = 0; ichannel < nchannels; ichannel++) {
        for (iq = 0; iq < nqmax; iq++) {
          q = qarray[iq];
          // printf("ichannel=%d, q=%g, delta=%g, ddeltadq=%g\n",
          //      ichannel,q,delta[ichannel][iq]*180.0/PI,
          //      ddeltadq[ichannel][iq]*180.0/PI);
          Wepsilon[ichannel][iq] = ddeltadq[ichannel][iq] - GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], delta[ichannel][iq])
                                   + GetIW(ell[ichannel], epsilon, q, q1q2, eta[iq], 0.0);
          Wepsilon[ichannel][iq] = 3.0 * Wepsilon[ichannel][iq] / (4.0 * PI * pow(epsilon, 3));
        }
      }
      // printf("Initialization finished\n");
    }

    double CWaveFunction_pn_phaseshift::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared, x, dpsi2, q, theta;
      double delta_1s0, delta_3s1, delta_3p0, delta_1p1, delta_3p1, delta_3p2;
      complex<double> psi, psia, hstar0, hstar1, psi0;
      complex<double> Xlm00, Xlm10, Xlm11;
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
          delta_1s0 = delta[0][iq];
          delta_3s1 = delta[1][iq];
          delta_3p0 = delta[2][iq];
          delta_1p1 = delta[3][iq];
          delta_3p1 = delta[4][iq];
          delta_3p2 = delta[5][iq];
          hstar0    = partwave[0][iq]->GetPhiIncoming(r) / x;
          hstar1    = partwave[1][iq]->GetPhiIncoming(r) / x;
          // XlmLM 9s Y_{LM}*(1/2)*i^L*sqrt(4*PI*(2*L+1))*hstar_L
          Xlm00 = 0.5 * sqrt(4.0 * PI) * SpherHarmonics::Ylm(0, 0, theta, 0.0) * hstar0;
          Xlm10 = ci * 0.5 * sqrt(12.0 * PI) * SpherHarmonics::Ylm(1, 0, theta, 0.0) * hstar1;
          Xlm11 = ci * 0.5 * sqrt(12.0 * PI) * SpherHarmonics::Ylm(1, 1, theta, 0.0) * hstar1;

          // First do the case for S=0;
          psi = psi0;
          // this refers to S=0, L=0, J=0 channel
          psi += Xlm00 * (Misc::ceiphi(-2.0 * delta_1s0) - 1.0);
          // S=0, L=1, J=1
          psi += Xlm10 * (Misc::ceiphi(-2.0 * delta_1p1) - 1.0);
          psisquared = 0.25 * real(psi * conj(psi));

          // Now let's do the case for S=1, M_S=+1
          psia = 0.0;  // Component with M_S=0, M_L=1;
          psi  = psi0;
          // S=1, L=0, J=1
          psi += Xlm00 * (Misc::ceiphi(-2.0 * delta_3s1) - 1.0);
          // S=1, L=1 and J=1,2,
          psi += Xlm10 * (0.5 * Misc::ceiphi(-2.0 * delta_3p1) + 0.5 * Misc::ceiphi(-2.0 * delta_3p2) - 1.0);
          psia = Xlm11 * 0.5 * (Misc::ceiphi(-2.0 * delta_3p1) - Misc::ceiphi(-2.0 * delta_3p2));
          psisquared += 0.5 * real(psi * conj(psi) + psia * conj(psia));
          // Term is doubled to account for M_S=-1

          // Now let's do the case with S=1, M_S=0;
          psia = 0.0;  // Component with M_S=-1,M_L=1 or M_S=1,M_L=-1;
          psi  = psi0;
          // S=1, L=0, J=1
          psi += Xlm00 * (Misc::ceiphi(-2.0 * delta_3s1) - 1.0);
          // S=1, L=1, J=0,2
          psi += Xlm10 * ((2.0 / 3.0) * Misc::ceiphi(-2.0 * delta_3p2) + (1.0 / 3.0) * Misc::ceiphi(-2.0 * delta_3p0) - 1.0);
          psia = Xlm11 * (Misc::ceiphi(-2.0 * delta_3p2) - Misc::ceiphi(-2.0 * delta_3p0)) / 3.0;
          psisquared += 0.25 * real(psi * conj(psi) + 2.0 * psia * conj(psia));
        }
      } else
        psisquared = real(psi0 * conj(psi0));
      if (psisquared < 0 && r > epsilon) { printf("psisquared<0, = %g, r=%g, q=%g\n", psisquared, r, q); }
      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }

    // the new pn phaseshift reader needs to be fixed
    /*void CWaveFunction_pn_phaseshift::read_phaseshifts(){
    #include "pn_phaseshiftdat.cc"
    int iqdata,iq,ichannel;
    double w1,w2,q;
    for(ichannel=0;ichannel<6;ichannel++){
    for(iq=0;iq<nqmax;iq++){
    q=qarray[iq];
    iqdata=int(floor(q)/delqdata);
    if(iqdata<nqdata){
    w1=(delq*double(iqdata+1)-q)/delqdata;
    w2=1.0-w1;
    delta[ichannel][iq]=w1*data_delta[ichannel][iqdata]
    +w2*data_delta[ichannel][iqdata+1];
    ddeltadq[ichannel][iq]=w1*data_ddeltadq[ichannel][iqdata]
    +w2*data_ddeltadq[ichannel][iqdata+1];
    }
    else{
    delta[ichannel][iq]=0.0;
    ddeltadq[ichannel][iq]=0.0;
    }
    }
    }
    }*/

    void CWaveFunction_pn_phaseshift::read_phaseshifts() {
      int iq, iread, iqsmooth, ichannel;
      const int NREAD = 34;
      double qsmooth, q, tlab, elab, plab, roots;
      double deltaread[6][NREAD], qread[NREAD];
      double delta_1s0, delta_3s1, delta_3p0, delta_1p1, delta_3p1, delta_3p2;
      double eps1, delta_1d2, delta_3d1, delta_3d2, delta_3d3;
      char dummy[200];
      FILE* fptr;
      iqsmooth = 1;
      qsmooth  = 25.0;
      printf("BEWARE, MAKE SURE YOU HAVE MOVED COPY OF NN_phaseshifts.dat to run directory\n");
      fptr = fopen("NN_phaseshifts.dat\0", "r");
      fgets(dummy, 200, fptr);
      for (iread = 0; iread < NREAD; iread++) {
        fscanf(fptr,
               "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
               &tlab,
               &delta_1s0,
               &delta_3s1,
               &delta_3d1,
               &eps1,
               &delta_3p0,
               &delta_1p1,
               &delta_3p1,
               &delta_3p2,
               &delta_1d2,
               &delta_3d2,
               &delta_3d3);
        deltaread[0][iread] = delta_1s0;
        deltaread[1][iread] = delta_3s1;
        deltaread[2][iread] = delta_3p0;
        deltaread[3][iread] = delta_1p1;
        deltaread[4][iread] = delta_3p1;
        deltaread[5][iread] = delta_3p2;
        if (iread > 0) {
          elab         = MPROTON + tlab;
          plab         = sqrt(elab * elab - MPROTON * MPROTON);
          roots        = sqrt((elab + MNEUTRON) * (elab + MNEUTRON) - plab * plab);
          qread[iread] = sqrt(Misc::triangle(roots, MPROTON, MNEUTRON));
        } else
          qread[iread] = 0.0;
        for (ichannel = 0; ichannel < 6; ichannel++)
          deltaread[ichannel][iread] = deltaread[ichannel][iread] * PI / 180.0;
      }
      fclose(fptr);

      for (iq = 0; iq < nqmax; iq++) {
        q = qarray[iq];
        if (q <= qread[NREAD - 1]) {
          iread = 0;
          while (q > qread[iread]) {
            iread += 1;
          }
          for (ichannel = 0; ichannel < 6; ichannel++) {
            delta[ichannel][iq] =
              (qread[iread] - q) * deltaread[ichannel][iread - 1] + (q - qread[iread - 1]) * deltaread[ichannel][iread];
            delta[ichannel][iq] = delta[ichannel][iq] / (qread[iread] - qread[iread - 1]);
            ddeltadq[ichannel][iq] =
              (deltaread[ichannel][iread] - deltaread[ichannel][iread - 1]) / (qread[iread] - qread[iread - 1]);
          }

        } else {
          delta[0][iq]    = 0.0;
          ddeltadq[0][iq] = 0.0;
          printf("Warning: qarray goes beyond max. for phaseshift data =%g\n", qread[NREAD - 1]);
          // exit(1);
        }
        iqsmooth = 6;
        qsmooth  = qread[iqsmooth];
        if (iread < iqsmooth) {
          for (ichannel = 2; ichannel < 6; ichannel++) {
            delta[ichannel][iq]    = deltaread[ichannel][iqsmooth] * (q * q * q / (qsmooth * qsmooth * qsmooth));
            ddeltadq[ichannel][iq] = 3.0 * delta[ichannel][iq] / q;
          }
        }
      }
    }
  }  // namespace ScottPratt
}  // namespace Hal
