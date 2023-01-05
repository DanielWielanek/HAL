/*
 * FemtoSourceDensity.h
 *
 *  Created on: 3 sty 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEDENSITY_H_
#define HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEDENSITY_H_

#include <RtypesCore.h>
#include <TVector3.h>

/**
 * class that returns probablity of having femtoscopic pair with give radii
 */

namespace Hal {
  class FemtoSourceDensity : public TObject {
    Int_t fNParams;
    Bool_t fAna1d;
    Bool_t fAna3d;
    Double_t* fParams;  //[fNParams]
  public:
    FemtoSourceDensity(Int_t params = 0, Bool_t ana1d = kFALSE, Bool_t ana3d = kTRUE) :
      fNParams(params), fParams(nullptr), fAna1d(ana1d), fAna3d(ana3d) {
      if (fNParams) fParams = new Double_t[params];
    };
    /**
     *
     * @param other
     */
    FemtoSourceDensity(const FemtoSourceDensity& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    FemtoSourceDensity& operator=(const FemtoSourceDensity& other);
    /**
     * set parameters for calculation of relative source density
     * @param params
     */
    virtual void SetBaseParameters(const Double_t* params);
    /**
     * returns  probability
     * @param
     * @param
     * @return
     */
    virtual Double_t GetProbDensity3d(const TVector3& /*vec*/, const Double_t* /*params*/) const { return 0; };
    /**
     *
     * @param r one dim radius
     * @param params
     * @return normalized probability (this->prob/base->prob)
     */

    virtual Double_t GetProbDensity1d(const Double_t /*r*/, const Double_t* /*params*/) const { return 0; };
    /**
     *  returns probability relative to base parameters
     * @param vec rout/rside/rlong
     * @param params source parameters (this->prob/base->prob)
     * @return
     */
    virtual Double_t GetRelativeProbDensity3d(const TVector3& vec, const Double_t* params);
    /**
     * return probability relative to base parameters
     * @param r radius
     * @param params source parameters
     * @return
     */
    virtual Double_t GetRelativeProbDensity1d(const Double_t r, const Double_t* params) const;
    /**
     *
     * @return true if model is 1d analytical
     */
    Bool_t IsAna1d() const { return fAna1d; }
    /**
     *
     * @return true if model is 3d analytical
     */
    Bool_t IsAna3d() const { return fAna3d; }
    virtual ~FemtoSourceDensity() {
      if (fParams) delete[] fParams;
    };
    ClassDef(FemtoSourceDensity, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_FEMTO_BASE_SOURCE_MODELS_FEMTOSOURCEDENSITY_H_ */
