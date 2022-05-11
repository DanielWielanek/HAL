/*
 * FlowPtVariable.h
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFLOWPTVARIABLE_H_
#define HALFLOWPTVARIABLE_H_

#include "FlowVariable.h"
/**
 * class that is used for drawing flow vs transverse momentum
 */
namespace Hal {
  class FlowPtVariable : public FlowVariable {
  public:
    FlowPtVariable();
    virtual Double_t GetVariable(Track* p);
    virtual FlowVariable* MakeCopy() const { return new FlowPtVariable(*this); };
    virtual ~FlowPtVariable();
    ClassDef(FlowPtVariable, 1)
  };
}  // namespace Hal

#endif /* HALFLOWPTVARIABLE_H_ */
