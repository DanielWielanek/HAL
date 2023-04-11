/*
 * CorrFitSHCFMultiDim.h
 *
 *  Created on: 22 mar 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITSHCFMULTIDIM_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITSHCFMULTIDIM_H_

#include "Array.h"
#include "CorrFitSHCF.h"

#include <RtypesCore.h>

namespace Hal {
  class CorrFitFunctor;
  class CorrFitSHCFMultiDim : public CorrFitSHCF {
  protected:
    Int_t fParameterIDs[3];
    Int_t fFunctorXbins, fFunctorYbins;
    CorrFitFunctor* fFunctor;
    mutable Array_1<Float_t>* fData;
    virtual void Check();
    virtual void RecalculateFunction() const;
    virtual void PrepareMinimizer();
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFitSHCFMultiDim(Int_t ndims = 5);
    void SetAutoLimits();
    void SetFunctorFromMap(TString filename);  // { fFunctor = new CorrFitFunctor(filename); };
    void SetFunctor(CorrFitFunctor* functor);  // {
    virtual void SetMinimizer(EMinAlgo alg);
    virtual ~CorrFitSHCFMultiDim();
    ClassDef(CorrFitSHCFMultiDim,1)
  };

}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITSHCFMULTIDIM_H_ */
