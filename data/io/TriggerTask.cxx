/*
 * TriggerTask.cxx
 *
 *  Created on: 22 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TriggerTask.h"
namespace Hal {

  TriggerTask::TriggerTask() {}

  Bool_t TriggerTask::EventGood() { return kTRUE; }

  TriggerTask::~TriggerTask() {}
}  // namespace Hal
