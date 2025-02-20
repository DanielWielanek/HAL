/*
 * CorrFit1DCFSquare.h
 *
 *  Created on: 22 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT1DCFSQUARE_H_
#define HALCORRFIT1DCFSQUARE_H_

#include "CorrFit1DCF.h"

/**
 * class for fitting background as polynomial acording to equation
 * Norm(1+A*q+B*q2)
 */
namespace Hal {
  class CorrFit1DCF_Square : public CorrFit1DCF {
    const Int_t fA = {1};
    const Int_t fB = {2};

  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    /**
     *
     * @param pol polynomial degree (max = 2)
     */
    CorrFit1DCF_Square(Int_t pol = 2);
    /**
     *
     * @return id for A parameter
     */
    inline Int_t AID() const { return fA; };
    /**
     *
     * @return id for B parameter
     */
    inline Int_t BID() const { return fB; };
    virtual ~CorrFit1DCF_Square();
    ClassDef(CorrFit1DCF_Square, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT1DCFSQUARE_H_ */
