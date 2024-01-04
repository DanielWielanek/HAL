/*
 * CorrFitConst.h
 *
 *  Created on: 4 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITCONST_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITCONST_H_

#include "Array.h"
#include "FemtoConst.h"

namespace Hal {
  namespace Femto {
    Int_t GetNSlices(TObject* obj);
    void MergeVertical(Hal::Femto::ECFType cfType, TObject* obj, Array_1<Float_t>* input, Array_1<Float_t>* output, Int_t pos);
  }  // namespace Femto
}  // namespace Hal

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITCONST_H_ */
