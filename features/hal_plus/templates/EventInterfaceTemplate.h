/*
 *Generated by hal-cmake
 */
#ifndef HAL_CMAKE_TEMPLATES_EVENTINTERFACETEMPLATE_H_
#define HAL_CMAKE_TEMPLATES_EVENTINTERFACETEMPLATE_H_

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

#endif /* HAL_CMAKE_TEMPLATES_EVENTINTERFACETEMPLATE_H_ */
