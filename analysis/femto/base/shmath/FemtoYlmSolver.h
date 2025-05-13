/*
 * FemtoYlmSolver.h
 *
 *  Created on: 24 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOYLMSOLVER_H_
#define HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOYLMSOLVER_H_

#include <TObject.h>

#include <complex>

#include "FemtoSHSlice.h"
#include "FemtoYlmIndexes.h"
#include "FemtoYlmMath.h"

class TH1;
namespace Hal {
  class FemtoSHCF;
  class FemtoYlmSolver : public TObject {
    Int_t fMaxJM          = {0};
    Int_t fMaxJM2_4       = {0};
    Int_t fFactorialsSize = {0};
    Int_t fMaxL           = {0};
    Bool_t fDebug         = {kFALSE};
    Bool_t fSumw          = {kFALSE};
    Int_t fDebugBin       = {-1};
    Double_t fNumEnt      = {0};
    Double_t fDenEnt      = {0};
    Double_t fNormFactor  = {0};
    Double_t fNormPurity  = {0};
    Double_t fNormRadius  = {0};
    Double_t fNormBohr    = {0};
    FemtoSHSlice fSlice;
    FemtoSHCF* fCF = {nullptr};

    std::vector<Double_t> fFactorials;
    FemtoYlmIndexes fLmVals;
    FemtoYlmMath fLmMath;
    void GetMtilde(std::complex<double>* aMat, double* aMTilde);
    void InvertYlmIndependentMatrix(double* inmat, double* outmat) const;
    void UnPackYlmMatrixIndependentOnly(double* inmat, double* outmat, int insize) const;
    void GetIndependentLM(int ibin, int& el, int& em, int& im) const;
    void DoMath(Bool_t recalc);
    void MakeFakeCovMatrix();
    /**
     * fix CF by uing T(l,m) = T(l,-m)*
     */
    void FixCF();
    void UpdateCF(Int_t bin);
    int PackYlmMatrixIndependentOnly(double* inmat, double* outmat) const;
    int PackYlmVectorIndependentOnly(double* invec, double* outvec) const;
    inline void GetElEmForIndex(int aIndex, double& aEl, double& aEm) const {
      aEl = fLmVals.GetEls(aIndex);
      aEm = fLmVals.GetEms(aIndex);
    }
    inline void GetElEmForIndex(int aIndex, int& aEl, int& aEm) const {
      aEl = fLmVals.GetElsi(aIndex);
      aEm = fLmVals.GetEmsi(aIndex);
    }
    inline Int_t GetMaxJM() const { return fMaxJM; }


  public:
    FemtoYlmSolver();
    FemtoYlmSolver(Int_t maxL, FemtoSHCF* cf, Bool_t sumw = kFALSE);
    void SetNormalizationArea(Double_t min, Double_t max);
    void Solve(Bool_t recalc);
    void SetDebugBin(Int_t dbg) { fDebugBin = dbg; }
    virtual ~FemtoYlmSolver();
    FemtoYlmSolver(const FemtoYlmSolver& other)            = default;
    FemtoYlmSolver& operator=(const FemtoYlmSolver& other) = delete;
    ClassDef(FemtoYlmSolver, 1)
  };  // namespace Hal

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOYLMSOLVER_H_ */
