/*
 * CorrFit3DCFMultiDim.h
 *
 *  Created on: 18 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT3DCFMULTIDIM_H_
#define HALCORRFIT3DCFMULTIDIM_H_


#include "Array.h"
#include "CorrFit3DCF.h"
#include "Splines.h"

namespace Hal {
  class CorrFitFunctor;

  class CorrFit3DCFMultiDim : public CorrFit3DCF {
  protected:
    Int_t fParameterIDs[3] = {0, 0, 0};
    Int_t fFunctorXbins = {0}, fFunctorYbins = {0};
    CorrFitFunctor* fFunctor        = {nullptr};
    mutable Array_1<Float_t>* fData = {nullptr};
    virtual void Check();
    virtual void RecalculateFunction() const;
    virtual void PrepareHalMinimizer() const;
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCFMultiDim(Int_t params = 5);
    void SetAutoLimits();
    void SetFunctorFromMap(TString filename);  // { fFunctor = new CorrFitFunctor(filename); };
    void SetFunctor(CorrFitFunctor* functor);  // {
    virtual void SetMinimizer(EMinAlgo alg);
    virtual ~CorrFit3DCFMultiDim();
    ClassDef(CorrFit3DCFMultiDim, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT3DCFMULTIDIM_H_ */
