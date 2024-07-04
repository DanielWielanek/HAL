/*
 * OTFGenerator.h
 *
 *  Created on: 3 lip 2024
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_OTFEVENTGENERATOR_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFEVENTGENERATOR_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>
#include <TString.h>

#include "Object.h"
#include "Task.h"

namespace OTF {
  class McEvent;
  class RecoEvent;
} /* namespace OTF */

class TH1D;
class TH2D;

namespace HalOTF {
  class Source;
  class EventGenerator : public Hal::Object {
    friend class HalOTF::Source;

  protected:
    TH2D* fSpectras            = {nullptr};
    TH1D* fMultiplicity        = {nullptr};
    Int_t fFixedMultiplicity   = {-1};
    Int_t fPids                = {211};
    Int_t fCharge              = {1};
    Int_t fCurrrentMult        = {0};
    Double_t fMass             = {0};
    Double_t fSmear            = {0};
    OTF::McEvent* fMcEvent     = {nullptr};
    OTF::RecoEvent* fRecoEvent = {nullptr};
    virtual void GenerateEvent();

  public:
    EventGenerator();
    /**
     *
     * @param h histogram x-axis eta, y-axis pt
     * @param pid pid of particle
     * @param m multiplicity
     */
    void SetSpiecies(const TH2D& h, Int_t pid);
    /**
     * set multiplicity histogram
     * @param h
     */
    void SetMultHisto(TH1D& h);
    /** fixes multiplicity  */
    void SetFixMult(Int_t mult);
    void SetSmear(Double_t smear) { fSmear = smear; }
    void SetEvents(OTF::McEvent* mc, OTF::RecoEvent* reco) {
      fRecoEvent = reco;
      fMcEvent   = mc;
    }
    virtual Bool_t Init();
    virtual ~EventGenerator();
    ClassDef(EventGenerator, 1)
  };

}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFEVENTGENERATOR_H_ */
