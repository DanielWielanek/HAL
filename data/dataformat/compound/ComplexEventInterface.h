/*
 * ComplexEventInterface.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_DATA_DATAFORMAT_COMPOUND_COMPLEXEVENTINTERFACE_H_
#define HAL_DATA_DATAFORMAT_COMPOUND_COMPLEXEVENTINTERFACE_H_

#include "EventInterface.h"
namespace Hal {

  class ComplexEventInterface : public EventInterface {
    EventInterface* fReal;
    EventInterface* fImag;

  protected:
    virtual void ConnectToTreeInternal(Hal::EventInterface::eMode mode);

  public:
    ComplexEventInterface(EventInterface* real = nullptr, EventInterface* img = nullptr);
    ComplexEventInterface(const ComplexEventInterface& other) = delete;
    EventInterface* GetReal() const { return fReal; }
    EventInterface* GetImag() const { return fImag; }
    virtual ~ComplexEventInterface();
    ClassDef(ComplexEventInterface, 1)
  };

} /* namespace Hal */

#endif /* HAL_DATA_DATAFORMAT_COMPOUND_COMPLEXEVENTINTERFACE_H_ */
