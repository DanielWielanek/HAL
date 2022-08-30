/*
 * EventImpactParameterCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALEVENTIMPACTPARAMETERCUT_H_
#define HALEVENTIMPACTPARAMETERCUT_H_

#include "EventMcCut.h"
/**
 * check impact parameter value
 */

namespace Hal {
  class EventImpactParameterCut : public EventMcCut {
  protected:
  public:
    EventImpactParameterCut();
    virtual Bool_t Pass(Event* event);
    virtual ~EventImpactParameterCut();
    ClassDef(EventImpactParameterCut, 1)
  };

  /**
   * check centrality of event by simple calculations that use impact parameter
   */
  class EventImpactCentralityCut : public EventMcCut {
    Double_t fBMax;
    Double_t fAlpha;
    Double_t ImpactToCentrality(const Double_t impact);

  public:
    EventImpactCentralityCut();
    /**
     * set maximum expected impact parameter for given kind of collisions
     * @param bmax maximum expected impact parameter
     */
    void SetBMax(Double_t bmax);
    Bool_t Init(Int_t task_id);
    virtual Bool_t Pass(Event* event);
    virtual ~EventImpactCentralityCut();
    ClassDef(EventImpactCentralityCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTIMPACTPARAMETERCUT_H_ */
