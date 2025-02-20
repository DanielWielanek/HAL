/*
 * EventMultiplicityCut.h
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALEVENTMULTIPLICITYCUT_H_
#define HALEVENTMULTIPLICITYCUT_H_

#include "EventCut.h"
#include "Track.h"

#include <TDatabasePDG.h>
/**
 * check total number of tracks in event
 */
namespace Hal {
  class EventMultiplicityCut : public EventCut {
  protected:
  public:
    EventMultiplicityCut();
    virtual Bool_t Pass(Event* event);
    virtual ~EventMultiplicityCut();
    ClassDef(EventMultiplicityCut, 1)
  };

  class EventTotalTrackNoCut : public EventCut {
  public:
    EventTotalTrackNoCut();
    virtual Bool_t Pass(Event* event);
    virtual ~EventTotalTrackNoCut() {};
    ClassDef(EventTotalTrackNoCut, 1)
  };

  /**
   * check multiplicity of charged particles in given range for given event
   */
  class EventMultiChargedCut : public EventCut {
    Double_t fScale;
    Double_t fMinEta, fMaxEta, fRange;
    TDatabasePDG* fPDG;

  public:
    EventMultiChargedCut();
    Bool_t Pass(Event* event);
    /**
     * set pseudorpaidity range for pairs calucalted as charged
     * @param min minimum rapidity
     * @param max maximum rapidity
     */
    void SetAveragingRange(Double_t min, Double_t max);
    /**
     * takes into account detector inefficiency, number of charged tracks is
     * multiplied by this value
     * @param scale scale for charged multiplicity value
     */
    void SetEfficiencyScale(Double_t scale);
    virtual Bool_t Init(Int_t task_id = 0);
    virtual ~EventMultiChargedCut();
    virtual Package* Report() const;
    ClassDef(EventMultiChargedCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTMULTIPLICITYCUT_H_ */
