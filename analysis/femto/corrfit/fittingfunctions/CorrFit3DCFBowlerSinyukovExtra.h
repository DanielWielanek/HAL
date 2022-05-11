/*
 * CorrFit3DCFBowlerSinyukovExtra.h
 *
 *  Created on: 31 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT3DCFBOWLERSINYUKOVEXTRA_H_
#define HALCORRFIT3DCFBOWLERSINYUKOVEXTRA_H_

#include "CorrFit3DCFBowlerSinyukov.h"

/**
 * for calcuation of radii in eliptical shape - assumes that BS-correction
 * scales eliptical
 */

namespace Hal {
  class CorrFit3DCFBowlerSinyukovEllipse : public CorrFit3DCFBowlerSinyukov {
  protected:
    /**
     * calculate "eliptical Rinv"
     * @param xout
     * @param xside
     * @param xlong
     * @param kout
     * @param kside
     * @param klong
     * @return
     */
    Double_t GetRinv(Double_t xout, Double_t xside, Double_t xlong, Double_t kout, Double_t kside, Double_t klong) const;
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCFBowlerSinyukovEllipse();
    virtual ~CorrFit3DCFBowlerSinyukovEllipse();
    ClassDef(CorrFit3DCFBowlerSinyukovEllipse, 1)
  };

  class CorrFit3DCFBowlerSinyukovClassic : public CorrFit3DCFBowlerSinyukov {
  protected:
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCFBowlerSinyukovClassic();
    static Int_t RCoul() { return 6; };
    virtual ~CorrFit3DCFBowlerSinyukovClassic();
    ClassDef(CorrFit3DCFBowlerSinyukovClassic, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT3DCFBOWLERSINYUKOVEXTRA_H_ */
