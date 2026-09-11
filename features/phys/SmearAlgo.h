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
    Bool_t fComputed      = {kFALSE};
    Bool_t fUseUnderflows = {kFALSE};
    Int_t fBins           = {1};

  public:
    /**
     * user underflows bins in histograms (if used)
     * @param underflows
     */
    SmearAlgo(Bool_t underflows = false) : fUseUnderflows(underflows) {};
    /**
     * init method should be called before call GetSmeared/GetUnsmeared
     * @return
     */
    virtual Bool_t Init() = 0;
    /**
     * return smeared function
     * @param raw
     * @return
     */
    virtual TVectorD GetSmeared(const TVectorD& raw) = 0;
    /**
     * return unsmeared function
     * @param raw
     * @return
     */
    virtual TVectorD GetUnsmeared(const TVectorD& raw) = 0;
    /**
     * return smeared vector note: contains n+2 bins (under+overflow)
     * @param raw
     * @return
     */
    virtual TVectorD GetSmeared(const TH1D& raw);
    virtual TVectorD GetUnsmeared(const TH1D& raw);
    virtual ~SmearAlgo() {};
    ClassDef(SmearAlgo, 1)
  };

  /**
   * class for smearing histograms, uses simple matrix method to make convolution of matrix
   */
  class SmearAlgoMatrix : public SmearAlgo {
  public:
    /**
     * invertion methods
     * kMatrix = standard method, invert matrix of transformation
     * kThikonov = Thikonov method
     */
    enum class EMethod { kNone, kMatrix, kThikonov1, kThikonov2 };

  protected:
    TMatrixD fSmearMatrix;
    TMatrixD fSmearMatrixRev;
    EMethod fInvertionMethod = {EMethod::kMatrix};
    std::vector<double> fParameters;

  public:
    using SmearAlgo::GetSmeared;
    using SmearAlgo::GetUnsmeared;
    SmearAlgoMatrix(Bool_t underflows = false) : SmearAlgo(underflows) {};
    /**
     * set smear matrix on X - true value, on Y - measured
     * @param vec
     */
    void SetSmearMatrix(TH2& vec);
    /**
     * build smear matrix from smearing function, smearing function describes gaussian sigma as a function on x-axis
     * @param f
     * @param raw
     */
    virtual void SetSmearFunction(TF1* f, const TH1D& raw);
    virtual Bool_t Init();
    virtual TVectorD GetSmeared(const TVectorD& raw);
    virtual TVectorD GetUnsmeared(const TVectorD& raw);
    /**
     * set invertion method
     * @param method
     */
    void SetInvertionMethod(EMethod method) { fInvertionMethod = method; };
    /**
     * set invertion parameters
     * for matrix - no parameters needed
     * for thikonov - only lambda is needed (first paramater)
     */
    void SetInvertionParameters(std::vector<double> vec) { fParameters = vec; };
    /**
     * find optimal lambda (closest by testing how far inversion->smearing is close to original function
     * @param raw smeared function
     * @param step step of lambda
     * @param lo lowest lambda
     * @param hi highstest lambda
     * @param test test type = "sim" when minimized value is (restored-obtained)^2
     * @return
     */
    Double_t FindOptimalLambdaThikonov2(const TH1D& raw);
    TMatrixD GetSmearingMatrix() const { return fSmearMatrix; }
    TMatrixD GetSmearingMatrixRev() const { return fSmearMatrixRev; }
    virtual ~SmearAlgoMatrix() {};
    ClassDef(SmearAlgoMatrix, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_PHYS_SMEARALGO_H_ */
