/*
 * SmearAlgo.h
 *
 *  Created on: 3 cze 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_PHYS_SMEARALGO_H_
#define HAL_FEATURES_PHYS_SMEARALGO_H_

#include <RtypesCore.h>
#include <TH2.h>
#include <TMatrix.h>
#include <TMatrixDfwd.h>

#include "Object.h"

class TF1;
class TF2;
namespace Hal {

  class SmearAlgo : public Object {
    /**
     * class for smearing histograms - abstract one
     */
  protected:
    TH2* fSmearMatrixH = {nullptr};
    TH1* fRawFunctionH = {nullptr};
    TF1* fSmearFunc1d  = {nullptr};
    TF2* fSmearFunc2d  = {nullptr};
    Bool_t fComputed   = {kFALSE};
    TMatrixD fSmearMatrix;
    TMatrixD fSmearMatrixRev;
    TMatrixD fFunction;
    Double_t fMin = {0}, fMax = {1};
    Int_t fBins = {1};
    TMatrixD GetVec(const TH2& vec) const;
    TMatrixD GetVec(const TH1& vec) const;
    void NormalizeMatrix(TMatrixD& matrix) const;

  public:
    SmearAlgo() {};
    /**
     * set smear matrix on X - true value, on Y - measured
     * @param vec
     */
    void SetSmearMatrix(TH2& vec);
    /**
     * the function that desribes RMS of measured parameter
     * @param fFunc
     */
    void SetSmearFunction(TF1* fFunc);
    virtual TH1D* GetSmeared(const TH1D& raw);
    virtual TH1D* GetUnsmeared(const TH1D& raw);
    /**
     * return smeared vector note: contains n+2 bins (under+overflow)
     * @param raw
     * @return
     */
    virtual TMatrixD GetSmearedVec(const TH1D& raw) = 0;
    virtual TMatrixD GetUnsmearedVec(const TH1D& /*raw*/) {
      MayNotUse("SmearAlgo");
      return TMatrixD(1, 1);
    };
    virtual ~SmearAlgo();
    ClassDef(SmearAlgo, 1)
  };

  /**
   * class for smearing histograms, uses simple matrix method to make convolution of matrix
   */
  class SmearAlgoMatrix : public SmearAlgo {
    virtual void Compute();

  public:
    SmearAlgoMatrix() {};
    static std::pair<TH2D*, TH1D*> GetFilledUpMatrix(TH2D& smear_matrix, TH1D& raw);
    virtual TMatrixD GetSmearedVec(const TH1D& raw);
    virtual TMatrixD GetUnsmearedVec(const TH1D& raw);
    virtual ~SmearAlgoMatrix() {};
    ClassDef(SmearAlgoMatrix, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_PHYS_SMEARALGO_H_ */
