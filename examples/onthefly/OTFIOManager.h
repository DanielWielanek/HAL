/*
 * OTFIOManager.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFIOMANAGER_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFIOMANAGER_H_

#include "IOManager.h"
#include "VirtualIOManager.h"

#include <vector>

#include <RtypesCore.h>
#include <TString.h>

class TTree;
class TBranch;

namespace HalOTF {
  class Source;
  class IOManager : public Hal::VirtualIOManager {
  public:
    IOManager(HalOTF::Source* source = nullptr, Int_t entries = 1);
    virtual ~IOManager();
    ClassDef(IOManager, 1)
  };
}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFIOMANAGER_H_ */
