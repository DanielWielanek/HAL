/*
 * DataFormatManager.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALDATAFORMATMANAGER_H_
#define HALDATAFORMATMANAGER_H_

#include <TObject.h>

#include "HalStd.h"

/**
 * manager of formats
 */
namespace Hal {
  class EventAna;
  class Event;
  class Track;

  class DataFormatManager : public TObject {
    friend class EventAna;
    Int_t fRegisteredFormats;
    Event** fDataFormatBuffered;     //[fRegisteredFormats]
    Event** fDataFormatNonBuffered;  //[fRegisteredFormats]
    TObjArray* fAutoSupportedFormats;
    static DataFormatManager* fgInstance;

  public:
    /**
     * default constructor
     */
    DataFormatManager();
    /**
     * regiter new format
     * @return id of newly registered format
     */
    Int_t RegisterFormat();
    /**
     * add new kind of supported format to automatical formats
     * @param event instabce of class that is used for new format
     */
    void AddAutoSupportedFormat(Event* event);
    /**
     * remove all stored formats and events
     */
    void Reset();
    /**
     * try to find anty kind of supported events in tree
     * @return pointer to supported format
     */
    Event* FindAutoSupportedFormat();
    /**
     * force overwrite format at given position
     * @param format event that represent new format
     * @param task_id id of overwritten format
     * @param silent - silent overwriting (don't display warning
     */
    void SetFormat(Event* format, Int_t task_id, EFormatDepth depth = EFormatDepth::kAll, Bool_t silent = kFALSE);
    /**
     *
     * @param task_id id of used format
     * @return event that point to event at current entry in tree in proper format
     */
    Event* GetEventFromTree(Int_t task_id);
    /**
     * return allocated representatnio of Event used with given format
     * @param task_id format/task id
     * @return newly allocated event
     */
    const Event* GetFormat(Int_t task_id, EFormatDepth format_depth = EFormatDepth::kAll) const;
    /**
     *
     * @param task_id format id
     * @return event optimized for given format of data
     */
    Event* GetNewEvent(Int_t task_id, EFormatDepth format_depth = EFormatDepth::kAll) const;
    /**
     * create pointer to track with proper format
     * @param task_id format id
     * @return pointer to track in proper format
     */
    Track* GetNewTrack(Int_t task_id, EFormatDepth format_depth = EFormatDepth::kAll) const;
    /**
     *
     * @param task_id format id
     * @return name of format with given id
     */
    TString GetFormatName(Int_t task_id, EFormatDepth format_depth = EFormatDepth::kAll) const;
    /**
     *
     * @param task_id format id
     * @return true if format has been registered and set
     */
    Bool_t FormatExist(Int_t task_id, EFormatDepth format_depth = EFormatDepth::kAll) const;
    /**
     *
     * @return instance of this
     */
    static DataFormatManager* Instance();
    virtual ~DataFormatManager();
    ClassDef(DataFormatManager, 1)
  };
}  // namespace Hal

#endif /* HALDATAFORMATMANAGER_H_ */
