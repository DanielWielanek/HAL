/*
 * ComplexEventInterface.cxx
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "ComplexEventInterface.h"

namespace Hal {

  ComplexEventInterface::ComplexEventInterface(EventInterface* real, EventInterface* img) : fReal(real), fImag(img) {}

  ComplexEventInterface::~ComplexEventInterface() {
    if (fReal) delete fReal;
    if (fImag) delete fImag;
  }

  void ComplexEventInterface::ConnectToTreeInternal(Hal::EventInterface::eMode mode) {
    fReal->ConnectToTree(mode);
    fImag->ConnectToTree(mode);
  }

} /* namespace Hal */
