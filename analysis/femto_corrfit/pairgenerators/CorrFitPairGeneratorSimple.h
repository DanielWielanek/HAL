/*
 * CorrFitPairGeneratorSimple.h
 *
 *  Created on: 27 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORSIMPLE_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORSIMPLE_H_

#include "CorrFitPairGenerator.h"
/**
 * generate "simple" pairs - 100 pairs per bin per event, NOTE - works correctly only for PRF system
 */

namespace Hal {

  class CorrFitPairGeneratorSimple : public CorrFitPairGenerator {
  protected:
    virtual void GenerateEvent();

  public:
    CorrFitPairGeneratorSimple() {};
    virtual ~CorrFitPairGeneratorSimple() {};
    ClassDef(CorrFitPairGeneratorSimple, 1)

      virtual Bool_t Init();
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORSIMPLE_H_ */
