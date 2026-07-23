/*
 * CutMonitorComplex.h
 *
 *  Created on: 1 sie 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUTMONITORCOMPLEX_H_
#define HAL_CUTS_CUTMONITORS_CUTMONITORCOMPLEX_H_

#include "CutMonitor.h"
#include "TwoTrack.h"

namespace Hal {

  class MonitorComplexPair : public TwoTrack {
    TwoTrack* fOrigin = {nullptr};

  public:
    MonitorComplexPair();
    void BuildReal(TwoTrack* pair);
    void BuildImag(TwoTrack* pair);
    virtual ~MonitorComplexPair();
    ClassDef(MonitorComplexPair, 1)
  };

  class ComplexMonitor : public CutMonitor {
  protected:
    CutMonitor* fMonitor = {nullptr};

    virtual void MakeComplexAxes(const CutOptions& opt);

  public:
    ComplexMonitor();
    ComplexMonitor(const ComplexMonitor& other);
    ComplexMonitor(const CutMonitor* other);
    ComplexMonitor& operator=(const ComplexMonitor& other);
    virtual Bool_t ObjMonitor() const;
    virtual Hal::Package* Report() const;
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual Hal::CutMonitor* MakeCopy(const CutOptions& opt) const = 0;
    virtual Bool_t AreSimilar(const Hal::CutMonitor& other) const;
    virtual ~ComplexMonitor();
    ClassDef(ComplexMonitor, 1)
  };


  class EventCutMonitorImaginary : public ComplexMonitor {

  public:
    EventCutMonitorImaginary() { fUpdateRatio = ECutUpdate::kEvent; };
    explicit EventCutMonitorImaginary(CutMonitor* other) : ComplexMonitor(other) {};
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Hal::CutMonitor* MakeCopy(const CutOptions& opt) const;
    virtual ~EventCutMonitorImaginary() {};
    ClassDef(EventCutMonitorImaginary, 1)
  };

  class TrackCutMonitorImaginary : public ComplexMonitor {

  public:
    TrackCutMonitorImaginary() { fUpdateRatio = ECutUpdate::kTrack; };
    explicit TrackCutMonitorImaginary(CutMonitor* other) : ComplexMonitor(other) {};
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Hal::CutMonitor* MakeCopy(const CutOptions& opt) const;
    virtual ~TrackCutMonitorImaginary() {};
    ClassDef(TrackCutMonitorImaginary, 1)
  };


  class TwoTrackCutMonitorImaginary : public ComplexMonitor {
    MonitorComplexPair fPair;

  public:
    TwoTrackCutMonitorImaginary() { fUpdateRatio = ECutUpdate::kTwoTrack; };
    explicit TwoTrackCutMonitorImaginary(CutMonitor* other) : ComplexMonitor(other) {};
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Hal::CutMonitor* MakeCopy(const CutOptions& opt) const;
    virtual ~TwoTrackCutMonitorImaginary() {};
    ClassDef(TwoTrackCutMonitorImaginary, 1)
  };


  class EventCutMonitorReal : public ComplexMonitor {

  public:
    EventCutMonitorReal() { fUpdateRatio = ECutUpdate::kEvent; };
    explicit EventCutMonitorReal(CutMonitor* other) : ComplexMonitor(other) {};
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Hal::CutMonitor* MakeCopy(const CutOptions& opt) const;
    virtual ~EventCutMonitorReal() {};
    ClassDef(EventCutMonitorReal, 1)
  };

  class TrackCutMonitorReal : public ComplexMonitor {

  public:
    TrackCutMonitorReal() { fUpdateRatio = ECutUpdate::kTrack; };
    explicit TrackCutMonitorReal(CutMonitor* other) : ComplexMonitor(other) {};
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Hal::CutMonitor* MakeCopy(const CutOptions& opt) const;
    virtual ~TrackCutMonitorReal() {};
    ClassDef(TrackCutMonitorReal, 1)
  };


  class TwoTrackCutMonitorReal : public ComplexMonitor {
    MonitorComplexPair fPair;

  public:
    TwoTrackCutMonitorReal() { fUpdateRatio = ECutUpdate::kTwoTrack; };
    explicit TwoTrackCutMonitorReal(CutMonitor* other) : ComplexMonitor(other) {};
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Hal::CutMonitor* MakeCopy(const CutOptions& opt) const;
    virtual ~TwoTrackCutMonitorReal() {};
    ClassDef(TwoTrackCutMonitorReal, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUTMONITORCOMPLEX_H_ */
