/*
 * TrackAna.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACKANA_H_
#define HALTRACKANA_H_

#include "EventAna.h"


namespace Hal {
  class Track;

  class TrackAnaChain;
  /**
   * base class for making track analysis
   */
  class TrackAna : public EventAna {
    friend class TrackAnaChain;

  protected:
    /**
     * total number of track collections
     */
    Int_t fTrackCollectionsNo;
    /**
     * current track collection
     */
    Int_t fCurrentTrackCollectionID;
    /**
     * track index in loop over tracks
     */
    Int_t fTrackIndex;
    /**
     * pointer to currently processed track
     */
    Track* fCurrentTrack;
    virtual void ProcessEvent();
    //--- for overwrite by user ---------------------------//
    /**
     * method for processing track, should be overwritten in derived class if they
     * process tracks
     */
    virtual void ProcessTrack();
    virtual void LinkCollections();
    virtual void CheckCutContainerCollections();
    virtual Task::EInitFlag Init();
    virtual Package* Report() const;

  public:
    TrackAna();
    /**
     * copy ctor
     * @param ana
     */
    TrackAna(const TrackAna& ana);
    TrackAna& operator=(const TrackAna& other);
    virtual void SetOption(Option_t* option);
    virtual void Exec(Option_t* opt);
    virtual void FinishTask();
    virtual ~TrackAna();
    ClassDef(TrackAna, 1)
  };
}  // namespace Hal
#endif /* HALTRACKANA_H_ */
