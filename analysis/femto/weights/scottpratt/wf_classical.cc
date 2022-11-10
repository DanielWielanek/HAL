#include "constants.h"
#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_classical::CWaveFunction_classical() : delq(0.0) {}

    double CWaveFunction_classical::CalcPsiSquared(double q, double r, double ctheta, double m1, double m2, int q1q2) {
      double psisquared = 1.0;
      double mu, eratio, root, sign, Jacobian1, Jacobian2, q0ratio;
      mu     = m1 * m2 / (m1 + m2);
      eratio = 2.0 * mu * q1q2 * ALPHA * HBARC / (r * q * q);  // ratio of PE to E
      if (q1q2 == 0 || fabs(eratio) < 1.0E-8) {
        psisquared = 1.0;
      } else if (eratio != eratio) {
        psisquared = 0.0;
      } else {
        // printf("mu=%g, eratio=%g, q=%g, r=%g, q1q2=%d, ALPHA=%g, HBARC=%g\n",mu,eratio,q,r,q1q2,ALPHA,HBARC);
        root = 1.0 - 2.0 * eratio / (1.0 + ctheta);
        if (root <= 0.0 || root != root) {
          psisquared = 0.0;
        } else {
          root    = sqrt(root);
          q0ratio = sqrt(1.0 - eratio);

          sign = 1.0;

          Jacobian1 = 1.0 + sign * pow(eratio / ((1.0 + sign * root) * (1.0 + ctheta)), 2) / root;
          if (Jacobian1 != Jacobian1) {
            printf("J1=%g, q0ratio=%g, eratio=%g\n", Jacobian1, q0ratio, eratio);
            printf("ctheta=%g, root=%g\n", ctheta, root);
            printf("q=%g, r=%g, q1q2=%d, ctheta=%g\n", q, r, q1q2, ctheta);
            exit(1);
          }
          // Avoid dividing 0/0, instead use limit for ctheta0=0
          sign      = -1.0;
          Jacobian2 = 1.0 + sign * pow(eratio / ((1.0 + sign * root) * (1.0 + ctheta)), 2) / root;
          if (Jacobian2 != Jacobian2) {
            printf("J2=%g, q0ratio=%g, eratio=%g\n", Jacobian2, q0ratio, eratio);
            printf("ctheta=%g, root=%g\n", ctheta, root);
            printf("q=%g, r=%g, q1q2=%d, ctheta=%g\n", q, r, q1q2, ctheta);
            exit(1);
          }

          psisquared = fabs(Jacobian1) + fabs(Jacobian2);
        }
        // there is an integrable singularity as root->0. To reduce noise, psisquared is cut off at 1000.0
        psisquared = psisquared / sqrt(1.0 + 0.000001 * pow(psisquared - 1.0, 2));
      }
      // there is an integrable singularity as root->0. To reduce noise, psisquared is cut off at 100.0
      // psisquared=psisquared/sqrt(1.0+0.0001*pow(psisquared-1.0,2));
      return psisquared;
    }

    double CWaveFunction_classical::CalcPsiSquared(int iq, double r, double ctheta, double m1, double m2, int q1q2) {
      return CalcPsiSquared((iq + 0.5) * delq, r, ctheta, m1, m2, q1q2);
    }
  }  // namespace ScottPratt
}  // namespace Hal
