/*
 * FemtoMathFuncs.cxx
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#include "FemtoMathFuncs.h"

namespace Hal {
  namespace Femto {
    double Gauss3D::Eval(double x, double y, double z) const {
      Double_t expo = 1.0 / (4. * fRx * fRx) * x * x + 1.0 / (4.0 * fRy * fRy) * y * y + 1.0 / (4.0 * fRz * fRz) * z * z;
      return fNorm * TMath::Exp(-expo);
    }
    void Gauss3D::SetParams(double* p) {
      fRx   = p[0];
      fRy   = p[1];
      fRz   = p[2];
      fNorm = 1.0 / (fRx * fRy * fRz * fNormScale);
    }

    Gaus3DSimd::Gaus3DSimd() {
      float scale = static_cast<float>(TMath::Power(TMath::TwoPi(), 1.5) * TMath::Sqrt2() * TMath::Sqrt2() * TMath::Sqrt2());
      fNormScale  = new Float_v();
      fNorm       = new Float_v();
      fRx         = new Float_v();
      fRy         = new Float_v();
      fRz         = new Float_v();
      *fNormScale = scale;
    }


    Float_v Gaus3DSimd::Eval(Float_v& x, Float_v& y, Float_v& z) const {
      Float_v expo = 0.25f * (x * x / (*fRx) / (*fRx) + y * y / (*fRy) / (*fRy) + z * z / (*fRz) / (*fRz));
      return (*fNorm) * exp(-expo);  // @suppress("Invalid arguments")
    }


    void Gaus3DSimd::SetParams(double* p) {
      float rx = static_cast<float>(p[0]);
      float ry = static_cast<float>(p[1]);
      float rz = static_cast<float>(p[2]);
      *fRx     = rx;
      *fRy     = ry;
      *fRz     = rz;

      Float_v one = 1.0f;
      *fNorm      = one / ((*fRx) * (*fRy) * (*fRz) * (*fNormScale));
    }

    Gaus3DSimd::~Gaus3DSimd() {
      delete fNorm;
      delete fNormScale;
      delete fRx;
      delete fRy;
      delete fRz;
    }

  }  // namespace Femto


} /* namespace Hal */
