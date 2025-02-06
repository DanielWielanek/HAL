/*
 * PropertyMonitorXY.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPROPERTYMONITORXY_H_
#define HALPROPERTYMONITORXY_H_

/**
 * class to monitor data properties (in contrast to cut monitor doesnt require
 * cuts
 */
#include "CutMonitorXY.h"
namespace Hal {
  class PropertyMonitorXY : public CutMonitorXY {

  protected:
    TString fXaxisName;
    TString fYaxisName;
    EFormatType fFormatType;
    virtual void CreateHistograms();

  public:
    /**
     *
     * @param xLabel x-axis name
     * @param yLabel y-axis name
     * @param update update ratio
     */
    PropertyMonitorXY(TString xLabel = "", TString yLabel = "", ECutUpdate update = ECutUpdate::kEvent);
    PropertyMonitorXY(const PropertyMonitorXY& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    PropertyMonitorXY& operator=(const PropertyMonitorXY& other);
    virtual Bool_t Init(Int_t task_id);
    virtual Bool_t ObjMonitor() const { return kTRUE; };
    virtual Bool_t AreSimilar(CutMonitor* other) const;
    virtual Package* Report() const;
    virtual CutMonitor* MakeCopy() const { return (CutMonitor*) this->Clone(); };
    virtual ~PropertyMonitorXY();
    ClassDef(PropertyMonitorXY, 1)
  };

  /**
   * class for monitoring properties of events by using field ID
   */

  class EventFieldMonitorXY : public PropertyMonitorXY {
    const Int_t fFieldIDX;
    const Int_t fFieldIDY;

  public:
    /**
     *
     * @param fieldIDX field to monitor on X-axis
     * @param fieldIDY field to monitor on Y-axis
     * @param upd update ratio
     */
    EventFieldMonitorXY(Int_t fieldIDX = 0, Int_t fieldIDY = 0);
    EventFieldMonitorXY(Int_t fieldIDX,
                        std::initializer_list<Double_t> xAxis,
                        Int_t fieldIDY,
                        std::initializer_list<Double_t> yAxis);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual Bool_t AreSimilar(CutMonitor* other) const;
    virtual CutMonitor* MakeCopy() const { return new EventFieldMonitorXY(*this); }
    virtual ~EventFieldMonitorXY() {};
    ClassDef(EventFieldMonitorXY, 1)
  };

  /**
   * class for monitoring properties of tracks by using field ID
   */

  class TrackFieldMonitorXY : public PropertyMonitorXY {
    const Int_t fFieldIDX;
    const Int_t fFieldIDY;

  public:
    /**
     *
     * @param fieldIDX field to monitor on X-axis
     * @param fieldIDY field to monitor on Y-axis
     * @param upd update ratio
     */
    TrackFieldMonitorXY(Int_t fieldIDX = 0, Int_t fieldIDY = 0);
    TrackFieldMonitorXY(Int_t fieldIDX,
                        std::initializer_list<Double_t> xAxis,
                        Int_t fieldIDY,
                        std::initializer_list<Double_t> yAxis);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t AreSimilar(CutMonitor* other) const;
    virtual Bool_t Init(Int_t task_id);
    virtual CutMonitor* MakeCopy() const { return new TrackFieldMonitorXY(*this); }
    virtual ~TrackFieldMonitorXY() {};
    ClassDef(TrackFieldMonitorXY, 1)
  };
}  // namespace Hal

#endif /* HALPROPERTYMONITORXY_H_ */
