/*
 * TrackTpcToFCut2.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKTPCTOFTHRESHOLDLESSCUT_H_
#define HALTRACKTPCTOFTHRESHOLDLESSCUT_H_

#include "TrackTpcToFCut.h"

namespace Hal {
  class TrackTpcToFThresholdlessCut : public TrackTpcToFCut {
  public:
    enum ParID { TpcHits = 0, Charge = 1, SigmaPion = 2, SigmaKaon = 3, SigmaProton = 4, SigmaElectron = 5, DeDx = 6, M2 = 7 };

    TrackTpcToFThresholdlessCut(TrackTpcCut* tpc = NULL, TrackToFMass2Cut* tof = NULL);
    virtual Bool_t Pass(Track* tr);
    virtual ~TrackTpcToFThresholdlessCut();
    ClassDef(TrackTpcToFThresholdlessCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKTPCTOFTHRESHOLDLESSCUT_H_ */
