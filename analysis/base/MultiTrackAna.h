/*
 * MultiTrackAna.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALMULTITRACKANA_H_
#define HALMULTITRACKANA_H_

#include "TrackAna.h"
/**
 * basic class for processing tracks, or trakc groups where buffering is
 * required
 */

namespace Hal {
  class MultiTrackAna : public TrackAna {
  protected:
    virtual void ProcessTrack();
    virtual void ProcessEvent();
    virtual void InitNewCutContainer();
    virtual Task::EInitFlag Init();
    virtual Task::EInitFlag CheckFormat();
    virtual Package* Report() const;
    /**
     * ctor for derived classes
     * @param tiers
     */
    MultiTrackAna(ECutUpdate tiers);

  public:
    MultiTrackAna();
    /**
     * copy ctor
     * @param ana
     */
    MultiTrackAna(const MultiTrackAna& ana);
    MultiTrackAna& operator=(const MultiTrackAna& other);
    /**
     * set size of buffer used in mixing
     * @param to_mix size of buffer
     */
    void SetMixSize(Int_t to_mix);
    /**
     *  set format buffered
     * @param format
     */
    void SetFormatBuffered(Event* format) { SetFormat(format, EFormatDepth::kBuffered); };
    virtual void FinishTask();
    virtual void SetOption(Option_t* option);
    virtual ~MultiTrackAna();
    ClassDef(MultiTrackAna, 1)
  };
}  // namespace Hal

#endif /* HALMULTITRACKANA_H_ */
