/*
 * V0BasicCut.cxx
 *
 *  Created on: 26 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "V0CandBasicCut.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "ExpEvent.h"
#include "McEvent.h"
#include "V0Candidate.h"

namespace Hal {
  V0CandBasicCut::V0CandBasicCut() : TwoTrackCut(6) {
    SetUnitName("DCA1to2 [cm[", DCA1to2());
    SetUnitName("DCAtoPV [cm]", DCAToPV());
    SetUnitName("DecLenght [cm]", DecLength());
    SetUnitName("#alpha [AU]", AlphaArm());
    SetUnitName("p_{T} [AU]", PtArm());
    SetUnitName("CosAngle [U]", CosAngle());
  }

  Bool_t V0CandBasicCut::Pass(TwoTrack* pair) {
    V0Candidate* cand = static_cast<V0Candidate*>(pair);
    const V0Track v0  = cand->GetV0Track();
    SetValue(v0.GetDauDist(), DCA1to2());
    SetValue(cand->GetDCA().Mag(), DCAToPV());
    SetValue(v0.GetDecLength(), DecLength());
    SetValue(v0.GetAlphaArm(), AlphaArm());
    SetValue(v0.GetPtArm(), PtArm());
    SetValue(v0.GetCosAngle(), CosAngle());
    return Validate();
  }

  V0CandBasicCut::~V0CandBasicCut() {}
}  // namespace Hal
