/*
 * CorrFitPairGeneratorYPtSubborn.h
 *
 *  Created on: 29 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTSUBBORN_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTSUBBORN_H_

#include "CorrFitPairGeneratorYPtKt.h"

#include "FemtoMiniPair.h"

#include <utility>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>


class TFile;
class TTree;
class TClonesArray;
namespace Hal {
  class FemtoMicroPair;
  class CorrFitPairGeneratorYPtKtStubborn : public CorrFitPairGeneratorYPtKt {
    virtual void GenerateEvent();

  public:
    CorrFitPairGeneratorYPtKtStubborn();
    virtual ~CorrFitPairGeneratorYPtKtStubborn();
    ClassDef(CorrFitPairGeneratorYPtKtStubborn, 1)
  };
}  // namespace Hal


#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTSUBBORN_H_ */
