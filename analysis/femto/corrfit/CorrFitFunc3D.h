/*
 * CorrFitFunc3D.h
 *
 *  Created on: 12 kwi 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITFUNC3D_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITFUNC3D_H_

#include "CorrFitFunc.h"

namespace Hal {

  class CorrFitFunc3D : public CorrFitFunc {
    friend class Femto1DCF;
    friend class Femto3DCF;
    friend class FemtoSHCF;
    Int_t fLambdaParIndex;
    Int_t fNormParIndex;
    Int_t fRoutParIndex;
    Int_t fRsideParIndex;
    Int_t fRlongParIndex;
    Int_t fRoutsideParIndex;
    Int_t fRoutlongParIndex;
    Int_t fRsidelongParIndex;

  protected:
    enum class e3DMode { kNormal3R, kRatio3R, kNormal6R, kRatio6R, kPlus3R, kPlus6R };
    const e3DMode fMode;


  public:
    CorrFitFunc3D(e3DMode mode = e3DMode::kNormal3R, Int_t par = -1, Int_t dim = 1);
    /**
     *
     * @return Rout
     */
    Double_t GetRout() const;
    /**
     *
     * @return Rout error
     */
    Double_t GetRoutError() const;
    /**
     *
     * @return Rside
     */
    Double_t GetRside() const;
    /**
     *
     * @return Rside error
     */
    Double_t GetRsideError() const;
    /**
     *
     * @return Rlong
     */
    Double_t GetRlong() const;
    /**
     *
     * @return Rlong error
     */
    Double_t GetRlongError() const;
    /**
     *
     * @return labmda value (can be negative for fermions)
     */
    Double_t GetLambda() const { return GetParameter(fLambdaParIndex); }
    /**
     *
     * @return lambda error
     */
    Double_t GetLambdaError() const { return GetParameter(fLambdaParIndex); };
    /**
     *
     * @return normalization
     */
    Double_t GetNorm() const { return GetParameter(fNormParIndex); };
    /**
     *
     * @return normalization error
     */
    Double_t GetNormError() const { return GetParError(fNormParIndex); };
    /**
     * set norm limits
     * @param min
     * @param max
     */
    void SetNormLimits(Double_t min, Double_t max) { SetParLimits(NormID(), min, max); }
    /**
     * set lambda limits
     * @param min
     * @param max
     */
    void SetLambdaLimits(Double_t min, Double_t max) { SetParLimits(LambdaID(), min, max); }
    /**
     * set limits of r-out
     * @param min
     * @param max
     */
    void SetRoutLimits(Double_t min, Double_t max) { SetParLimits(RoutID(), min, max); }
    /**
     * set limits of r-side
     * @param min
     * @param max
     */
    void SetRsideLimits(Double_t min, Double_t max);
    /**
     * set limits of r-long
     * @param min
     * @param max
     */
    void SetRlongLimits(Double_t min, Double_t max);
    /**
     *
     * @return id of rout parameter
     */
    inline Int_t RoutID() const { return fRoutParIndex; };
    /**
     *
     * @return id of rside parameter (or similar parameter)
     */
    inline Int_t RsideID() const { return fRsideParIndex; }
    /**
     *
     * @return id of rlong parameter (or similar parameter)
     */
    inline Int_t RlongID() const { return fRlongParIndex; }
    /**
     *
     * @return id of rout-side parameter (if available)
     */
    inline Int_t RoutsideID() const { return fRoutsideParIndex; }
    /**
     *
     * @return id of rout-side parameter (if available)
     */
    inline Int_t RoutlongID() const { return fRoutlongParIndex; }
    /**
     *
     * @return id of rside-long parameter (if available)
     */
    inline Int_t RsidelongID() const { return fRsidelongParIndex; }
    /**
     *
     * @return id of lambda parameter (if available)
     */
    inline Int_t LambdaID() const { return fLambdaParIndex; }
    /**
     *
     * @return id of normalization parameter (if available)
     */
    inline Int_t NormID() const { return fNormParIndex; }
    virtual ~CorrFitFunc3D() {};
    ClassDef(CorrFitFunc3D, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITFUNC3D_H_ */
