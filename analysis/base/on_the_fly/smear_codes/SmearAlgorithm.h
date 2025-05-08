/*
 * SmothAlgorithm.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSMOTHALGORITHM_H_
#define HALSMOTHALGORITHM_H_

#include "Task.h"
#include <TObject.h>

namespace Hal {
  class Package;
  class ComplexEvent;
  class ComplexTrack;

  /**
   * abstract class for smearing algorithms
   */

  class SmearAlgorithm : public TObject {
  public:
    SmearAlgorithm() {};
    /**
     * initalize this class
     * @return status of initialization
     */
    virtual Task::EInitFlag Init() { return Task::EInitFlag::kSUCCESS; };
    /**
     *
     * @return package with information about this object
     */
    virtual Package* Report() const;
    /**
     * make copy of this object
     * @return
     */
    virtual SmearAlgorithm* MakeCopy() const = 0;
    virtual ~SmearAlgorithm() {};
    ClassDef(SmearAlgorithm, 1)
  };

  /**
   * abstract class for modifying events - later such events can be used as
   * "reconstructed" if user use smeared format, currently only event reaction
   * plane can be replaced and track momenta
   */
  class EventSmear : public SmearAlgorithm {
  public:
    EventSmear();
    /**
     * modify event parameters
     * @param mod event to modify
     */
    virtual void ModifyEvent(ComplexEvent* mod) = 0;
    virtual Package* Report() const;
    virtual ~EventSmear();
    ClassDef(EventSmear, 1)
  };

  /**
   * class that don't modify event
   */
  class EventSmearVirtual : public EventSmear {
  public:
    EventSmearVirtual();
    virtual void ModifyEvent(ComplexEvent* mod);
    virtual SmearAlgorithm* MakeCopy() const;
    virtual Package* Report() const;
    virtual ~EventSmearVirtual();
    ClassDef(EventSmearVirtual, 1)
  };

  /**
   * abstract class for modifying tracks (smearing) @see EventSmear, currently
   * only track momenta can be smeared
   */
  class TrackSmear : public SmearAlgorithm {
  public:
    TrackSmear();
    /**
     * modify track parameters
     * @param mod smeared track
     */
    virtual void ModifyTrack(ComplexTrack* mod) = 0;
    virtual ~TrackSmear();
    ClassDef(TrackSmear, 1)
  };

  /**
   * class that don't modify track
   */
  class TrackSmearVirtual : public TrackSmear {
  public:
    TrackSmearVirtual();
    virtual void ModifyTrack(ComplexTrack* mod);
    virtual SmearAlgorithm* MakeCopy() const;
    virtual ~TrackSmearVirtual();
    ClassDef(TrackSmearVirtual, 1)
  };
}  // namespace Hal
#endif /* HALSMOTHALGORITHM_H_ */
