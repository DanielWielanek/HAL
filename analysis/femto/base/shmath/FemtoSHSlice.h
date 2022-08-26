/*
 * FemtoSHSlice.h
 *
 *  Created on: 24 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOSHSLICE_H_
#define HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOSHSLICE_H_

#include <TObject.h>

#include "Array.h"
/**
 *  represents singe bin of cf
 */
namespace Hal {
  class FemtoSHCF;
  class FemtoYlmSolver;
  class FemtoSHSlice : public TObject {
    friend class FemtoYlmSolver;
    const Int_t fMaxJM;
    std::vector<double> fCFImag;
    std::vector<double> fCFReal;
    std::vector<double> fCFImagE;
    std::vector<double> fCFRealE;
    std::vector<double> fNumReal;
    std::vector<double> fNumImag;
    std::vector<double> fDenReal;
    std::vector<double> fDenImag;
    Array_2<Double_t> fCovCF;
    Array_2<Double_t> fCovNum;

  protected:
    FemtoSHSlice() : fMaxJM(0) {};

  public:
    FemtoSHSlice(Int_t L);
    virtual ~FemtoSHSlice();
    void BuildSlice(FemtoSHCF& cf, Int_t slice);
    FemtoSHSlice(const FemtoSHSlice& other) = default;
    FemtoSHSlice& operator=(const FemtoSHSlice& other) = default;
    ClassDef(FemtoSHSlice, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOSHSLICE_H_ */
