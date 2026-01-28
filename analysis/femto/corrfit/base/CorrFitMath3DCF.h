/*
 * CorrFitMath.h
 *
 *  Created on: 21 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITMATH_H_
#define HALCORRFITMATH_H_

#include "CorrFit3DCF.h"

namespace Hal {
  class CorrFitMath3DCF : public CorrFit3DCF {
    Int_t fSubFunctionsNo;
    CorrFit3DCF** fSubFunction;  //[fSubFunctionsNo]
  protected:
    void ParametersChanged() const;
    void SetSubfunction(CorrFit3DCF* f, Int_t index) { fSubFunction[index] = f; }
    CorrFit3DCF* GetSubfunction(Int_t index) const { return fSubFunction[index]; };
    /**
     * call CalculateCF(Double_t *x, Double_t *params
     * @param subfunct  -subfucnttion id
     * @param x
     * @param params
     * @return
     */
    Double_t Call(Int_t subfucnt, const Double_t* x, const Double_t* params) const;
    CorrFit3DCF* GetSubFunction(Int_t f) const { return fSubFunction[f]; };

  public:
    /**
     * default ctor
     * @param parameters_no
     * @param subfunct
     */
    CorrFitMath3DCF(Int_t parameters_no = 1, Int_t subfunct = 0);
    Int_t GetSubunctionsNo() const { return fSubFunctionsNo; };
    virtual ~CorrFitMath3DCF();
    ClassDef(CorrFitMath3DCF, 1)
  };

  /**
   * class for fitting two CF's as one following equation is used:
   * CF = CF1+CF2. Standard id's for parameters (like Lambda()) should not be used
   * because order of parameters is modified.
   */
  class CorrFit3DCF_Sum : public CorrFitMath3DCF {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    /**
     * default constructor
     * @param f1 first function
     * @param f2 second function
     */
    CorrFit3DCF_Sum(CorrFit3DCF* f1, CorrFit3DCF* f2);
    virtual ~CorrFit3DCF_Sum();
    ClassDef(CorrFit3DCF_Sum, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITMATH_H_ */
