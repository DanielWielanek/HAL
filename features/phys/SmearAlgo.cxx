/*
 * SmearAlgo.cxx
 *
 *  Created on: 3 cze 2025
 *      Author: daniel
 */

#include "SmearAlgo.h"

#include <TDecompSVD.h>
#include <TF1.h>
#include <TF2.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TMatrixT.h>
#include <TMatrixTBase.h>
#include <TRandom.h>

#include <iomanip>  //TODO remove

#include "Cout.h"
#include "Std.h"
#include "StdHist.h"
using std::cout;
using std::endl;
namespace Hal {
  namespace HMath = Hal::Std::Math;

  TMatrixD SmearAlgo::GetSmeared(const TH1D& raw) {
    return HMath::GetMatrix(raw, HMath::EOrientation::kVertical, fUseUnderflows);
  }

  TMatrixD SmearAlgo::GetUnsmeared(const TH1D& raw) {
    return HMath::GetMatrix(raw, HMath::EOrientation::kVertical, fUseUnderflows);
  }

  //===================================================================================================================================

  void SmearAlgoMatrix::SetSmearMatrix(TH2& vec) {
    auto temp = Hal::Std::Math::GetMatrix(vec, false, fUseUnderflows);
    HMath::DiagonalOnEmpty(temp);  // diagonalize empty
    fSmearMatrix.ResizeTo(temp.GetNrows(), temp.GetNcols());
    HMath::NormalizeMatrix(temp, HMath::ENormalization::kByColum);
    fSmearMatrix = temp;
  }

  void SmearAlgoMatrix::SetSmearFunction(TF1* f, const TH1D& raw) {
    double min, max;
    int bins;
    Hal::Std::GetAxisPar(raw, bins, min, max, "x");
    double step = (max - min) / double(bins);
    double low  = min - step;
    //     double high  = fMax + step;
    Int_t size = raw.GetNbinsX();
    if (fUseUnderflows) size += 2;

    TMatrixD temp(size, size);
    if (fUseUnderflows) {
      for (int i = 0; i < size; i++) {
        double sim_q = low + double(i) * step - 0.5 * step;
        for (int j = 0; j < size; j++) {
          double reco_q = low + double(j) * step - 0.5 * step;
          double rms    = f->Eval(sim_q);
          double val    = TMath::Gaus(reco_q, 0, rms, false);
          temp[j][i]    = val;
        }
      }
    } else {
      for (int i = 0; i < size; i++) {
        double sim_q = low + double(i) * step + 0.5 * step;
        for (int j = 0; j < size; j++) {
          double reco_q = low + double(j) * step + 0.5 * step;
          double rms    = f->Eval(sim_q);
          double val    = TMath::Gaus(reco_q, 0, rms, false);
          temp[j][i]    = val;
        }
      }
    }
    HMath::DiagonalOnEmpty(temp);  // diagonalize empty
    fSmearMatrix.ResizeTo(temp.GetNrows(), temp.GetNcols());
    HMath::NormalizeMatrix(temp, HMath::ENormalization::kByColum);
    fSmearMatrix = temp;
  }

  Bool_t SmearAlgoMatrix::Init() {
    if (fComputed) { return false; }
    fSmearMatrixRev.ResizeTo(fSmearMatrix.GetNrows(), fSmearMatrix.GetNcols());
    fSmearMatrixRev = fSmearMatrix;
    fSmearMatrixRev.Invert();
    fComputed = true;
    return false;
  };

  TMatrixD SmearAlgoMatrix::GetSmeared(const TMatrixD& raw) { return fSmearMatrix * raw; }

  TMatrixD SmearAlgoMatrix::GetUnsmeared(const TMatrixD& raw) {

    if (fInvertionMethod == EMethod::kThikonov1) {
      if (fParameters.size() < 1) {
        std::cout << ClassName() << " no lambda set, set 0.1 " << std::endl;
        SetInvertionParameters({0.1});
      }
      return HMath::TikhonovUnfold(raw, fSmearMatrix, fParameters[0]);
    } else if (fInvertionMethod == EMethod::kThikonov2) {
      if (fParameters.size() < 1) {
        std::cout << ClassName() << " no lambda set, set 0.1 " << std::endl;
        SetInvertionParameters({0.1});
      }
      return HMath::TikhonovUnfold2(raw, fSmearMatrix, fParameters[0]);
    } else if (fInvertionMethod == EMethod::kMatrix) {
      return fSmearMatrixRev * raw;
    }
    // none of above? return raw
    return raw;
  }

  //===================================================================================================================================

  Double_t SmearAlgoMatrix::FindOptimalLambdaThikonov2(const TH1D& raw) {
    auto rawVect = HMath::GetMatrix(raw, HMath::EOrientation::kVertical, fUseUnderflows);
    const int n  = rawVect.GetNrows();
    std::vector<double> errors;
    int start = 1;
    int end   = raw.GetNbinsX();
    if (fUseUnderflows) {
      start = 0;
      end   = end + 1;
    }
    for (int i = start; i <= end; i++) {
      errors.push_back(raw.GetBinError(i));
    }

    std::vector<TMatrixD> samples;
    const int samplesNo = 20;
    for (int i = 0; i < samplesNo; i++) {
      auto test = rawVect;
      for (int j = 0; j < rawVect.GetNrows(); j++) {
        test[j][0] = test[j][0] + gRandom->Gaus(0, errors[j]);
      }
      samples.push_back(test);
    }

    // Skan lambdy logarytmicznie
    const int nLambda      = 100;
    const double lambdaMin = 1e-8;
    const double lambdaMax = 1e+2;

    double bestLambda = lambdaMax;
    double bestChi    = 1E+9;

    for (int il = 0; il < nLambda; ++il) {
      const double logLambda = std::log10(lambdaMin) + il * (std::log10(lambdaMax) - std::log10(lambdaMin)) / (nLambda - 1);

      const double lambda = std::pow(10.0, logLambda);
      double chi2sum      = 0.0;
      for (int iS = 0; iS < samplesNo; iS++) {
        // Unfold
        TMatrixD unfolded = Hal::Std::Math::TikhonovUnfold2(samples[iS], fSmearMatrix, lambda);

        // Ponowne rozmycie
        TMatrixD refolded = GetSmeared(unfolded);

        // chi2
        double chi2 = 0.0;

        for (int i = 0; i < n; ++i) {
          if (errors[i] <= 0.0) continue;

          const double diff = refolded[i][0] - rawVect[i][0];

          chi2 += diff * diff;  /// (errors[i] * errors[i]);
        }
        chi2sum = ++chi2;
      }
      if (chi2sum < bestChi) { bestLambda = lambda; }


      std::cout << "lambda = " << lambda << "   chi2/N = " << chi2sum / n << std::endl;
    }
    return bestLambda;
  }

} /* namespace Hal */
