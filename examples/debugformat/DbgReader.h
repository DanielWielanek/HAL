/*
 * DbgReader.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_DBGREADER_H_
#define HAL_EXAMPLES_ONTHEFLY_DBGREADER_H_

#include "Reader.h"

#include "DbgData.h"

#include <vector>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

namespace HalDbg {
  class McEvent;
  class RecoEvent;
} /* namespace HalDbg */

/**
 * class that generates on the fly data
 */
namespace HalDbg {
  class Reader : public Hal::Reader {
  protected:
    TH2D* fSpectras            = {nullptr};
    Bool_t fOwner              = {kFALSE};
    Bool_t fRegister           = {kFALSE};
    Int_t fMultiplicity        = {1};
    Int_t fPids                = {211};
    Int_t fCharge              = {1};
    Int_t fCount               = {0};
    Int_t fModuloCount         = {10};
    Double_t fMass             = {0.};
    Double_t fSmear            = {0};
    Dbg::McEvent* fMcEvent     = {nullptr};
    Dbg::RecoEvent* fRecoEvent = {nullptr};


  public:
    Reader();
    /**
     *
     * @param h histogram x-axis eta, y-axis pt
     * @param pid pid of particle
     * @param m multiplicity
     */
    void SetSpiecies(const TH2D& h, Int_t pid, Double_t multiplicity);
    void SetSmear(Double_t smear) { fSmear = smear; }
    void Register(Bool_t reg) { fRegister = reg; }
    void Exec(Option_t* opt);
    void SetEventIdModulo(Int_t mod) { fModuloCount = mod; }
    virtual Hal::Task::EInitFlag Init();
    virtual ~Reader();
    ClassDef(Reader, 1)
  };
}  // namespace HalDbg
#endif /* HAL_EXAMPLES_ONTHEFLY_DBGREADER_H_ */
