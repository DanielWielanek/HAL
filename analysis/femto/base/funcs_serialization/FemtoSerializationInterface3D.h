/*
 * FemtoSerializationInterface3D.h
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACE3D_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACE3D_H_

#include "FemtoSerializationInterface.h"

#include "CorrFitVerticalSlices.h"
#include "Femto3DCF.h"

#include <RtypesCore.h>

namespace Hal {
  class FemtoSerializationInterface3D : public FemtoSerializationInterfaceTemplate<Femto3DCF, CorrFitVerticalSlices3D> {
  protected:
    virtual Bool_t GetBinsNo();
    virtual void ConvertCF(ECopyDir dir);
    virtual void ConvertSlice(ECopyDir dir);
    virtual void ConvertCFSimple(ECopyDir dir);
    virtual void ConvertSliceSimple(ECopyDir dir);

  public:
    FemtoSerializationInterface3D() {};
    virtual ~FemtoSerializationInterface3D() {};
    ClassDef(FemtoSerializationInterface3D, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACE3D_H_ */
