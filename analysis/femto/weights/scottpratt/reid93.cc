#include "wavefunction.h"
namespace Hal {
  namespace ScottPratt {
    /*******************************************************************************
    ** Reid93 soft core phenomenological potential, updated version,
    ** including one-pion-exchange with neutral-pion and charged-pion
    ** masses; coupling f^2=0.075. Tensor potential is regularized
    ** to equal zero at r=0
    ** Reference: V.G.J. Stoks et al., Phys. Rev. C 49, 2950 (1994).
    **------------------------------------------------------------------------------
    ** C version 1.0: August 2001
    **           1.1: February 2004: a nasty bug is fixed
    **
    ** E-mail: info@nn-online.org
    ** WWW: http://nn-online.org
    **------------------------------------------------------------------------------
    ** IN:  const double *r       = distance in fermi
    **      const char *type      = reaction, "PP", "NP", "PN", or "NN"
    **      const char *pname     = name of the partial wave (see below),
    **                              maximum total angular momentum j = 9
    ** OUT: double *v11,*v12,*v22 = potential matrix v(2,2) in MeV on LSJ-basis
    **                              *v11 = v(1,1)
    **                              *v12 = v(1,2) = v(2,1)
    **                              *v22 = v(2,2)
    **                              For uncoupled channels *v12 = *v22 = 0.0
    **------------------------------------------------------------------------------
    ** The variable 'pname' contains the name of the partial wave in spectral
    ** notation:  singlets:                 1S0  1P1  1D2  1F3  1G4 ...
    **            triplets uncoupled:       3P0  3P1  3D2  3F3  3G4 ...
    **            triplets coupled:              3C1  3C2  3C3  3C4 ...
    ** where 3C1 denotes   3S1 - epsilon_1 - 3D1 channel
    **       3C2 denotes   3P2 - epsilon_2 - 3F2 channel
    **       ...
    *******************************************************************************/

