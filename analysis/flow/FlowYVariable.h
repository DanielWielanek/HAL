/*
 * FlowYVariable.h
 *
 *  Created on: 05-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFLOWYVARIABLE_H_
#define HALFLOWYVARIABLE_H_

#include "FlowVariable.h"

/**
 * class used for drawing plot vs rapidity
 */
namespace Hal {
  class FlowYVariable : public FlowVariable {
  public:
    FlowYVariable();
    virtual Double_t GetVariable(Track* p);
    virtual FlowVariable* MakeCopy() const { return new FlowYVariable(*this); };
    virtual ~FlowYVariable();
    ClassDef(FlowYVariable, 1)
  };
}  // namespace Hal

#endif /* HALFLOWYVARIABLE_H_ */
