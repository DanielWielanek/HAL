/*
 * DbgSource.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_DBGSOURCE_H_
#define HAL_EXAMPLES_ONTHEFLY_DBGSOURCE_H_

#include "Source.h"
namespace Hal {
  class IOManager;
}

namespace HalDbg {
  class Source : public Hal::Source {
    Int_t fEvents;
    Hal::IOManager* fManager;

  public:
    Source(Int_t entries = 0);
    TString GetSourceName() const { return fFileName[0]; };
    virtual Hal::IOManager* GetIOManager() const;
    virtual ~Source();
    ClassDef(Source, 1)
  };
}  // namespace HalDbg
#endif /* HAL_EXAMPLES_ONTHEFLY_DBGSOURCE_H_ */
