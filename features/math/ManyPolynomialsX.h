/*
 * PolynomialX.h
 *
 *  Created on: 16 gru 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_MANYPOLYNOMIALSX_H_
#define HAL_FEATURES_HIST_MANYPOLYNOMIALSX_H_

#include "Object.h"

class TF1;

namespace Hal {
  /**
   * represents the multiple polynomial fuctions
   */
  class ManyPolynomialsX : public Object {
    std::vector<std::vector<double>> fTerms;
    std::vector<double> fRanges;
    TF1* fFunc         = {nullptr};
    Bool_t fInit       = {kFALSE};
    Double_t fRawValue = {0};
    Double_t Eval(Double_t* x, Double_t* params) const;
    Int_t FindBin(Double_t x) const;
    std::vector<double> GetDeriverativeTerms(const std::vector<double>& x) const;
    Bool_t IsInitialzed(TString str) const;

  public:
    ManyPolynomialsX();
    /**
     * value returned outside ranges
     * @param val
     */
    void SetRawValue(Double_t val) { fRawValue = val; }
    /**
     * define ranges {A,B,C} generate range <A,B> (B,C>
     * @param ranges
     */
    void SetRanges(const std::vector<double>& ranges);
    /**
     * set terms for given range start with constant later x therm, x^2 etc.
     * @param terms
     * @param range
     */
    void SetTerms(const std::vector<double>& terms, int range);
    /**
     * set terms for given value start with constant later x therm, x^2 etc.
     * @param terms
     * @param range
     */
    void SetTermsByValue(const std::vector<double>& terms, double value);
    /**
     * return integral
     * @param low
     * @param hi
     * @return
     */
    Double_t Integral(Double_t low, Double_t hi);
    /**
     * return terms of polynomial
     * @param i polynomial number
     * @return
     */
    std::vector<Double_t> GetTerms(Int_t i) const { return fTerms[i]; }
    /**
     * initialize class and compute stuff
     */
    void Init();
    /**
     *
     * @return TF1 used to drawing NOTE: change of TF1 parameters does not affect the TF1 because it takes parameters from this
     * class
     */
    TF1* GetTF1() const { return fFunc; };
    ManyPolynomialsX* GetDeriverative() const;
    ManyPolynomialsX* GetDeriverative2() const;
    virtual ~ManyPolynomialsX();
    ClassDef(ManyPolynomialsX, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_MANYPOLYNOMIALSX_H_ */
