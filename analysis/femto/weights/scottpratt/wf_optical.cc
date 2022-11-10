#include "wavefunction.h"

// for non-identical particles: square will with complex optical potential

namespace Hal {
  namespace ScottPratt {
    CWaveFunction_optical::CWaveFunction_optical(string parsfilename) : CWaveFunction() {
      ParsInit(parsfilename);
      llmax.resize(nqmax);
      kmax = 100;
      etavec.resize(nqmax);
      sigma_annihilation.resize(nqmax);
      CL.resize(nqmax);
      sigmaL.resize(nqmax);
      a.resize(nqmax);

      A.resize(nqmax);
      B.resize(nqmax);
      qinside.resize(nqmax);

      int q1q2set;
      double m1set, m2set, VRset, VIset, Rset;
      q1q2set = parameters.getI("Q1Q2", -1);
      m1set   = parameters.getD("M1", 938.272);
      m2set   = parameters.getD("M2", 938.272);
      VRset   = parameters.getD("OPTICAL_VR", -8.26);
      VIset   = parameters.getD("OPTICAL_VI", 25.99);
      Rset    = parameters.getD("OPTICAL_A", 1.892);
      Reset(q1q2set, m1set, m2set, VRset, VIset, Rset);
      InitArrays();
      InitWaves();
    }

    CWaveFunction_optical::~CWaveFunction_optical() {
      int iq, ll;
      for (iq = 0; iq < nqmax; iq++) {
        for (ll = 0; ll < llmax[iq]; ll++) {
          a[iq][ll].resize(0);
        }
        A[iq].resize(0);
        B[iq].resize(0);
        a[iq].resize(0);
        CL[iq].resize(0);
      }
      A.resize(0);
      B.resize(0);
      a.resize(0);
      CL.resize(0);
      llmax.resize(0);
    }

    void CWaveFunction_optical::Reset(int q1q2set, double m1set, double m2set, double VRset, double VIset, double Rset) {
      int iq, L, ll;
      double K, phiq, delK, kmagR, qmag2;
      m1   = m1set;
      m2   = m2set;
      mu   = m1 * m2 / (m1 + m2);
      q1q2 = q1q2set;
      VR   = VRset;
      VI   = VIset;
      R    = Rset;

      for (iq = 0; iq < nqmax; iq++) {
        kmagR = delq * iq * R / HBARC;
        if (kmagR > 2.5) {
          llmax[iq] = 8 + lrint(kmagR - 2);
        } else
          llmax[iq] = 8;
      }

      for (iq = 0; iq < nqmax; iq++) {
        if (a[iq].size() > 0) {
          for (L = 0; L <= llmax[iq]; L++) {
            a[iq][L].resize(0);
          }
        }
        a[iq].resize(llmax[iq] + 1);
        CL[iq].resize(llmax[iq] + 1);
        sigmaL[iq].resize(llmax[iq] + 1);
        for (L = 0; L <= llmax[iq]; L++) {
          a[iq][L].resize(kmax);
        }
      }

      if (q1q2 != 0)
        COULOMB = true;
      else
        COULOMB = false;
      nchannels = 0;
      delK      = delq;
      for (iq = 0; iq < nqmax; iq++) {
        K           = (iq + 0.5) * delK;
        qmag2       = fabs(2.0 * mu * VI) + fabs(K * K - 2.0 * mu * VR);
        phiq        = 0.5 * atan2(-2.0 * mu * VI, K * K - 2.0 * mu * VR);
        qinside[iq] = sqrt(qmag2) * exp(ci * phiq);
        etavec[iq]  = ALPHA * q1q2 * mu / qinside[iq];
      }

      GetCLsigmaL();
      GetExpansionCoefficients();

      for (iq = 0; iq < nqmax; iq++) {
        A[iq].resize(llmax[iq] + 1);
        B[iq].resize(llmax[iq] + 1);
        for (ll = 0; ll <= llmax[iq]; ll++) {
          A[iq][ll] = B[iq][ll] = 0.0;
        }
      }
      GetAB();
    }

    void CWaveFunction_optical::ClearInfo() {
      int iq, L;
      for (iq = 0; iq < nqmax; iq++) {
        for (L = 0; L <= llmax[iq]; L++) {
          a[iq][L].clear();
        }
        a[iq].clear();
        CL[iq].clear();
        sigmaL[iq].clear();
      }
      qinside.clear();
      a.clear();
      CL.clear();
      sigmaL.clear();
    }

