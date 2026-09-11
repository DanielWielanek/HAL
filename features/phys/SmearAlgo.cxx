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

  TVectorD SmearAlgo::GetSmeared(const TH1D& raw) { return GetSmeared(Hal::Std::Math::GetVector(raw, fUseUnderflows)); }

  TVectorD SmearAlgo::GetUnsmeared(const TH1D& raw) { return GetUnsmeared(Hal::Std::Math::GetVector(raw, fUseUnderflows)); }

  //===================================================================================================================================

  void SmearAlgoMatrix::SetSmearMatrix(TH2& vec) {

    auto temp = Hal::Std::Math::GetMatrix(vec, false, fUseUnderflows);
    Hal::Std::Math::DiagonalOnEmpty(temp, true);  // diagonalize empty
    fSmearMatrix.ResizeTo(temp.GetNrows(), temp.GetNcols());
    Hal::Std::Math::NormalizeMatrixByRow(temp);
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
    Hal::Std::Math::DiagonalOnEmpty(temp, true);  // diagonalize empty
    fSmearMatrix.ResizeTo(temp.GetNrows(), temp.GetNcols());
    Hal::Std::Math::NormalizeMatrixByRow(temp);
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

  TVectorD SmearAlgoMatrix::GetSmeared(const TVectorD& raw) { return fSmearMatrix * raw; }

  TVectorD SmearAlgoMatrix::GetUnsmeared(const TVectorD& raw) {

    if (fInvertionMethod == EMethod::kThikonov1) {
      if (fParameters.size() < 1) {
        std::cout << ClassName() << " no lambda set, set 0.1 " << std::endl;
        SetInvertionParameters({0.1});
      }
      return Hal::Std::Math::TikhonovUnfold(raw, fSmearMatrix, fParameters[0]);
    } else if (fInvertionMethod == EMethod::kThikonov2) {
      if (fParameters.size() < 1) {
        std::cout << ClassName() << " no lambda set, set 0.1 " << std::endl;
        SetInvertionParameters({0.1});
      }
      return Hal::Std::Math::TikhonovUnfold2(raw, fSmearMatrix, fParameters[0]);
    } else if (fInvertionMethod == EMethod::kMatrix) {
      return fSmearMatrixRev * raw;
    }
    // none of above? return raw
    return raw;
  }

  //===================================================================================================================================

  Double_t SmearAlgoMatrix::FindOptimalLambdaThikonov2(const TH1D& raw) {
    auto rawVect = Hal::Std::Math::GetVector(raw, fUseUnderflows);
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

    std::vector<TVectorD> samples;
    const int samplesNo = 20;
    for (int i = 0; i < samplesNo; i++) {
      auto test = rawVect;
      for (int j = 0; j < rawVect.GetNrows(); j++) {
        test[j] = test[j] + gRandom->Gaus(0, errors[j]);
      }
      samples.push_back(test);
    }

    // Skan lambdy logarytmicznie
    const int nLambda      = 100;
    const double lambdaMin = 1e-8;
    const double lambdaMax = 1e+2;

    double bestLambda = lambdaMax;

    for (int il = 0; il < nLambda; ++il) {
      const double logLambda = std::log10(lambdaMin) + il * (std::log10(lambdaMax) - std::log10(lambdaMin)) / (nLambda - 1);

      const double lambda = std::pow(10.0, logLambda);
      double chi2sum      = 0.0;
      for (int iS = 0; iS < samplesNo; iS++) {
        // Unfold
        TVectorD unfolded = Hal::Std::Math::TikhonovUnfold2(samples[iS], fSmearMatrix, lambda);

        // Ponowne rozmycie
        TVectorD refolded = GetSmeared(unfolded);

        // chi2
        double chi2 = 0.0;

        for (int i = 0; i < n; ++i) {
          if (errors[i] <= 0.0) continue;

          const double diff = refolded[i] - rawVect[i];

          chi2 += diff * diff;  /// (errors[i] * errors[i]);
        }
        chi2sum = ++chi2;
      }

      std::cout << "lambda = " << lambda << "   chi2/N = " << chi2sum / n << std::endl;
    }
    return 1;
  }

} /* namespace Hal */
