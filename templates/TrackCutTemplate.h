/*
 * TrackCutTemplates.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_TEMPLATES_TRACKCUTTEMPLATE_H_
#define HAL_TEMPLATES_TRACKCUTTEMPLATE_H_

#include <Hal/TrackCut.h>

namespace MyHal {
  class Hal::Package;
  class TrackCutTemplate : public Hal::TrackCut {
  public:
    TrackCutTemplate();
    Int_t Init(Int_t taskID);
    Bool_t Pass(Hal::Track* pair);
    Hal::Package* Report() const;
    virtual ~TrackCutTemplate();
    ClassDef(TrackCutTemplate, 1)
  };

}  // namespace MyHal

#endif /* HAL_TEMPLATES_TRACKCUTTEMPLATE_H_ */