    void
    WaveFunctionRoutines::creid93(const double* r, const char* pname, const char* type, double* v11, double* v12, double* v22) {
      const double a[5][5] = {{0.1756084, -14.14234, 151.8489, -686.823, 1104.157},
                              {-42.24976, 207.2246, -335.4364, -1.98925, -61.78469},
                              {29.12845, 151.169, 8.151964, 58.32103, -20.74743},
                              {-0.5840566, -10.2931, 22.63391, 23.16915, -1.959172},
                              {-2.608488, 10.90858, -0.4374212, -21.48862, -0.6584788}};
      const double b[5][5] = {{-22.34989, 255.1761, -1063.549, 1609.196, -3.505968},
                              {-4.248612, -5.352001, 182.7642, -392.7086, 58.12273},
                              {-2.904577, 38.02497, 0.3395927, 0.8318097, 1.923895},
                              {0.0913746, -12.74773, 145.86, -643.2461, 1022.217},
                              {-0.046164, 7.950192, -1.925573, 50.66234, 8.359896}};
      const double f0pi    = 0.075;
      const double fcpi    = 0.075;
      const double hbc     = 197.327053;
      const double mpi0    = 134.9739;
      const double mpic    = 139.5675;
      const double mpis    = 139.5675;

      const double r1 = 1.0;
      const double r2 = 2.0;
      const double r3 = 3.0;
      const double r4 = 4.0;
      const double r5 = 5.0;
      const double r6 = 6.0;
      const double r8 = 8.0;

      int n, s, l, j, i;
      double mpi, ri, rj, x, f;
      double vc = 0.0, vt = 0.0, vls, vspi, vspis, vtpi;


      /*
      **  Determine the quantumnumbers; doesn't validate input though!
      */
      s = (*(pname) == '1') ? 0 : 1;
      n = (toupper(*(pname + 1)) == 'C') ? 2 : 1;
      j = atoi(&pname[2]);
      if (!strcmp(pname, "3P0")) {
        l = 1;
      } else if (n == 2) {
        l = j - 1;
      } else {
        l = j;
      }
      i = (s + l + 1) % 2;

      ri = double(i);
      rj = double(j);

      /*
      **  OPE contribution to the potential
      */
      x     = mpi0 / hbc * *r;
      f     = f0pi * mpi0 * mpi0 * mpi0 / (mpis * mpis) / 3.0;
      vspis = f * r93_p(&r1, &r8, &x);
      vspi  = f * r93_y(&r1, &r8, &x);
      vtpi  = f * r93_z(&r1, &r8, &x);
      if (!strcmp(type, "NP") || !strcmp(type, "PN")) {
        x     = mpic / hbc * *r;
        f     = (4.0 * ri - 2.0) * fcpi * mpic * mpic * mpic / (mpis * mpis) / 3.0;
        vspis = f * r93_p(&r1, &r8, &x) - vspis;
        vspi  = f * r93_y(&r1, &r8, &x) - vspi;
        vtpi  = f * r93_z(&r1, &r8, &x) - vtpi;
      }


      /*
      **  Non-OPE contribution to the potential
      */
      mpi  = (mpi0 + 2.0 * mpic) / 3.0;
      x    = mpi / hbc * *r;
      *v11 = *v12 = *v22 = 0.0;

      if (!strcmp(pname, "1S0")) {
        if (!strcmp(type, "PP") || !strcmp(type, "NN")) {
          *v11 = a[0][0] * r93_y(&r2, &r8, &x) + a[0][1] * r93_y(&r3, &r8, &x) + a[0][2] * r93_y(&r4, &r8, &x)
                 + a[0][3] * r93_y(&r5, &r8, &x) + a[0][4] * r93_y(&r6, &r8, &x);
        } else if (!strcmp(type, "NP") || !strcmp(type, "PN")) {
          *v11 = b[0][0] * r93_y(&r3, &r8, &x) + b[0][1] * r93_y(&r4, &r8, &x) + b[0][2] * r93_y(&r5, &r8, &x)
                 + b[0][3] * r93_y(&r6, &r8, &x);
        }
      } else if (!strcmp(pname, "1D2")) {
        *v11 = a[1][0] * r93_y(&r4, &r8, &x) + a[1][1] * r93_y(&r5, &r8, &x) + a[1][2] * r93_y(&r6, &r8, &x);
      } else if (!strcmp(pname, "1G4")) {
        *v11 = a[1][3] * r93_y(&r3, &r8, &x);
      } else if (!strcmp(pname, "3P0")) {
        *v11 = a[2][0] * r93_y(&r3, &r8, &x) + a[2][1] * r93_y(&r5, &r8, &x) + a[1][4] * r93_z(&r3, &r8, &x) / 3.0;
      } else if (!strcmp(pname, "3P1")) {
        *v11 = a[2][2] * r93_y(&r3, &r8, &x) + a[2][3] * r93_y(&r5, &r8, &x) + a[2][4] * r93_z(&r3, &r8, &x) / 3.0;
      } else if (!strcmp(pname, "3F3")) {
        *v11 = a[3][4] * r93_y(&r3, &r8, &x);
      } else if (!strcmp(pname, "1P1")) {
        *v11 = b[1][0] * r93_y(&r3, &r8, &x) + b[1][1] * r93_y(&r4, &r8, &x) + b[1][2] * r93_y(&r5, &r8, &x)
               + b[1][3] * r93_y(&r6, &r8, &x);
      } else if (!strcmp(pname, "1F3")) {
        *v11 = b[0][4] * r93_y(&r3, &r8, &x) + b[1][4] * r93_y(&r5, &r8, &x);
      } else if (!strcmp(pname, "3D2")) {
        *v11 = b[2][0] * r93_y(&r3, &r8, &x) + b[2][1] * r93_y(&r5, &r8, &x) + b[2][2] * r93_z(&r3, &r8, &x) / 3.0;
      } else if (!strcmp(pname, "3G4")) {
        *v11 = b[2][3] * r93_y(&r3, &r8, &x);
      } else if (!strcmp(pname, "3C2") || !strcmp(pname, "3C4")) {
        vc = a[3][0] * r93_y(&r3, &r8, &x) + a[3][1] * r93_y(&r4, &r8, &x) + a[3][2] * r93_y(&r5, &r8, &x)
             + a[3][3] * r93_y(&r6, &r8, &x);
        vt = (a[4][0] * r93_z(&r4, &r8, &x) + a[4][1] * r93_z(&r6, &r8, &x)) / 3.0;
        if (!strcmp(pname, "3C2")) {
          vls = a[4][2] * r93_w(&r3, &r8, &x) + a[4][3] * r93_w(&r5, &r8, &x);
        } else /* "3C4" */
        {
          vls = a[4][4] * r93_w(&r3, &r8, &x);
        }
        *v11 = vc + (rj - 1.0) * vls - 2.0 * (rj - 1.0) / (2.0 * rj + 1.0) * vt;
        *v22 = vc - (rj + 2.0) * vls - 2.0 * (rj + 2.0) / (2.0 * rj + 1.0) * vt;
        *v12 = 6.0 * sqrt(rj * (rj + 1.0)) / (2.0 * rj + 1.0) * vt;
      } else if (!strcmp(pname, "3C1") || !strcmp(pname, "3C3")) {
        vc = b[3][0] * r93_y(&r2, &r8, &x) + b[3][1] * r93_y(&r3, &r8, &x) + b[3][2] * r93_y(&r4, &r8, &x)
             + b[3][3] * r93_y(&r5, &r8, &x) + b[3][4] * r93_y(&r6, &r8, &x);
        vt = (b[2][4] * r93_z(&r4, &r8, &x) + b[4][4] * r93_z(&r6, &r8, &x)) / 3.0;
        if (!strcmp(pname, "3C1")) {
          vls = b[4][0] * r93_w(&r3, &r8, &x) + b[4][1] * r93_w(&r5, &r8, &x);
        } else /* "3C3" */
        {
          vls = b[4][2] * r93_w(&r3, &r8, &x) + b[4][3] * r93_w(&r5, &r8, &x);
        }
        *v11 = vc + (rj - 1.0) * vls - 2.0 * (rj - 1.0) / (2.0 * rj + 1.0) * vt;
        *v22 = vc - (rj + 2.0) * vls - 2.0 * (rj + 2.0) / (2.0 * rj + 1.0) * vt;
        *v12 = 6.0 * sqrt(rj * (rj + 1.0)) / (2.0 * rj + 1.0) * vt;
      } else {
        switch (s) {
          case 0:
            switch (i) {
              case 1:
                *v11 = a[0][0] * r93_y(&r2, &r8, &x) + a[0][1] * r93_y(&r3, &r8, &x) + a[0][2] * r93_y(&r4, &r8, &x)
                       + a[0][3] * r93_y(&r5, &r8, &x) + a[0][4] * r93_y(&r6, &r8, &x);
                break;
              case 0:
                *v11 = b[1][0] * r93_y(&r3, &r8, &x) + b[1][1] * r93_y(&r4, &r8, &x) + b[1][2] * r93_y(&r5, &r8, &x)
                       + b[1][3] * r93_y(&r6, &r8, &x);
                break;
            }
            break;
          case 1:
            switch (i) {
              case 1:
                vc = a[3][0] * r93_y(&r3, &r8, &x) + a[3][1] * r93_y(&r4, &r8, &x) + a[3][2] * r93_y(&r5, &r8, &x)
                     + a[3][3] * r93_y(&r6, &r8, &x);
                vt = (a[4][0] * r93_z(&r4, &r8, &x) + a[4][1] * r93_z(&r6, &r8, &x)) / 3.0;
                break;
              case 0:
                vc = b[3][0] * r93_y(&r2, &r8, &x) + b[3][1] * r93_y(&r3, &r8, &x) + b[3][2] * r93_y(&r4, &r8, &x)
                     + b[3][3] * r93_y(&r5, &r8, &x) + b[3][4] * r93_y(&r6, &r8, &x);
                vt = (b[2][4] * r93_z(&r4, &r8, &x) + b[4][4] * r93_z(&r6, &r8, &x)) / 3.0;
                break;
            }
            switch (n) {
              case 1:
                if (l == (j - 1)) {
                  *v11 = vc - 2.0 * (rj - 1.0) / (2.0 * rj + 1.0) * vt;
                } else if (l == j) {
                  *v11 = vc + 2.0 * vt;
                } else if (l == (j + 1)) {
                  *v11 = vc - 2.0 * (rj + 2.0) / (2.0 * rj + 1.0) * vt;
                }
                break;
              case 2:
                *v11 = vc - 2.0 * (rj - 1.0) / (2.0 * rj + 1.0) * vt;
                *v22 = vc - 2.0 * (rj + 2.0) / (2.0 * rj + 1.0) * vt;
                *v12 = 6.0 * sqrt(rj * (rj + 1.0)) / (2.0 * rj + 1.0) * vt;
                break;
            }
            break;
        }
      }

      *v11 *= mpi;
      *v12 *= mpi;
      *v22 *= mpi;


      /*
      **  Add OPE and non-OPE potentials
      */
      switch (n) {
        case 1:
          if (s == 0) {
            if (l == 0) {
              *v11 += -3.0 * vspis;
            } else {
              *v11 += -3.0 * vspi;
            }
          } else if (l == j) {
            *v11 += vspi + 2.0 * vtpi;
          } else if (!strcmp(pname, "3P0")) {
            *v11 += vspi - 2.0 * (rj + 2.0) / (2.0 * rj + 1.0) * vtpi;
          }
          break;
        case 2:
          if (l == 0) {
            *v11 += vspis - 2.0 * (rj - 1.0) / (2.0 * rj + 1.0) * vtpi;
          } else {
            *v11 += vspi - 2.0 * (rj - 1.0) / (2.0 * rj + 1.0) * vtpi;
          }
          *v22 += vspi - 2.0 * (rj + 2.0) / (2.0 * rj + 1.0) * vtpi;
          *v12 += 6.0 * sqrt(rj * (rj + 1.0)) / (2.0 * rj + 1.0) * vtpi;
          break;
      }
      return;
    }
    /******************************************************************************/
    double WaveFunctionRoutines::r93_y(const double* n, const double* m, const double* x) {
      double y;

      if (*x < 0.0001) {
        y = -(*n) + (*m) / 2.0 + (*n) * (*n) / (2.0 * (*m));
      } else {
        y = exp(-(*n) * (*x)) / (*x) - exp(-(*m) * (*x)) / (*x) * (1.0 + ((*m) * (*m) - (*n) * (*n)) * (*x) / (2.0 * (*m)));
      }

      return y;
    }
    /******************************************************************************/
    double WaveFunctionRoutines::r93_p(const double* n, const double* m, const double* x) {
      double n2 = (*n) * (*n);
      double m2 = (*m) * (*m);
      double d, y;

      if (*x < 0.0001) {
        d = (*m) * (m2 - n2) / (2.0 * n2);
        y = -(*n) + (*m) - d + (*x) * (n2 / 2.0 + m2 / 2.0 + (*m) * d);
      } else {
        y = exp(-(*n) * (*x)) / (*x) - exp(-(*m) * (*x)) / (*x) * (1.0 + (m2 - n2) * (*m) * (*x) / (2.0 * n2));
      }
      return y;
    }
    /******************************************************************************/
    double WaveFunctionRoutines::r93_w(const double* n, const double* m, const double* x) {
      double n2 = (*n) * (*n);
      double m2 = (*m) * (*m);
      double x2, y;

      if (*x < 0.0001) {
        y = (2.0 * (*n) - 3.0 * (*m) + m2 * (*m) / n2) / 6.0 + (*x) * (2.0 * m2 - n2 - m2 * m2 / n2) / 8.0;
      } else {
        x2 = (*x) * (*x);
        y  = exp(-(*n) * (*x)) / (*x) * (1.0 / ((*n) * (*x)) + 1.0 / (n2 * x2))
            - exp(-(*m) * (*x)) / (*x) * (1.0 / ((*m) * (*x)) + 1.0 / (m2 * x2)) * m2 / n2
            - exp(-(*m) * (*x)) / (*x) * (m2 - n2) / (2.0 * n2);
      }
      return y;
    }
    /******************************************************************************/
    double WaveFunctionRoutines::r93_z(const double* n, const double* m, const double* x) {
      double n2 = (*n) * (*n);
      double m2 = (*m) * (*m);
      double x2, y;

      if (*x < 0.0001) {
        y = (*x) * (n2 + 3.0 * m2 * m2 / n2 - 4.0 * m2) / 8.0;
      } else {
        x2 = (*x) * (*x);
        y  = exp(-(*n) * (*x)) / (*x) * (1.0 + 3.0 / ((*n) * (*x)) + 3.0 / (n2 * x2))
            - exp(-(*m) * (*x)) / (*x) * (1.0 + 3.0 / ((*m) * (*x)) + 3.0 / (m2 * x2)) * m2 / n2
            - exp(-(*m) * (*x)) * (1.0 + 1.0 / ((*m) * (*x))) * *m * (m2 - n2) / (2.0 * n2);
      }
      return y;
    }
  }  // namespace ScottPratt
}  // namespace Hal
