/*
 * FlowVariable.cxx
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FlowVariable.h"
namespace Hal {
  FlowVariable::FlowVariable(TString axisName, TString axisUnit) : fName(axisName), fUnit(axisUnit) {}

  TString FlowVariable::GetAxisName() { return fName; }

  Double_t FlowVariable::GetVariable(Track* /*p*/) { return 0.; }

  FlowVariable::~FlowVariable() {
    // TODO Auto-generated destructor stub
  }

  TString FlowVariable::GetAxisUnit() { return fUnit; }

  FlowVirtualVariable::FlowVirtualVariable() : FlowVariable("virtual", "virtual") {}

  FlowVirtualVariable::~FlowVirtualVariable() {}
}  // namespace Hal