    void CWaveFunction_optical::GetCLsigmaL() {
      int iq, ll;
      double prefactor, cmag;
      complex<double> CL2, gamow, etaq, cg;
      for (iq = 0; iq < int(etavec.size()); iq++) {
        etaq      = etavec[iq];
        gamow     = 2.0 * PI * etaq / (exp(2.0 * PI * etaq) - 1.0);
        CL2       = 1.0;
        prefactor = 1.0;
        for (ll = 0; ll <= llmax[iq]; ll++) {
          if (ll > 0) {
            prefactor *= 1.0 / ((2.0 * ll + 1.0) * (ll));
            CL2 *= (double(ll * ll) + etaq * etaq);
          }
          cmag = sqrt(real(gamow * CL2 * conj(gamow * CL2)));
          // phi=0.5*atan2(imag(CL2),real(CL2));
          CL[iq][ll]     = sqrt(cmag) * prefactor;  //*(cos(phi)+ci*sin(phi));
          cg             = CoulWave::cgamma(ll + 1.0 + ci * etaq);
          sigmaL[iq][ll] = atan2(imag(cg), real(cg));
        }
      }
    }

    void CWaveFunction_optical::GetAB() {
      int iq, ll;
      double etak, kR, K, qmag2, phi, delsigma;  // sigma is annihilation cross section
      vector<double> FL, GL, FLprime, GLprime;
      complex<double> qR;
      // Compare to https://en.wikipedia.org/wiki/Coulomb_wave_function HL is HL+ and HLstar is HL-
      // Difference from wiki is that FL = GL -i*HL here (matches convention in GSL library)
      complex<double> FLinside, FLprimeinside, HL, HLprime, HLstar, HLprimestar, numer, denom;
      for (iq = 0; iq < nqmax; iq++) {
        sigma_annihilation[iq] = 0.0;
        if (int(FL.size()) != llmax[iq] + 1) {
          FL.resize(llmax[iq] + 1);
          GL.resize(llmax[iq] + 1);
          FLprime.resize(llmax[iq] + 1);
          GLprime.resize(llmax[iq] + 1);
        }
        K           = (iq + 0.5) * delq;
        kR          = K * R / HBARC;
        etak        = ALPHA * q1q2 * mu / K;
        qmag2       = fabs(2.0 * mu * VI) + fabs(K * K - 2.0 * mu * VR);
        qinside[iq] = sqrt(qmag2);
        phi         = 0.5 * atan2(-2.0 * mu * VI, K * K - 2.0 * mu * VR);
        qinside[iq] *= exp(ci * phi);
        qR = qinside[iq] * R / HBARC;

        CoulWave::GetFGPrimeArray(llmax[iq], kR, etak, FL, GL, FLprime, GLprime);

        for (ll = 0; ll <= llmax[iq]; ll++) {
          GetF_Complex(iq, ll, qR, FLinside, FLprimeinside);
          HL          = FL[ll] - ci * GL[ll];
          HLstar      = conj(HL);
          HLprime     = FLprime[ll] - ci * GLprime[ll];
          HLprimestar = conj(HLprime);
          numer       = kR * FLinside * HLprime - qR * FLprimeinside * HL;
          denom       = qR * FLprimeinside * HLstar - kR * FLinside * HLprimestar;
          B[iq][ll]   = numer / denom;
          A[iq][ll]   = 0.5 * (HL + B[iq][ll] * HLstar) / FLinside;
          delsigma    = 2.0 * real(1.0 - B[iq][ll]) - real((1.0 - B[iq][ll]) * conj(1.0 - B[iq][ll]));
          sigma_annihilation[iq] += delsigma * (2.0 * ll + 1.0);
          /*
          if(ll==0){
                  printf("------------------------\n");
                  printf("kR=%g,qR=(%g,%g), etak=(%g,%g)\n",kR,real(qR),imag(qR),real(etak),imag(etak));
                  printf("iq=%d ll=%d : B=(%g,%g),
          A=(%g,%g)\n",iq,ll,real(B[iq][ll]),imag(B[iq][ll]),real(A[iq][ll]),imag(A[iq][ll])); printf("|B|^2=%g,
          |A|^2=%g\n",real(B[iq][ll]*conj(B[iq][ll])),real(A[iq][ll]*conj(A[iq][ll]))); double
          Bstrength=real((B[iq][ll]-1.0)*conj(B[iq][ll]-1.0)); printf("|B-1|^2=%g\n",Bstrength);
          }
          */
        }
        sigma_annihilation[iq] = PI * HBARC * HBARC * sigma_annihilation[iq] / (K * K);
      }
    }

