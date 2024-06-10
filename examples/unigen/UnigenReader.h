/*
 * UnigenReader.h
 *
 *  Created on: 18 kwi 2024
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_UNIGEN_UNIGENREADER_H_
#define HAL_EXAMPLES_UNIGEN_UNIGENREADER_H_

#include "Reader.h"

/**
 * reader to convert unigen events on the fly
 */
namespace HalUni {
  class UnigenEvent;
  class UnigenEventInterface;
  class UnigenReader : public Hal::Reader {
    UnigenEvent* fUniEvent           = {nullptr};
    UnigenEventInterface* fInterface = {nullptr};

  public:
    UnigenReader();
    virtual void Exec(Option_t* opt);
    virtual Hal::Task::EInitFlag Init();
    virtual ~UnigenReader();
    ClassDef(UnigenReader, 1)
  };

} /* namespace HalUni */

#endif /* HAL_EXAMPLES_UNIGEN_UNIGENREADER_H_ */
