/*
 * FemtoFsiParsed.h
 *
 *  Created on: 30 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOFSIPARSED_H_
#define HALFEMTOFSIPARSED_H_
#include "TComplex.h"
#include "TObject.h"
#include <complex>

namespace Hal {
  class FemtoWeightGeneratorLednicky2;
  class FemtoFsiParsed : public TObject {
    friend class FemtoWeightGeneratorLednicky2;

  protected:
    int ifirst;
    //===================
    typedef struct {
      Double_t r, i;
    } complex;

    Int_t fJrat;

    struct fsi_poc__ {
      Double_t amn, am1, am2, cn, c1, c2, ac1, ac2;
    };
    fsi_poc__ fFsiPoc;

    struct fsi_mom__ {
      Double_t p1x, p1y, p1z, e1, p1, p2x, p2y, p2z, e2, p2;
    };
    fsi_mom__ fFsiMom;

    struct fsi_prf__ {
      Double_t ppx, ppy, ppz, ak, aks, x, y, z__, t, rp, rps;
    };
    fsi_prf__ fFsiPrf;

    struct fsi_coor__ {
      Double_t x1, y1, z1, t1, r1, x2, y2, z2, t2, r2;
    };
    fsi_coor__ fFsiCoor;

    struct fsi_ns__ {
      int ll, ns, ich, isi, iqs, i3c, i3s;
    };
    fsi_ns__ fFsiNs;

    struct fsi_ffpn__ {
      complex ff12, ff21;
    };
    fsi_ffpn__ fFsiFppn;

    struct fsi_p12__ {
      Double_t p12x, p12y, p12z, e12, p12, am12, epm;
    };
    fsi_p12__ fSi_p12;

    struct fsi_cons__ {
      Double_t spi, dr, w;
    };
    fsi_cons__ fFsiCons;

    union fsi_ach__ {
      struct {
        Double_t hpr, ac, ach, achr;
        int ispin, mspin;
      } _1;
      struct {
        Double_t hpr, ac, ach, achr;
        int jj, mspin;
      } _2;
    };
    fsi_ach__ fFsiAch;

    Int_t fIch;

    Double_t fEta;

    union {
      struct {
        Double_t help[20], am, ams, dm;
      } _1;
      struct {
        complex c__[10];
        Double_t am, ams, dm;
      } _2;
    } fsi_c__;

    struct fsi_aapap__1_ {
      Double_t aapapr[6] /* was [3][2] */, aapapi[6] /* was [3][2]
                                                      */
        ;
    };

#define fsi_aapap__1 (*(struct fsi_aapap__1_*) &fsi_aapap__)

    struct {
      Double_t ak2, ak2s, aak, hcp2, amu2_amu1__;
    } fsi_2cha__;

    struct {
      Double_t rho[10];
    } fsi_spin__;

    struct {
      Double_t fd[10], rd[10];
    } fsi_fd__;

    struct {
      Double_t aa;
    } fsi_aa__;

    struct {
      Double_t rb[10], eb[10], bk[10], cdk[10], sdk[10], sbkrb[10], sdkk[10];
    } fsi_sw__;

    struct fsi_fdh__1_ {
      Double_t fdh[300] /* was [30][10] */, rdh[300] /* was [30][10] */, ebh[300] /* was [30][10] */, rbh[300] /* was [30][10] */;
    };

#define fsi_fdh__1 (*(struct fsi_fdh__1_*) &fsi_fdh__)

    struct fsi_rhoh__1_ {
      Double_t rhoh[300] /* was [30][10] */;
    };

#define fsi_rhoh__1 (*(struct fsi_rhoh__1_*) &fsi_rhoh__)

    struct fsi_amch__1_ {
      Double_t am1h[30], am2h[30], c1h[30], c2h[30];
      int mspinh[30];
    };

