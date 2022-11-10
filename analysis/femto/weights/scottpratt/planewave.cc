#include "constants.h"
#include "misc.h"
#include "sf.h"
#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    CPlaneWave::CPlaneWave(double etaset, int Q1Q2, double qset) {
      complex<double> a, b, a0, b0;
      complex<double> c1top1, c2top2, bot;
      complex<double> c1top2, c2top1;
      double xmax;
      int j, ix;
      ci   = complex<double>(0.0, 1.0);
      eta  = etaset;
      q1q2 = Q1Q2;
      q    = qset;
      delx = 0.02;

      xmax = 50.0;
      if ((q * 30.0 / HBARC) > xmax) xmax = delx * (floor(1.0 + (q * 30.0 / HBARC) / delx));
      nxmax      = int(floor(xmax / delx));
      hyperarray = new complex<double>[nxmax + 1];

      if (q1q2 != 0) {
        couly = CoulWave::cgamma(1.0 + ci * eta);
        couly = couly * exp(-0.5 * eta * PI);
        a     = -ci * eta;
        b     = 1.0;
        a0    = a;
        b0    = b;
        for (j = 1; j <= 60; j++) {
          chype[j] = a / (b * double(j));
          a        = a + 1.0;
          b        = b + 1.0;
        }
        c1top1 = 1.0;
        c2top1 = 1.0;
        c1top2 = 1.0;
        c2top2 = 1.0;
        bot    = 1.0;
        for (j = 1; j <= 10; j++) {
          c1top1    = c1top1 * (double(j) + a0 - 1.0);
          c2top1    = c2top1 * (double(j) - a0);
          c1top2    = c1top2 * (double(j) - b0 + a0);
          c2top2    = c2top2 * (double(j) + b0 - a0 - 1.0);
          bot       = bot * double(j);
          chype1[j] = (c1top1 * c1top2) / (bot);
          chype2[j] = (c2top1 * c2top2) / (bot);
        }
        cfact1 = 1.0 / (CoulWave::cgamma(b0 - a0));
        cfact2 = 1.0 / (CoulWave::cgamma(a0));
      }
      for (ix = 0; ix <= nxmax; ix++)
        hyperarray[ix] = hyper(-ci * eta, 1.0, ci * (delx * (ix + 0.5)));
    }

    complex<double> CPlaneWave::planewave(double r, double ctheta) {
      complex<double> answer;
      double zq, arg, x, wlow, whigh;
      int ixlow;
      /* See appendix of Messiah.  "cgamma" is the gamma function. "hyper" is
         appropriate hyperbolic function.  This is explained in Messiah's
         section on coul wave func.s. Notation should be explanatory
         when reading the book.
         This has been modified so that the outgoing waves correspond to a plane
         wave rather than the incoming waves */
      if (q1q2 != 0) {
        zq = -r * ctheta;  // (By flipping ctheta, then taking c.c. below, we get
        // wave that leaves with momentum q, rather than enters with q
        x     = q * (r - zq) / HBARC;
        ixlow = int(floor(-0.5 + x / delx));
        if (x > delx && ixlow < nxmax) {
          wlow   = (delx * (ixlow + 1) - x) / delx;
          whigh  = (x - delx * ixlow) / delx;
          answer = couly * (wlow * hyperarray[ixlow] + whigh * hyperarray[ixlow + 1]);
        } else {
          answer = couly * hyper(-ci * eta, 1.0, ci * x);
        }
        arg    = zq * q / HBARC;
        arg    = arg - 2.0 * PI * floor(arg / (2.0 * PI));
        answer = answer * (cos(arg) + ci * sin(arg));
        answer = conj(answer);
      } else {
        arg    = q * r * ctheta / HBARC;
        answer = cos(arg) + ci * sin(arg);
      }
      return answer;
    }

    /* **************************************************** */


    complex<double> CPlaneWave::hyper(complex<double> a, complex<double> b, complex<double> cz) {
      complex<double> cw1, cw2, cf1, cf2, czarg, czstarj, answer, delcf1, delcf2;
      double realcz, imagcz;
      const double rcrit = 12.0;
      double dmag;
      int j;
      realcz = real(cz);
      imagcz = imag(cz);
      dmag   = fabs(sqrt(realcz * realcz + imagcz * imagcz));
      if (dmag < rcrit) {
        cf1     = 1.0;
        czstarj = 1.0;
        for (j = 1; j <= 60; j++) {
          czstarj = czstarj * cz * chype[j];
          cf1     = cf1 + czstarj;
          realcz  = real(czstarj);
          imagcz  = imag(czstarj);
          if (fabs(sqrt(realcz * realcz + imagcz * imagcz)) < 1.0E-6) goto GOOD_ENOUGH;
        }
        printf("hyper not coverging!.\n");
      GOOD_ENOUGH:
        answer = cf1;
      } else {
        cf1    = 1.0;
        cf2    = 1.0;
        delcf1 = delcf2 = 1.0;
        for (j = 1; j <= 10; j++) {
          delcf1 = delcf1 * (-cz);
          cf1    = cf1 + chype1[j] / delcf1;
          delcf2 = delcf2 * cz;
          cf2    = cf2 + chype2[j] / delcf2;
        }
        cw1    = cf1 * cfact1 * (Misc::cpow(-cz, -a));
        czarg  = cz - ci * 2.0 * PI * floor(real(-ci * cz / (2.0 * PI)));
        cw2    = cf2 * cfact2 * Misc::cpow(cz, a - b) * Misc::cexp(czarg);
        answer = cw1 + cw2;
      }
      return answer;
    }
  }  // namespace ScottPratt
}  // namespace Hal
