/*
 * CorrFit3DCFGauss2.h
 *
 *  Created on: 21 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT3DCFGAUSS2_H_
#define HALCORRFIT3DCFGAUSS2_H_

#include "CorrFit3DCF.h"
/**
 * class for fitting two gaussian source, important note - Rout,Rside,Rlong
 * correspond to radii of first source
 */

namespace Hal {
  class CorrFit3DCF_Gauss2 : public CorrFit3DCF {
    Int_t fRoutId   = {5};
    Int_t fRsideId  = {6};
    Int_t fRlongId  = {7};
    Int_t fLambdaId = {8};

  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCF_Gauss2();
    /**
     *
     * @return id of parameter R-out-1
     */
    inline Int_t Rout1ID() const { return RoutID(); };
    /**
     *
     * @return id of parameter R-out-2
     */
    inline Int_t Rout2ID() const { return fRoutId; };
    /**
     *
     * @return id of parameter R-side-1
     */
    inline Int_t Rside1ID() const { return RsideID(); };
    /**
     *
     * @return id of parameter R-side2
     */
    inline Int_t Rside2ID() const { return fRsideId; };
    /**
     *
     * @return id of parameter R-long1
     */
    inline Int_t Rlong1ID() const { return RlongID(); };
    /**
     *
     * @return id of parameter R-long2
     */
    inline Int_t Rlong2ID() const { return fRlongId; };
    /**
     *
     * @return id of paramter lambda1
     */
    inline Int_t Lambda1ID() const { return LambdaID(); };
    /**
     *
     * @return id of lambda2
     */
    inline Int_t Lambda2ID() const { return fLambdaId; }
    virtual ~CorrFit3DCF_Gauss2();
    ClassDef(CorrFit3DCF_Gauss2, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT3DCFGAUSS2_H_ */
