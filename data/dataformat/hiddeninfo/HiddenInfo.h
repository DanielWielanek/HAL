/*
 * HiddenInfo.h
 *
 *  Created on: 24 lis 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_DATA_DATAFORMAT_HIDDENINFO_HIDDENINFO_H_
#define HAL_DATA_DATAFORMAT_HIDDENINFO_HIDDENINFO_H_

#include "TObject.h"
namespace Hal {
  /**
   * class for storing "hidden info"
   */
  class HiddenInfo : public TObject {
  public:
    HiddenInfo() {};
    virtual ~HiddenInfo() {};
    ClassDef(HiddenInfo, 1)
  };
} /* namespace Hal */

#endif /* HAL_DATA_DATAFORMAT_HIDDENINFO_HIDDENINFO_H_ */
