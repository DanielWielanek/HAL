/*
 * FastAxisCalc.h
 *
 *  Created on: 11 paź 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_PHYS_FASTAXISCALC_H_
#define HAL_FEATURES_PHYS_FASTAXISCALC_H_

#include <TObject.h>

class TAxis;

namespace Hal {
  /**
   * class to get number of bin histogram with uniform bin width
   */
  class FastAxisCalc : public TObject {
    Double_t fMin = {0}, fMax = {1.0}, fStep = {100.0};
    Int_t fMinBin = {0};
    Int_t fMaxBin = {100};

  public:
    FastAxisCalc() {};
    FastAxisCalc(const TAxis& x);
    void Recalc(const TAxis& x);
    void RoundToMinusOne();
    inline Int_t GetBin(Double_t val) const {
      if (val < fMin) return fMinBin;
      if (val >= fMax) return fMaxBin;
      return (val - fMin) * fStep + 1.0;
    }
    virtual void Print(Option_t* option = "") const;
    virtual ~FastAxisCalc() {};
    ClassDef(FastAxisCalc, 1)
  };
}  // namespace Hal

#endif /* HAL_FEATURES_PHYS_FASTAXISCALC_H_ */
