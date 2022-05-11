/*
 * CorrFit3DCFGausCross.h
 *
 *  Created on: 01-05-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFIT3DCFGAUSCROSS_H_
#define HALCORRFIT3DCFGAUSCROSS_H_
#include "CorrFit3DCF.h"
/**
 * class for fitting 3-dim CF's with only quantum statistic effects where are
 * present all radii components
 */

namespace Hal {
  class CorrFit3DCF_GaussCross : public CorrFit3DCF {
    static const Int_t fgRoutlong;
    static const Int_t fgRoutside;
    static const Int_t fgRsidelong;

  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCF_GaussCross();
    /**
     *
     * @return number of parameter R-out-long
     */
    inline static Int_t Routlong() { return fgRoutlong; };
    /**
     *
     * @return number of parameter R-out-side
     */
    inline static Int_t Routside() { return fgRoutside; };
    /**
     *
     * @return number of parameter R-side-long
     */
    inline static Int_t Rsidelong() { return fgRsidelong; };
    virtual ~CorrFit3DCF_GaussCross();
    ClassDef(CorrFit3DCF_GaussCross, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT3DCFGAUSCROSS_H_ */
