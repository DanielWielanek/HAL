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
    virtual ENumProperty GetModelNumProp() const { return FemtoSourceModel::ENumProperty::kFullyAnalytical; };
    virtual Double_t GetProbDensity1d(const Double_t r, const Double_t* params) const;
    virtual Double_t GetProbDensity3d(const TVector3& vec, const Double_t* params) const;
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
    virtual ENumProperty GetModelNumProp() const { return FemtoSourceModel::ENumProperty::kFullyAnalytical; };
    virtual Double_t GetProbDensity1d(const Double_t r, const Double_t* params) const;
    virtual Double_t GetProbDensity3d(const TVector3& vec, const Double_t* params) const;
    void GenerateCoordinates(FemtoPair* Pair);
    virtual ~FemtoSourceModelGauss3D();
    ClassDef(FemtoSourceModelGauss3D, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOSOURCEMODELGAUSS_H_ */
