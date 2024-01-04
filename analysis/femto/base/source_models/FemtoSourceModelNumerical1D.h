/*
 * FemtoSourceModelNumerical1D.h
 *
 *  Created on: 23 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_HALFEMTOSOURCEMODELNUMERICAL1D_H_
#define HALFEMTO_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_HALFEMTOSOURCEMODELNUMERICAL1D_H_

#include "FemtoPair.h"
#include "FemtoSourceDensity.h"
#include "FemtoSourceModel.h"

class TH1D;
namespace Hal {
  class FemtoSourceModelNumerical1D : public FemtoSourceModel1D {
    TH1D* fRawDistribution;
    Double_t ff;
    std::vector<Double_t> fRandomDistributionX;
    std::vector<Double_t> fRandomDistributionY;

  public:
    FemtoSourceModelNumerical1D();
    FemtoSourceModelNumerical1D(const FemtoSourceModelNumerical1D& other);
    void SetRadiusDistribution(const TH1D& distribution);
    FemtoSourceModel* MakeCopy() const;
    void GenerateCoordinates(FemtoPair* Pair);
    TH1D* GetRaw() const { return fRawDistribution; }
    std::vector<Double_t> GetVectX() const { return fRandomDistributionX; }
    std::vector<Double_t> GetVectY() const { return fRandomDistributionY; }
    virtual Package* Report() const;
    virtual ~FemtoSourceModelNumerical1D();
    ClassDef(FemtoSourceModelNumerical1D, 1)
  };

  class FemtoSourceDensityNumerical1D : public FemtoSourceDensity {
    TH1D* fRawDistribution;

  public:
    FemtoSourceDensityNumerical1D() : FemtoSourceDensity(1, kTRUE, kTRUE), fRawDistribution(nullptr) {};
    void SetRadiusDistribution(const TH1D& distribution);
    TH1D* GetRaw() const { return fRawDistribution; }
    virtual Double_t GetProbDensity1d(const Double_t r, const Double_t* params) const;
    virtual Double_t GetProbDensity3d(const TVector3& r, const Double_t* params) const;
    virtual ~FemtoSourceDensityNumerical1D() {};
    ClassDef(FemtoSourceDensityNumerical1D, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_HALFEMTOSOURCEMODELNUMERICAL1D_H_ */
