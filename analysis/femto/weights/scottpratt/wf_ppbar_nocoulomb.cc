#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_ppbar_nocoulomb::CWaveFunction_ppbar_nocoulomb(string parsfilename) : CWaveFunction() {
      ParsInit(parsfilename);
      nchannels = 0;
      VR        = parameters.getD("OPTICAL_VR", -8.26);
      VI        = parameters.getD("OPTICAL_VI", 25.99);
      a         = parameters.getD("OPTICAL_A", 1.892);
      double qmaga, qmag2, ka, k;
      complex<double> qa, numer, denom;
      vector<complex<double>> jl, hl, jlprime, hlprime;
      double phi;
      int ll, iq;
      m1 = m2 = MPROTON;
      mu      = MPROTON * 0.5;
      llmax.resize(nqmax);
      for (iq = 0; iq < nqmax; iq++) {
        qmaga = delq * iq * a / HBARC;
        if (qmaga > 2.5) {
          llmax[iq] = 4 + lrint(qmaga - 2);
        } else
          llmax[iq] = 4;
      }

      A.resize(nqmax);
      B.resize(nqmax);
      qinside.resize(nqmax);
      for (iq = 0; iq < nqmax; iq++) {
        A[iq].resize(llmax[iq] + 1);
        B[iq].resize(llmax[iq] + 1);
        for (ll = 0; ll <= llmax[iq]; ll++) {
          A[iq][ll] = B[iq][ll] = 0.0;
        }
      }

      for (iq = 0; iq < nqmax; iq++) {
        k           = (iq + 0.5) * delq;
        ka          = k * a / HBARC;
        qmag2       = 2.0 * mu * VI + fabs(k * k - 2.0 * mu * VR);
        qinside[iq] = sqrt(qmag2);
        qmaga       = sqrt(qmag2) * a / HBARC;
        phi         = 0.5 * atan2(2.0 * mu * VI, k * k - 2.0 * mu * VR);
        qinside[iq] *= exp(ci * phi);
        if (sin(phi) < 0.0) {
          printf("what???? sin(phi)=%g, phi=%g\n", sin(phi), phi * 180.0 / PI);
          exit(1);
        }
        qa = qmaga * cos(phi) + ci * qmaga * sin(phi);
        GetBessel(qa, iq, jl, jlprime);
        GetHankel(ka, iq, hl, hlprime);
        for (ll = 0; ll <= llmax[iq]; ll++) {
          denom     = hl[ll] * jlprime[ll] * qa - ka * jl[ll] * hlprime[ll];
          numer     = conj(hlprime[ll] * ka) * jl[ll] - conj(hl[ll]) * jlprime[ll] * qa;
          B[iq][ll] = numer / denom;
          A[iq][ll] = 0.5 * (B[iq][ll] * hl[ll] + conj(hl[ll])) / jl[ll];
          if (ll == 0) {
            printf("K=%g\n", k);
            printf("A[%d][L=%d]=(%g,%g)\n", iq, ll, real(A[iq][ll]), imag(A[iq][ll]));
            printf("B[%d][L=%d]=(%g,%g)\n", iq, ll, real(B[iq][ll]), imag(B[iq][ll]));
          }
        }
      }
    }

    double CWaveFunction_ppbar_nocoulomb::CalcPsiSquared(int iq, double r, double ctheta) {
      complex<double> psi, Yl, x;
      double Amag, q = (0.5 + iq) * delq, x0, psisquared;
      int ll;
      x0 = q * r / HBARC;
      vector<complex<double>> jl, hl, jlprime, hlprime, jl0, jlprime0;
      psi = exp(ci * x0 * ctheta);
      if (r < a) {
        x = qinside[iq] * r / HBARC;
        GetBessel(x0, iq, jl0, jlprime0);
        GetBessel(x, iq, jl, jlprime);
        for (ll = 0; ll <= llmax[iq]; ll++) {
          Amag = fabs(real(A[iq][ll] * conj(A[iq][ll])));
          if (Amag > 1.0E-20 && Amag < 10000.0) {
            Yl = SpherHarmonics::legendre(ll, ctheta);
            psi += (2.0 * ll + 1.0) * pow(ci, ll) * Yl * (A[iq][ll] * jl[ll] - jl0[ll]);
          }
        }
      } else {
        GetHankel(x0, iq, hl, hlprime);
        for (ll = 0; ll <= llmax[iq]; ll++) {
          Yl = SpherHarmonics::legendre(ll, ctheta);
          psi += 0.5 * (2.0 * ll + 1.0) * pow(ci, ll) * Yl * (B[iq][ll] - 1.0) * hl[ll];
        }
      }
      psisquared = real(psi * conj(psi));
      return psisquared;
    }

    void CWaveFunction_ppbar_nocoulomb::GetBessel(complex<double> x,
                                                  int iq,
                                                  vector<complex<double>>& jl,
                                                  vector<complex<double>>& jlprime) {
      int ll;
      complex<double> j1, prefactor, xarg, factll0, factll, djl;
      double x2 = real(x * conj(x)), sign;
      int k, kmax = 12;

      /*
      double fact[50],doublefact[50];
      fact[0]=fact[1]=doublefact[0]=doublefact[1]=1.0;
      for(k=2;k<50;k++){
      fact[k]=fact[k-1]*k;
      doublefact[k]=doublefact[k-2]*k;
      } */

      if (int(jl.size()) <= llmax[iq]) {
        jl.resize(llmax[iq] + 1);
        jlprime.resize(llmax[iq] + 1);
      }
      if (x2 > 0.25) {
        jl[0]      = sin(x) / x;
        j1         = sin(x) / (x * x) - cos(x) / x;
        jlprime[0] = -j1;
        if (llmax[iq] > 0) {
          jl[1]      = j1;
          jlprime[1] = jl[0] - 2.0 * jl[1] / x;
        }
        for (ll = 2; ll <= llmax[iq]; ll++) {
          jl[ll]      = -jl[ll - 2] + (2.0 * ll - 1.0) * jl[ll - 1] / x;
          jlprime[ll] = jl[ll - 1] - (ll + 1.0) * jl[ll] / x;
        }
      } else {
        factll0 = 1.0;
        xarg    = 0.5 * x * x;
        for (ll = 0; ll <= llmax[iq]; ll++) {
          prefactor = pow(x, ll);
          jl[ll]    = 0.0;
          sign      = 1.0;
          factll0   = factll0 / (2 * ll + 1.0);
          factll    = factll0;
          k         = 0;
          do {
            djl = prefactor * factll;
            jl[ll] += djl;
            sign   = -sign;
            factll = factll * xarg * sign / ((k + 1.0) * (2.0 * k + 2.0 * ll + 3.0));
            k += 1;
          } while (k <= kmax && fabs(real(djl * conj(djl))) > 1.0E-20);
        }
        if (llmax[iq] == 0) {
          j1         = sin(x) / (x * x) - cos(x) / x;
          jlprime[0] = -j1;
        } else
          jlprime[0] = -jl[1];
        for (ll = 1; ll <= llmax[iq]; ll++) {
          jlprime[ll] = jl[ll - 1] - (ll + 1.0) * jl[ll] / x;
        }
      }
    }

    void
    CWaveFunction_ppbar_nocoulomb::GetHankel(double x, int iq, vector<complex<double>>& hl, vector<complex<double>>& hlprime) {
      double j, y, j1, y1;
      int ll;
      if (int(hl.size()) <= llmax[iq]) {
        hl.resize(llmax[iq] + 1);
        hlprime.resize(llmax[iq] + 1);
      }
      for (ll = 0; ll <= llmax[iq]; ll++) {
        j      = gsl_sf_bessel_jl(ll, x);
        y      = gsl_sf_bessel_yl(ll, x);
        hl[ll] = j + ci * y;
        if (ll == 1) {
          j1 = j;
          y1 = y;
        }
      }
      if (llmax[iq] == 0) {
        j1 = gsl_sf_bessel_jl(1, x);
        y1 = gsl_sf_bessel_yl(1, x);
      }
      hlprime[0] = -(j1 + ci * y1);
      for (ll = 1; ll <= llmax[iq]; ll++) {
        hlprime[ll] = hl[ll - 1] - (ll + 1.0) * hl[ll] / x;
      }
    }

    CWaveFunction_ppbar_nocoulomb::~CWaveFunction_ppbar_nocoulomb() {}
  }  // namespace ScottPratt
}  // namespace Hal