    double CWaveFunction_optical::CalcPsiSquared(int iq, double r, double ctheta) {
      complex<double> psi, x;
      vector<double> FL, GL, FLprime, GLprime;
      complex<double> FLinside, FLprimeinside, HL, HLstar;
      double Amag, K = (0.5 + iq) * delq, x0, psisquared, etak, Pl;
      int ll;
      if (int(FL.size()) != llmax[iq] + 1) {
        FL.resize(llmax[iq] + 1);
        GL.resize(llmax[iq] + 1);
        FLprime.resize(llmax[iq] + 1);
        GLprime.resize(llmax[iq] + 1);
      }

      etak = ALPHA * q1q2 * mu / K;
      x0   = K * r / HBARC;
      psi  = planewave[iq]->planewave(r, ctheta);

      CoulWave::GetFGPrimeArray(llmax[iq], x0, etak, FL, GL, FLprime, GLprime);
      if (r < R) {
        x = qinside[iq] * r / HBARC;
        for (ll = 0; ll <= llmax[iq]; ll++) {
          Amag = fabs(real(A[iq][ll] * conj(A[iq][ll])));
          if (Amag > 1.0E-20 && Amag < 10000.0) {
            GetF_Complex(iq, ll, x, FLinside, FLprimeinside);
            Pl = SpherHarmonics::legendre(ll, ctheta);
            psi += (2.0 * ll + 1.0) * pow(ci, ll) * exp(-ci * sigmaL[iq][ll]) * Pl * (A[iq][ll] * FLinside / x0 - FL[ll] / x0);
          }
        }
      } else {
        for (ll = 0; ll <= llmax[iq]; ll++) {
          HL     = FL[ll] - ci * GL[ll];
          HLstar = conj(HL);
          Pl     = SpherHarmonics::legendre(ll, ctheta);
          psi += 0.5 * (2.0 * ll + 1.0) * pow(ci, ll) * exp(-ci * sigmaL[iq][ll]) * Pl * (B[iq][ll] - 1.0) * HLstar / x0;
        }
      }
      psisquared = real(psi * conj(psi));
      return psisquared;
    }


    // Meligy (1958, NPA)
    void CWaveFunction_optical::GetExpansionCoefficients() {
      int k, ll, iq;
      complex<double> etaq, x;
      for (iq = 0; iq < int(etavec.size()); iq++) {
        etaq = etavec[iq];
        for (ll = 0; ll <= llmax[iq]; ll++) {
          a[iq][ll][0] = 1.0;
          a[iq][ll][1] = 0.0;
          x            = etaq / (ll + 1.0);
          for (k = 2; k <= kmax; k++) {
            a[iq][ll][k] = -2.0 * x * (k - 1.0) * a[iq][ll][k - 1] - (1.0 + x * x) * a[iq][ll][k - 2];
            a[iq][ll][k] = a[iq][ll][k] / double(k * (2 * ll + k + 1));
          }
        }
      }
    }

    void CWaveFunction_optical::GetF_Complex(int iq, int L, complex<double> rho, complex<double>& F, complex<double>& Fprime) {
      int k;
      complex<double> Phi, Phiprime, sum = 1.0, sumprime = 0.0, dsum = 1.0, dsumprime, etaq;
      Fprime   = 0.0;
      etaq     = etavec[iq];
      Phi      = exp(etaq * rho / (L + 1.0));
      Phiprime = Phi * etaq / (L + 1.0);
      k        = 2;
      while (k < kmax && (k < 10 || real(dsum * conj(dsum)) > 1.0E-30)) {
        dsum      = a[iq][L][k] * pow(rho, k);
        dsumprime = a[iq][L][k] * double(k) * pow(rho, k - 1);
        sum += dsum;
        sumprime += dsumprime;
        k += 1;
      }
      F = Phi * sum * pow(rho, L + 1) * CL[iq][L];
      Fprime += (Phiprime * sum + Phi * sumprime) * pow(rho, L + 1) * CL[iq][L];
      Fprime += F * (L + 1.0) / rho;
    }

  }  // namespace ScottPratt
}  // namespace Hal
