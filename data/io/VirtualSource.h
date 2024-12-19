/*
 * VirtualRootSource.h
 *
 *  Created on: 19 gru 2024
 *      Author: daniel
 */

#ifndef HAL_DATA_IO_VIRTUALSOURCE_H_
#define HAL_DATA_IO_VIRTUALSOURCE_H_

#include <RtypesCore.h>
#include <TString.h>
#include <vector>

#include "IOManager.h"
#include "Source.h"

class TTree;

namespace Hal {
  class VirtualIOManager;
  /**
   * class that represents source without a physical input (e.g. for generation data on the fly)
   */
  class VirtualSource : public Source {
    friend class VirtualIOManager;

  protected:
    Int_t fEvents    = {0};
    Bool_t fRegister = {kFALSE};
    /**
     * register input data and sent pointers to generators, this cannot be done until Init method
     * because data manager does not exist yet
     */
    virtual void RegisterInputs() {};
    VirtualSource(IOManager* mngr, Int_t nevents = 1000);

  public:
    VirtualSource(Int_t nevents = 1000);
    virtual Bool_t Init();
    /**
     * register generated data in output
     */
    virtual void Register() { fRegister = kTRUE; }
    virtual void GetEvent(Int_t i, Int_t flag);
    virtual ~VirtualSource() {};
    ClassDef(VirtualSource, 1)
  };
} /* namespace Hal */

#endif /* HAL_DATA_IO_VIRTUALSOURCE_H_ */
