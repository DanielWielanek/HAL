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

#include "FemtoSourceDensity.h"

#include "FemtoSourceModel.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TMatrixDfwd.h>
#include <TVector3.h>

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

  class FemtoSourceModelGauss3DCross : public FemtoSourceModel3D {
    TMatrixD fAMatrix;
    TMatrixD fCovMatrix;
    Double_t fRowA[3];
    Double_t fRowB[3];
    Double_t fRowC[3];

  public:
    /**
     * default ctor
     */
    FemtoSourceModelGauss3DCross();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModelGauss3DCross(const FemtoSourceModelGauss3DCross& model);
    FemtoSourceModel* MakeCopy() const;
    /**
     * set out radius
     * @param out radius
     */
    void SetOutSideRadius(Double_t os) { SetParameter(os, 3); };
    /**
     * set side radius
     * @param side radius
     */
    void SetOutLongRadius(Double_t sl) { SetParameter(sl, 4); };
    /**
     * set long radius
     * @param longr radius
     */
    void SetSideLongRadius(Double_t ol) { SetParameter(ol, 5); };

    Double_t GetOutSide() const { return GetParameter(3); };
    Double_t GetOutLong() const { return GetParameter(4); };
    Double_t GetSideLong() const { return GetParameter(5); };
    void GenerateCoordinates(FemtoPair* Pair);
    Bool_t Init();
    virtual ~FemtoSourceModelGauss3DCross() {};
    ClassDef(FemtoSourceModelGauss3DCross, 1)
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
