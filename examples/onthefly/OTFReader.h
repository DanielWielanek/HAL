/*
 * OTFReader.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFREADER_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFREADER_H_

#include "Reader.h"

#include "OTFData.h"

#include <vector>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

namespace HalOTF {
  class McEvent;
  class RecoEvent;
  class ComplexEvent;
} /* namespace HalOTF */

/**
 * class that generates on the fly data
 */
namespace HalOTF {
  class Reader : public Hal::Reader {
  protected:
    enum class ETranslate { kNone, kMc, kReco, kComplex };
    ETranslate fTranslate = {ETranslate::kNone};
    OTF::McEvent* fMcEvent;
    OTF::RecoEvent* fRecoEvent;
    HalOTF::ComplexEvent* fHalComplexEvent   = {nullptr};
    HalOTF::RecoEvent* fHalRecoEvent         = {nullptr};
    HalOTF::McEvent* fHalMcEvent             = {nullptr};
    Hal::EventInterface* fTranslateInterface = {nullptr};

  public:
    Reader();
    /**
     * specify how to translate events (if want to use a direct acces to data
     * @param opt reco - for reco translation, mc - for sim translatation, complex or mc+reco - for complex translation
     * note, last Reader task should translate events
     */
    void Translate(TString opt);
    virtual void Exec(Option_t* opt);
    virtual Hal::Task::EInitFlag Init();
    virtual ~Reader();
    ClassDef(Reader, 1)
  };
}  // namespace HalOTF
#endif /* HAL_EXAMPLES_ONTHEFLY_OTFREADER_H_ */
