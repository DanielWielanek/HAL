/*
 * FemtoSerializationInterfaceSH.h
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACETEMPLATESH_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACETEMPLATESH_H_


#include "FemtoSerializationInterface.h"

#include "CorrFitVerticalSlices.h"
#include <FemtoSHCF.h>
namespace Hal {

  class FemtoSerializationInterfaceSH : public FemtoSerializationInterfaceTemplate<FemtoSHCF, CorrFitVerticalSlicesSH> {
  protected:
    virtual Bool_t GetBinsNo();
    virtual void ConvertCF(ECopyDir dir);
    virtual void ConvertSlice(ECopyDir dir);
    virtual void ConvertCFSimple(ECopyDir dir);
    virtual void ConvertSliceSimple(ECopyDir dir);
    Int_t fMaxJM = {0};
    void PostInit();

  public:
    FemtoSerializationInterfaceSH() {};
    virtual ~FemtoSerializationInterfaceSH() {};
    ClassDef(FemtoSerializationInterfaceSH, 1)
  };

}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACETEMPLATESH_H_ */
