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
} /* namespace HalOTF */


namespace HalOTF {
  class Reader : public Hal::Reader {
  protected:
    TH2D* fSpectras;
    Bool_t fOwner;
    Bool_t fRegister;
    Int_t fMultiplicity;
    Int_t fPids;
    Int_t fCharge;
    Double_t fMass;
    Double_t fSmear;
    OTF::McEvent* fMcEvent;
    OTF::RecoEvent* fRecoEvent;


  public:
    Reader();
    /**
     *
     * @param h histogram x-axis eta, y-axis pt
     * @param pid pid of particle
     * @param m multiplicity
     */
    void SetSpiecies(const TH2D& h, Int_t pid, Double_t m);
    void SetSmear(Double_t smear) { fSmear = smear; }
    void Register(Bool_t reg) { fRegister = reg; }
    void Exec(Option_t* opt);
    virtual Hal::Task::EInitFlag Init();
    virtual ~Reader();
    ClassDef(Reader, 1)
  };
}  // namespace HalOTF
#endif /* HAL_EXAMPLES_ONTHEFLY_OTFREADER_H_ */
