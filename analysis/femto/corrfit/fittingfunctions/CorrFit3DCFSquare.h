/*
 * CorrFit3DSquare.h
 *
 *  Created on: 22 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT3DSQUARE_H_
#define HALCORRFIT3DSQUARE_H_
#include "CorrFit3DCF.h"

/**
 * class for estimate background, represent polynomial up to second degree
 * N(1+Ax*q_{out}+Bx*q_{oust}^2+...)
 */

namespace Hal {
  class CorrFit3DCF_Square : public CorrFit3DCF {
    const Int_t fgAx = {0};
    const Int_t fgBx = {1};
    const Int_t fgAy = {2};
    const Int_t fgBy = {3};
    const Int_t fgAz = {5};
    const Int_t fgBz = {6};

  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    /**
     *
     * @param pol polynomial degree
     */
    CorrFit3DCF_Square(Int_t pol = 2);
    /**
     *
     * @return id of Ax parameter
     */
    inline Int_t AxID() const { return fgAx; };
    /**
     *
     * @return id of Ax parameter
     */
    inline Int_t BxID() const { return fgBx; };
    /**
     *
     * @return id of Ay parameter
     */
    inline Int_t AyID() const { return fgAy; };
    /**
     *
     * @return id of By parameter
     */
    inline Int_t ByID() const { return fgBy; };
    /**
     *
     * @return id of Az parameter
     */
    inline Int_t AzID() const { return fgAz; };
    /**
     *
     * @return id of Bz parameter
     */
    inline Int_t BzID() const { return fgBz; };
    virtual ~CorrFit3DCF_Square();
    ClassDef(CorrFit3DCF_Square, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT3DSQUARE_H_ */
