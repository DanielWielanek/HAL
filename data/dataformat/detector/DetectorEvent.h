/*
 * DetectorEvent.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALDETECTOREVENT_H_
#define HALDETECTOREVENT_H_

#include "DataFormat.h"

#include <TObject.h>

/**
 * abstract class for holding information about event from given type of
 * detector
 */
namespace Hal {
  class DetectorEvent : public TObject {
  public:
    DetectorEvent();
    virtual ~DetectorEvent();
    ClassDef(DetectorEvent, 1)
  };
}  // namespace Hal

#endif /* HALDETECTOREVENT_H_ */
