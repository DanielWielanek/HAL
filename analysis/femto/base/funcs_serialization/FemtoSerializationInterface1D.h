/*
 * FemtoSerialization1D.h
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACE1D_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACE1D_H_

#include "FemtoSerializationInterface.h"

#include "CorrFitVerticalSlices.h"
#include <Femto1DCF.h>
namespace Hal {

  class FemtoSerializationInterface1D : public FemtoSerializationInterfaceTemplate<Femto1DCF, CorrFitVerticalSlices1D> {
  protected:
    virtual Bool_t GetBinsNo();
    virtual void ConvertCF(ECopyDir dir);
    virtual void ConvertSlice(ECopyDir dir);
    virtual void ConvertCFSimple(ECopyDir dir);
    virtual void ConvertSliceSimple(ECopyDir dir);


  public:
    FemtoSerializationInterface1D() {};
    virtual ~FemtoSerializationInterface1D() {};
    ClassDef(FemtoSerializationInterface1D, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACE1D_H_ */
