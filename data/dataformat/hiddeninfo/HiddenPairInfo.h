/*
 * HiddenPairInfo.h
 *
 *  Created on: 18 lis 2026
 *      Author: daniel
 */

#ifndef HAL_DATA_DATAFORMAT_HIDDENINFO_HIDDENPAIRINFO_H_
#define HAL_DATA_DATAFORMAT_HIDDENINFO_HIDDENPAIRINFO_H_

#include <TObject.h>

namespace Hal {

  class HiddenPairInfo : public TObject {
  public:
    HiddenPairInfo();
    virtual ~HiddenPairInfo();
    ClassDef(HiddenPairInfo, 1)
  };

} /* namespace Hal */

#endif /* HAL_DATA_DATAFORMAT_HIDDENINFO_HIDDENPAIRINFO_H_ */
