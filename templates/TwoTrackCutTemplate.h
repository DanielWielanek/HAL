/*
 * TwoTrackCutTemplates.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_TEMPLATES_TWOTRACKCUTTEMPLATE_H_
#define HAL_TEMPLATES_TWOTRACKCUTTEMPLATE_H_

#include <Hal/TwoTrackCut.h>

namespace MyHal {
  class Hal::Package;
  class TwoTrackCutTemplate : public Hal::TwoTrackCut {
  public:
    TwoTrackCutTemplate();
    Int_t Init(Int_t taskID);
    Bool_t Pass(Hal::TwoTrack* pair);
    Hal::Package* Report() const;
    virtual ~TwoTrackCutTemplate() {};
    ClassDef(TwoTrackCutTemplate, 1)
  };

}  // namespace MyHal

#endif /* HAL_TEMPLATES_TWOTRACKCUTTEMPLATE_H_ */
