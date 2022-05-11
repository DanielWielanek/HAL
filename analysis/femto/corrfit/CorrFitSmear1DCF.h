/*
 * CorrFitSmear1DCF.h
 *
 *  Created on: 4 sty 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITSMEAR1DCF_H_
#define HALCORRFITSMEAR1DCF_H_

#include "CorrFitKisiel.h"
namespace Hal {
  class CorrFitSmear1DCF : public CorrFitKisiel {
  protected:
    Bool_t fSmeared;
    TH2D* fSmearingMap;
    virtual void Check();
    void RescaleSmearingMap();

  public:
    CorrFitSmear1DCF();
    void LoadSmearingMap(TH2D* map);
    TH2D* GetSmearedMap() const { return fSmearingMap; };
    virtual ~CorrFitSmear1DCF();
    ClassDef(CorrFitSmear1DCF, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITSMEAR1DCF_H_ */
