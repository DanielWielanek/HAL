/*
 * CutMonitorTranslator.cxx
 *
 *  Created on: 4 sie 2025
 *      Author: daniel
 */

#include "CutMonitorFieldIdTranslator.h"

#include "DataFormat.h"

namespace Hal {

  void CutMonitorFieldIdTranslator::SwitchToRe() {
    for (int i = 0; i < 3; i++) {
      if (!Hal::DataFieldID::IsRe(fFieldIds[i])) fFieldIds[i] += Hal::DataFieldID::ReStep;
    }
  }

  void CutMonitorFieldIdTranslator::SwitchToIm() {
    for (int i = 0; i < 3; i++) {
      if (!Hal::DataFieldID::IsIm(fFieldIds[i])) fFieldIds[i] += Hal::DataFieldID::ImStep;
    }
  }

} /* namespace Hal */
