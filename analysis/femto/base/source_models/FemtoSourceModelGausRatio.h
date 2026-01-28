/*
 * FemtoSourceModelGaus3DRelative.h
 *
 *  Created on: 15 paź 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEMODELGAUSRATIO_H_
#define HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEMODELGAUSRATIO_H_


#include "FemtoSourceDensity.h"

#include "FemtoSourceModel.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TMatrixDfwd.h>
#include <TVector3.h>

namespace Hal {
  class FemtoSourceModelGaussDelta : public FemtoSourceModel3D {
  public:
    /**
     * default ctor
     */
    FemtoSourceModelGaussDelta();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModelGaussDelta(const FemtoSourceModelGaussDelta& model);
    FemtoSourceModel* MakeCopy() const;
    void GenerateCoordinates(FemtoPair* Pair);
    virtual ~FemtoSourceModelGaussDelta();
    ClassDef(FemtoSourceModelGaussDelta, 1)
  };
  class FemtoSourceDensityGausDelta : public FemtoSourceDensity {
  public:
    FemtoSourceDensityGausDelta();
    Double_t GetProbDensity1d(const Double_t r, const Double_t* params) const;
    Double_t GetProbDensity3d(const TVector3& vec, const Double_t* params) const;
    virtual ~FemtoSourceDensityGausDelta() {};
    ClassDef(FemtoSourceDensityGausDelta, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEMODELGAUSRATIO_H_ */
