/*
 * CorrFit1DCFMultiDim.h
 *
 *  Created on: 29 lip 2025
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFIT1DCFMULTIDIM_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFIT1DCFMULTIDIM_H_

#include "CorrFit1DCF.h"


namespace Hal {
  class CorrFitFunctor;
  class CorrFit1DCFMultiDim : public CorrFit1DCF {
  protected:
    CorrFitFunctor* fFunctor        = {nullptr};
    mutable Array_1<Float_t>* fData = {nullptr};
    virtual void Check();
    virtual void RecalculateFunction() const;
    virtual void PrepareHalMinimizer() const;
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCFMultiDim(Int_t pars = 3, Bool_t autoswap = kTRUE);
    void SetAutoLimits();
    void SetFunctorFromMap(TString filename);  // { fFunctor = new CorrFitFunctor(filename); };
    void SetFunctor(CorrFitFunctor* functor);
    virtual void SetMinimizer(EMinAlgo alg);
    virtual ~CorrFit1DCFMultiDim();
    CorrFit1DCFMultiDim(const CorrFit1DCFMultiDim& other) = delete;
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFIT1DCFMULTIDIM_H_ */
