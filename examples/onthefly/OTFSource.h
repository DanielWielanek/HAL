/*
 * OTFSource.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFSOURCE_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFSOURCE_H_

#include "Source.h"
namespace Hal {
  class IOManager;
}

namespace HalOTF {
  class Source : public Hal::Source {
    Int_t fEvents;

  public:
    Source(Int_t entries = 0);
    virtual Hal::IOManager* GetIOManager() const;
    Bool_t Init() { return kTRUE; }
    virtual ~Source();
    ClassDef(Source, 1)
  };
}  // namespace HalOTF
#endif /* HAL_EXAMPLES_ONTHEFLY_OTFSOURCE_H_ */
