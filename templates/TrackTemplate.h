/*
 * TrackTemplate.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_TEMPLATES_TRACKTEMPLATE_H_
#define HAL_TEMPLATES_TRACKTEMPLATE_H_

#include <Hal/Track.h>
namespace MyHal {

  class TrackTemplate : public Hal::Track {
    /**
     * add your fields, your getters, and setters
     */
  public:
    TrackTemplate();
    Int_t Init(Int_t taskID);
    Bool_t Pass(Hal::Track* pair);
    Hal::Package* Report() const;
    virtual ~TrackTemplate();
    ClassDef(TrackTemplate, 1)
  };

}  // namespace MyHal

#endif /* HAL_TEMPLATES_TRACKTEMPLATE_H_ */
