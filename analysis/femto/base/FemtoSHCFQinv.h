/*
 * FemtoSHCFQinv.h
 *
 *  Created on: 1 lip 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_FEMTOSHCFQINV_H_
#define HAL_ANALYSIS_FEMTO_BASE_FEMTOSHCFQINV_H_

#include "FemtoSHCF.h"
namespace Hal {

  class FemtoSHCFQinv : public FemtoSHCF {
    TH1D* fQinvDist;

  protected:
    virtual void FastAdd(const FemtoSHCF* obj);

  public:
    FemtoSHCFQinv();
    FemtoSHCFQinv(TString name, Int_t maxL, Int_t bins, Double_t min, Double_t max, Femto::EKinematics kinematics);
    virtual void FillNumObj(TObject* obj);
    virtual void FillDenObj(TObject* obj);
    TH1D* GetQinv() const { return fQinvDist; }
    virtual ~FemtoSHCFQinv();
    ClassDef(FemtoSHCFQinv, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_FEMTOSHCFQINV_H_ */
