/*
 * TrackPdgBinCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKPDGBINCUT_H_
#define HALTRACKPDGBINCUT_H_

#include "TrackCut.h"

namespace Hal {

  class TrackPdgBinCut : public TrackCut {
  public:
    TrackPdgBinCut();
    virtual Bool_t Pass(Track* track);
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels() const;
    virtual ~TrackPdgBinCut();
    ClassDef(TrackPdgBinCut, 1);
  };
}  // namespace Hal
#endif /* HALTRACKPDGBINCUT_H_ */
