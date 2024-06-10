/*
 * CorrFit1DCFPoly.h
 *
 *  Created on: 28 lis 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT1DCFPOLY_H_
#define HALCORRFIT1DCFPOLY_H_

#include "CorrFit1DCF.h"
/**
 * fit correlation function by polynomial Norm*(Ax+Bx2+Cx3)
 */

namespace Hal {
  class CorrFit1DCF_Poly : public CorrFit1DCF {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    /**
     *
     * @param degree - polynomial degree (max is 3)
     */
    CorrFit1DCF_Poly(Int_t degree = 2);
    Int_t AID() const { return 0; };
    Int_t BID() const { return 1; };
    Int_t CID() const { return 3; };
    virtual ~CorrFit1DCF_Poly();
    ClassDef(CorrFit1DCF_Poly, 1)
  };

  class CorrFit1DCF_Poly2 : public CorrFit1DCF_Poly {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCF_Poly2(Int_t degree);
    virtual ~CorrFit1DCF_Poly2();
    ClassDef(CorrFit1DCF_Poly2, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT1DCFPOLY_H_ */
