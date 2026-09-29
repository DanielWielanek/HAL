/*
 * ShNumDecomposer.cxx
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#include "ShGridDecomposer.h"

#include <TMath.h>
#include <cmath>
#include <gsl/gsl_sf_legendre.h>
#include <stddef.h>

#include "Std.h"

namespace Hal {
  namespace Sh {
    GridDecomposerBase::GridDecomposerBase(double step, int nbins, int maxL, int nTheta, int nPhi) :
      fBins(nbins),
      fMaxL(maxL),
      fNtheta(nTheta),
      fNphi(nPhi),
      fStep(step),
      fDeltaAngle((M_PI / fNtheta) * (2.0 * M_PI / fNphi)),
      fIndexes(fMaxL) {
      Hal::Std::ResizeVector2D(fSlmRe, fIndexes.GetMaxJM(), fBins);
      Hal::Std::ResizeVector2D(fSlmIm, fIndexes.GetMaxJM(), fBins);
    }

    GridDecomposerScalar::GridDecomposerScalar(double step, int nbins, int maxL, int nTheta, int nPhi) :
      GridDecomposerBase(step, nbins, maxL, nTheta, nPhi) {
      fThetaTable.resize(fNtheta);
      fPhiTable.resize(fNphi);
      double dtheta = M_PI / fNtheta;
      double dphi   = 2.0 * M_PI / fNphi;
      for (int it = 0; it < fNtheta; it++) {
        double theta           = (it + 0.5) * dtheta;
        fThetaTable[it].angle  = theta;
        fThetaTable[it].cosine = cos(theta);
        fThetaTable[it].sine   = sin(theta);
      }
      for (int ip = 0; ip < fNphi; ip++) {
        double phi           = (ip + 0.5) * dphi;
        fPhiTable[ip].angle  = phi;
        fPhiTable[ip].cosine = cos(phi);
        fPhiTable[ip].sine   = sin(phi);
      }

      // fBuffer.resize(fIndexes.GetMaxJM());
    }

    void GridDecomposerScalar::Decompose(double* params) {
      fFunction->SetParams(params);
      double rstart      = fStep * 0.5;
      const double scale = TMath::Sqrt(4 * TMath::Pi()) * fDeltaAngle;
#pragma omp parallel
      {
#pragma omp for schedule(static)
        for (int rbin = 0; rbin < fBins; rbin++) {
          double r = rstart + fStep * rbin;
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

    void GridDecomposerScalar::ComputeLegendres(std::vector<double>& flat, std::vector<std::vector<double>>& Plm, double x) {
      gsl_sf_legendre_array(GSL_SF_LEGENDRE_SPHARM, fMaxL, x, flat.data());

      for (int l = 0; l <= fMaxL; ++l) {
        for (int m = 0; m <= l; ++m) {
          size_t idx = gsl_sf_legendre_array_index(l, m);
          Plm[l][m]  = flat[idx];
        }
      }
    }

    std::vector<std::complex<double>> GridDecomposerScalar::InnerDecompose(int /*rbin*/, double val) {
      std::vector<std::complex<double>> sum((fMaxL + 1) * (fMaxL + 1), {0, 0});
      size_t n = gsl_sf_legendre_array_n(fMaxL);
      // std::vector<std::complex<double>> C_local(nC, {0.0, 0.0});
      std::vector<double> flat(n);
      std::vector<std::vector<double>> Plm(fMaxL + 1);
      for (int l = 0; l <= fMaxL; ++l) {
        Plm[l].resize(l + 1);
      }

      for (int it = 0; it < fNtheta; it++) {
        // double theta = fThetaTable[it].angle;
        double sinT = fThetaTable[it].sine;
        double cosT = fThetaTable[it].cosine;
        ComputeLegendres(flat, Plm, cosT);
        for (int ip = 0; ip < fNphi; ip++) {
          // double phi  = fPhiTable[ip].angle;
          double cosP = fPhiTable[ip].cosine;
          double sinP = fPhiTable[ip].sine;
          double x    = val * sinT * cosP;
          double y    = val * sinT * sinP;
          double z    = val * cosT;

          double S = fFunction->Eval(x, y, z);

          std::complex<double> eiphi(cosP, sinP);
          std::complex<double> em(1.0, 0.0);

          for (int m = 0; m <= fMaxL; m++) {
            std::complex<double> eim = em;
            for (int l = m; l <= fMaxL; l++) {
              int id                 = l * l + l + m;
              id                     = fIndexes.GetIndex(l, m);
              std::complex<double> Y = Plm[l][m] * eim;
              sum[id] += S * std::conj(Y) * sinT;
            }
            em *= eiphi;
          }
        }
      }

      return sum;
    }

    GridDecomposerScalar::~GridDecomposerScalar() {
      if (fFunction) delete fFunction;
    }

  }  // namespace Sh
} /* namespace Hal */