#define fsi_amch__1 (*(struct fsi_amch__1_*) &fsi_amch__)

    struct {
      Double_t v, cvk;
    } fsi_cvk__;

    struct {
      Double_t b, p;
    } fsi_bp__;

    struct {
      complex f[10];
    } fsi_rr__;

    struct {
      Double_t sh, chh;
    } fsi_shh__;

    struct {
      complex f12, f21;
    } fsi_fff__;

    struct {
      complex eidc;
    } fsi_coulph__;

    struct {
      Double_t bpr, ppr;
    } fsi_deriv__;

    struct {
      Double_t dak, hac;
      int ifun;
    } fsi_dak__;

    typedef struct {
      int cierr;
      int ciunit;
      int ciend;
      char* cifmt;
      int cirec;
    } cilist;
    /* Initialized data */
    typedef struct {
      int aerr;
      int aunit;
    } alist;
    typedef struct {
      int icierr;
      char* iciunit;
      int iciend;
      char* icifmt;
      int icirlen;
      int icirnum;
    } icilist;
    struct double_forty {
      Double_t e_1[40];
    };
    double_forty fFsiAapi;
    double_forty fFsiApin;

    struct double_eighty {
      Double_t e_1[80];
    };
    double_eighty fFsiAand;
    struct double_sixty {
      Double_t e_1[60];
    };
    double_sixty fFsiAadd;

    struct double_nine {
      Double_t e_1[9];
    };
    double_nine fFsiAakk;

    struct double_eleven {
      Double_t e_1[12];
    };
    double_eleven fsi_aapap__;

    struct double_thousand2 {
      Double_t e_1[1200];
    };
    double_thousand2 fsi_fdh__;

    struct double_threehundgred {
      Double_t e_1[300];
    };
    double_threehundgred fsi_rhoh__;

    struct {
      Double_t e_1[120];
      int e_2[30];
    } fsi_amch__;
    int c__1;
    complex c_b5;
    complex c_b2;
    int c__2;
    complex c_b33;
    complex c_b34;

    //=============
    TComplex GetC(Int_t i) const;
    void SetC(TComplex Z, Int_t i);

    /*	int read_file__(char *key, char *ch8, int *int4, Double_t *real8, int
     *ierr, int *nunit, int key_len, int ch8_len);
     */

    Double_t r_imag(complex* x) { return (Double_t)(x->i); }
    void c_div(complex* a, complex* b, complex* c) {
      std::complex<Double_t> z1(b->r, b->i);
      std::complex<Double_t> z2(c->r, c->i);
      std::complex<Double_t> div = z1 / z2;
      Double_t im, re;
      re   = div.real();
      im   = div.imag();
      a->r = re;
      a->i = im;
    }
    void c_exp(complex* a, complex* b) {
      std::complex<Double_t> z1(b->r, b->i);
      std::complex<Double_t> ex = std::exp(z1);
      Double_t im, re;
      re   = ex.real();
      im   = ex.imag();
      a->r = re;
      a->i = im;
    }
    void c_log(complex* a, complex* b) {
      std::complex<Double_t> z1(b->r, b->i);
      std::complex<Double_t> ex = std::log(z1);
      Double_t im, re;
      re   = ex.real();
      im   = ex.imag();
      a->r = re;
      a->i = im;
    }
    void c_sin(complex* a, complex* b) {
      std::complex<Double_t> z1(b->r, b->i);
      std::complex<Double_t> ex = std::sin(z1);
      Double_t im, re;
      re   = ex.real();
      im   = ex.imag();
      a->r = re;
      a->i = im;
    }
    void z_div(complex* a, complex* b, complex* c) {
      std::complex<Double_t> z1(b->r, b->i);
      std::complex<Double_t> z2(c->r, c->i);
      std::complex<Double_t> div = z1 / z2;
      Double_t im, re;
      re   = div.real();
      im   = div.imag();
      a->r = re;
      a->i = im;
    }

    void Ltran(Double_t* p0, Double_t* p, Double_t* ps);
    void Ltranb(Double_t* p0, Double_t* ps, Double_t* p);
    void Fsinucl(Double_t r_amn__, Double_t r_cn__);
    void Fsipn(Double_t& weif);
    void Fsiin(int i_itest__, int i_ich__, int i_iqs__, int i_isi__, int i_i3c__);
    void Llini(int lll, int i_ns__, int i_itest__);

    void Fsiw(int j, Double_t& weif, Double_t& wei, Double_t& wein);
    void Cgamma(complex& ret_val, complex& z__);
    void SetMomentum(Double_t* pp1, Double_t* pp2);
    void SetPosition(Double_t* xt1, Double_t* xt2);
    void Ltran12();
    void Boosttoprf();
    Double_t c_abs(complex* x) {
      //	std::cout<<"S="<<std::sqrt(x->i * x->i + x->r + x->r)<<" from =
      //"<<x->r<<" "<<x->i<<std::endl;
      return std::sqrt(x->i * x->i + x->r * x->r);
    }
    Double_t d_imag(complex* x) { return x->i; };

    Double_t DfrSin(Double_t x);
    Double_t DfrCos(Double_t x);
    void Vz(Double_t& wei);
    void Fsiwf(Double_t& wei);

    void Ff0(complex& ret_val, Double_t rho, Double_t h__);
    void Ff1(complex& ret_val, Double_t rho, Double_t h__);
    void Ff(Double_t rho, Double_t h__);
    void Firt();
    void Ckkb();

    void Cpap();

    Double_t Exf(Double_t x);
    Double_t G(Double_t ak);
    Double_t Gpin(Double_t x, int j);
    Double_t Gnd(Double_t x, int j);
    Double_t Gdd(Double_t x, int j);
    Double_t Gpipi(Double_t x, int j);
    Double_t Hc(Double_t xa);
    Double_t Acp(Double_t x);
    void Deriw(Double_t x, Double_t h__);
    void Gst(Double_t x, Double_t h__);
    void Seq(Double_t x, Double_t h__);
    void Seqa(Double_t h__);
    void Fas(complex& ret_val, Double_t rks);

  public:
    FemtoFsiParsed();
    virtual ~FemtoFsiParsed();
    ClassDef(FemtoFsiParsed, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOFSIPARSED_H_ */
