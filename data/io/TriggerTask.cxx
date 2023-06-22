/*
 * TriggerTask.cxx
 *
 *  Created on: 22 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TriggerTask.h"

#include "Package.h"
#include "Parameter.h"

namespace Hal {

  TriggerTask::TriggerTask() {}

  TriggerTask::~TriggerTask() {}

  Package* TriggerTask::Report() const {
    Package* pack = new Package(this);
    pack->AddObject(new ParameterBool("Active", fActive));
    return pack;
  }

}  // namespace Hal
