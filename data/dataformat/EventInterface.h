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
  public:
    enum class eMode { kRead, kWrite, kWriteVirtual };

  private:
    Bool_t fConnected;

  protected:
    virtual void ConnectToTreeInternal(eMode mode) = 0;

  public:
    EventInterface();
    virtual void ConnectToTree(eMode mode);
    virtual ~EventInterface();
    ClassDef(EventInterface, 1)
  };
}  // namespace Hal
#endif /* HAL_DATAFORMAT_HALEVENTINTERFACE_H_ */
