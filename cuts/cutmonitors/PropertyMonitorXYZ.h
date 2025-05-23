/*
 * PropertyMonitorXYZ.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPROPERTYMONITORXYZ_H_
#define HALPROPERTYMONITORXYZ_H_

#include "CutMonitorXYZ.h"

/**
 * class to monitor data properties (in contrast to cut monitor doesnt require
 * cuts
 */

namespace Hal {

  class PropertyMonitorXYZ : public CutMonitorXYZ {

  protected:
    TString fXaxisName;
    TString fYaxisName;
    TString fZaxisName;
    EFormatType fFormatType;
    virtual void CreateHistograms();

  public:
    /**
     *
     * @param xLabel x-axis name
     * @param yLabel y-axis name
     * @param zLabel z-axis name
     * @param update update ratio
     */
    PropertyMonitorXYZ(TString xLabel = "", TString yLabel = "", TString zLabel = "", ECutUpdate update = ECutUpdate::kEvent);
    PropertyMonitorXYZ(const PropertyMonitorXYZ& other);
    /**
     * Assignment operator
     * @param other
     * @return
     */
    PropertyMonitorXYZ& operator=(const PropertyMonitorXYZ& other);
    virtual Bool_t Init(Int_t task_id);
    virtual Bool_t ObjMonitor() const { return kTRUE; };
    virtual Bool_t AreSimilar(const CutMonitor& other) const;
    virtual Package* Report() const;
    virtual CutMonitor* MakeCopy() const { return (CutMonitor*) this->Clone(); };
    virtual ~PropertyMonitorXYZ();
    ClassDef(PropertyMonitorXYZ, 1)
  };

  /**
   * class for monitoring properties of events by using field ID
   */
  class EventFieldMonitorXYZ : public PropertyMonitorXYZ {
    const Int_t fFieldIDX;
    const Int_t fFieldIDY;
    const Int_t fFieldIDZ;

  public:
    /**
     *
     * @param fieldIDX field to monitor on X-axis
     * @param fieldIDY field to monitor on Y-axis
     * @param fieldIDZ field to monitor on Z-axis
     * @param upd
     */
    EventFieldMonitorXYZ(Int_t fieldIDX = 0, Int_t fieldDY = 0, Int_t fieldDZ = 0);
    EventFieldMonitorXYZ(Int_t fieldIDX,
                         std::initializer_list<Double_t> xAxis,
                         Int_t fieldDY,
                         std::initializer_list<Double_t> yAxis,
                         Int_t fieldDZ,
                         std::initializer_list<Double_t> zAxis);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t AreSimilar(const CutMonitor& other) const;
    virtual Bool_t Init(Int_t task_id);
    virtual CutMonitor* MakeCopy() const { return new EventFieldMonitorXYZ(*this); }
    virtual ~EventFieldMonitorXYZ() {};
    ClassDef(EventFieldMonitorXYZ, 1)
  };


  /**
   * class for monitoring properties of tracks by using field ID
   */
  class TrackFieldMonitorXYZ : public PropertyMonitorXYZ {
    const Int_t fFieldIDX;
    const Int_t fFieldIDY;
    const Int_t fFieldIDZ;

  public:
    /**
     *
     * @param fieldIDX field to monitor on X-axis
     * @param fieldIDY field to monitor on Y-axis
     * @param fieldIDZ field to monitor on Z-axis
     * @param upd
     */
    TrackFieldMonitorXYZ(Int_t fieldIDX = 0, Int_t fieldDY = 0, Int_t fieldDZ = 0);
    TrackFieldMonitorXYZ(Int_t fieldIDX,
                         std::initializer_list<Double_t> xAxis,
                         Int_t fieldDY,
                         std::initializer_list<Double_t> yAxis,
                         Int_t fieldDZ,
                         std::initializer_list<Double_t> zAxis);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t AreSimilar(const CutMonitor& other) const;
    virtual Bool_t Init(Int_t task_id);
    virtual CutMonitor* MakeCopy() const { return new TrackFieldMonitorXYZ(*this); }
    virtual ~TrackFieldMonitorXYZ() {};
    ClassDef(TrackFieldMonitorXYZ, 1)
  };
}  // namespace Hal
#endif /* HALPROPERTYMONITORXYZ_H_ */
