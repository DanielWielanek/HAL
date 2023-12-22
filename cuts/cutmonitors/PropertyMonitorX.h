/*
 * PropertyMonitorX.h
 *
 *  Created on: 21 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPROPERTYMONITORX_H_
#define HALPROPERTYMONITORX_H_

#include "CutMonitorX.h"

/**
 * class to monitor data properties (in contrast to cut monitor doesnt require
 * cuts
 */
namespace Hal {
  class PropertyMonitorX : public CutMonitorX {
  protected:
    TString fXaxisName;
    TString fYaxisName;
    EFormatType fFormatType;
    virtual void CreateHistograms();

  public:
    /**
     *
     * @param xLabel name of x axis
     * @param yLabel name of y axis
     * @param ratio update ratio
     */
    PropertyMonitorX(TString xLabel = "", TString yLabel = "", ECutUpdate ratio = ECutUpdate::kEvent);
    PropertyMonitorX(const PropertyMonitorX& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    PropertyMonitorX& operator=(const PropertyMonitorX& other);
    virtual Bool_t Init(Int_t task_id);
    virtual Bool_t ObjMonitor() const { return kTRUE; };
    virtual CutMonitor* MakeCopy() const { return new PropertyMonitorX(*this); };
    virtual Package* Report() const;
    virtual ~PropertyMonitorX();
    ClassDef(PropertyMonitorX, 1)
  };

  /**
   * class for monitoring properties of events by using field ID
   */

  class EventFieldMonitorX : public PropertyMonitorX {
    const Int_t fFieldID;

  public:
    /**
     *
     * @param fieldID filed to monitor
     * @param upd update ration
     */
    EventFieldMonitorX(Int_t fieldID = 0);
    /**
     *
     * @param fieldID
     * @param list axis conf
     */
    EventFieldMonitorX(Int_t fieldID, std::initializer_list<Double_t> xAxis);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual CutMonitor* MakeCopy() const { return new EventFieldMonitorX(*this); }
    virtual ~EventFieldMonitorX() {};
    ClassDef(EventFieldMonitorX, 1)
  };

  /**
   * class for monitoring properties of tracks by using field ID
   */

  class TrackFieldMonitorX : public PropertyMonitorX {
    const Int_t fFieldID;

  public:
    /**
     *
     * @param fieldID filed to monitor
     * @param upd update ration
     */
    TrackFieldMonitorX(Int_t fieldID = 0);
    /**
     *
     * @param fieldID
     * @param list axis conf
     */
    TrackFieldMonitorX(Int_t fieldID, std::initializer_list<Double_t> xAxis);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual CutMonitor* MakeCopy() const { return new TrackFieldMonitorX(*this); }
    virtual ~TrackFieldMonitorX() {};
    ClassDef(TrackFieldMonitorX, 1)
  };
}  // namespace Hal
#endif /* HALPROPERTYMONITORX_H_ */
