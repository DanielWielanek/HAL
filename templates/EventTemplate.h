/*
 * EventTemplate.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_TEMPLATES_EVENTTEMPLATE_H_
#define HAL_TEMPLATES_EVENTTEMPLATE_H_

#include <Hal/Event.h>

namespace MyHal {

  class EventTemplate : public Hal::Event {
  public:
    EventTemplate();
    Hal::EventInterface* CreateInterface() const;
    virtual void Update(Hal::EventInterface* interface);
    virtual void Clear(Option_t* opt = " ") {};
    virtual Bool_t ExistInTree() const;
    virtual TString GetFormatName() const;
    virtual ~EventTemplate();
    ClassDef(EventTemplate, 1)
  };

}  // namespace MyHal

#endif /* HAL_TEMPLATES_EVENTTEMPLATE_H_ */
