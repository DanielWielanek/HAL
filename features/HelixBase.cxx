/*
 * HalHelixBase.cxx
 *
 *  Created on: 7 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HelixBase.h"

#include <TMath.h>
#include <iostream>

namespace Hal {
  Double_t HelixBase::fgHelixBz = 0.5;

  HelixBase::~HelixBase() {}

  HelixBase::HelixBase() :
    fX0(0),
    fY0(0),
    fZ0(0),
    fPhi0(0),
    fPhiSin(0),
    fPhiCos(0),
    fLambda(0),
    fLambdaCos(0),
    fLambdaSin(0),
    fCurv(0),
    fXc(0),
    fYc(0),
    fH(0) {}

  HelixBase::HelixBase(const HelixBase& t) :
    TObject(t),
    fX0(t.fX0),
    fY0(t.fY0),
    fZ0(t.fZ0),
    fPhi0(t.fPhi0),
    fPhiCos(t.fPhiCos),
    fPhiSin(t.fPhiSin),
    fLambda(t.fLambda),
    fLambdaCos(t.fLambdaCos),
    fLambdaSin(t.fLambdaSin),
    fCurv(t.fCurv),
    fXc(t.fXc),
    fYc(t.fYc),
    fH(t.fH) {}

  HelixBase::HelixBase(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) : HelixBase() {
    Double_t pt = mom.Pt();
    if (TMath::Abs(conversion) < 0.00000001) conversion = -1.0 / 0.299792458 / GetBz();
    fH    = -TMath::Sign(1.0, GetBz() * charge);
    fCurv = charge / (conversion * pt) * 0.01;  // C [cm]
    if (conversion * pt == 0) fCurv = 0;        // singularity !
    fCurv      = TMath::Abs(fCurv);
    fLambda    = TMath::ATan2(mom.Pz(), pt);
    fLambdaCos = TMath::Cos(fLambda);
    fLambdaSin = TMath::Sin(fLambda);

    Double_t Psi = mom.Phi();
    fPhi0        = TVector2::Phi_mpi_pi(Psi - fH * TMath::PiOver2());
    fPhiSin      = TMath::Sin(fPhi0);
    fPhiCos      = TMath::Cos(fPhi0);
    fXc          = x.X() - fPhiCos / fCurv;
    fYc          = x.Y() - fPhiSin / fCurv;
    //


    fX0 = x.X();  // x0
    fY0 = x.Y();  // y0
    fZ0 = x.Z();  // z0
  }

  Int_t HelixBase::BaseIntersection(Double_t R, TVector3& x1, TVector3& x2) const {
    Double_t s1, s2;
    BasePathLength(R, s1, s2);
    if (s1 == s2) {
      x1 = BasePosition(s1);
      return 1;
    } else if (s1 == MaxPath()) {
      return 0;
    };
    if (s1 < s2) {
      x1 = BasePosition(s1);
      x2 = BasePosition(s2);
    } else {
      x2 = BasePosition(s1);
      x1 = BasePosition(s2);
    }
    return 2;
  }

  Double_t HelixBase::GetPeriod() const {
    if (fCurv == 0) {
      return 1E+9;
    } else {  //
      return TMath::Abs(TMath::TwoPi() / (fH * GetCurv() * fLambdaCos));
    }
  }

  void HelixBase::BasePrint() const {
    std::cout << "*********** HALHELIX ***********" << std::endl;
    std::cout << Form("X0 %4.4f %4.4f %4.4f", fX0, fY0, fZ0) << std::endl;
    std::cout << Form("Xc = %4.4f Yc=%4.4f L=%4.4f ", fXc, fYc, fLambda) << std::endl;
    std::cout << Form("Phi0=%4.2f Curv=%4.4f", fPhi0 / TMath::DegToRad(), fCurv) << std::endl;
  }

  void HelixBase::BaseSetParams(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) {
    Double_t pt = mom.Pt();
    if (TMath::Abs(conversion) < 0.00000001) conversion = -1 / 0.299792458 / GetBz();
    fH    = -TMath::Sign(1.0, GetBz() * charge);
    fCurv = charge / (conversion * pt);       // C
    if (conversion * pt == 0) fCurv = 0;      // singularity !
    fCurv        = TMath::Abs(fCurv * 0.01);  // goto cm
    fLambda      = TMath::ATan2(mom.Pz(), pt);
    fLambdaCos   = TMath::Cos(fLambda);
    fLambdaSin   = TMath::Sin(fLambda);
    Double_t Psi = mom.Phi();
    fPhi0        = Psi - fH * TMath::PiOver2();
    if (fPhi0 < -TMath::Pi()) fPhi0 += TMath::TwoPi();
    if (fPhi0 > TMath::Pi()) fPhi0 -= TMath::TwoPi();
    fPhiSin = TMath::Sin(fPhi0);
    fPhiCos = TMath::Cos(fPhi0);
    fXc     = x.X() - fPhiCos / fCurv;
    fYc     = x.Y() - fPhiSin / fCurv;
    //
    fX0 = x.X();  // x0
    fY0 = x.Y();  // y0
    fZ0 = x.Z();  // z0
  }

  void HelixBase::BaseSetParams(Double_t x,
                                Double_t y,
                                Double_t z,
                                Double_t px,
                                Double_t py,
                                Double_t pz,
                                Double_t charge,
                                Double_t conversion) {
    Double_t pt = TMath::Sqrt(px * px + py * py);
    if (TMath::Abs(conversion) < 0.00000001) conversion = -1 / 0.299792458 / GetBz();
    fH    = -TMath::Sign(1.0, GetBz() * charge);
    fCurv = charge / (conversion * pt);     // C
    if (conversion * pt == 0) fCurv = 0;    // singularity !
    fCurv      = TMath::Abs(fCurv * 0.01);  // goto cm
    fLambda    = TMath::ATan2(pz, pt);
    fLambdaCos = TMath::Cos(fLambda);
    fLambdaSin = TMath::Sin(fLambda);

    Double_t Psi = 0;
    if (pt != 0 && pz != 0) Psi = TMath::ATan2(py, px);
    fPhi0 = Psi - fH * TMath::PiOver2();
    if (fPhi0 < -TMath::Pi()) fPhi0 += TMath::TwoPi();
    if (fPhi0 > TMath::Pi()) fPhi0 -= TMath::TwoPi();
    fPhiSin = TMath::Sin(fPhi0);
    fPhiCos = TMath::Cos(fPhi0);
    fXc     = x - fPhiCos / fCurv;
    fYc     = y - fPhiSin / fCurv;
    //
    fX0 = x;  // x0
    fY0 = y;  // y0
    fZ0 = z;  // z0
  }


  void HelixBase::SetMagField(Double_t Bz) { fgHelixBz = Bz; }

  Double_t HelixBase::GetBz() { return fgHelixBz; }

  HelixBase& HelixBase::operator=(const HelixBase& helix) {
    if (this != &helix) {
      fY0     = helix.fY0;
      fZ0     = helix.fZ0;
      fX0     = helix.fX0;
      fPhi0   = helix.fPhi0;
      fLambda = helix.fLambda;
      fCurv   = helix.fCurv;
      fXc     = helix.fXc;
      fYc     = helix.fYc;
      fH      = helix.fH;
    }
    return *this;
  }

  TVector3 HelixBase::BasePosition(Double_t s) const {
    if (fCurv == 0) { return TVector3(fX0 - s * fLambdaCos * fPhiSin, fY0 + s * fLambdaCos * fPhiCos, fZ0 + s * fLambdaSin); }
    Double_t overC = 1.0 / fCurv;
    return TVector3(fX0 + (TMath::Cos(fPhi0 + s * fH * fCurv * fLambdaCos) - fPhiCos) * overC,
                    fY0 + (TMath::Sin(fPhi0 + s * fH * fCurv * fLambdaCos) - fPhiSin) * overC,
                    fZ0 + s * fLambdaSin);
  }

  TVector3 HelixBase::BaseGetStartPoint() const { return TVector3(fX0, fY0, fZ0); }

  Double_t HelixBase::FudgePathLenght(const TVector3& vec) const {
    Double_t s;
    Double_t dx = vec.X() - fX0;
    Double_t dy = vec.Y() - fY0;

    if (fCurv == 0) {
      s = (dy * fPhiCos - dx * fPhiSin) / fLambdaCos;
    } else {
      s = TMath::ATan2(dy * fPhiCos - dx * fPhiSin, 1. / fCurv + dx * fPhiCos + dy * fPhiSin) / (fH * fCurv * fLambdaCos);
    }
    return s;
  }

  void HelixBase::BasePathLength(Double_t r, Double_t& s1, Double_t& s2) const {
    //
    // The math is taken from Maple with C(expr,optimized) and
    // some hand-editing. It is not very nice but efficient.
    // 'first' is the smallest of the two solutions (may be negative)
    // 'second' is the other.
    //
    if (fCurv == 0) {
      Double_t t1  = fLambdaCos * (fX0 * fPhiSin - fY0 * fPhiCos);
      Double_t t12 = fY0 * fY0;
      Double_t t13 = fPhiCos * fPhiCos;
      Double_t t15 = r * r;
      Double_t t16 = fX0 * fX0;
      Double_t t20 = -fLambdaCos * fLambdaCos * (2.0 * fX0 * fPhiSin * fY0 * fPhiCos + t12 - t12 * t13 - t15 + t13 * t16);
      if (t20 < 0.) {
        s1 = s2 = 1E+9;
        return;
      }
      t20 = TMath::Sqrt(t20);
      s1  = (t1 - t20) / (fLambdaCos * fLambdaCos);
      s2  = (t1 + t20) / (fLambdaCos * fLambdaCos);
    } else {
      Double_t t1  = fY0 * fCurv;
      Double_t t2  = fPhiSin;
      Double_t t3  = fCurv * fCurv;
      Double_t t4  = fY0 * t2;
      Double_t t5  = fPhiCos;
      Double_t t6  = fX0 * t5;
      Double_t t8  = fX0 * fX0;
      Double_t t11 = fY0 * fY0;
      Double_t t14 = r * r;
      Double_t t15 = t14 * fCurv;
      Double_t t17 = t8 * t8;
      Double_t t19 = t11 * t11;
      Double_t t21 = t11 * t3;
      Double_t t23 = t5 * t5;
      Double_t t32 = t14 * t14;
      Double_t t35 = t14 * t3;
      Double_t t38 = 8.0 * t4 * t6 - 4.0 * t1 * t2 * t8 - 4.0 * t11 * fCurv * t6 + 4.0 * t15 * t6 + t17 * t3 + t19 * t3
                     + 2.0 * t21 * t8 + 4.0 * t8 * t23 - 4.0 * t8 * fX0 * fCurv * t5 - 4.0 * t11 * t23
                     - 4.0 * t11 * fY0 * fCurv * t2 + 4.0 * t11 - 4.0 * t14 + t32 * t3 + 4.0 * t15 * t4 - 2.0 * t35 * t11
                     - 2.0 * t35 * t8;
      Double_t t40 = (-t3 * t38);
      if (t40 < 0.) {
        s1 = s2 = 1E+9;
        return;
      }
      t40 = TMath::Sqrt(t40);

      Double_t t43 = fX0 * fCurv;
      Double_t t45 = 2.0 * t5 - t35 + t21 + 2.0 - 2.0 * t1 * t2 - 2.0 * t43 - 2.0 * t43 * t5 + t8 * t3;
      Double_t t46 = fH * fLambdaCos * fCurv;

      s1 = (-fPhi0 + 2.0 * TMath::ATan((-2.0 * t1 + 2.0 * t2 + t40) / t45)) / t46;
      s2 = -(fPhi0 + 2.0 * TMath::ATan((2.0 * t1 - 2.0 * t2 + t40) / t45)) / t46;

      //
      //   Solution can be off by +/- one period, select smallest
      //
      Double_t p = GetPeriod();
      if (!TMath::IsNaN(s1)) {
        if (TMath::Abs(s1 - p) < TMath::Abs(s1))
          s1 = s1 - p;
        else if (TMath::Abs(s1 + p) < TMath::Abs(s1))
          s1 = s1 + p;
      }
      if (!TMath::IsNaN(s2)) {
        if (TMath::Abs(s2 - p) < TMath::Abs(s2))
          s2 = s2 - p;
        else if (TMath::Abs(s2 + p) < TMath::Abs(s2))
          s2 = s2 + p;
      }
    }
    if (s1 > s2) {
      Double_t temp = s1;
      s1            = s2;
      s2            = temp;
    }
  }

  void HelixBase::BasePathLength(Double_t r, Double_t x, Double_t y, Double_t& s1, Double_t& s2) const {
    HelixBase helix(*this);
    helix.fX0 = fX0 - x;
    helix.fY0 = fY0 - y;
    helix.BasePathLength(r, s1, s2);
    helix.fX0 = fX0 + x;
    helix.fY0 = fY0 + y;
  }

  Double_t HelixBase::BasePathLength(const TVector3& point, Bool_t scanPeriods) const {
    Double_t dx    = point.X() - fX0;
    Double_t dy    = point.Y() - fY0;
    Double_t dz    = point.Z() - fZ0;
    Double_t s     = 0;
    Double_t Phase = TMath::ATan2(fYc - fY0, fXc - fX0) + TMath::Pi();
    if (Phase < -TMath::Pi()) { Phase += TMath::TwoPi(); }
    if (Phase > TMath::Pi()) { Phase -= TMath::TwoPi(); }
    Double_t CosPhase  = TMath::Cos(Phase);
    Double_t SinPhase  = TMath::Sin(Phase);
    Double_t period    = 0;
    Bool_t Singularity = kFALSE;

    if (fCurv == 0) {
      s      = (dy * CosPhase - dx * SinPhase) / fLambdaCos;
      period = 1E+9;
    } else {  //
      const Double_t overC = 1.0 / fCurv;
      s      = atan2(dy * CosPhase - dx * SinPhase, overC + dx * CosPhase + dy * SinPhase) / (fH * fCurv * fLambdaCos);
      period = TMath::Abs(TMath::TwoPi() / (fH * fCurv * fLambdaCos));

      // 	    using namespace units;
      // #endif
      //	    const Double_t MaxPrecisionNeeded = 0.000001;  // micrometer;
      const Double_t MaxPrecisionNeeded = 0.000001;
      const int MaxIterations           = 100;

      //
      // The math is taken from Maple with C(expr,optimized) and
      // some hand-editing. It is not very nice but efficient.
      //
      Double_t t34 = fCurv * fLambdaCos * fLambdaCos;
      Double_t t41 = fLambdaSin * fLambdaSin;
      Double_t t6, t7, t11, t12, t19;

      //
      // Get a first guess by using the dca in 2D. Since
      // in some extreme cases we might be off by n periods
      // we add (subtract) periods in case we get any closer.
      //

      if (scanPeriods) {
        Double_t ds = period;
        int j, jmin = 0;

        Double_t d, dmin = (BasePosition(s) - point).Mag2();
        for (j = 1; j < MaxIterations; j++) {
          d = (BasePosition(s + j * ds) - point).Mag2();
          if (d < dmin) {
            dmin = d;
            jmin = j;
          } else
            break;
        }
        for (j = -1; - j < MaxIterations; j--) {
          d = (BasePosition(s + j * ds) - point).Mag2();
          if (d < dmin) {
            dmin = d;
            jmin = j;
          } else
            break;
        }
        if (jmin) s += jmin * ds;
      }

      //
      // Newtons method:
      // Stops after MaxIterations iterations or if the required
      // precision is obtained. Whatever comes first.
      //
      Double_t sOld = s;

      for (int i = 0; i < MaxIterations; i++) {
        t6  = Phase + s * fH * fCurv * fLambdaCos;
        t7  = cos(t6);
        t11 = dx - overC * (t7 - CosPhase);
        t12 = sin(t6);
        t19 = dy - overC * (t12 - SinPhase);
        s -= (t11 * t12 * fH * fLambdaCos - t19 * t7 * fH * fLambdaCos - (dz - s * fLambdaSin) * fLambdaSin)
             / (t12 * t12 * fLambdaCos * fLambdaCos + t11 * t7 * t34 + t7 * t7 * fLambdaCos * fLambdaCos + t19 * t12 * t34 + t41);
        if (TMath::Abs(sOld - s) < MaxPrecisionNeeded) break;
        sOld = s;
      }
    }
    return s;
  }

  Double_t HelixBase::BasePathLength(Double_t x, Double_t y) const { return FudgePathLenght(TVector3(x, y, 0)); }

  void HelixBase::PathLengths(const HelixBase& h, Double_t& s1, Double_t& s2) const {
    if ((fCurv == 0 && h.fCurv != 0) || ((fCurv != 0 && h.fCurv == 0))) {
      s1 = s2 = 1E+9;
      return;
    }


    if (fCurv == 0) {
      //
      //  Analytic solution
      //
      TVector3 dv = h.BaseGetStartPoint() - BaseGetStartPoint();
      TVector3 a(-fLambdaCos * fPhiSin, fLambdaCos * fPhiCos, fLambdaSin);
      TVector3 b(-h.fLambdaCos * h.fPhiSin, h.fLambdaCos * h.fPhiCos, h.fLambdaSin);
      Double_t ab = a * b;
      Double_t g  = dv * a;
      Double_t k  = dv * b;
      s2          = (k - ab * g) / (ab * ab - 1.);
      s1          = g + s2 * ab;
      return;
    } else {
      //
      //  First step: get dca in the xy-plane as start value
      //
      Double_t dx = h.BaseGetXcenter() - BaseGetXcenter();
      Double_t dy = h.BaseGetYcenter() - BaseGetYcenter();
      Double_t dd = ::sqrt(dx * dx + dy * dy);
      Double_t r1 = 1. / fCurv;
      Double_t r2 = 1. / h.GetCurv();

      Double_t cosAlpha = (r1 * r1 + dd * dd - r2 * r2) / (2 * r1 * dd);

      Double_t s;
      Double_t xx, yy;
      if (TMath::Abs(cosAlpha) < 1) {  // two solutions
        const Double_t overDD = 1.0 / dd;
        Double_t sinAlpha     = TMath::Sin(TMath::ACos(cosAlpha));
        xx                    = BaseGetXcenter() + r1 * (cosAlpha * dx - sinAlpha * dy) * overDD;
        yy                    = BaseGetYcenter() + r1 * (sinAlpha * dx + cosAlpha * dy) * overDD;
        s                     = BasePathLength(xx, yy);
        xx                    = BaseGetXcenter() + r1 * (cosAlpha * dx + sinAlpha * dy) * overDD;
        yy                    = BaseGetYcenter() + r1 * (cosAlpha * dy - sinAlpha * dx) * overDD;
        Double_t a            = BasePathLength(xx, yy);
        if (h.Distance(BasePosition(a)) < h.Distance(BasePosition(s))) s = a;
      } else {                                  // no intersection (or exactly one)
        int rsign = ((r2 - r1) > dd ? -1 : 1);  // set -1 when *this* helix is
        // completely contained in the other
        xx = BaseGetXcenter() + rsign * r1 * dx / dd;
        yy = BaseGetYcenter() + rsign * r1 * dy / dd;
        s  = BasePathLength(xx, yy);
      }

      //
      //   Second step: scan in decreasing intervals around seed 's'
      //
      //	const Double_t MinStepSize = 0.001;  // 10*micrometer;
      //	const Double_t MinRange    = 10.;  // 10*centimeter;
      const Double_t MinStepSize = 10 * 0.000001;
      const Double_t MinRange    = 10 * 0.01;
      Double_t dmin              = h.Distance(BasePosition(s));
      Double_t range             = TMath::Max(2 * dmin, MinRange);
      Double_t ds                = range / 10;
      Double_t slast             = -999999, ss, d;
      s1                         = s - range / 2.;
      s2                         = s + range / 2.;

      while (ds > MinStepSize) {
        for (ss = s1; ss < s2 + ds; ss += ds) {
          d = h.Distance(BasePosition(ss));
          if (d < dmin) {
            dmin = d;
            s    = ss;
          }
          slast = ss;
        }
        //
        //  In the rare cases where the minimum is at the
        //  the border of the current range we shift the range
        //  and start all over, i.e we do not decrease 'ds'.
        //  Else we decrease the search intervall around the
        //  current minimum and redo the scan in smaller steps.
        //
        if (s == s1) {
          d = 0.8 * (s2 - s1);
          s1 -= d;
          s2 -= d;
        } else if (s == slast) {
          d = 0.8 * (s2 - s1);
          s1 += d;
          s2 += d;
        } else {
          s1 = s - ds;
          s2 = s + ds;
          ds /= 10;
        }
      }
      s1 = s;
      s2 = h.BasePathLength(BasePosition(s));
    }
  }

  Double_t HelixBase::Distance(const TVector3& p, Bool_t scanPeriods) const {
    return (BasePosition(BasePathLength(p, scanPeriods)) - p).Mag();
  }

  HelixBase::HelixBase(Double_t x,
                       Double_t y,
                       Double_t z,
                       Double_t px,
                       Double_t py,
                       Double_t pz,
                       Double_t charge,
                       Double_t conversion) :
    HelixBase(TVector3(x, y, z), TVector3(px, py, pz), charge, conversion) {}

  void HelixBase::BaseShift(Double_t x, Double_t y, Double_t z) {
    fXc += x;
    fYc += y;
    fX0 += x;
    fY0 += y;
    fZ0 += z;
  }

  Double_t HelixBase::GetRotationDirection() const { return TMath::Sign(1, GetH() * TMath::Cos(GetDipAngle())); }

  TVector3 HelixBase::BaseMomentum(Double_t s) const {
    TVector3 origin = BasePosition(s);
    if (fCurv != 0) {
      Double_t newPhase         = TMath::ATan2(origin.Y() - fYc, origin.X() - fXc);
      const Double_t conversion = -1.0 / 0.299792458 / GetBz();
      const Double_t charge     = -TMath::Sign(1.0, GetBz() * fH);
      Double_t pt               = TMath::Abs(charge / (conversion * fCurv) * 0.01);
      // set origin mOroigin
      // set phase
      if (newPhase < -TMath::Pi()) newPhase += TMath::TwoPi();
      if (newPhase > TMath::Pi()) newPhase -= TMath::TwoPi();
      Double_t shiftPhase = newPhase + fH * TMath::PiOver2();
      return TVector3(pt * TMath::Cos(shiftPhase), pt * TMath::Sin(shiftPhase), pt * TMath::Tan(fLambda));
    }
    return TVector3(0, 0, 0);
  }

  Double_t HelixBase::GetPathAbsMin(Double_t s1, Double_t s2) const {
    const Double_t period = GetPeriod();
    if (period <= 0) return TMath::Min(s1, s2);
    while (s1 < 0)
      s1 += period;
    while (s2 < 0)
      s2 += period;
    return TMath::Min(s1, s2);
  }

  void HelixBase::BaseFullEval(Double_t s, TVector3& mom, TVector3& pos) const {
    pos = BasePosition(s);
    if (fCurv != 0) {
      Double_t newPhase         = TMath::ATan2(pos.Y() - fYc, pos.X() - fXc);
      const Double_t conversion = -1.0 / 0.299792458 / GetBz();
      const Double_t charge     = -TMath::Sign(1.0, GetBz() * fH);
      Double_t pt               = TMath::Abs(charge / (conversion * fCurv) * 0.01);
      if (newPhase < -TMath::Pi()) newPhase += TMath::TwoPi();
      if (newPhase > TMath::Pi()) newPhase -= TMath::TwoPi();
      Double_t shiftPhase = newPhase + fH * TMath::PiOver2();
      mom.SetXYZ(pt * TMath::Cos(shiftPhase), pt * TMath::Sin(shiftPhase), pt * TMath::Tan(fLambda));
      return;
    }
    mom.SetXYZ(0, 0, 0);
  }
}  // namespace Hal
