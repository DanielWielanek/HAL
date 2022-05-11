/*
 * PdgBinPairCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPDGBINPAIRCUT_H_
#define HALPDGBINPAIRCUT_H_

#include "McTrack.h"
#include "TwoTrackCut.h"

namespace Hal {
  class PdgBinPairCut : public TwoTrackCut {
    Int_t PidToID(McTrack* tr) const;

  public:
    PdgBinPairCut();
    virtual Bool_t Pass(TwoTrack* pair);
    static Int_t FirstParticle() { return 0; }
    static Int_t SecondParticle() { return 1; };
    std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t i) const;
    virtual ~PdgBinPairCut();
    ClassDef(PdgBinPairCut, 1)
  };
}  // namespace Hal
#endif /* HALPDGBINPAIRCUT_H_ */
