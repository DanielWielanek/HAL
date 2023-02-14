/*
 * CorrFitHDFuncSH.h
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITHDFUNCSH_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITHDFUNCSH_H_

#include "CorrFitHDFunc.h"

namespace Hal {
  class CorrFitHDFuncSH : public CorrFitHDFunc {
  public:
    CorrFitHDFuncSH();
    virtual ~CorrFitHDFuncSH();
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITHDFUNCSH_H_ */
