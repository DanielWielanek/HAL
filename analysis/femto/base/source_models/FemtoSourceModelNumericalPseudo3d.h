/*
 * FemtoSourceModelPseudo3d.h
 *
 *  Created on: 3 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEMODELNUMERICALPSEUDO3D_H_
#define HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEMODELNUMERICALPSEUDO3D_H_
#include "FemtoPair.h"
#include "FemtoSourceDensity.h"
#include "FemtoSourceModel.h"

class TH1D;

namespace Hal {
  class FemtoSourceModelNumericalPseudo3d : public FemtoSourceModel3D {
    TH1D* fRawDistribution = {nullptr};


  public:
    FemtoSourceModelNumericalPseudo3d();
    FemtoSourceModelNumericalPseudo3d(const FemtoSourceModelNumericalPseudo3d& other);
    void SetRadiusDistribution(const TH1D& distribution);
    void GenerateCoordinates(FemtoPair* Pair);
    FemtoSourceModel* MakeCopy() const { return new FemtoSourceModelNumericalPseudo3d(*this); }
    virtual ~FemtoSourceModelNumericalPseudo3d();
    ClassDef(FemtoSourceModelNumericalPseudo3d, 1)
  };
  class FemtoSourceDensityNumericalPseudo3d : public FemtoSourceDensity {
    TH1D* fRawDistribution = {nullptr};
    Double_t fMin = {0}, fDx = {0};

  public:
    FemtoSourceDensityNumericalPseudo3d() {};
    FemtoSourceDensityNumericalPseudo3d(const FemtoSourceDensityNumericalPseudo3d& other);
    void SetRadiusDistribution(const TH1D& distribution);
    virtual Double_t GetProbDensity1d(const Double_t r, const Double_t* params) const { return 0; };
    virtual Double_t GetProbDensity3d(const TVector3& r, const Double_t* params) const;
    virtual ~FemtoSourceDensityNumericalPseudo3d();
    ClassDef(FemtoSourceDensityNumericalPseudo3d, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEMODELNUMERICALPSEUDO3D_H_ */
