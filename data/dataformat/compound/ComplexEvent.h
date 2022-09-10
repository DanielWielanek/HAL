/*
 * EventCombined.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCOMPLEXEVENT_H_
#define HALCOMPLEXEVENT_H_

#include "Event.h"

#include <TString.h>

#include "Std.h"

/**
 * class for storing two formats, Real event presents current data whereas
 * "imaginary event" is hidden NOTE - compression of source here doesn't work
 * correctly so use uncompressed event or only don't use raw tracks
 */
namespace Hal {
  class ComplexEvent : public Event {
  protected:
    static Int_t* fgIndexMap;       //!
    static Int_t* fgSumMap;         //!
    static Int_t fgIndexArraySize;  //!
    static Int_t fgSumMapSize;      //!
    Event* fRealEvent;
    Event* fImgEvent;
    void CalculateCompressionMapImg(Int_t* map, Int_t map_size, ComplexEvent* event);
    void ShallowCopyTracks(Event* event);
    void ShallowCopyCompressTracks(Event* event, Int_t* map, Int_t* mapID, Int_t map_size);
    void ShallowCopyEvent(Event* event);
    ComplexEvent(TString track_class, Event* real, Event* img);

  public:
    ComplexEvent(Event* real = NULL, Event* img = NULL);
    ComplexEvent(const ComplexEvent& other);
    EventInterface* CreateSource() const;
    virtual void Clear(Option_t* opt = " ");
    virtual void Update(EventInterface* interface);
    virtual Bool_t HasHiddenSettings() const;
    virtual Bool_t ExistInTree() const;
    virtual Bool_t IsCompatible(const Event* non_buffered) const;
    virtual Float_t GetFieldVal(Int_t fieldID) const;
    virtual TString GetFieldName(Int_t fieldID) const;
    virtual EFormatType GetFormatType() const { return EFormatType::kComplexReco; };
    Event* GetImgEvent() const { return fImgEvent; };
    Event* GetRealEvent() const { return fRealEvent; };
    Track* GetNewImgTrack() const { return fImgEvent->GetNewTrack(); };
    Track* GetNewRealTrack() const { return fRealEvent->GetNewTrack(); };
    virtual ~ComplexEvent();
    ClassDef(ComplexEvent, 1)
  };
}  // namespace Hal
#endif /* HALCOMPLEXEVENT_H_ */
