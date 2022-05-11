/*
 * CorrFitMath1D.h
 *
 *  Created on: 20 lis 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITMATH1DCF_H_
#define HALCORRFITMATH1DCF_H_

#include "CorrFit1DCF.h"

namespace Hal {
  class CorrFitMath1DCF : public CorrFit1DCF {
    Int_t fSubFunctionsNo;
    CorrFit1DCF** fSubFunctions;  //[fSubFunctionsNo]
  protected:
    /**
     * call subfuction CalculateCF(x,params)
     * @param subfucnt subfunction index
     * @param x vector or values
     * @param params vector of parameters
     * @return value of CF at x
     */
    Double_t Call(Int_t subfucnt, const Double_t* x, const Double_t* params) const;
    /**
     * set subfuction at given position
     * @param f subfuction to set
     * @param i index
     */
    void SetSubfunction(CorrFit1DCF* f, Int_t i) { fSubFunctions[i] = f; };
    virtual void Check();
    CorrFit1DCF* GetSubFunction(Int_t f) const { return fSubFunctions[f]; };

  public:
    /**
     * deafult constructor
     * @param parameters_no number of paramters
     * @param subfunction number of functions inside of this function
     */
    CorrFitMath1DCF(Int_t parameters_no = 1, Int_t subfunction = 0);
    /**
     *
     * @return number of funstions "inside of this function
     */
    Int_t GetSubfucntionsNo() const { return fSubFunctionsNo; };
    virtual void Draw(Option_t* draw_option = "full");
    virtual ~CorrFitMath1DCF();
    ClassDef(CorrFitMath1DCF, 1)
  };

  /**
   * class for fitting two CF's as one following equation is used:
   * CF = CF1+CF2. Standard id's for parameters (like Lambda()) should not be used
   * because order of parameters is modified.
   */
  class CorrFit1DCF_Sum : public CorrFitMath1DCF {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    /**
     * default constructor
     * @param f1 first function
     * @param f2 second function
     */
    CorrFit1DCF_Sum(CorrFit1DCF* f1 = NULL, CorrFit1DCF* f2 = NULL);
    virtual ~CorrFit1DCF_Sum();
    ClassDef(CorrFit1DCF_Sum, 1)
  };
  /**
   * class for fitting two CF's as one but with fixed radius
   * CF = CF1+CF2. Standard id's for parameters (like Lambda()) should not be used
   * because order of parameters is modified.
   */
  class CorrFit1DCF_SumRsame : public CorrFitMath1DCF {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;
    Int_t fFirstRadius;
    Int_t fSecondRadius;
    /**
     * number of parameters in second CF
     */
    Int_t fSecondParamtersNo;
    /**
     * array of parameters for secon function, little to big but I don't want toa
     * dd new un
     */
    Double_t* fNewParams;  //[fSecondParamtersNo]
  public:
    /**
     * default constructor
     * @param f1 first function
     * @param f2 second function
     */
    CorrFit1DCF_SumRsame(CorrFit1DCF* f1 = NULL, CorrFit1DCF* f2 = NULL);
    virtual ~CorrFit1DCF_SumRsame();
    ClassDef(CorrFit1DCF_SumRsame, 1)
  };
  /**
   * class for fitting function like CF*poly
   */
  class CorrFit1DCF_Multi : public CorrFitMath1DCF {
  protected:
    Int_t fFirstFuncParams;
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCF_Multi(CorrFit1DCF* sign, CorrFit1DCF* back);
    virtual ~CorrFit1DCF_Multi();
    ClassDef(CorrFit1DCF_Multi, 1)
  };
}  // namespace Hal

#endif /* HALCORRFITMATH1DCF_H_ */
