/*
 * TrackMcParentCut.h
 *
 *  Created on: 15 gru 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_MC_TRACKMCPARENTCUT_H_
#define HAL_CUTS_TRACKCUTS_MC_TRACKMCPARENTCUT_H_

#include "TrackMcCut.h"

namespace Hal {
  class McTrack;
  /**
   * abstract class for cut on parent of mc track
   */
  class TrackMcParentCut : public TrackMcCut {
  protected:
    virtual Bool_t InnerPass(McTrack* track) = 0;

  public:
    TrackMcParentCut(Int_t size = 1);
    virtual Bool_t Pass(Track* track);
    virtual ~TrackMcParentCut() {};
    ClassDef(TrackMcParentCut, 1)
  };

  /**
   * class that check the parent pdg
   */
  class TrackMcParentPdgCut : public TrackMcParentCut {
    virtual Bool_t InnerPass(McTrack* track);

  public:
    TrackMcParentPdgCut();
    virtual ~TrackMcParentPdgCut() {};
    ClassDef(TrackMcParentPdgCut, 1)
  };

}  // namespace Hal

#endif /* HAL_CUTS_TRACKCUTS_MC_TRACKMCPARENTCUT_H_ */
