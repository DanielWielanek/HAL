/*
 * ShGridDecomposerGridDecomposerSimd.cxx
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#include "ShGridDecomposerSimd.h"

#include <TMath.h>
#include <cmath>
#include <complex>
#include <gsl/gsl_sf_legendre.h>
#include <stddef.h>
#include <vector>

#include "ShIndexes.h"
#include "SimpleMathFunc.h"
#include "StdSimd.h"
#include "StdSimdComplex.h"

namespace Hal {
  namespace Sh {
    GridDecomposerSimd::GridDecomposerSimd(double step, int nbins, int maxL, int nTheta, int nPhi) :
      GridDecomposerBase(step,
                         nbins,
                         maxL,
                         nTheta,
                         std::ceil(static_cast<double>(nPhi) / Hal::Simd::Float_size()) * Hal::Simd::Float_size()),
      fEfPhi(fNphi / Hal::Simd::Float_size()) {
      fThetaTable.resize(fNtheta);
      fPhiTable.resize(fEfPhi);
      float dtheta   = M_PI / fNtheta;
      float dphi     = 2.0 * M_PI / fNphi;
      const int simd = Hal::Simd::Float_size();
      for (int it = 0; it < fNtheta; it++) {
        float theta            = (it + 0.5) * dtheta;
        fThetaTable[it].angle  = theta;
        fThetaTable[it].cosine = (float) cos(theta);
        fThetaTable[it].sine   = (float) sin(theta);
      }
      for (int ip = 0; ip < fEfPhi; ip++) {
        for (int ipp = 0; ipp < simd; ipp++) {
          float phi                 = (ip * simd + ipp + 0.5) * dphi;
          fPhiTable[ip].angle[ipp]  = phi;
          fPhiTable[ip].cosine[ipp] = (float) cos(phi);
          fPhiTable[ip].sine[ipp]   = (float) sin(phi);
        }
        /*double phi           = (ip + 0.5) * dphi;
        fPhiTable[ip].angle  = phi;
        fPhiTable[ip].cosine = cos(phi);
        fPhiTable[ip].sine   = sin(phi);*/
      }
    }

    void GridDecomposerSimd::Decompose(double* params) {
      fFunction->SetParams(params);
      double rstart      = fStep * 0.5;
      float Step         = fStep;
      const double scale = TMath::Sqrt(4 * TMath::Pi()) * fDeltaAngle;
#pragma omp parallel
      {
#pragma omp for schedule(static)
        for (int rbin = 0; rbin < fBins; rbin++) {
          double r = rstart + Step * rbin;
          auto val = InnerDecompose(rbin, r);
          for (int l = 0; l <= fMaxL; l++) {
            for (int m = 0; m <= l; m++) {
              int idx1           = fIndexes.GetIndex(l, m);
              int idx2           = fIndexes.GetIndex(l, -m);
              fSlmRe[idx1][rbin] = val[idx1].real() * scale;
              fSlmIm[idx1][rbin] = val[idx1].imag() * scale;
              if (m != 0) {
                fSlmRe[idx2][rbin] = val[idx1].real() * scale;
                fSlmIm[idx2][rbin] = val[idx1].imag() * scale;
              }
            }
          }
        }
      }
    }

    void GridDecomposerSimd::ComputeLegendres(std::vector<double>& flat, std::vector<std::vector<Float_v>>& Plm, double x) {
      gsl_sf_legendre_array(GSL_SF_LEGENDRE_SPHARM, fMaxL, x, flat.data());

      for (int l = 0; l <= fMaxL; ++l) {
        for (int m = 0; m <= l; ++m) {
          size_t idx = gsl_sf_legendre_array_index(l, m);
          float tmp  = (float) flat[idx];
          Plm[l][m]  = tmp;
        }
      }
    }

    std::vector<std::complex<double>> GridDecomposerSimd::InnerDecompose(int /*iBin*/, double Val) {
      const int nJMsize = (fMaxL + 1) * (fMaxL + 1);
      std::vector<FComplex_v> sum(nJMsize, {0.f, 0.f});
      size_t n    = gsl_sf_legendre_array_n(fMaxL);
      Float_v val = (float) Val;
      std::vector<double> flat(n);
      std::vector<std::vector<Float_v>> Plm(fMaxL + 1);
      for (int l = 0; l <= fMaxL; ++l) {
        Plm[l].resize(l + 1);
      }

      Float_v zero = 0.0f;
      Float_v one  = 1.0f;
      for (int it = 0; it < fNtheta; it++) {
        // Float_v theta    = fThetaTable[it].angle;
        Float_v sinT     = fThetaTable[it].sine;
        Float_v cosT     = fThetaTable[it].cosine;
        double cosScalar = (double) cosT[0];
        ComputeLegendres(flat, Plm, cosScalar);
        for (int ip = 0; ip < fEfPhi; ip++) {
          // Float_v phi  = fPhiTable[ip].angle;
          Float_v cosP = fPhiTable[ip].cosine;
          Float_v sinP = fPhiTable[ip].sine;
          Float_v x    = val * sinT * cosP;
          Float_v y    = val * sinT * sinP;
          Float_v z    = val * cosT;
          Float_v S    = fFunction->Eval(x, y, z);
          FComplex_v eiphi(cosP, sinP);
          FComplex_v em(one, zero);
          for (int m = 0; m <= fMaxL; m++) {
            FComplex_v eim = em;
            for (int l = m; l <= fMaxL; l++) {
              int id       = fIndexes.GetIndex(l, m);
              FComplex_v Y = eim * Plm[l][m];
              sum[id] += Y.conj() * (S * sinT);
            }
            em *= eiphi;
          }
        }
      }


      std::vector<std::complex<double>> sumSc = {0, 0};
      sumSc.resize(nJMsize);
      for (int j = 0; j < nJMsize; j++)
        for (size_t iSimd = 0; iSimd < Hal::Simd::Float_size(); iSimd++) {
          std::complex<double> z(sum[j].re()[iSimd], sum[j].im()[iSimd]);
          sumSc[j] += z;
        }

      return sumSc;
    }


    GridDecomposerSimd::~GridDecomposerSimd() {
      if (fFunction) delete fFunction;
    }

  }  // namespace Sh
} /* namespace Hal */
