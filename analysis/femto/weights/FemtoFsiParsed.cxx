/*
 * FemtoFsiParsed.cxx
 *
 *  Created on: 30 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoFsiParsed.h"

#include <TMath.h>
#include <iostream>

namespace Hal {
  FemtoFsiParsed::FemtoFsiParsed() {
    fFsiAapi    = {.26,   .25,   .348,  -.0637, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                -.028, -.082, .2795, -.0086, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
    fFsiApin    = {1.2265, 15.63, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                -.75,   76.88, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
    fFsiAand    = {-3.295,  -881.9, 4537., 28645.,  0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                -13.837, 11.505, 0.,    10.416,  0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                -32.18,  10.14,  0.,    0.,      0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                -60.213, 13.33,  0.,    -70.309, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
    fFsiAadd    = {1061.7, .003194, 568.49, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                0.,     0.,      0.,     0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                -1085., 19870.,  0.,     0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
    fFsiAakk    = {.247677, .01947977, .2997015, .9604, .96511, .094, .11, .333, .222};
    fsi_aapap__ = {-.94, -1.98, .1, -1.4, .37, .1, .3, .267, -.01, 1.66, .553, -.01};
    fsi_fdh__   = {
      17.,      7.8,      23.7,     2230.1218,  .225,     .081,     -.063,    -.65,     -2.73,     .137,     -.071,    -.148,
      .112,     1e-6,     1e-6,     -.36,       1e-6,     1e-6,     1.344,    1e-6,     1e-6,      1e-6,     1e-6,     1e-6,
      1e-6,     -5.628,   2.18,     2.4,        2.81,     .001,     -5.4,     -5.4,     -5.4,      0.,       0.,       0.,
      0.,       -6.35,    -11.88,   0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       1.93,      1.84,     .5,       .001,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      2.7,      2.8,      2.7,      1.12139906, -44.36,   64.,      784.9,    477.9,    2.27,      0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       -69.973,  0.,       0.,        0.,       0.,       0.,
      0.,       3.529,    3.19,     3.15,       2.95,     0.,       1.7,      1.7,      1.7,       0.,       0.,       0.,
      0.,       2.,       2.63,     0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       3.35,      3.37,     2.95,     0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      .1149517, .1046257, .1148757, .118601,    .7947389, 2.281208, 8.7,      .4,       .1561219,  0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       .1013293, .1020966,   .1080476, .001,     .1847221, .1847221, .1847221,  0.,       0.,       0.,
      0.,       .4,       .1150687, 0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       .09736083, .0970831, .1080476, .001,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      2.545739, 2.779789, 2.585795, 5.023544,   .124673,  .392518,  .09,      2.,       4.058058,  0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       2.252623, 2.278575,   2.234089, .001,     2.003144, 2.003144, 2.003144,  0.,       0.,       0.,
      0.,       2.,       4.132163, 0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       2.272703,  2.256355, 2.234089, .001,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.,
      0.,       0.,       0.,       0.,         0.,       0.,       0.,       0.,       0.,        0.,       0.,       0.};
    fsi_rhoh__ = {.25, .25, .25, 1.,    1.,  1., 1.,  .3333, .3333, 1.,  1.,  1.,  1.,  1.,    1.,  1., 1.,  .1111, 1.,    .25,
                  1.,  1.,  1.,  .3333, .25, 1., .25, .25,   .25,   .25, .75, .75, .75, 0.,    0.,  0., 0.,  .6667, .6667, 0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  .3333, 0.,    .75, 0.,  0.,  0.,  .6667, .75, 0., .75, .75,   .75,   .75,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  .5556, 0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,
                  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.,  0.,  0.,  0.,  0.,    0.,  0., 0.,  0.,    0.,    0.};
    fsi_amch__ = {
      .93956563,  .93827231, .93956563, 3.72737978, .13957,     .13498,     .13957,    .93956563,  .93827231,  .13957,
      .13957,     .13957,    .13957,    .493677,    .493677,    .493677,    .493677,   1.87561339, 1.87561339, 2.80892165,
      2.80892165, .497672,   .497672,   1.87561339, .93827231,  .93827231,  .93827231, .93956563,  1.115684,   .93827231,
      .93956563,  .93827231, .93827231, 3.72737978, .13957,     .13498,     .13957,    1.87561339, 1.87561339, .493677,
      .493677,    .93827231, .93827231, .493677,    .493677,    .93827231,  .93827231, 1.87561339, 3.72737978, 2.80892165,
      3.72737978, .497672,   .497672,   2.80892165, 2.80892165, 3.72737978, 1.115684,  1.115684,   1.115684,   .93827231,
      0.,         1.,        0.,        2.,         1.,         0.,         1.,        0.,         1.,         1.,
      1.,         1.,        -1.,       1.,         1.,         1.,         -1.,       1.,         1.,         1.,
      1.,         0.,        0.,        1.,         1.,         1.,         1.,        0.,         0.,         1.,
      0.,         1.,        1.,        2.,         -1.,        0.,         1.,        1.,         1.,         -1.,
      1.,         1.,        1.,        -1.,        1.,         1.,         1.,        1.,         2.,         1.,
      2.,         0.,        0.,        1.,         1.,         2.,         0.,        0.,         0.,         -1.,
      2,          2,         2,         1,          1,          1,          1,         2,          2,          1,
      1,          1,         1,         1,          1,          1,          1,         3,          1,          2,
      1,          1,         1,         2,          2,          1,          2,         2,          2,          2};
    c__1   = 1;
    c_b5   = {(Double_t) 1., (Double_t) 0.};
    c_b2   = {1., 0.};
    c__2   = 2;
    c_b33  = {.6667, 0.};
    c_b34  = {.3333, 0.};
    ifirst = 0;
    fEta   = 0;
    fIch   = 0;
    fJrat  = 0;
  }

  void FemtoFsiParsed::Fsiw(int j, Double_t& weif, Double_t& wei, Double_t& wein) {
    Double_t c1n, c2n;
    c1n = fFsiPoc.c1 * fFsiPoc.cn;
    if (c1n != 0.) { fFsiPoc.ac1 = 137.036 / (c1n * fFsiMom.e1); }
    /* m1-->E1 */
    c2n = fFsiPoc.c2 * fFsiPoc.cn;
    if (c2n != 0.) { fFsiPoc.ac2 = 137.036 / (c2n * fFsiMom.e2); }
    /* ----------------------------------------------------------- */
    /* m2-->E2 */

    Fsipn(weif);

    /* weight due to particle-nucleus FSI */
    fJrat = 0;
    Boosttoprf();
    Fsiwf(wei);

    /* weight due to particle-particle-nucleus FSI */
    wein = wei;
    if (fFsiNs.i3c * j != 0) {
      fFsiFppn.ff12.r = 1., fFsiFppn.ff12.i = 0.;
      fFsiFppn.ff21.r = 1., fFsiFppn.ff21.i = 0.;
      fJrat = 1;
      Vz(wein);

      /* weight due to particle-particle FSI */
    }
  }

  void FemtoFsiParsed::Ltran(Double_t* p0, Double_t* p, Double_t* ps) {
    Double_t d1, d2, d3;

    /* Local variables */
    static Double_t h, am0, pp0, p0s, epm;

    /* ==>calculating particle 4-momentum PS={PSX,PSY,PSZ,ES} */
    /*   in rest frame of a system 0 with 4-momentum P0={P0X,P0Y,P0Z,E0} */
    /*   from its 4-momentum P={PX,PY,PZ,E} */
    /* ----------------------------------------------------------------------- */
    /* Parameter adjustments */
    --ps;
    --p;
    --p0;

    /* Function Body */
    /* Computing 2nd power */
    d1 = p0[1];
    /* Computing 2nd power */
    d2 = p0[2];
    /* Computing 2nd power */
    d3  = p0[3];
    p0s = d1 * d1 + d2 * d2 + d3 * d3;
    /* Computing 2nd power */
    d1    = p0[4];
    am0   = TMath::Sqrt(d1 * d1 - p0s);
    epm   = p0[4] + am0;
    pp0   = p[1] * p0[1] + p[2] * p0[2] + p[3] * p0[3];
    h     = (pp0 / epm - p[4]) / am0;
    ps[1] = p[1] + p0[1] * h;
    ps[2] = p[2] + p0[2] * h;
    ps[3] = p[3] + p0[3] * h;
    ps[4] = (p0[4] * p[4] - pp0) / am0;
  }

  void FemtoFsiParsed::Ltranb(Double_t* p0, Double_t* ps, Double_t* p) {
    Double_t d__1, d__2, d__3;

    /* Local variables */
    Double_t hs, am0, p0s, epm, psp0;

    /* ==>calculating particle 4-momentum P={PX,PY,PZ,E} */
    /*   from its 4-momentum PS={PSX,PSY,PSZ,ES} */
    /*   in rest frame of a system 0 with 4-momentum P0={P0X,P0Y,P0Z,E0} */
    /* ----------------------------------------------------------------------- */
    /* Parameter adjustments */
    --p;
    --ps;
    --p0;

    /* Function Body */
    /* Computing 2nd power */
    d__1 = p0[1];
    /* Computing 2nd power */
    d__2 = p0[2];
    /* Computing 2nd power */
    d__3 = p0[3];
    p0s  = d__1 * d__1 + d__2 * d__2 + d__3 * d__3;
    /* Computing 2nd power */
    d__1 = p0[4];
    am0  = TMath::Sqrt(d__1 * d__1 - p0s);
    epm  = p0[4] + am0;
    psp0 = ps[1] * p0[1] + ps[2] * p0[2] + ps[3] * p0[3];
    hs   = (psp0 / epm + ps[4]) / am0;
    p[1] = ps[1] + p0[1] * hs;
    p[2] = ps[2] + p0[2] * hs;
    p[3] = ps[3] + p0[3] * hs;
    p[4] = (p0[4] * ps[4] + psp0) / am0;
  }

  void FemtoFsiParsed::Ltran12() {
    Double_t d__1, d__2, d__3;
    /* Local variables */
    Double_t h1, ee, p112, p1s, p2s, p12s;

    /* ==>calculating particle momentum in PRF {EE,PPX,PPY,PPZ} from */
    /* -  the momentum of the first particle {E1,P1X,P1Y,P1Z) in NRF */
    /* part. momenta in NRF */
    /* 4-coord. of emis. points in NRF */
    /*   fm --> 1/GeV */
    fFsiCoor.x1 *= fFsiCons.w;
    fFsiCoor.y1 *= fFsiCons.w;
    fFsiCoor.z1 *= fFsiCons.w;
    fFsiCoor.t1 *= fFsiCons.w;
    fFsiCoor.x2 *= fFsiCons.w;
    fFsiCoor.y2 *= fFsiCons.w;
    fFsiCoor.z2 *= fFsiCons.w;
    fFsiCoor.t2 *= fFsiCons.w;
    /*   calculating Ri, Pi and Ei */
    fFsiCoor.r1 = TMath::Sqrt(fFsiCoor.x1 * fFsiCoor.x1 + fFsiCoor.y1 * fFsiCoor.y1 + fFsiCoor.z1 * fFsiCoor.z1);
    fFsiCoor.r2 = TMath::Sqrt(fFsiCoor.x2 * fFsiCoor.x2 + fFsiCoor.y2 * fFsiCoor.y2 + fFsiCoor.z2 * fFsiCoor.z2);
    p1s         = fFsiMom.p1x * fFsiMom.p1x + fFsiMom.p1y * fFsiMom.p1y + fFsiMom.p1z * fFsiMom.p1z;
    p2s         = fFsiMom.p2x * fFsiMom.p2x + fFsiMom.p2y * fFsiMom.p2y + fFsiMom.p2z * fFsiMom.p2z;
    fFsiMom.p1  = TMath::Sqrt(p1s);
    fFsiMom.p2  = TMath::Sqrt(p2s);
    fFsiMom.e1  = TMath::Sqrt(fFsiPoc.am1 * fFsiPoc.am1 + p1s);
    fFsiMom.e2  = TMath::Sqrt(fFsiPoc.am2 * fFsiPoc.am2 + p2s);
    /* ----------------------------------------------------------------------- */
    fSi_p12.e12  = fFsiMom.e1 + fFsiMom.e2;
    fSi_p12.p12x = fFsiMom.p1x + fFsiMom.p2x;
    fSi_p12.p12y = fFsiMom.p1y + fFsiMom.p2y;
    fSi_p12.p12z = fFsiMom.p1z + fFsiMom.p2z;
    /* Computing 2nd power */
    d__1 = fSi_p12.p12x;
    /* Computing 2nd power */
    d__2 = fSi_p12.p12y;
    /* Computing 2nd power */
    d__3 = fSi_p12.p12z;
    p12s = d__1 * d__1 + d__2 * d__2 + d__3 * d__3;
    /* Computing 2nd power */
    d__1         = fSi_p12.e12;
    fSi_p12.am12 = TMath::Sqrt(d__1 * d__1 - p12s);
    fSi_p12.epm  = fSi_p12.e12 + fSi_p12.am12;
    fSi_p12.p12  = TMath::Sqrt(p12s);
    p112         = fFsiMom.p1x * fSi_p12.p12x + fFsiMom.p1y * fSi_p12.p12y + fFsiMom.p1z * fSi_p12.p12z;
    h1           = (p112 / fSi_p12.epm - fFsiMom.e1) / fSi_p12.am12;
    fFsiPrf.ppx  = fFsiMom.p1x + fSi_p12.p12x * h1;
    fFsiPrf.ppy  = fFsiMom.p1y + fSi_p12.p12y * h1;
    fFsiPrf.ppz  = fFsiMom.p1z + fSi_p12.p12z * h1;
    ee           = (fSi_p12.e12 * fFsiMom.e1 - p112) / fSi_p12.am12;
    /* Computing 2nd power */
    d__1 = ee;
    /* Computing 2nd power */
    d__2        = fFsiPoc.am1;
    fFsiPrf.aks = d__1 * d__1 - d__2 * d__2;
    fFsiPrf.ak  = TMath::Sqrt(fFsiPrf.aks);
    /* W      WRITE(6,38)'AK ',AK,'K ',PPX,PPY,PPZ,EE */
    /* L38: */
  }

  void FemtoFsiParsed::Fsipn(Double_t& weif) {
    /* System generated locals */
    Double_t d__1, d__2;
    complex z__1, z__2;
    /* Local variables */
    Double_t hs, xh;
    Double_t rhos;

    /*  calculating particle-nucleus Coulomb Wave functions FFij */
    /* part. momenta in NRF */
    /* 4-coord. of emis. points in NRF */
    fFsiFppn.ff12.r = 1., fFsiFppn.ff12.i = 0.;
    fFsiFppn.ff21.r = 1., fFsiFppn.ff21.i = 0.;
    /*      ACH=1.D0 */
    /*      WEIF=1.D0 */
    if (fFsiNs.i3c == 0) { return; }
    fIch = (int) fFsiPoc.c1;
    if (fIch == 0) { goto L11; }
    xh              = fFsiPoc.ac1 * fFsiMom.p1;
    fFsiAch._1.ach  = Acp(xh);
    fFsiAch._1.achr = TMath::Sqrt(fFsiAch._1.ach);
    fEta            = 0.;
    if (xh != 0.) { fEta = 1 / xh; }
    rhos = fFsiMom.p1 * fFsiCoor.r1;
    hs   = fFsiCoor.x1 * fFsiMom.p1x + fFsiCoor.y1 * fFsiMom.p1y + fFsiCoor.z1 * fFsiMom.p1z;
    Ff1(z__2, rhos, hs);
    z__1.r = fFsiFppn.ff12.r * z__2.r - fFsiFppn.ff12.i * z__2.i, z__1.i = fFsiFppn.ff12.r * z__2.i + fFsiFppn.ff12.i * z__2.r;
    fFsiFppn.ff12.r = z__1.r, fFsiFppn.ff12.i = z__1.i;
    if (fFsiNs.iqs == 0) { goto L11; }
    rhos = fFsiMom.p1 * fFsiCoor.r2;
    hs   = fFsiCoor.x2 * fFsiMom.p1x + fFsiCoor.y2 * fFsiMom.p1y + fFsiCoor.z2 * fFsiMom.p1z;
    Ff1(z__2, rhos, hs);
    z__1.r = fFsiFppn.ff21.r * z__2.r - fFsiFppn.ff21.i * z__2.i, z__1.i = fFsiFppn.ff21.r * z__2.i + fFsiFppn.ff21.i * z__2.r;
    fFsiFppn.ff21.r = z__1.r, fFsiFppn.ff21.i = z__1.i;
  L11:
    fIch = (int) fFsiPoc.c2;
    if (fIch == 0) { goto L10; }
    xh              = fFsiPoc.ac2 * fFsiMom.p2;
    fFsiAch._1.ach  = Acp(xh);
    fFsiAch._1.achr = TMath::Sqrt(fFsiAch._1.ach);
    fEta            = 0.;
    if (xh != 0.) { fEta = 1 / xh; }
    rhos = fFsiMom.p2 * fFsiCoor.r2;
    hs   = fFsiCoor.x2 * fFsiMom.p2x + fFsiCoor.y2 * fFsiMom.p2y + fFsiCoor.z2 * fFsiMom.p2z;
    Ff1(z__2, rhos, hs);
    z__1.r = fFsiFppn.ff12.r * z__2.r - fFsiFppn.ff12.i * z__2.i, z__1.i = fFsiFppn.ff12.r * z__2.i + fFsiFppn.ff12.i * z__2.r;
    fFsiFppn.ff12.r = z__1.r, fFsiFppn.ff12.i = z__1.i;
    /* W      WRITE(6,41)'AC2 ',AC2,'ACH ',ACH,'ETA ',ETA,'RHOS ',RHOS,'HS ',HS */
    /* L41: */
    /* W      WRITE(6,40)'FF12 ',DREAL(FF12),DIMAG(FF12) */
    if (fFsiNs.iqs == 0) { goto L10; }
    rhos = fFsiMom.p2 * fFsiCoor.r1;
    hs   = fFsiCoor.x1 * fFsiMom.p2x + fFsiCoor.y1 * fFsiMom.p2y + fFsiCoor.z1 * fFsiMom.p2z;
    Ff1(z__2, rhos, hs);
    z__1.r = fFsiFppn.ff21.r * z__2.r - fFsiFppn.ff21.i * z__2.i, z__1.i = fFsiFppn.ff21.r * z__2.i + fFsiFppn.ff21.i * z__2.r;
    fFsiFppn.ff21.r = z__1.r, fFsiFppn.ff21.i = z__1.i;
  /* W      WRITE(6,41)'AC1 ',AC1,'ACH ',ACH,'ETA ',ETA,'RHOS ',RHOS,'HS ',HS */
  /* W      WRITE(6,40)'FF21 ',DREAL(FF21),DIMAG(FF21) */
  /* L40: */
  L10:
    /*  WEIF = the weight due to the Coulomb particle-nucleus interaction */
    /* Computing 2nd power */
    d__1 = fFsiFppn.ff12.r;
    /* Computing 2nd power */
    d__2 = d_imag(&fFsiFppn.ff12);
    weif = d__1 * d__1 + d__2 * d__2;
    if (fFsiNs.iqs == 1) {
      /* Computing 2nd power */
      d__1 = fFsiFppn.ff21.r;
      /* Computing 2nd power */
      d__2 = d_imag(&fFsiFppn.ff21);
      weif = (weif + d__1 * d__1 + d__2 * d__2) * .5;
    }
  }

  FemtoFsiParsed::~FemtoFsiParsed() {
    // TODO Auto-generated destructor stub
  }

  Double_t FemtoFsiParsed::Gpipi(Double_t x, int j) {
    /* System generated locals */
    Double_t ret_val, d__1;

    /* Local variables */
    Double_t om, xx;

    /* --- GPIPI = k*COTG(DELTA), X=k^2 */
    /* --  J=1(2) corresponds to isospin=0(2) */
    om      = TMath::Sqrt(x + fsi_c__._1.ams);
    xx      = x / fsi_c__._1.ams;
    ret_val = om / fFsiAapi.e_1[j * 20 - 20];
    /* Computing 2nd power */
    d__1 = fFsiAapi.e_1[j * 20 - 20];
    ret_val *= (fFsiAapi.e_1[j * 20 - 18] - d__1 * d__1) * xx + 1 + fFsiAapi.e_1[j * 20 - 17] * xx * xx;
    ret_val /= (fFsiAapi.e_1[j * 20 - 18] + fFsiAapi.e_1[j * 20 - 19] / fFsiAapi.e_1[j * 20 - 20]) * xx + 1;
    return ret_val;
  } /* gpipi_ */

  Double_t FemtoFsiParsed::Gpin(Double_t x, int j) {
    /* System generated locals */
    Double_t ret_val;

    /* --- GPIN = k*COTG(DELTA), X=k^2 */
    /* --  J=1(2) corresponds to piN isospin=1/2(3/2) */
    ret_val = 1 / fFsiApin.e_1[j * 20 - 20] + fFsiApin.e_1[j * 20 - 19] * .5 * x;
    return ret_val;
  } /* gpin_ */

  Double_t FemtoFsiParsed::Gnd(Double_t x, int j) {
    /* System generated locals */
    Double_t ret_val;

    /* Local variables */
    int i__;
    Double_t xx;

    /* --- GND = k*COTG(DELTA), X=k^2 */
    /* --- J=1(2) corresp. to nd(pd), S=1/2, */
    /* --- J=3(4) corresp. to nd(pd), S=3/2 */
    xx      = x;
    ret_val = 1 / fFsiAand.e_1[j * 20 - 20] + fFsiAand.e_1[j * 20 - 19] * .5 * x;
    for (i__ = 4; i__ <= 4; ++i__) {
      xx *= x;
      /* L1: */
      ret_val += fFsiAand.e_1[i__ + j * 20 - 21] * xx;
    }
    ret_val /= fFsiAand.e_1[j * 20 - 18] * x + 1;
    return ret_val;
  } /* gnd_ */

  Double_t FemtoFsiParsed::Gdd(Double_t x, int j) {
    /* System generated locals */
    Double_t ret_val;
    /* Local variables */
    Double_t e, er, tand;

    /* --- GDD = k*COTG(DELTA), X=k^2 */
    /* --- J=1,2,3 corresp. to S=0,1,2 */
    e  = x / 2 / fsi_c__._2.am;
    er = TMath::Sqrt(e);
    if (j == 1) {
      ret_val = er * (fFsiAadd.e_1[0] * exp(-e / fFsiAadd.e_1[1]) - fFsiAadd.e_1[2]);
      ret_val /= fFsiCons.dr;
      /* from degree to radian */
      tand = TMath::Tan(ret_val);
      if (tand == 0.) { tand = 1e-10; }
      ret_val = TMath::Sqrt(x) / tand;
    }
    if (j == 2) { ret_val = 1e10; }
    if (j == 3) {
      ret_val = er * (fFsiAadd.e_1[40] + fFsiAadd.e_1[41] * e);
      ret_val /= fFsiCons.dr;
      /* from degree to radian */
      tand = TMath::Tan(ret_val);
      if (tand == 0.) { tand = 1e-10; }
      ret_val = TMath::Sqrt(x) / tand;
    }
    return ret_val;
  } /* gdd_ */

  Double_t FemtoFsiParsed::DfrSin(Double_t x) {
    /* Initialized data */

    Double_t c0     = 1.2533141373155003;
    int nb          = 15;
    int nc1         = 25;
    int nc2         = 28;
    Double_t b[16]  = {.630414043145705392,
                      -.423445114057053335,
                      .376171726433436566,
                      -.162494891545095674,
                      .038222557786330087,
                      -.005645634771321909,
                      5.74549519768974e-4,
                      -4.287071532102e-5,
                      2.451207499233e-6,
                      -1.10988418409e-7,
                      4.082497317e-9,
                      -1.24498302e-10,
                      3.200484e-12,
                      -7.0324e-14,
                      1.336e-15,
                      -2.2e-17};
    Double_t c1[26] = {.990560479373497549,
                       -.012183509831478997,
                       -.00248274282311306,
                       2.66094952647247e-4,
                       -1.079068987406e-6,
                       -4.883681753933e-6,
                       9.99055266368e-7,
                       -7.5092717372e-8,
                       -1.9079487573e-8,
                       9.090797293e-9,
                       -1.966236033e-9,
                       1.64772911e-10,
                       6.3079714e-11,
                       -3.6432219e-11,
                       1.053693e-11,
                       -1.716438e-12,
                       -1.07124e-13,
                       2.04099e-13,
                       -9.0064e-14,
                       2.5506e-14,
                       -4.036e-15,
                       -5.7e-16,
                       7.62e-16,
                       -3.63e-16,
                       1.18e-16,
                       -2.5e-17};
    Double_t c2[29] = {.0465577987375164561,
                       .044992130201239414,
                       -.0017542871396514532,
                       -1.465340025810678e-4,
                       3.91330408630159e-5,
                       -3.4932286597731e-6,
                       -3.153530032345e-7,
                       1.876582008529e-7,
                       -3.7755280493e-8,
                       2.665516501e-9,
                       1.0881448122e-9,
                       -5.355007671e-10,
                       1.315765447e-10,
                       -1.52860881e-11,
                       -3.3947646e-12,
                       2.7020267e-12,
                       -9.463142e-13,
                       2.071565e-13,
                       -1.26931e-14,
                       -1.39756e-14,
                       8.5929e-15,
                       -3.107e-15,
                       7.515e-16,
                       -6.48e-17,
                       -5.22e-17,
                       3.86e-17,
                       -1.65e-17,
                       5e-18,
                       -9e-19};

    /* System generated locals */
    Double_t ret_val, d__1;

    /* Local variables */
    Double_t h__;
    int i__;
    Double_t r__, s, v, y, b0, b1, b2, alfa;

    v = TMath::Abs(x);
    if (v < 8.) {
      y = v * .125;
      /* Computing 2nd power */
      d__1 = y;
      h__  = d__1 * d__1 * 2 - 1;
      alfa = h__ + h__;
      b1   = 0.;
      b2   = 0.;
      for (i__ = nb; i__ >= 0; --i__) {
        b0 = b[i__] + alfa * b1 - b2;
        b2 = b1;
        /* L4: */
        b1 = b0;
      }
      h__ = TMath::Sqrt(v) * y * (b0 - b2);
    } else {
      r__  = 1 / v;
      h__  = r__ * 10 - 1;
      alfa = h__ + h__;
      b1   = 0.;
      b2   = 0.;
      for (i__ = nc1; i__ >= 0; --i__) {
        b0 = c1[i__] + alfa * b1 - b2;
        b2 = b1;
        /* L5: */
        b1 = b0;
      }
      s  = b0 - h__ * b2;
      b1 = 0.;
      b2 = 0.;
      for (i__ = nc2; i__ >= 0; --i__) {
        b0 = c2[i__] + alfa * b1 - b2;
        b2 = b1;
        /* L6: */
        b1 = b0;
      }
      h__ = c0 - TMath::Sqrt(r__) * (s * TMath::Cos(v) + (b0 - h__ * b2) * TMath::Sin(v));
    }
    if (x < 0.) { h__ = -h__; }
    ret_val = h__;
    return ret_val;
  } /* dfrsin_ */

  Double_t FemtoFsiParsed::DfrCos(Double_t x) {
    /* Initialized data */

    Double_t c0     = 1.2533141373155003;
    int na          = 16;
    int nc1         = 25;
    int nc2         = 28;
    Double_t a[17]  = {.764351386641860002,
                      -.431355475476601793,
                      .432881999797266531,
                      -.26973310338387111,
                      .084160453208769354,
                      -.01546524484461382,
                      .00187855423439822,
                      -1.62649776188875e-4,
                      1.0573976563833e-5,
                      -5.36093398892e-7,
                      2.1816584549e-8,
                      -7.29016212e-10,
                      2.0373325e-11,
                      -4.8344e-13,
                      9.865e-15,
                      -1.75e-16,
                      3e-18};
    Double_t c1[26] = {.990560479373497549,
                       -.012183509831478997,
                       -.00248274282311306,
                       2.66094952647247e-4,
                       -1.079068987406e-6,
                       -4.883681753933e-6,
                       9.99055266368e-7,
                       -7.5092717372e-8,
                       -1.9079487573e-8,
                       9.090797293e-9,
                       -1.966236033e-9,
                       1.64772911e-10,
                       6.3079714e-11,
                       -3.6432219e-11,
                       1.053693e-11,
                       -1.716438e-12,
                       -1.07124e-13,
                       2.04099e-13,
                       -9.0064e-14,
                       2.5506e-14,
                       -4.036e-15,
                       -5.7e-16,
                       7.62e-16,
                       -3.63e-16,
                       1.18e-16,
                       -2.5e-17};
    Double_t c2[29] = {.0465577987375164561,
                       .044992130201239414,
                       -.0017542871396514532,
                       -1.465340025810678e-4,
                       3.91330408630159e-5,
                       -3.4932286597731e-6,
                       -3.153530032345e-7,
                       1.876582008529e-7,
                       -3.7755280493e-8,
                       2.665516501e-9,
                       1.0881448122e-9,
                       -5.355007671e-10,
                       1.315765447e-10,
                       -1.52860881e-11,
                       -3.3947646e-12,
                       2.7020267e-12,
                       -9.463142e-13,
                       2.071565e-13,
                       -1.26931e-14,
                       -1.39756e-14,
                       8.5929e-15,
                       -3.107e-15,
                       7.515e-16,
                       -6.48e-17,
                       -5.22e-17,
                       3.86e-17,
                       -1.65e-17,
                       5e-18,
                       -9e-19};

    /* System generated locals */
    Double_t ret_val, d__1;

    /* Local variables */
    Double_t h__;
    int i__;
    Double_t r__, s, v, b0, b1, b2, alfa;

    v = TMath::Abs(x);
    if (v < 8.) {
      /* Computing 2nd power */
      d__1 = v;
      h__  = d__1 * d__1 * .03125 - 1;
      alfa = h__ + h__;
      b1   = 0.;
      b2   = 0.;
      for (i__ = na; i__ >= 0; --i__) {
        b0 = a[i__] + alfa * b1 - b2;
        b2 = b1;
        /* L1: */
        b1 = b0;
      }
      h__ = TMath::Sqrt(v) * (b0 - h__ * b2);
    } else {
      r__  = 1 / v;
      h__  = r__ * 10 - 1;
      alfa = h__ + h__;
      b1   = 0.;
      b2   = 0.;
      for (i__ = nc1; i__ >= 0; --i__) {
        b0 = c1[i__] + alfa * b1 - b2;
        b2 = b1;
        /* L2: */
        b1 = b0;
      }
      s  = b0 - h__ * b2;
      b1 = 0.;
      b2 = 0.;
      for (i__ = nc2; i__ >= 0; --i__) {
        b0 = c2[i__] + alfa * b1 - b2;
        b2 = b1;
        /* L3: */
        b1 = b0;
      }
      h__ = c0 - TMath::Sqrt(r__) * ((b0 - h__ * b2) * TMath::Cos(v) - s * TMath::Sin(v));
    }
    if (x < 0.) { h__ = -h__; }
    ret_val = h__;
    return ret_val;
  } /* dfrcos_ */

  void FemtoFsiParsed::Cgamma(complex& ret_val, complex& z__) {
    /* Initialized data */

    Double_t pi      = (Double_t) 3.14159265358979324;
    Double_t c1      = (Double_t) 2.5066282746310005;
    Double_t c__[16] = {(Double_t) 41.624436916439068,
                        (Double_t) -51.224241022374774,
                        (Double_t) 11.338755813488977,
                        (Double_t) -.747732687772388,
                        (Double_t) .008782877493061,
                        (Double_t) -1.899030264e-6,
                        (Double_t) 1.946335e-9,
                        (Double_t) -1.99345e-10,
                        (Double_t) 8.433e-12,
                        (Double_t) 1.486e-12,
                        (Double_t) -8.06e-13,
                        (Double_t) 2.93e-13,
                        (Double_t) -1.02e-13,
                        (Double_t) 3.7e-14,
                        (Double_t) -1.4e-14,
                        (Double_t) 6e-15};

    /* Format strings */
    //  static char fmt_101[] = "(\002ARGUMENT EQUALS NON-POSITIVE INTEGER =
    //  \\002,1p,e15.1)";
    /* System generated locals */
    int i__1;
    complex q__1, q__2, q__3, q__4, q__5, q__6, q__7;

    /* Local variables */
    complex f, h__;
    int k;
    complex s, u, v;
    Double_t x;
    //   char errtxt[80];

    /* Fortran I/O blocks */
    // icilist io___9 = { 0, errtxt, 0, fmt_101, 80, 1 };
    u.r = z__.r, u.i = z__.i;
    x = u.r;
    if (r_imag(&u) == (Double_t) 0. && -TMath::Abs(x) == (Double_t)((int) x)) {
      f.r = (Double_t) 0., f.i = (Double_t) 0.;
      h__.r = (Double_t) 0., h__.i = (Double_t) 0.;

      /*       CALL MTLPRT(NAME,'C305.1',ERRTXT) */
    } else {
      if (x >= (Double_t) 1.) {
        f.r = (Double_t) 1., f.i = (Double_t) 0.;
        v.r = u.r, v.i = u.i;
      } else if (x >= (Double_t) 0.) {
        c_div(&q__1, &c_b5, &u);
        f.r = q__1.r, f.i = q__1.i;
        q__1.r = u.r + 1, q__1.i = u.i;
        v.r = q__1.r, v.i = q__1.i;
      } else {
        f.r = (Double_t) 1., f.i = (Double_t) 0.;
        q__1.r = 1 - u.r, q__1.i = -u.i;
        v.r = q__1.r, v.i = q__1.i;
      }
      h__.r = (Double_t) 1., h__.i = (Double_t) 0.;
      s.r = c__[0], s.i = (Double_t) 0.;
      for (k = 1; k <= 15; ++k) {
        q__3.r = v.r - k, q__3.i = v.i;
        i__1   = k - 1;
        q__4.r = v.r + i__1, q__4.i = v.i;
        c_div(&q__2, &q__3, &q__4);
        q__1.r = q__2.r * h__.r - q__2.i * h__.i, q__1.i = q__2.r * h__.i + q__2.i * h__.r;
        h__.r = q__1.r, h__.i = q__1.i;
        //    std::cout<<"L"<<k<<" ("<<h__.r<<" "<<h__.i<<")"<<std::endl;
        /* L1: */
        i__1   = k;
        q__2.r = c__[i__1] * h__.r, q__2.i = c__[i__1] * h__.i;
        q__1.r = s.r + q__2.r, q__1.i = s.i + q__2.i;
        s.r = q__1.r, s.i = q__1.i;
      }
      q__1.r = v.r + (Double_t) 4.5, q__1.i = v.i;
      h__.r = q__1.r, h__.i = q__1.i;
      q__6.r = v.r - (Double_t) .5, q__6.i = v.i;
      c_log(&q__7, &h__);
      q__5.r = q__6.r * q__7.r - q__6.i * q__7.i, q__5.i = q__6.r * q__7.i + q__6.i * q__7.r;
      q__4.r = q__5.r - h__.r, q__4.i = q__5.i - h__.i;
      c_exp(&q__3, &q__4);
      q__2.r = c1 * q__3.r, q__2.i = c1 * q__3.i;
      q__1.r = q__2.r * s.r - q__2.i * s.i, q__1.i = q__2.r * s.i + q__2.i * s.r;
      h__.r = q__1.r, h__.i = q__1.i;
      if (x < (Double_t) 0.) {
        q__2.r = pi, q__2.i = (Double_t) 0.;
        q__5.r = pi * u.r, q__5.i = pi * u.i;
        c_sin(&q__4, &q__5);
        q__3.r = q__4.r * h__.r - q__4.i * h__.i, q__3.i = q__4.r * h__.i + q__4.i * h__.r;
        c_div(&q__1, &q__2, &q__3);
        h__.r = q__1.r, h__.i = q__1.i;
      }
    }

    q__1.r = f.r * h__.r - f.i * h__.i, q__1.i = f.r * h__.i + f.i * h__.r;
    ret_val.r = q__1.r, ret_val.i = q__1.i;
    z__.r = ret_val.r;
    z__.i = ret_val.i;
    return;
  }

  void FemtoFsiParsed::Ckkb() {
    /*
     Double_t S4 = fFsiPrf.aks + fFsiAakk.e_1[0];
     Double_t S = 4*S4;
     Double_t  AKPIPI = TMath::Sqrt(S4 - fFsiAakk.e_1[1]);
     Double_t AAKK_2 = fFsiAakk.e_1[2];
     Double_t  EETA2 = (S+fFsiAakk.e_1[2]-fFsiAakk.e_1[1])*
     (S+fFsiAakk.e_1[2]-fFsiAakk.e_1[1])/4/S;
     Double_t AKPIETA = TMath::Sqrt(EETA2 - fFsiAakk.e_1[2]);
     SetC(TComplex(), 0);
     */

    /* System generated locals */
    Double_t d__1, d__2, d__3;
    complex z__1, z__2, z__3, z__4;

    /* Local variables */
    Double_t s, s4, eeta2, akpipi, akpieta;

    /* saturated by S*(980) and delta(982) resonances */
    /* calculates KK-b scattering amplitude, */
    s4     = fFsiPrf.aks + fFsiAakk.e_1[0];
    s      = s4 * 4;
    akpipi = TMath::Sqrt(s4 - fFsiAakk.e_1[1]);
    /* Computing 2nd power */
    d__1    = s + fFsiAakk.e_1[2] - fFsiAakk.e_1[1];
    eeta2   = d__1 * d__1 / 4 / s;
    akpieta = TMath::Sqrt(eeta2 - fFsiAakk.e_1[2]);
    d__1    = fFsiAakk.e_1[5] / 2;
    z__2.r = d__1, z__2.i = 0.;
    d__2   = fFsiAakk.e_1[3] - s;
    d__3   = -fFsiPrf.ak * fFsiAakk.e_1[5] - akpipi * fFsiAakk.e_1[6];
    z__3.r = d__2, z__3.i = d__3;
    z_div(&z__1, &z__2, &z__3);
    fsi_c__._2.c__[0].r = z__1.r, fsi_c__._2.c__[0].i = z__1.i;
    d__1   = fFsiAakk.e_1[7] / 2;
    z__3.r = d__1, z__3.i = 0.;
    d__2   = fFsiAakk.e_1[4] - s;
    d__3   = -fFsiPrf.ak * fFsiAakk.e_1[7] - akpieta * fFsiAakk.e_1[8];
    z__4.r = d__2, z__4.i = d__3;
    z_div(&z__2, &z__3, &z__4);
    z__1.r = fsi_c__._2.c__[0].r + z__2.r, z__1.i = fsi_c__._2.c__[0].i + z__2.i;
    fsi_c__._2.c__[0].r = z__1.r, fsi_c__._2.c__[0].i = z__1.i;

  } /* ckkb_ */

  void FemtoFsiParsed::Cpap() {
    Double_t AM2 = .93956563;
    ;
    Double_t AM        = fsi_c__._2.am;
    Double_t AMU2_AMU1 = AM2 / AM;
    Double_t AKS       = fFsiPrf.aks;
    Double_t AK2S0     = AMU2_AMU1 * AKS;
    Double_t AK2S      = fsi_2cha__.ak2s;
    AK2S               = AK2S0 - 2 * AM2 * (AM2 - AM);
    //	TComplex AK2(fsi_2cha__.ak2.r,fsi_2cha__.ak2.i);
    if (AK2S >= 0) {
      //		AK2= TComplex(TMath::Sqrt(AK2S),0);//k2
      fsi_2cha__.ak2 = TMath::Sqrt(AK2S);
    } else {
      fsi_2cha__.ak2 = TMath::Sqrt(-AK2S);
      //		AK2 = TComplex(0,TMath::Sqrt(-AK2S)); //kappa2
    }
    Int_t ISPIN   = fFsiAch._1.ispin;
    Double_t a_re = fsi_aapap__1.aapapr[fFsiAch._1.ispin * 3 - 1] * fFsiPrf.aks / 2 - .016 - fsi_2cha__.hcp2;
    Double_t a_im = fsi_aapap__1.aapapi[fFsiAch._1.ispin * 3 - 1] * fFsiPrf.aks / 2 - fsi_2cha__.aak;
    TComplex C10  = GetC(6 + (ISPIN - 1) * 2) + TComplex(a_re, a_im);
    a_re          = fsi_aapap__1.aapapr[fFsiAch._1.ispin * 3 - 1] * AK2S0 / 2;
    a_im          = fsi_aapap__1.aapapi[fFsiAch._1.ispin * 3 - 1] * AK2S0 / 2;
    TComplex C5   = GetC(6 + (ISPIN - 1) * 2 - 1) + TComplex(a_re, a_im);
    if (AK2S >= 0) {
      C5 = C5 - TComplex(0, fsi_2cha__.ak2);
    } else {
      C5 = C5 + TComplex(fsi_2cha__.ak2, 0);
    }
    C10 = C10 * C5 - GetC(7 + (ISPIN - 1) * 2 - 1) * GetC(7 + (ISPIN - 1) * 2 - 1);
    SetC(C5 / C10, ISPIN - 1);
    SetC(-GetC(7 + (ISPIN - 1) * 2 - 1) / C10, ISPIN + 2 - 1);
    SetC(C5, 5 - 1);
    SetC(C10, 10 - 1);
  } /* cpap_ */

  void FemtoFsiParsed::Fsiin(int i_itest__, int i_ich__, int i_iqs__, int i_isi__, int i_i3c__) {
    /* Initialized data */

    /* Local variables */
    int j1, j2, itest;

    ++ifirst;
    /* =======< constants >======================== */
    fFsiCons.w = 5.0684237202230102;
    /* from fm to 1/GeV */

    fFsiCons.spi = TMath::Sqrt(TMath::Pi());
    fFsiCons.dr  = 180. / TMath::Pi();
    /* from radian to degree */
    fFsiPoc.ac1 = 1e10;
    fFsiPoc.ac2 = 1e10;
    /* =======< condition de calculs >============= */
    //   nunit = 11;
    /*      NUNIT=4 ! for SUN in Prague */
    /* ++      CALL readint4(NUNIT,'ITEST     ',ITEST) */
    /* ++      CALL readint4(NUNIT,'NS        ',NS) */
    /* for IBM in Nantes */
    itest = i_itest__;
    if (itest == 1) {
      /* ++      CALL readint4(NUNIT,'ICH       ',ICH) */
      /* ++      CALL readint4(NUNIT,'IQS       ',IQS) */
      /* ++      CALL readint4(NUNIT,'ISI       ',ISI) */
      /* ++      CALL readint4(NUNIT,'I3C       ',I3C) */
      fFsiNs.ich = i_ich__;
      fFsiNs.iqs = i_iqs__;
      fFsiNs.isi = i_isi__;
      fFsiNs.i3c = i_i3c__;
    }
    /* ============================================ */
    if (ifirst <= 1) {
      for (j1 = 1; j1 <= 30; ++j1) {
        for (j2 = 1; j2 <= 10; ++j2) {
          fsi_fdh__1.fdh[j1 + j2 * 30 - 31] *= fFsiCons.w;
          fsi_fdh__1.rdh[j1 + j2 * 30 - 31] *= fFsiCons.w;
          /* L3: */
          fsi_fdh__1.rbh[j1 + j2 * 30 - 31] *= fFsiCons.w;
        }
      }
      /*     print *,"FD,RD,EB,RB: ",FDH(J1,J2),RDH(J1,J2),EBH(J1,J2),RBH(J1,J2)
       */
    }
    /* =================================== */
  } /* fsiin_ */

  void FemtoFsiParsed::Llini(int lll, int i_ns__, int i_itest__) {
    /* Initialized data */

    //     int ifirst = 0;
    /* System generated locals */
    int i__1, i__2, i__3, i__4, i__5, i__6;
    Double_t d__1, d__2;
    complex z__1, z__2, z__3, z__4, z__5;

    /* Local variables */
    Double_t h__;
    int i3;
    Double_t c12, hh;
    int jh, jj;
    Double_t akh;
    int msp;
    Double_t daks, aksh;
    Double_t gpi1h, gpi2h;

    /* ===> Initialisation for a given LL value. */
    /*     =========================================== */
    /* ++----- add to be able to call several time------- */
    ++ifirst;
    /* ===> LL - Initialisation ======================================== */
    /* ---- Setting particle masses and charges */
    fFsiNs.ll   = lll;
    fFsiNs.ns   = i_ns__;
    fFsiPoc.am1 = fsi_amch__1.am1h[fFsiNs.ll - 1];
    fFsiPoc.am2 = fsi_amch__1.am2h[fFsiNs.ll - 1];
    fFsiPoc.c1  = fsi_amch__1.c1h[fFsiNs.ll - 1];
    fFsiPoc.c2  = fsi_amch__1.c2h[fFsiNs.ll - 1];
    /*      print *,"llini: ",AM1,AM2,C1,C2 */
    /* ---> Switches: */
    /*     ISI=1(0)  the strong interaction between the two particles ON (OFF) */
    /*     IQS=1(0)  the quantum statistics ON (OFF); */
    /*               should be OFF for nonidentical particles */
    /*     I3C=1(0)  the Coulomb interaction with the nucleus ON (OFF) */
    /*     I3S=1(0)  the strong interaction with the nucleus ON (OFF) */
    /*     ICH=1(0)  if C1*C2 is different from 0 (is equal to 0) */
    /* -    To switch off the Coulomb force between the two particles */
    /*     put ICH=0 and substitute the strong amplitude parameters by */
    /*     the ones not affected by Coulomb interaction */
    /* ---- --------------------------------------------------------------------
     */
    if (i_itest__ != 1) {
      fFsiNs.ich = 0;
      if (fFsiPoc.c1 * fFsiPoc.c2 != 0.) { fFsiNs.ich = 1; }
      fFsiNs.iqs = 0;
      if (fFsiPoc.c1 + fFsiPoc.am1 == fFsiPoc.c2 + fFsiPoc.am2) { fFsiNs.iqs = 1; }
      fFsiNs.i3s = 0;
      /* only this option is available */
      fFsiNs.isi = 1;
      fFsiNs.i3c = 1;
    }
    /* ---> Calcul. twice the reduced mass (AM), the relative */
    /*     mass difference (DM) and the Bohr radius (AC) */
    fsi_c__._2.am  = fFsiPoc.am1 * 2 * fFsiPoc.am2 / (fFsiPoc.am1 + fFsiPoc.am2);
    fsi_c__._2.ams = fsi_c__._2.am * fsi_c__._2.am;
    fsi_c__._2.dm  = (fFsiPoc.am1 - fFsiPoc.am2) / (fFsiPoc.am1 + fFsiPoc.am2);
    fFsiAch._1.ac  = 1e10;
    c12            = fFsiPoc.c1 * fFsiPoc.c2;
    if (c12 != 0.) { fFsiAch._1.ac = 274.072 / (c12 * fsi_c__._2.am); }
    /* ---Setting spin factors */
    fFsiAch._1.mspin = fsi_amch__1.mspinh[fFsiNs.ll - 1];
    /*      print *,"MSPIN: ",MSPIN */
    msp = fFsiAch._1.mspin;
    for (fFsiAch._1.ispin = 1; fFsiAch._1.ispin <= 10; ++fFsiAch._1.ispin) {
      /* L91: */
      fsi_spin__.rho[fFsiAch._1.ispin - 1] = fsi_rhoh__1.rhoh[fFsiNs.ll + fFsiAch._1.ispin * 30 - 31];
    }
    /* 91      print *,"RHO: ",ISPIN,RHO(ISPIN) */
    /* ---> Integration limit AA in the spherical wave approximation */
    fsi_aa__.aa = 0.;
    /* c      IF(NS.EQ.2.OR.NS.EQ.4)AA=.5D0 !!in 1/GeV --> 0.1 fm */
    if (fFsiNs.ns == 2 || fFsiNs.ns == 4) { fsi_aa__.aa = 6.; }
    /* ---> Setting scatt. length (FD), eff. radius (RD) and, if possible, */
    /* --   also the corresp. square well parameters (EB, RB) */
    /* !in 1/GeV --> 1.2 fm */
    i__1 = msp;
    for (jj = 1; jj <= i__1; ++jj) {
      fFsiAch._1.ispin    = jj;
      fsi_fd__.fd[jj - 1] = fsi_fdh__1.fdh[fFsiNs.ll + jj * 30 - 31];
      fsi_fd__.rd[jj - 1] = fsi_fdh__1.rdh[fFsiNs.ll + jj * 30 - 31];
      fsi_sw__.eb[jj - 1] = fsi_fdh__1.ebh[fFsiNs.ll + jj * 30 - 31];
      fsi_sw__.rb[jj - 1] = fsi_fdh__1.rbh[fFsiNs.ll + jj * 30 - 31];
      /*         print *,"FD,RD,EB,RB: ",FD(JJ),RD(JJ),EB(JJ),RB(JJ) */
      /* ---Resets FD and RD for a nucleon-deuteron system (LL=8,9) */
      if (fFsiNs.ll == 8 || fFsiNs.ll == 9) {
        jh                  = fFsiNs.ll - 7 + (jj << 1) - 2;
        fsi_fd__.fd[jj - 1] = fFsiAand.e_1[jh * 20 - 20];
        fsi_fd__.rd[jj - 1] = fFsiAand.e_1[jh * 20 - 19] - fFsiAand.e_1[jh * 20 - 18] * 2 / fFsiAand.e_1[jh * 20 - 20];
      }
      /* ---Resets FD and RD for a pion-pion system (LL=5,6,7) */
      if (fFsiNs.ll == 5 || fFsiNs.ll == 6 || fFsiNs.ll == 7) {
        if (fFsiNs.ll == 7) { fsi_fd__.fd[jj - 1] = fFsiAapi.e_1[20] / fsi_c__._2.am; }
        if (fFsiNs.ll == 5) { fsi_fd__.fd[jj - 1] = (fFsiAapi.e_1[0] * .6667 + fFsiAapi.e_1[20] * .3333) / fsi_c__._2.am; }
        if (fFsiNs.ll == 6) { fsi_fd__.fd[jj - 1] = (fFsiAapi.e_1[0] * .3333 + fFsiAapi.e_1[20] * .6667) / fsi_c__._2.am; }
        fFsiPrf.aks = 0.;
        daks        = 1e-5;
        aksh        = fFsiPrf.aks + daks;
        akh         = TMath::Sqrt(aksh);
        gpi1h       = Gpipi(aksh, c__1);
        gpi2h       = Gpipi(aksh, c__2);
        h__         = 1 / fsi_fd__.fd[jj - 1];
        if (fFsiNs.ll == 7) {
          i__2   = jj - 1;
          d__1   = -akh;
          z__2.r = gpi2h, z__2.i = d__1;
          z_div(&z__1, &c_b2, &z__2);
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
        }
        if (fFsiNs.ll == 5) {
          i__2   = jj - 1;
          d__1   = -akh;
          z__3.r = gpi1h, z__3.i = d__1;
          z_div(&z__2, &c_b33, &z__3);
          d__2   = -akh;
          z__5.r = gpi2h, z__5.i = d__2;
          z_div(&z__4, &c_b34, &z__5);
          z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
        }
        if (fFsiNs.ll == 6) {
          i__2   = jj - 1;
          d__1   = -akh;
          z__3.r = gpi1h, z__3.i = d__1;
          z_div(&z__2, &c_b34, &z__3);
          d__2   = -akh;
          z__5.r = gpi2h, z__5.i = d__2;
          z_div(&z__4, &c_b33, &z__5);
          z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
        }
        z_div(&z__1, &c_b2, &fsi_c__._2.c__[jj - 1]);
        hh                  = z__1.r;
        fsi_fd__.rd[jj - 1] = (hh - h__) * 2 / daks;
      }
      /* ---Resets FD and RD for a pion-nucleon system (LL=12,13) */
      if (fFsiNs.ll == 12 || fFsiNs.ll == 13) {
        if (fFsiNs.ll == 12) { fsi_fd__.fd[jj - 1] = fFsiApin.e_1[20]; }
        if (fFsiNs.ll == 13) { fsi_fd__.fd[jj - 1] = fFsiApin.e_1[0] * .6667 + fFsiApin.e_1[20] * .3333; }
        fFsiPrf.aks = 0.;
        daks        = 1e-5;
        aksh        = fFsiPrf.aks + daks;
        akh         = TMath::Sqrt(aksh);
        gpi1h       = Gpin(aksh, c__1);
        gpi2h       = Gpin(aksh, c__2);
        h__         = 1 / fsi_fd__.fd[jj - 1];
        if (fFsiNs.ll == 12) {
          i__2   = jj - 1;
          d__1   = -akh;
          z__2.r = gpi2h, z__2.i = d__1;
          z_div(&z__1, &c_b2, &z__2);
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
        }
        if (fFsiNs.ll == 13) {
          i__2   = jj - 1;
          d__1   = -akh;
          z__3.r = gpi1h, z__3.i = d__1;
          z_div(&z__2, &c_b33, &z__3);
          d__2   = -akh;
          z__5.r = gpi2h, z__5.i = d__2;
          z_div(&z__4, &c_b34, &z__5);
          z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
        }
        z_div(&z__1, &c_b2, &fsi_c__._2.c__[jj - 1]);
        hh                  = z__1.r;
        fsi_fd__.rd[jj - 1] = (hh - h__) * 2 / daks;
      }
      /* ---fm to 1/GeV for pp-bar system */
      if (fFsiNs.ll == 30) {
        if (ifirst <= 1) {
          for (i3 = 1; i3 <= 3; ++i3) {
            fsi_aapap__1.aapapr[i3 + jj * 3 - 4] *= fFsiCons.w;
            /* L4: */
            fsi_aapap__1.aapapi[i3 + jj * 3 - 4] *= fFsiCons.w;
          }
          /*     4       print *,"AAPAPR,AAPAPI: ",AAPAPR(I3,JJ),AAPAPI(I3,JJ) */
          /* ---  Calculates complex elements M11=M22=C(6), M12=M21=C(7) for I=0
           */
          /* ---  at k*=0                  M11=M22=C(8), M12=M21=C(9) for I=1 */
          i__2   = ((jj - 1) << 1) + 6;  // suggest parentheses around ‘-’
          i__3   = jj * 3 - 3;
          i__4   = jj * 3 - 3;
          z__3.r = fsi_aapap__1.aapapr[i__3], z__3.i = fsi_aapap__1.aapapi[i__4];
          d__1   = 2.;
          z__2.r = d__1 * z__3.r, z__2.i = d__1 * z__3.i;
          i__5   = jj * 3 - 2;
          i__6   = jj * 3 - 2;
          z__4.r = fsi_aapap__1.aapapr[i__5], z__4.i = fsi_aapap__1.aapapi[i__6];
          z__1.r = z__2.r * z__4.r - z__2.i * z__4.i, z__1.i = z__2.r * z__4.i + z__2.i * z__4.r;
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
          /* 2a_0Sa_1S */
          i__2   = ((jj - 1) << 1) + 5;  // suggest parentheses around ‘-’
          d__1   = fsi_aapap__1.aapapr[jj * 3 - 3] + fsi_aapap__1.aapapr[jj * 3 - 2];
          d__2   = fsi_aapap__1.aapapi[jj * 3 - 3] + fsi_aapap__1.aapapi[jj * 3 - 2];
          z__2.r = d__1, z__2.i = d__2;
          z_div(&z__1, &z__2,
                &fsi_c__._2.c__[((jj - 1) << 1) + 6]);  // suggest parentheses around ‘-’
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
          /* M11=M22 */
          i__2   = ((jj - 1) << 1) + 6;  // suggest parentheses around ‘-’
          d__1   = fsi_aapap__1.aapapr[jj * 3 - 3] - fsi_aapap__1.aapapr[jj * 3 - 2];
          d__2   = fsi_aapap__1.aapapi[jj * 3 - 3] - fsi_aapap__1.aapapi[jj * 3 - 2];
          z__3.r = d__1, z__3.i = d__2;
          z__2.r = -z__3.r, z__2.i = -z__3.i;
          z_div(&z__1, &z__2,
                &fsi_c__._2.c__[((jj - 1) << 1) + 6]);  // suggest parentheses around ‘-’
          fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
          /* M12=M21 */
        }
      }
      /* ---Calculation continues for any system (any LL) */
      /* L55: */
    }
  } /* llini_ */

  /* ======================================================= */

  /* ++  This routine is used to init mass and charge of the nucleus. */
  void FemtoFsiParsed::Fsinucl(Double_t r_amn__, Double_t r_cn__) {
    fFsiPoc.amn = r_amn__;
    fFsiPoc.cn  = r_cn__;
  } /* fsinucl_ */

  /* ====================================================== */

  void FemtoFsiParsed::SetMomentum(Double_t* pp1, Double_t* pp2) {
    /* particle momenta in NRF */
    /*      Print *,"momentum",pp1,pp2 */
    /* Parameter adjustments */
    --pp2;
    --pp1;

    /* Function Body */
    fFsiMom.p1x = pp1[1];
    fFsiMom.p1y = pp1[2];
    fFsiMom.p1z = pp1[3];
    fFsiMom.p2x = pp2[1];
    fFsiMom.p2y = pp2[2];
    fFsiMom.p2z = pp2[3];
  } /* fsimomentum_ */

  /* ====================================================== */

  void FemtoFsiParsed::SetPosition(Double_t* xt1, Double_t* xt2) {
    /* 4-coord. of emis. points in NRF */
    /* lc      print *,'fsi',xt1,xt2 */
    /* Parameter adjustments */
    --xt2;
    --xt1;

    /* Function Body */
    fFsiCoor.x1 = xt1[1];
    fFsiCoor.y1 = xt1[2];
    fFsiCoor.z1 = xt1[3];
    fFsiCoor.t1 = xt1[4];
    fFsiCoor.x2 = xt2[1];
    fFsiCoor.y2 = xt2[2];
    fFsiCoor.z2 = xt2[3];
    fFsiCoor.t2 = xt2[4];
  } /* fsiposition_ */

  /* ====================================================== */
  /* ====================================================== */

  void FemtoFsiParsed::Boosttoprf() {
    /* Local variables */
    Double_t h1, ts, xs, ys, zs, rs12;

    /* part. momenta in NRF */
    /* 4-coord. of emis. points in NRF */
    xs          = fFsiCoor.x1 - fFsiCoor.x2;
    ys          = fFsiCoor.y1 - fFsiCoor.y2;
    zs          = fFsiCoor.z1 - fFsiCoor.z2;
    ts          = fFsiCoor.t1 - fFsiCoor.t2;
    rs12        = xs * fSi_p12.p12x + ys * fSi_p12.p12y + zs * fSi_p12.p12z;
    h1          = (rs12 / fSi_p12.epm - ts) / fSi_p12.am12;
    fFsiPrf.x   = xs + fSi_p12.p12x * h1;
    fFsiPrf.y   = ys + fSi_p12.p12y * h1;
    fFsiPrf.z__ = zs + fSi_p12.p12z * h1;
    fFsiPrf.t   = (fSi_p12.e12 * ts - rs12) / fSi_p12.am12;
    fFsiPrf.rps = fFsiPrf.x * fFsiPrf.x + fFsiPrf.y * fFsiPrf.y + fFsiPrf.z__ * fFsiPrf.z__;
    fFsiPrf.rp  = TMath::Sqrt(fFsiPrf.rps);
    /* W      WRITE(6,38)'RP ',RP,'X ',X,Y,Z,T */
    /* L38: */
    fsi_cvk__.cvk =
      (fSi_p12.p12x * fFsiPrf.ppx + fSi_p12.p12y * fFsiPrf.ppy + fSi_p12.p12z * fFsiPrf.ppz) / (fSi_p12.p12 * fFsiPrf.ak);
    fsi_cvk__.v = fSi_p12.p12 / fSi_p12.e12;
    return;
  } /* boosttoprf_ */

  void FemtoFsiParsed::Fsiwf(Double_t& wei) {
    /* System generated locals */
    int i__1, i__2, i__3;
    Double_t d__1;
    complex z__1, z__2, z__3, z__4, z__5;

    /* Local variables */
    Double_t a1, a2, h1;
    int jj, jh;
    Double_t xh, ts, xs, ys, zs;
    Double_t gak, akf;
    Double_t hcp, hra, rs12, xra;
    int msp;
    Double_t gpi2, gpi1;
    Double_t akach;

    /* ==> Prepares necessary quantities and call VZ(WEI) to calculate */
    /*    the weight due to FSI */
    /* part. momenta in NRF */
    /* 4-coord. of emis. points in NRF */
    /* ==>calculating relative 4-coordinates of the particles in PRF */
    /* -  {T,X,Y,Z} from the relative coordinates {TS,XS,YS,ZS} in NRF */
    /* k* (kappa) for 2-nd */
    xs          = fFsiCoor.x1 - fFsiCoor.x2;
    ys          = fFsiCoor.y1 - fFsiCoor.y2;
    zs          = fFsiCoor.z1 - fFsiCoor.z2;
    ts          = fFsiCoor.t1 - fFsiCoor.t2;
    rs12        = xs * fSi_p12.p12x + ys * fSi_p12.p12y + zs * fSi_p12.p12z;
    h1          = (rs12 / fSi_p12.epm - ts) / fSi_p12.am12;
    fFsiPrf.x   = xs + fSi_p12.p12x * h1;
    fFsiPrf.y   = ys + fSi_p12.p12y * h1;
    fFsiPrf.z__ = zs + fSi_p12.p12z * h1;
    fFsiPrf.t   = (fSi_p12.e12 * ts - rs12) / fSi_p12.am12;
    fFsiPrf.rps = fFsiPrf.x * fFsiPrf.x + fFsiPrf.y * fFsiPrf.y + fFsiPrf.z__ * fFsiPrf.z__;
    fFsiPrf.rp  = TMath::Sqrt(fFsiPrf.rps);
    /* W      WRITE(6,38)'RP ',RP,'X ',X,Y,Z,T */
    /* L38: */
    fsi_cvk__.cvk =
      (fSi_p12.p12x * fFsiPrf.ppx + fSi_p12.p12y * fFsiPrf.ppy + fSi_p12.p12z * fFsiPrf.ppz) / (fSi_p12.p12 * fFsiPrf.ak);
    fsi_cvk__.v = fSi_p12.p12 / fSi_p12.e12;
    /*      ACH=1.D0 */
    if (fFsiNs.ich == 0) { goto L21; }
    xh              = fFsiAch._1.ac * fFsiPrf.ak;
    fFsiAch._1.ach  = Acp(xh);
    fFsiAch._1.achr = TMath::Sqrt(fFsiAch._1.ach);
    fEta            = 0.;
    if (xh != 0.) { fEta = 1 / xh; }
    /* ---HCP, HPR needed (e.g. in GST) if ICH=1 */
    hcp            = Hc(xh);
    fFsiAch._1.hpr = hcp + .1544313298;
  /*      AAK=ACH*AK    ! */
  /*      HCP2=2*HCP/AC ! needed to calculate C(JJ) for charged particles */
  L21:
    msp  = fFsiAch._1.mspin;
    i__1 = msp;
    for (jj = 1; jj <= i__1; ++jj) {
      fFsiAch._1.ispin = jj;
      if (fFsiNs.ns != 1) { goto L22; }
      /* ---Calc. quantities for the square well potential; */
      /* -- for LL=6-26 the square well potential is not possible or available */
      if (fFsiNs.ll == 4) { goto L22; }
      /* Computing 2nd power */
      d__1                = fsi_sw__.eb[jj - 1];
      fsi_sw__.bk[jj - 1] = TMath::Sqrt(d__1 * d__1 + fFsiPrf.aks);
      xra                 = fsi_sw__.rb[jj - 1] * 2 / fFsiAch._1.ac;
      hra                 = fsi_sw__.bk[jj - 1] * fsi_sw__.rb[jj - 1];
      Seq(xra, hra);
      fsi_sw__.sbkrb[jj - 1] = hra * fsi_bp__.b;
      hra                    = fFsiPrf.ak * fsi_sw__.rb[jj - 1];
      Gst(xra, hra);
      fsi_sw__.sdk[jj - 1]  = fsi_shh__.sh;
      fsi_sw__.cdk[jj - 1]  = fsi_shh__.chh;
      fsi_sw__.sdkk[jj - 1] = fsi_sw__.rb[jj - 1];
      if (fFsiPrf.ak != 0.) { fsi_sw__.sdkk[jj - 1] = fsi_shh__.sh / fFsiPrf.ak; }
      if (fFsiNs.ich == 1) { fsi_sw__.sdk[jj - 1] = fFsiAch._1.ach * fsi_sw__.sdk[jj - 1]; }
    L22:
      /* -----------------------------------------------------------------------
       */
      /* ---Calc. the strong s-wave scattering amplitude = C(JJ) */
      /* -- divided by Coulomb penetration factor squared (if ICH=1) */
      if (fFsiNs.ns != 1) { goto L230; }
      if (fFsiNs.ll != 4) { goto L230; }
      /* SW scat. amplitude used for alfa-alfa only */
      gak   = G(fFsiPrf.ak);
      akach = fFsiPrf.ak;
      if (fFsiNs.ich == 1) { akach = fFsiPrf.ak * fFsiAch._1.ach; }
      i__2   = jj - 1;
      d__1   = -akach;
      z__2.r = gak, z__2.i = d__1;
      z_div(&z__1, &c_b2, &z__2);
      fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
      /* amplitude for the SW-potential */
      goto L30;
    L230:
      if (fFsiNs.ll == 5 || fFsiNs.ll == 6 || fFsiNs.ll == 7) { goto L20; }
      /* pipi */
      if (fFsiNs.ll == 12 || fFsiNs.ll == 13) { goto L20; }
      /* piN */
      if (fFsiNs.ll == 8 || fFsiNs.ll == 9 || fFsiNs.ll == 18) { goto L20; }
      /* Nd, dd */
      if (fFsiNs.ll == 14 || fFsiNs.ll == 17 || fFsiNs.ll == 23) { goto L27; }
      /* K+K-, K-p, K0K0-b */
      if (fFsiNs.ll == 30) { goto L28; }
      /* pp-bar */
      a1 = fsi_fd__.rd[jj - 1] * fsi_fd__.fd[jj - 1] * fFsiPrf.aks;
      a2 = a1 * .5 + 1;
      if (fFsiNs.ich == 1) { a2 -= hcp * 2 * fsi_fd__.fd[jj - 1] / fFsiAch._1.ac; }
      akf = fFsiPrf.ak * fsi_fd__.fd[jj - 1];
      if (fFsiNs.ich == 1) { akf *= fFsiAch._1.ach; }
      i__2   = jj - 1;
      i__3   = jj - 1;
      z__2.r = fsi_fd__.fd[i__3], z__2.i = 0.;
      d__1   = -akf;
      z__3.r = a2, z__3.i = d__1;
      z_div(&z__1, &z__2, &z__3);
      fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
      goto L30;
    L20:
      /* ---Calc. scatt. ampl. C(JJ) for pipi, piN and Nd, dd */
      jh = fFsiNs.ll - 7 + (jj << 1) - 2;
      if (fFsiNs.ll == 8 || fFsiNs.ll == 9) { gpi2 = Gnd(fFsiPrf.aks, jh); }
      if (fFsiNs.ll == 18) { gpi2 = Gdd(fFsiPrf.aks, jj); }
      if (fFsiNs.ll == 5 || fFsiNs.ll == 6 || fFsiNs.ll == 7) { gpi2 = Gpipi(fFsiPrf.aks, c__2); }
      if (fFsiNs.ll == 12 || fFsiNs.ll == 13) { gpi2 = Gpin(fFsiPrf.aks, c__2); }
      i__2   = jj - 1;
      d__1   = -fFsiPrf.ak;
      z__2.r = gpi2, z__2.i = d__1;
      z_div(&z__1, &c_b2, &z__2);

      fsi_c__._2.c__[i__2].r = z__1.r,
      fsi_c__._2.c__[i__2].i = z__1.i;  // z__1 jest ok
      /* pi+pi+, nd, pd, pi+p, dd */
      if (fFsiNs.ll != 5 && fFsiNs.ll != 6 && fFsiNs.ll != 13) { goto L27; }
      if (fFsiNs.ll == 5 || fFsiNs.ll == 6) { gpi1 = Gpipi(fFsiPrf.aks, c__1); }
      if (fFsiNs.ll == 13) { gpi1 = Gpin(fFsiPrf.aks, c__1); }
      if (fFsiNs.ll == 5 || fFsiNs.ll == 13) {
        i__2   = jj - 1;
        d__1   = -fFsiPrf.ak;
        z__3.r = gpi1, z__3.i = d__1;
        z_div(&z__2, &c_b33, &z__3);
        i__3   = jj - 1;
        z__4.r = fsi_c__._2.c__[i__3].r * .3333, z__4.i = fsi_c__._2.c__[i__3].i * .3333;
        z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
        fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
      }
      /* pi+pi-,pi */
      if (fFsiNs.ll == 6) {
        i__2   = jj - 1;
        d__1   = -fFsiPrf.ak;
        z__3.r = gpi1, z__3.i = d__1;
        z_div(&z__2, &c_b34, &z__3);
        i__3   = jj - 1;
        z__4.r = fsi_c__._2.c__[i__3].r * .6667, z__4.i = fsi_c__._2.c__[i__3].i * .6667;
        z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
        fsi_c__._2.c__[i__2].r = z__1.r, fsi_c__._2.c__[i__2].i = z__1.i;
      }
    /* pi0pi0 */
    L27:
      /* ---Calc. K+K-, K0K0-b or K-p s-wave scatt. ampl. */
      if (fFsiNs.ll == 14 || fFsiNs.ll == 23) { Ckkb(); }
      /*      IF(LL.EQ.17)C(JJ)=DCMPLX(-3.29D0,3.55D0)     ! Martin'76 (K-p) */
      /*      IF(LL.EQ.17)C(JJ)=DCMPLX(3.29D0,3.55D0)     ! Martin'76 (K-p) WRONG
       * SIGN!!! */
      if (fFsiNs.ll == 17) {
        i__2                   = jj - 1;
        fsi_c__._2.c__[i__2].r = -2.585, fsi_c__._2.c__[i__2].i = 4.156;
      }
      /*      IF(LL.EQ.17)C(JJ)=DCMPLX(-3.371D0,3.244D0)   ! Martin'81 (K-p) */
      /* ---Calc. pi+pi-, pi+pi+, pd, pi+p, pi-p, K+K- or K-p s-wave scatt. ampl.
       */
      /* -- divided by Coulomb penetration factor squared (if ICH=1) */
      /* Borasoy'04 (K-p) */
      if (fFsiNs.ich == 0) { goto L30; }
      fsi_2cha__.aak = fFsiAch._1.ach * fFsiPrf.ak;

      fsi_2cha__.hcp2 = hcp * 2 / fFsiAch._1.ac;
      /* needed to calculate C(JJ) for charged particles */
      i__2 = jj - 1;

      z_div(&z__4, &c_b2, &fsi_c__._2.c__[jj - 1]);

      z__3.r = z__4.r - fsi_2cha__.hcp2, z__3.i = z__4.i;
      d__1   = fFsiPrf.ak - fsi_2cha__.aak;
      z__5.r = 0., z__5.i = d__1;
      z__2.r = z__3.r + z__5.r, z__2.i = z__3.i + z__5.i;
      z_div(&z__1, &c_b2, &z__2);
      fsi_c__._2.c__[i__2].r = z__1.r,
      fsi_c__._2.c__[i__2].i = z__1.i;  // tu jest ok
      goto L30;
    L28:
      /* ---Calc. pp-bar s-wave scatt. ampl. */
      Cpap();
    L30:;
    }
    /* *********************************************************************** */
    Vz(wei);
    return;
  } /* fsiwf_ */

  void FemtoFsiParsed::Vz(Double_t& wei)  // chyba cos nie bundli tutaj
  {
    /* System generated locals */
    int i__1, i__2, i__3;
    Double_t r__1;
    Double_t d__1, d__2;
    complex q__1;
    complex z__1, z__2;

    /* Local variables */
    complex g;
    complex z8;
    Double_t hh;
    int jj;
    Double_t hs;
    int msp;
    complex psi12, psi21;
    Double_t rhos;
    int jsign;

    /* ==> Calculates the weight WEI due to FSI */
    /* k* (kappa) for 2-nd */
    wei = 0.;
    if (fJrat == 1) { goto L11; }
    rhos = fFsiPrf.ak * fFsiPrf.rp;
    hs   = fFsiPrf.x * fFsiPrf.ppx + fFsiPrf.y * fFsiPrf.ppy + fFsiPrf.z__ * fFsiPrf.ppz;
    if (rhos < 15. && rhos + TMath::Abs(hs) < 20.) { goto L2; }
    /* ---Calc. EIDC=exp(i*Coul.Ph.); */
    /* -- used in calc. of hypergeom. f-s in SEQA, FAS at k*R > 15, 20 */
    //   std::cout<<"fphiA\t"<<fEta<<std::endl;//OK
    r__1   = (Double_t) fEta;
    q__1.r = (Double_t) 1., q__1.i = r__1;
    z8.r = q__1.r, z8.i = q__1.i;

    Cgamma(q__1, z8);
    z8.r = q__1.r, z8.i = q__1.i;
    r__1 = c_abs(&z8);
    //	 std::cout<<"Z8 cabs"<<z8.r<<" "<<z8.i<<" x "<<r__1<<std::endl;
    q__1.r = z8.r / r__1, q__1.i = z8.i / r__1;
    //	std::cout<<"\t"<<fsi_coulph__.eidc.r<<"
    //"<<fsi_coulph__.eidc.i<<std::endl;
    fsi_coulph__.eidc.r = q__1.r, fsi_coulph__.eidc.i = q__1.i;
  L2:
    Ff(rhos, hs);
  L11:
    msp = fFsiAch._1.mspin;
    if (fFsiNs.isi == 0) { goto L4; }
    /* the strong interaction ON (OFF) if ISI=1(0) */
    if (fFsiPrf.rp < fsi_aa__.aa) { goto L4; }
    if (fJrat != 1) { Firt(); }
    if (fFsiNs.iqs == 0) { goto L5; }
    /* the quantum statistics ON (OFF) if IQS=1(0) */
    jsign = -1;
    i__1  = msp;
    for (jj = 1; jj <= i__1; ++jj) {
      jsign  = -jsign;
      i__2   = jj - 1;
      i__3   = jj - 1;
      z__1.r = fsi_rr__.f[i__2].r * fsi_c__._2.c__[i__3].r - fsi_rr__.f[i__2].i * fsi_c__._2.c__[i__3].i,
      z__1.i = fsi_rr__.f[i__2].r * fsi_c__._2.c__[i__3].i + fsi_rr__.f[i__2].i * fsi_c__._2.c__[i__3].r;
      g.r = z__1.r, g.i = z__1.i;
      if (fFsiNs.ich == 1) {
        z__1.r = fFsiAch._1.achr * g.r, z__1.i = fFsiAch._1.achr * g.i;
        g.r = z__1.r, g.i = z__1.i;
      }
      z__2.r = fsi_fff__.f12.r + g.r, z__2.i = fsi_fff__.f12.i + g.i;
      z__1.r = fFsiFppn.ff12.r * z__2.r - fFsiFppn.ff12.i * z__2.i, z__1.i = fFsiFppn.ff12.r * z__2.i + fFsiFppn.ff12.i * z__2.r;
      psi12.r = z__1.r, psi12.i = z__1.i;
      z__2.r = fsi_fff__.f21.r + g.r, z__2.i = fsi_fff__.f21.i + g.i;
      z__1.r = fFsiFppn.ff21.r * z__2.r - fFsiFppn.ff21.i * z__2.i, z__1.i = fFsiFppn.ff21.r * z__2.i + fFsiFppn.ff21.i * z__2.r;
      psi21.r = z__1.r, psi21.i = z__1.i;
      d__1   = (Double_t) jsign;
      z__2.r = d__1 * psi21.r, z__2.i = d__1 * psi21.i;
      z__1.r = psi12.r + z__2.r, z__1.i = psi12.i + z__2.i;
      g.r = z__1.r, g.i = z__1.i;
      /* L1: */
      /* Computing 2nd power */
      d__1 = g.r;
      /* Computing 2nd power */
      d__2 = d_imag(&g);
      wei += fsi_spin__.rho[jj - 1] * (d__1 * d__1 + d__2 * d__2);
    }
    goto L8;
  L5:
    i__1 = msp;
    for (jj = 1; jj <= i__1; ++jj) {
      i__2   = jj - 1;
      i__3   = jj - 1;
      z__1.r = fsi_rr__.f[i__2].r * fsi_c__._2.c__[i__3].r - fsi_rr__.f[i__2].i * fsi_c__._2.c__[i__3].i,
      z__1.i = fsi_rr__.f[i__2].r * fsi_c__._2.c__[i__3].i + fsi_rr__.f[i__2].i * fsi_c__._2.c__[i__3].r;
      g.r = z__1.r, g.i = z__1.i;
      if (fFsiNs.ich == 1) {
        z__1.r = fFsiAch._1.achr * g.r, z__1.i = fFsiAch._1.achr * g.i;
        g.r = z__1.r, g.i = z__1.i;
      }
      /* W      WRITE(6,38)'JJ ',JJ,'F ',DREAL(F(JJ)),DIMAG(F(JJ)) */
      /* W      WRITE(6,38)'JJ ',JJ,'C ',DREAL(C(JJ)),DIMAG(C(JJ)) */
      /* W      WRITE(6,38)'JJ ',JJ,'G ',DREAL(G),DIMAG(G) */
      /* W      WRITE(6,38)'JJ ',JJ,'F12+G ',DREAL(F12+G),DIMAG(F12+G) */
      /* W      WRITE(6,38)'JJ ',JJ,'F21+G ',DREAL(F21+G),DIMAG(F21+G) */
      /* L38: */
      z__2.r = fsi_fff__.f12.r + g.r, z__2.i = fsi_fff__.f12.i + g.i;
      z__1.r = fFsiFppn.ff12.r * z__2.r - fFsiFppn.ff12.i * z__2.i, z__1.i = fFsiFppn.ff12.r * z__2.i + fFsiFppn.ff12.i * z__2.r;
      psi12.r = z__1.r, psi12.i = z__1.i;
      /* L6: */
      /* Computing 2nd power */
      d__1 = psi12.r;
      /* Computing 2nd power */
      d__2 = d_imag(&psi12);
      wei += fsi_spin__.rho[jj - 1] * (d__1 * d__1 + d__2 * d__2);
    }
    /* --- Account for nn-bar->pp-bar channel --------------------------- */
    if (fFsiNs.ll == 30) {
      i__1 = msp;
      for (jj = 1; jj <= i__1; ++jj) {
        i__2 = jj + 1;
        /* Computing 2nd power */
        d__1 = fsi_c__._2.c__[i__2].r;
        /* Computing 2nd power */
        d__2 = d_imag(&fsi_c__._2.c__[jj + 1]);
        hh   = fsi_spin__.rho[jj - 1] * (d__1 * d__1 + d__2 * d__2) * fsi_2cha__.amu2_amu1__ * fFsiAch._1.ach / fFsiPrf.rps;
        if (fsi_2cha__.ak2s < 0.) { hh *= exp(fFsiPrf.rp * -2 * fsi_2cha__.ak2); }
        /* L61: */
        wei += hh;
      }
    }
    /* ------------------------------------------------------------------ */
    return;
  L4:
    z__1.r  = fFsiFppn.ff12.r * fsi_fff__.f12.r - fFsiFppn.ff12.i * fsi_fff__.f12.i,
    z__1.i  = fFsiFppn.ff12.r * fsi_fff__.f12.i + fFsiFppn.ff12.i * fsi_fff__.f12.r;
    psi12.r = z__1.r, psi12.i = z__1.i;
    //	std::cout<<"C++"<<fsi_fff__.f12.r<<"+"<<fsi_fff__.f12.i<<std::endl;///źle
    if (fFsiNs.iqs == 0) { goto L50; }
    /* the quantum statistics ON (OFF) if IQS=1(0) */
    z__1.r  = fFsiFppn.ff21.r * fsi_fff__.f21.r - fFsiFppn.ff21.i * fsi_fff__.f21.i,
    z__1.i  = fFsiFppn.ff21.r * fsi_fff__.f21.i + fFsiFppn.ff21.i * fsi_fff__.f21.r;
    psi21.r = z__1.r, psi21.i = z__1.i;
    jsign = -1;
    i__1  = msp;
    for (jj = 1; jj <= i__1; ++jj) {
      jsign  = -jsign;
      d__1   = (Double_t) jsign;
      z__2.r = d__1 * psi21.r, z__2.i = d__1 * psi21.i;
      z__1.r = psi12.r + z__2.r, z__1.i = psi12.i + z__2.i;
      g.r = z__1.r, g.i = z__1.i;
      /* L3: */
      /* Computing 2nd power */
      d__1 = g.r;
      /* Computing 2nd power */
      d__2 = d_imag(&g);
      wei += fsi_spin__.rho[jj - 1] * (d__1 * d__1 + d__2 * d__2);
    }
    goto L8;
  L50:
    /* Computing 2nd power */
    d__1 = psi12.r;
    /* Computing 2nd power */
    d__2 = d_imag(&psi12);

    wei = d__1 * d__1 + d__2 * d__2;
    //	std::cout << d__1 << " " << d__2 << std::endl;
    //	std::cout << "||>" << *wei << std::endl;
    return;
  L8:
    wei /= 2;
    return;
  } /* vz_ */

  void FemtoFsiParsed::Firt() {
    /* System generated locals */
    int i__1, i__2, i__3, i__4;
    Double_t d__1;
    complex z__1, z__2;

    /* Local variables */
    Double_t h__;
    int i__;
    Double_t a1, a2, h1, a12, c12, a21;
    int jj;
    Double_t hm, s12, hp, fc1, fc2;
    complex ch1;
    Double_t fs1, fs2, shh, shk, xra;
    int msp;
    Double_t skr;
#define rss ((Double_t*) &fFsiPrf + 9)
#define tss ((Double_t*) &fFsiPrf + 8)
    Double_t tssa, tssh;
    /* ---CALC. THE F(JJ) */
    /* -- F(JJ)*C(JJ)= DEVIATION OF THE BETHE-SALPETER AMPL. FROM PLANE WAVE */
    msp  = fFsiAch._1.mspin;
    i__1 = msp;
    for (jj = 1; jj <= i__1; ++jj) {
      if (jj > 1) { goto L3; }
      xra = *rss * 2 / fFsiAch._1.ac;
      if (fFsiPrf.ak != 0.) { goto L2; }
      shk           = 1.;
      fsi_shh__.sh  = 0.;
      shh           = fsi_shh__.sh;
      fsi_shh__.chh = 1 / *rss;
      goto L3;
    L2:
      h__ = fFsiPrf.ak * *rss;
      Gst(xra, h__);
      fsi_shh__.sh /= *rss;
      fsi_shh__.chh /= *rss;
      shh = fsi_shh__.sh;
      if (fFsiNs.ich == 1) { shh = fFsiAch._1.ach * fsi_shh__.sh; }
    L3:
      if (fFsiNs.ns == 2) { goto L1; }
    /* ---F= ASYMPTOTIC FORMULA (T= 0 APPROX.); NS= 4 */
    L6:
      i__2   = jj - 1;
      z__1.r = fsi_shh__.chh, z__1.i = shh;
      fsi_rr__.f[i__2].r = z__1.r, fsi_rr__.f[i__2].i = z__1.i;
      if (fFsiNs.ns != 1) { goto L10; }
      /* ---F INSIDE THE SQUARE-WELL (T= 0 APPROX.); NS= 1 */
      if (*rss >= fsi_sw__.rb[jj - 1]) { goto L10; }
      if (fFsiPrf.ak != 0. && jj == 1) { shk = fsi_shh__.sh / fFsiPrf.ak; }
      h__ = fsi_sw__.bk[jj - 1] * *rss;
      Gst(xra, h__);
      skr    = fsi_bp__.b * fsi_sw__.bk[jj - 1];
      i__2   = jj - 1;
      i__3   = jj - 1;
      i__4   = jj - 1;
      z__2.r = fsi_sw__.cdk[i__3], z__2.i = fsi_sw__.sdk[i__4];
      z__1.r = skr * z__2.r, z__1.i = skr * z__2.i;
      fsi_rr__.f[i__2].r = z__1.r, fsi_rr__.f[i__2].i = z__1.i;
      d__1   = fsi_sw__.sdkk[jj - 1] * skr - shk * fsi_sw__.sbkrb[jj - 1];
      z__2.r = d__1, z__2.i = 0.;

      z_div(&z__1, &z__2, &fsi_c__._2.c__[jj - 1]);

      ch1.r = z__1.r, ch1.i = z__1.i;
      i__2   = jj - 1;
      i__3   = jj - 1;
      z__2.r = fsi_rr__.f[i__3].r + ch1.r, z__2.i = fsi_rr__.f[i__3].i + ch1.i;
      i__4   = jj - 1;
      z__1.r = z__2.r / fsi_sw__.sbkrb[i__4], z__1.i = z__2.i / fsi_sw__.sbkrb[i__4];
      fsi_rr__.f[i__2].r = z__1.r, fsi_rr__.f[i__2].i = z__1.i;
      goto L10;
    L1:
      /* ---F= ASYMPTOTIC FORMULA (T= 0 NOT REQUIRED); NS= 2 */
      if (jj > 1) { goto L8; }
      if (*tss == 0.) { goto L6; }
      tssa = TMath::Abs(*tss);
      if (fsi_c__._2.dm != 0.) { goto L11; }
      h__ = fsi_c__._2.am * .5 / tssa;
      if (fFsiPrf.ak != 0.) { goto L4; }
      hm = h__ * fFsiPrf.rps;
      if (hm >= 3e15) { goto L6; }
      fs1 = DfrSin(hm);
      fc1 = DfrCos(hm);
      fc2 = fc1;
      fs2 = fs1;
      goto L5;
    L4:
      h1 = fFsiPrf.ak * tssa / fsi_c__._2.am;
      /* Computing 2nd power */
      d__1 = *rss - h1;
      hm   = h__ * (d__1 * d__1);
      /* Computing 2nd power */
      d__1 = *rss + h1;
      hp   = h__ * (d__1 * d__1);
      if (hp >= 3e15) { goto L6; }
      fs1 = DfrSin(hm);
      fc1 = DfrCos(hm);
      fs2 = DfrSin(hp);
      fc2 = DfrCos(hp);
      goto L5;
    L11:
      fs1 = 0.;
      fs2 = 0.;
      fc1 = 0.;
      fc2 = 0.;
      for (i__ = 1; i__ <= 2; ++i__) {
        if (i__ == 1) { tssh = tssa * (fsi_c__._2.dm + 1); }
        if (i__ == 2) { tssh = tssa * (1 - fsi_c__._2.dm); }
        h__ = fsi_c__._2.am * .5 / tssh;
        if (fFsiPrf.ak != 0.) { goto L12; }
        hm = h__ * fFsiPrf.rps;
        if (hm >= 3e15) { goto L6; }
        fs1 += DfrSin(hm) / 2;
        fc1 += DfrCos(hm) / 2;
        if (i__ == 1) { goto L13; }
        fc2 = fc1;
        fs2 = fs1;
        goto L13;
      L12:
        h1 = fFsiPrf.ak * tssh / fsi_c__._2.am;
        /* Computing 2nd power */
        d__1 = *rss - h1;
        hm   = h__ * (d__1 * d__1);
        /* Computing 2nd power */
        d__1 = *rss + h1;
        hp   = h__ * (d__1 * d__1);
        if (hp >= 3e15) { goto L6; }
        fs1 += DfrSin(hm) / 2;
        fc1 += DfrCos(hm) / 2;
        fs2 += DfrSin(hp) / 2;
        fc2 += DfrCos(hp) / 2;
      L13:;
      }
    L5:
      c12    = fc1 + fs2;
      s12    = fs1 + fc2;
      a12    = fs1 - fc2;
      a21    = fs2 - fc1;
      a2     = (fsi_shh__.chh * (a12 + a21) + fsi_shh__.sh * (a12 - a21)) * .5 + shh;
      a1     = (fsi_shh__.chh * (c12 + s12) + fsi_shh__.sh * (c12 - s12)) * .5;
      i__2   = jj - 1;
      z__2.r = a1, z__2.i = a2;
      z__1.r = z__2.r * .3989422, z__1.i = z__2.i * .3989422;
      fsi_rr__.f[i__2].r = z__1.r, fsi_rr__.f[i__2].i = z__1.i;
      goto L10;
    L8:
      i__2               = jj - 1;
      fsi_rr__.f[i__2].r = fsi_rr__.f[0].r, fsi_rr__.f[i__2].i = fsi_rr__.f[0].i;
    L10:;
    }
    return;
  } /* firt_ */
  Double_t FemtoFsiParsed::Exf(Double_t x) {
    if (x < -15.) { return 0; }
    return TMath::Exp(x);
  } /* exf_ */

  void FemtoFsiParsed::Seq(Double_t x, Double_t h__) {
    /* Initialized data */
    Double_t BH[3] = {1, x * 0.5, 0};
    Double_t PH[3] = {1, 0, 0};
    Double_t B     = fsi_bp__.b;  // TODO
    B              = 1 + BH[1];
    Double_t P     = fsi_bp__.p;  // TODO
    P              = 1;
    Double_t HS    = h__ * h__;
    Int_t J        = 0;
    Bool_t calc    = kTRUE;
    Double_t Z;

    do {
      J     = J + 1;
      BH[2] = (x * BH[1] - HS * BH[0]) / ((J + 1) * (J + 2));
      PH[2] = (x * PH[1] - HS * PH[0] - (2 * J + 1) * x * BH[1]) / (J * (J + 1));
      B     = B + BH[2];
      P     = P + PH[2];
      Z     = TMath::Abs(BH[1]) + TMath::Abs(BH[2]) + TMath::Abs(PH[1]) + TMath::Abs(PH[2]);
      //    std::cout<<"Z"<<Z<<std::endl;
      if (Z < 1e-7) {
        calc       = kFALSE;
        fsi_bp__.b = B;
        fsi_bp__.p = P;
        return;
      }
      BH[0] = BH[1];
      BH[1] = BH[2];
      PH[0] = PH[1];
      PH[1] = PH[2];
    } while (calc);
    /*
     fsi_bp__.b += BH[2];
     fsi_bp__.p += PH[2];

     Double_t err = 1e-7;

     int j;
     Double_t z__, bh[3], ph[3], hs;

     // ---CALC. FUNCTIONS B, P (EQS. (17) OF G-K-L-L);
     // -- NEEDED TO CALC. THE CONFLUENT HYPERGEOMETRIC FUNCTION GST.
     bh[0] = 1.;
     ph[0] = 1.;
     ph[1] = 0.;
     bh[1] = x * .5;
     fsi_bp__.b = bh[1] + 1;
     fsi_bp__.p = 1.;
     hs = h__ * h__;
     j = 0;
     L2:
     ++j;
     bh[2] = (x * bh[1] - hs * bh[0]) / ((j + 1) * (j + 2));
     ph[2] = (x * ph[1] - hs * ph[0] - ((j << 1) + 1) * x * bh[1]) / (j * (j
     + 1));
     fsi_bp__.b += bh[2];
     fsi_bp__.p += ph[2];
     z__ = TMath::Abs(bh[1]) + TMath::Abs(bh[2]) + TMath::Abs(ph[1]) +
     TMath::Abs(ph[2]); if (z__ < err) { std::cout<<"SEQ"<<fsi_bp__.b-B<<"
     "<<fsi_bp__.p-P<<std::endl; return ;
     }
     bh[0] = bh[1];
     bh[1] = bh[2];
     ph[0] = ph[1];
     ph[1] = ph[2];
     goto L2;
     */
  } /* seq_ */

  void FemtoFsiParsed::Seqa(Double_t h__) {
    Double_t ARG = h__ - fEta * TMath::Log(2.0 * h__);
    TComplex GST(TMath::Cos(ARG), TMath::Sin(ARG));
    TComplex EIDC(fsi_coulph__.eidc.r, fsi_coulph__.eidc.i);  // TODO
    Double_t ACH  = fFsiAch._1.ach;
    Double_t ACHR = fFsiAch._1.achr;
    GST           = ACHR * EIDC * GST;
    Double_t CHH  = GST.Re();
    Double_t SH   = GST.Im() / ACH;
    Double_t B    = SH / h__;
    fsi_shh__.chh = CHH;
    fsi_shh__.sh  = SH;
    fsi_bp__.b    = B;
    //=====================================
    /* System generated locals */
    Double_t d__1, d__2;
    complex z__1, z__2;

    /* Local variables */
    Double_t arg;
    complex gst;

    /* ---CALC. FUNCTIONS CHH=REAL(GST), SH=IMAG(GST)/ACH, B=SH/H */
    /* -- IN THE ASYMPTOTIC REGION H=K*R >> 1. */
    arg    = h__ - fEta * TMath::Log(h__ * 2);
    d__1   = TMath::Cos(arg);
    d__2   = TMath::Sin(arg);
    z__1.r = d__1, z__1.i = d__2;
    gst.r = z__1.r, gst.i = z__1.i;
    z__2.r = fFsiAch._1.achr * fsi_coulph__.eidc.r, z__2.i = fFsiAch._1.achr * fsi_coulph__.eidc.i;
    z__1.r = z__2.r * gst.r - z__2.i * gst.i, z__1.i = z__2.r * gst.i + z__2.i * gst.r;
    gst.r = z__1.r, gst.i = z__1.i;
    fsi_shh__.chh = gst.r;
    fsi_shh__.sh  = d_imag(&gst) / fFsiAch._1.ach;
    fsi_bp__.b    = fsi_shh__.sh / h__;
    //    std::cout<<"SEQA"<< fsi_shh__.chh-CHH<<" "<< fsi_shh__.sh-SH<<"
    //    "<<fsi_bp__.b  - B<<std::endl;
    return;
  } /* seqa_ */

  void FemtoFsiParsed::Ff(Double_t rho, Double_t h__) {
    /* System generated locals */
    Double_t d__1;
    complex z__1, z__2;

    /* Local variables */
    Double_t c__, s;
    //   Double_t rhom, rhop;

    /* ---Calc. F12, F21; */
    /* -- F12= FF0* plane wave,  FF0=F*ACHR, */
    /* ---F is the confluent hypergeometric function, */
    /* -- ACHR=sqrt(ACH), where ACH is the Coulomb factor */
    c__    = TMath::Cos(h__);
    s      = TMath::Sin(h__);
    d__1   = -s;
    z__1.r = c__, z__1.i = d__1;
    fsi_fff__.f12.r = z__1.r, fsi_fff__.f12.i = z__1.i;
    z__1.r = c__, z__1.i = s;
    fsi_fff__.f21.r = z__1.r, fsi_fff__.f21.i = z__1.i;
    //  std::cout<<"C++FFF"<<fsi_fff__.f12.r <<" "<<fsi_fff__.f12.i<<std::endl;
    // std::cout<<"C++FFF"<<fsi_fff__.f21.r <<" "<<fsi_fff__.f21.i<<std::endl;
    if (fFsiNs.ich == 0) { return; }
    //   rhop = rho + h__;
    //  rhom = rho - h__;
    z__2.r = fsi_fff__.f12.r, z__2.i = fsi_fff__.f12.i;
    //    std::cout<<"C++FFA >"<<z__2.r<<" "<<z__2.i<<") "<<*rho<<"
    //    "<<*h__<<std::endl;
    Ff0(z__2, rho, h__);
    //    std::cout<<"C++ out1 < ("<<z__2.r<<" "<<z__2.i<<")"<<std::endl;
    z__1.r = z__2.r * fsi_fff__.f12.r - z__2.i * fsi_fff__.f12.i, z__1.i = z__2.r * fsi_fff__.f12.i + z__2.i * fsi_fff__.f12.r;
    fsi_fff__.f12.r = z__1.r, fsi_fff__.f12.i = z__1.i;
    d__1   = -(h__);
    z__2.r = fsi_fff__.f21.r, z__2.i = fsi_fff__.f21.i;
    //  std::cout<<"C++FF0 <("<<z__1.r<<" "<<z__1.i<<") "<<std::endl;
    //    std::cout<<"C++FFB >"<<z__2.r<<" "<< z__2.i<<") "<<*rho<<"
    //    "<<-*h__<<std::endl;
    Ff0(z__2, rho, d__1);
    //    std::cout<<"C++ out2 < ("<<z__2.r<<" "<<z__2.i<<")"<<std::endl;
    z__1.r = z__2.r * fsi_fff__.f21.r - z__2.i * fsi_fff__.f21.i, z__1.i = z__2.r * fsi_fff__.f21.i + z__2.i * fsi_fff__.f21.r;
    //  std::cout<<"C++FF0 <("<<z__1.r<<" "<<z__1.i<<") "<<std::endl;
    fsi_fff__.f21.r = z__1.r, fsi_fff__.f21.i = z__1.i;
    fsi_fff__.f21.r = z__1.r, fsi_fff__.f21.i = z__1.i;
    //    std::cout<<"C++ FFA < ("<<fsi_fff__.f12.r<<"
    //    "<<fsi_fff__.f12.i<<")"<<std::endl; std::cout<<"C++ FFB <
    //    ("<<fsi_fff__.f21.r<<" "<<fsi_fff__.f21.i<<")"<<std::endl;
    return;
  } /* ff_ */

  void FemtoFsiParsed::Fas(complex& ret_val, Double_t rks) {
    TComplex FAS;
    Double_t D1, D2;
    D1 = TMath::Log(rks) * fEta;
    D2 = fEta * fEta / rks;
    TComplex ZZ1(TMath::Cos(D1), TMath::Sin(D1));
    TComplex EIDC(fsi_coulph__.eidc.r, fsi_coulph__.eidc.i);  // TODO
    ZZ1       = ZZ1 / EIDC;
    FAS       = (TComplex(1, -D2)) * ZZ1;
    FAS       = FAS - TComplex(TMath::Cos(rks), TMath::Sin(rks)) * fEta / rks / ZZ1;
    ret_val.r = FAS.Re();
    ret_val.i = FAS.Im();
  } /* fas_ */

  /* Double Complex */ void FemtoFsiParsed::Ff0(complex& ret_val, Double_t rho, Double_t h__) {
    /* Initialized data */
    // std::cout<<"C+ ff0 in ("<<ret_val->r<<"
    // "<<ret_val->i<<")\t"<<*rho<<std::endl;
    Double_t err = 1e-5;

    /* System generated locals */
    int i__1;
    Double_t d__1;
    complex z__1, z__2;

    /* Local variables */
    complex a;
    int j = 0;
    complex s, z__;
    Double_t zi, zr;
    complex alf;
    complex alf1;
    Double_t rhop;

    /* -- FF0=F*ACHR */
    /* -- F is the confluent hypergeometric function */
    /* -- (Eq. (15) of G-K-L-L), F= 1 at r* << AC */
    /* -- ACHR=sqrt(ACH), where ACH is the Coulomb factor */
    s.r = 1., s.i = 0.;
    ret_val.r = s.r, ret_val.i = s.i;
    rhop = rho + h__;
    /* C      GOTO 5 ! rejects the approx. calcul. of hyperg. f-ion F */
    if (rhop < 20.) { goto L5; }
    Fas(z__1, rhop);
    ret_val.r = z__1.r, ret_val.i = z__1.i;
    /* approx. calc. */
    return;
  L5:
    d__1   = -fEta;
    z__1.r = 0., z__1.i = d__1;
    alf.r = z__1.r, alf.i = z__1.i;
    z__1.r = alf.r - 1, z__1.i = alf.i;
    alf1.r = z__1.r, alf1.i = z__1.i;
    z__1.r = 0., z__1.i = rhop;
    z__.r = z__1.r, z__.i = z__1.i;
    j = 0;
  L3:
    ++j;
    z__2.r = alf1.r + j, z__2.i = alf1.i;
    i__1   = j * j;
    d__1   = (Double_t) i__1;
    z__1.r = z__2.r / d__1, z__1.i = z__2.i / d__1;
    a.r = z__1.r, a.i = z__1.i;
    z__2.r = s.r * a.r - s.i * a.i, z__2.i = s.r * a.i + s.i * a.r;
    z__1.r = z__2.r * z__.r - z__2.i * z__.i, z__1.i = z__2.r * z__.i + z__2.i * z__.r;
    s.r = z__1.r, s.i = z__1.i;
    z__1.r = ret_val.r + s.r, z__1.i = ret_val.i + s.i;
    ret_val.r = z__1.r, ret_val.i = z__1.i;
    zr = (d__1 = s.r, TMath::Abs(d__1));
    zi = (d__1 = d_imag(&s), TMath::Abs(d__1));
    if (zr + zi > err) { goto L3; }
    z__1.r = fFsiAch._1.achr * ret_val.r, z__1.i = fFsiAch._1.achr * ret_val.i;
    ret_val.r = z__1.r, ret_val.i = z__1.i;
    //   std::cout<<"C+ ff0 out ("<<ret_val->r<<" "<<ret_val->i<<")\t"<<std::endl;
    return;
  } /* ff0_ */

  Double_t FemtoFsiParsed::Hc(Double_t xa) {
    /* Initialized data */

    Double_t bn[15] = {.08333333333,
                       .008333333333,
                       .00396825396825,
                       .004166666667,
                       .007575757576,
                       .02109279609,
                       .08333333333,
                       .4432598039,
                       3.05395433,
                       26.45621212,
                       281.4601449,
                       3607.510546,
                       54827.58333,
                       974936.8235,
                       20052695.8};

    /* System generated locals */
    int i__1;
    Double_t ret_val, d__1;

    /* Local variables */
    int i__, n;
    Double_t s, x, x2, ds, xp;
    int ima;

    /* ---HC = h-function of Landau-Lifshitz: h(x)=Re[psi(1-i/x)]+ln(x) */
    /* -- psi(x) is the digamma function (the logarithmic derivative of */
    /* -- the gamma function) */
    x = TMath::Abs(xa);
    if (x < .33) {
      x2      = x * x;
      xp      = x2;
      ret_val = 0.;
      ima     = 9;
      if (x < .1) { ima = 3; }
      i__1 = ima;
      for (i__ = 1; i__ <= i__1; ++i__) {
        ret_val += xp * bn[i__ - 1];
        /* L4: */
        xp = x2 * xp;
      }
      return ret_val;
    }
    /* C      IF(X.GE.3.5D0) GO TO 2 */
    s = 0.;
    n = 0;

    do {
      ++n;
      /* Computing 2nd power */
      d__1 = n * x;
      ds   = 1. / n / (d__1 * d__1 + 1);
      s += ds;
    } while (ds > 1e-13);
    ret_val = s - .5772156649 + TMath::Log(x);
    return ret_val;

  } /* hc_ */

  Double_t FemtoFsiParsed::Acp(Double_t x) {
    /* --- ACP = COULOMB PENETRATION FACTOR */
    if (x < .05 && x >= 0.) { return 1e-6; }
    Double_t y = 6.2831853 / x;
    return y / (Exf(y) - 1);
  } /* acp_ */

  void FemtoFsiParsed::Gst(Double_t x, Double_t h__) {
    if (fFsiNs.ich == 1) {  // goto 2
      if (h__ > 15.0) {     // goto 4
        Seqa(h__);
      } else {
        Seq(x, h__);
        /* exact calculation */
        fsi_shh__.sh  = h__ * fsi_bp__.b;
        fsi_shh__.chh = fsi_bp__.p + fsi_bp__.b * x * (TMath::Log((TMath::Abs(x))) + fFsiAch._1.hpr);
      }
    } else {
      fsi_shh__.sh  = TMath::Sin(h__);
      fsi_shh__.chh = TMath::Cos(h__);
      fsi_bp__.b    = 1.;
      if (h__ != 0.) { fsi_bp__.b = fsi_shh__.sh / h__; }
      fsi_bp__.p = fsi_shh__.chh;
    }

    /*

     if (fFsiNs.ich == 1) {
     goto L2;
     }

     fsi_shh__.sh = TMath::Sin(h__);
     fsi_shh__.chh = TMath::Cos(h__);
     fsi_bp__.b = 1.;
     if (h__ != 0.) {
     fsi_bp__.b = fsi_shh__.sh / h__;
     }
     fsi_bp__.p = fsi_shh__.chh;
     return;
     L2:
     if (h__ > 15.) {
     goto L4;
     }

     Seq(x, h__);

     fsi_shh__.sh = h__ * fsi_bp__.b;
     fsi_shh__.chh = fsi_bp__.p + fsi_bp__.b * x * (TMath::Log((TMath::Abs(x))) +
     fFsiAch._1.hpr);
     return;
     L4:
     Seqa(x, h__);
     return;
     */
  } /* gst_ */

  void FemtoFsiParsed::Ff1(complex& ret_val, Double_t rho, Double_t h__) {
    /* System generated locals */
    Double_t r__1;
    complex q__1;
    complex z__1;

    /* Local variables */
    complex z8;

    /* ---FF1=FF0; used for particle-nucleus system */
    /* -- FF0=F12*ACHR */
    /* -- F12 is the confluent hypergeometric function */
    /* -- (Eq. (15) of G-K-L-L), F12= 1 at r* << AC */
    /* -- ACHR=sqrt(ACH), where ACH is the Coulomb factor */
    ret_val.r = 1., ret_val.i = 0.;
    if (fIch == 0) { goto L2; }
    if (rho < 15. && rho + h__ < 20.) { goto L2; }
    /* ---Calc. EIDC=exp(i*Coul.Ph.); */
    /* -- used in calc. of hypergeom. f-s in SEQA, FAS at k*R > 15, 20 */
    r__1   = (Double_t) fEta;
    q__1.r = (Double_t) 1., q__1.i = r__1;
    z8.r = q__1.r, z8.i = q__1.i;
    Cgamma(q__1, z8);
    //    std::cout<<"Z8 cabs"<<z8.r<<" "<<z8.i<<" x "<<q__1.r<<"
    //    "<<q__1.i<<std::endl;
    z8.r = q__1.r, z8.i = q__1.i;
    r__1   = c_abs(&z8);
    q__1.r = z8.r / r__1, q__1.i = z8.i / r__1;
    fsi_coulph__.eidc.r = q__1.r, fsi_coulph__.eidc.i = q__1.i;
  L2:
    Ff0(z__1, rho, h__);
    ret_val.r = z__1.r, ret_val.i = z__1.i;
    return;
  } /* ff1_ */

  Double_t FemtoFsiParsed::G(Double_t ak) {
    /* System generated locals */
    Double_t ret_val, d__1;

    /* Local variables */
    Double_t h__, x;
    Double_t gg, xh, zz;
    Double_t hcp, aks;
    Double_t brho;
    Double_t brhop, brhos, prhos;

    /* ---Used to calculate SW scattering amplitude for alpa-alpha system */
    /* -- and for sear.f (square well potential search) */
    /* ---NOTE THAT SCATT. AMPL.= 1/CMPLX(G(AK),-AK*ACH) */
    fsi_dak__.hac  = 0.;
    fFsiAch._2.ach = 1.;
    if (fFsiNs.ich != 0) {
      xh             = fFsiAch._2.ac * ak;
      hcp            = Hc(xh);
      fFsiAch._2.hpr = hcp + .1544313298;
      fFsiAch._2.ach = Acp(xh);
      fsi_dak__.hac  = hcp * 2 / fFsiAch._2.ac;
    }
    /* Computing 2nd power */
    d__1 = ak;
    aks  = d__1 * d__1;
    /* Computing 2nd power */
    d__1                           = fsi_sw__.eb[fFsiAch._2.jj - 1];
    fsi_sw__.bk[fFsiAch._2.jj - 1] = TMath::Sqrt(aks + d__1 * d__1);
    /* kappa=kp */
    x   = fsi_sw__.rb[fFsiAch._2.jj - 1] * 2 / fFsiAch._2.ac;
    h__ = fsi_sw__.bk[fFsiAch._2.jj - 1] * fsi_sw__.rb[fFsiAch._2.jj - 1];
    /* kp*d */
    Gst(x, h__);
    brho = fsi_bp__.b;
    /* B(kp,d) */
    fsi_sw__.sbkrb[fFsiAch._2.jj - 1] = fsi_shh__.sh;
    /* kp*d*B(kp,d) */
    Deriw(x, h__);
    brhop = fsi_deriv__.bpr;
    /* B'(kp,d)= dB(kp,r)/dln(r) at r=d */
    h__ = ak * fsi_sw__.rb[fFsiAch._2.jj - 1];
    Gst(x, h__);
    fsi_sw__.cdk[fFsiAch._2.jj - 1] = fsi_shh__.chh;
    /* ReG(k,d) */
    brhos = fsi_bp__.b;
    /*  B(k,d) */
    prhos = fsi_bp__.p;
    /*  P(k,d) */
    fsi_sw__.sdk[fFsiAch._2.jj - 1] = fsi_shh__.sh;
    if (fFsiNs.ich != 0) {
      fsi_sw__.sdk[fFsiAch._2.jj - 1] = fFsiAch._2.ach * fsi_shh__.sh;
      /* ImG(k,d) */
      if (ak == 0. && fFsiAch._2.ac < 0.) { fsi_sw__.sdk[fFsiAch._2.jj - 1] = x * (Double_t) 3.14159 * fsi_bp__.b; }
    }
    fsi_sw__.sdkk[fFsiAch._2.jj - 1] = fsi_sw__.rb[fFsiAch._2.jj - 1];
    if (ak != 0.) { fsi_sw__.sdkk[fFsiAch._2.jj - 1] = fsi_shh__.sh / ak; }
    /* d*B(k,d) */
    Deriw(x, h__);
    /* PPR=P'(k,d)= dP(k,r)/dln(r) at r=d */
    zz = fsi_deriv__.ppr - prhos;
    if (fFsiNs.ich == 1) { zz += x * (brhos + fsi_deriv__.bpr * (TMath::Log((TMath::Abs(x))) + fFsiAch._2.hpr)); }
    /*   ZZ= P'(k,d)-P(k,d)+x*{B(k,d)+B'(k,d)*[ln!x!+2*C-1+h(k*ac)]} */
    gg = (brhop * fsi_sw__.cdk[fFsiAch._2.jj - 1] - brho * zz) / fsi_sw__.rb[fFsiAch._2.jj - 1];
    /*   GG= [B'(kp,d)*ReG(k,d)-B(kp,d)*ZZ]/d */
    ret_val = gg / (brho * fsi_deriv__.bpr - brhop * brhos);
    /*    G= GG/[B(kp,d)*B'(k,d)-B'(kp,d)*B(k,d)] */
    return ret_val;
  } /* g_ */

  void FemtoFsiParsed::Deriw(Double_t x, Double_t h__) {
    /* System generated locals */
    Double_t d__1;

    /* Local variables */
    Double_t b1, q1, hh, hhh;

    /* ---CALLED BY F-N G(AK) */
    hh   = 1e-4;
    d__1 = h__ - hh;
    Gst(x, d__1);
    q1   = fsi_bp__.p;
    b1   = fsi_bp__.b;
    d__1 = h__ + hh;
    Gst(x, d__1);
    hhh             = hh + hh;
    fsi_deriv__.bpr = h__ * (fsi_bp__.b - b1) / hhh;
    fsi_deriv__.ppr = h__ * (fsi_bp__.p - q1) / hhh;
    if (fFsiNs.ich == 0) return;
    d__1 = x - hh;
    Gst(d__1, h__);
    q1   = fsi_bp__.p;
    b1   = fsi_bp__.b;
    d__1 = x + hh;
    Gst(d__1, h__);
    fsi_deriv__.bpr += x * (fsi_bp__.b - b1) / hhh;
    fsi_deriv__.ppr += x * (fsi_bp__.p - q1) / hhh;
  } /* deriw_ */

  /* ================================================================ */
  /*
   int FemtoFsiParsed::read_file__(char *key, char *ch8, int *int4,
   Double_t *real8, int *ierr, int *nunit, int key_len,
   int ch8_len)
   {
   //std::cout<<"READING"<<std::endl;
   //
   //    System generated locals
   alist al__1, al__2;


   Local variables
   static char test[10], type__[4];

   //     Fortran I/O blocks
   static cilist io___167 = { 0, 0, 0, "(A10,2X,A4)", 0 };
   static cilist io___170 = { 0, 0, 0, "(18X,A8,54X)", 0 };
   static cilist io___171 = { 0, 0, 0, "(18X,I8,54X)", 0 };
   static cilist io___172 = { 0, 0, 0, "(18X,F10.5,52X)", 0 };


   *ierr = 0;
   al__1.aerr = 0;
   al__1.aunit = *nunit;
   f_rew(&al__1);
   L1:
   io___167.ciunit = *nunit;
   s_rsfe(&io___167);
   do_fio(&c__1, test, (int)10);
   do_fio(&c__1, type__, (int)4);
   e_rsfe();
   if (s_cmp(test, key, (int)10, (int)10) == 0) {
   al__2.aerr = 0;
   al__2.aunit = *nunit;
   f_back(&al__2);
   if (s_cmp(type__, "CHAR", (int)4, (int)4) == 0) {
   io___170.ciunit = *nunit;
   s_rsfe(&io___170);
   do_fio(&c__1, ch8, (int)8);
   e_rsfe();
   }
   if (s_cmp(type__, "INT4", (int)4, (int)4) == 0) {
   io___171.ciunit = *nunit;
   s_rsfe(&io___171);
   do_fio(&c__1, (char *)&(*int4), (int)sizeof(int));
   e_rsfe();
   }
   if (s_cmp(type__, "REA8", (int)4, (int)4) == 0) {
   io___172.ciunit = *nunit;
   s_rsfe(&io___172);
   do_fio(&c__1, (char *)&(*real8), (int)sizeof(Double_t));
   e_rsfe();
   }
   } else {
   if (s_cmp(test, "* E.O.F. *", (int)10, (int)10) != 0) {
   goto L1;
   } else {
   *ierr = 1;
   }
   }

   //    IF(IERR.EQ.1)STOP
   //standard end of comment
   return 0;
   }
   */

  TComplex FemtoFsiParsed::GetC(Int_t i) const { return TComplex(fsi_c__._2.c__[i].r, fsi_c__._2.c__[i].i); }

  void FemtoFsiParsed::SetC(TComplex Z, Int_t i) {
    fsi_c__._2.c__[i].r = Z.Re();
    fsi_c__._2.c__[i].i = Z.Im();
  }
}  // namespace Hal
