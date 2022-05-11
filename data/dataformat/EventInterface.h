/*
 * EventInterface.h
 *
 *  Created on: 4 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_DATAFORMAT_HALEVENTINTERFACE_H_
#define HAL_DATAFORMAT_HALEVENTINTERFACE_H_

#include <TLorentzVector.h>
#include <TObject.h>

namespace Hal {
  class EventInterface : public TObject {
  protected:
    /**
     * link source object with tree (was called LinkWithTree) used for reading
     * events
     */
    virtual void ConnectToTree() = 0;

  public:
    enum class eMode { kRead, kWrite, kWriteVirtual };
    EventInterface();
    virtual void LinkWithTree(eMode mode);
    virtual ~EventInterface();
    ClassDef(EventInterface, 1)
  };
}  // namespace Hal
#endif /* HAL_DATAFORMAT_HALEVENTINTERFACE_H_ */
