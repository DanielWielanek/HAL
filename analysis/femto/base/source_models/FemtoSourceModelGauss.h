/*
 * FemtoSourceModelGaus.h
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOSOURCEMODELGAUSS_H_
#define HALFEMTOSOURCEMODELGAUSS_H_

#include "FemtoPair.h"
#include "FemtoSourceDensity.h"
#include "FemtoSourceModel.h"

/**
 * class that generate 1-dim gaussian source
 */
namespace Hal {
  class FemtoSourceModelGauss1D : public FemtoSourceModel1D {
  public:
    FemtoSourceModelGauss1D();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModelGauss1D(const FemtoSourceModelGauss1D& model);
    FemtoSourceModel* MakeCopy() const;
    void GenerateCoordinates(FemtoPair* Pair);
    virtual ~FemtoSourceModelGauss1D();
    ClassDef(FemtoSourceModelGauss1D, 1)
  };

  /**
   * class that generate 3-dimenional gaussian source
   */
  class FemtoSourceModelGauss3D : public FemtoSourceModel3D {
  public:
    /**
     * default ctor
     */
    FemtoSourceModelGauss3D();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModelGauss3D(const FemtoSourceModelGauss3D& model);
    FemtoSourceModel* MakeCopy() const;
    void GenerateCoordinates(FemtoPair* Pair);
    virtual ~FemtoSourceModelGauss3D();
    ClassDef(FemtoSourceModelGauss3D, 1)
  };


  class FemtoSourceDensityGaus1d : public FemtoSourceDensity {
  public:
    FemtoSourceDensityGaus1d();
    Double_t GetProbDensity1d(const Double_t r, const Double_t* params) const;
    Double_t GetProbDensity3d(const TVector3& vec, const Double_t* params) const;
    virtual ~FemtoSourceDensityGaus1d() {};
    ClassDef(FemtoSourceDensityGaus1d, 1)
  };

  class FemtoSourceDensityGaus3d : public FemtoSourceDensity {
  public:
    FemtoSourceDensityGaus3d();
    Double_t GetProbDensity1d(const Double_t r, const Double_t* params) const;
    Double_t GetProbDensity3d(const TVector3& vec, const Double_t* params) const;
    virtual ~FemtoSourceDensityGaus3d() {};
    ClassDef(FemtoSourceDensityGaus3d, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOSOURCEMODELGAUSS_H_ */
