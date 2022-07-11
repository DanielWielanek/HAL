/*
 * CorrFitSHFrom3D.h
 *
 *  Created on: 7 lip 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_SH_CORRFITSHFROM3D_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_SH_CORRFITSHFROM3D_H_

#include "CorrFitSHCF.h"
/**
 * class for calculation of CF from known 3D func
 * very slow !!
 */

class TF2;
namespace Hal {

  class CorrFitSHFrom3D : public CorrFitSHCF {

  protected:
    mutable TF2* fHarmonic;
    Double_t Integral(Double_t q, Double_t ilm) const;
    virtual Double_t Calc3DCartesian(Double_t* x, Double_t* par) const;
    void ShToCart(Double_t* cart, Double_t* sph) const;

    Double_t Legendres_Fx(Double_t* x, Double_t* par) const;
    /**
     *
     * @param x - q inv
     * @param params parameters of fit
     * @return 0, fill YLmBuffer
     */
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFitSHFrom3D(Int_t params = 3);
    virtual ~CorrFitSHFrom3D();
    ClassDef(CorrFitSHFrom3D, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_SH_CORRFITSHFROM3D_H_ */
