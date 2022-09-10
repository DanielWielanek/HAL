/*
 * EventInterfaceTemplate.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_TEMPLATES_EVENTINTERFACETEMPLATE_H_
#define HAL_TEMPLATES_EVENTINTERFACETEMPLATE_H_

#include <Hal/EventInterface.h>

namespace MyHal {
  class EventTemplate;
  class EventInterfaceTemplate : public Hal::EventInterface {
    /**
     * make friendship with your event, you will not have to write all these getters
     */
    friend class EventTemplate;

    TObject* fEvent;
    Bool_t fCanDelete;

  protected:
    virtual void ConnectToTreeInternal(eMode mode);

  public:
    EventInterfaceTemplate();
    virtual ~EventInterfaceTemplate();
    ClassDef(EventInterfaceTemplate, 1)
  };

}  // namespace MyHal

#endif /* HAL_TEMPLATES_EVENTINTERFACETEMPLATE_H_ */
