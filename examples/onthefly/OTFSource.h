/*
 * OTFSource.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFSOURCE_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFSOURCE_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <vector>

#include "Source.h"
#include "VirtualSource.h"

namespace Hal {
  class IOManager;
  class VirtualIOManager;
}  // namespace Hal
namespace HalOTF {
  class EventGenerator;
  class IOManager;
}  // namespace HalOTF
namespace OTF {
  class McEvent;
  class RecoEvent;
}  // namespace OTF

namespace HalOTF {
  class Source : public Hal::VirtualSource {
    friend class HalOTF::IOManager;
    friend class Hal::VirtualIOManager;
    std::vector<HalOTF::EventGenerator*> fGenerators;
    OTF::McEvent* fMcEvent     = {nullptr};
    OTF::RecoEvent* fRecoEvent = {nullptr};

  protected:
    /**
     * register output data and sent pointers to generators, this cannot be done ini Init method
     * because data manager does not exist yet
     */
    virtual void RegisterInputs();

  public:
    Source(Int_t entries = 0);
    void AddEventGenerator(HalOTF::EventGenerator* evgen) { fGenerators.push_back(evgen); }
    Bool_t Init();
    void GetEvent(Int_t i, Int_t flag);
    virtual ~Source();
    ClassDef(Source, 1)
  };
}  // namespace HalOTF
#endif /* HAL_EXAMPLES_ONTHEFLY_OTFSOURCE_H_ */
