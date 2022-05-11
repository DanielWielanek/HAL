/*
 * TwoTrackCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKCUT_H_
#define HALTWOTRACKCUT_H_

#include "Cut.h"
namespace Hal {
  class TwoTrack;

  /**
   * basic class for all two-track cuts
   */
  class TwoTrackCut : public Cut {
  public:
    /**
     * default constructor
     * @param i number of parameter for checking
     */
    TwoTrackCut(const Int_t i = 1);
    virtual ~TwoTrackCut();
    /**
     * check pair
     * @param pair pair to check
     * @return true if pair is fine, false if should be rejected
     */
    virtual Bool_t Pass(TwoTrack* pair) = 0;
    virtual Package* Report() const;
    ClassDef(TwoTrackCut, 1)
  };
}  // namespace Hal

#endif /* HALTWOTRACKCUT_H_ */
