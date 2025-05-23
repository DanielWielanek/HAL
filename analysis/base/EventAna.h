/*
 * EventAna.h
 *
 *  Created on: 05-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALEVENTANA_H_
#define HALEVENTANA_H_

#include "Cut.h"
#include "CutMonitor.h"
#include "Std.h"
#include "Task.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>


class TDatabasePDG;

namespace Hal {
  class Event;
  class CutsAndMonitors;
  class DataFormatManager;
  class MemoryMapManager;

  // s#define _HAL_CLEAR_BUFFER_ // realy need to clear bufferes?
  class EventAnaChain;
  /**
   * Base class for physical analysis
   */
  class EventAna : public Task {
    friend EventAnaChain;

  protected:
    /**
     * map of format options in bit
     * BIT(0) - disable /enable compression
     * BIT(1) - disable/enable source
     * BIT(2) - direct access
     * BIT(3) - disable/enable reader
     * BIT(4) - disable/enable format checking
     */
    Int_t fFormatOption;
    enum eBitFormat { kCompression = 0, kSource = 1, kDirectAcesss = 2, kReader = 3, kChecking = 4 };
    /**
     * number of processed events
     */
    UInt_t fProcessedEvents;
    /**
     * size of buffer for mixing (not used by event and track analysis
     */
    Int_t fMixSize;
    /**
     * total number of event collections
     */
    Int_t fEventCollectionsNo;
    /**
     * point to current event collection number
     */
    Int_t fCurrentEventCollectionID;
    /**
     * number of tiers in CutContainer, used during creating first cut, if
     * number is too small some cut's will not be added and code my crash
     */
    const ECutUpdate fTiers;
    /**
     * pointer to instance of  TDatabasePDG
     */
    TDatabasePDG* fPDG;
    /**
     * hold information about all cuts and cut monitors
     */
    CutContainer* fCutContainer;
    /**
     * hold all events and tracks
     */
    MemoryMapManager* fMemoryMap;
    /**
     * currently processed event
     */
    Event* fCurrentEvent;
    /**
     * comment about this analysis
     */
    TString fComment;
    /**
     * true if object has been initialized
     */
    Int_t fInit;
    /**
     *
     * @return name of input file
     */
    TString GetInputFileName() const { return fInFileName; };
    /**
     * set input file name
     * @param name
     */
    void SetInputFileName(TString name) { fInFileName = name; };
    /**
     * initialize memory map manager
     */
    virtual void InitMemoryMap();


  private:
    /**
     * true if this task is in chain
     */
    Bool_t fInChain;
    std::vector<TString> fTagList;
    Int_t fTaskID;
    TString fInFileName;
    DataFormatManager* fDataFormatManager;  //->
#ifdef _HAL_CLEAR_BUFFER_
    Bool_t fIsLastTask;
#endif

  protected:
    /**
     *
     * @return true if taks in chain
     */
    Bool_t IsInChain() const { return fInChain; };
    /**
     * mark taks as "in chain"
     */
    void MarkAsInChain() { fInChain = kTRUE; };
    /**
     * add object to existing metadata list
     * @param main_pack pointer to package created by Report method
     * @param obj object to add
     */
    void AddToAnaMetadata(Package* main_pack, TObject* obj) const;
    /**
     * initialize cut container
     * @return return kFATAL if something is wrong with cut containers
     */
    Task::EInitFlag InitCutContainer();
    /**
     * check if format set in analysis is ok
     * @return
     */
    virtual Task::EInitFlag CheckFormat();
    /**
     *
     * @return ID of task (and usually and format) used in this task (each task have
     * own ID even if use the same format
     */
    inline Int_t GetTaskID() const { return fTaskID; };
    /**
     * link collections
     */
    virtual void LinkCollections();
    /**
     * process event, can be be overwritten in derived classes
     */
    virtual void ProcessEvent() {};
    /**
     * init new cut container if current is NULL (may happen if user don't add any
     * cut,  should be overwritten in derived classes
     */
    virtual void InitNewCutContainer();
    /**
     * checking cut containers.  should be overwritten in derived classes
     */
    virtual void CheckCutContainerCollections();
    /**
     * create report that will be saved, derived classes should call this method
     * for base class and add own objects
     * @return report from analysis
     */
    virtual Package* Report() const;
#ifdef _HAL_CLEAR_BUFFER_
    /**
     * force clean memory after last analysis
     */
    void ClearMemory();
#endif
    /**
     * initialize task,  in principle all derived classes should usually call this method, this method do following steps:
     * - CheckFormat() - check if format is set/good
     * -# SetInputFile() - set name of the input file
     * -# SetFormatOption(EFormatOption::kReaderAccess) - set reader format if no format is set
     * - InitCutContainer() - initialize/check cut container
     * -# InitNewCutContainer() - init new container if no container is set
     * -# CheckCutContainerCollections()
     * -# LinkCollections()
     * -# fCutContainer->Init(GetTaskID())
     * - InitMemoryMap() - initialize map to store events
     * -#SetMixSize() - sets default mix size
     * -#fMemoryMap->Init(GetTaskID(), TESTBIT(fFormatOption, eBitFormat::kCompression), brName);
     * - AddTags() add tags
     *
     * @return status of initialization
     */
    virtual Task::EInitFlag Init();
    /**
     * constructor for derived classes
     * @param tiers
     */
    EventAna(ECutUpdate tiers);

  public:
    /**
     * flags for setting format options NOTE: some of these flags might disable other
     */
    enum class EFormatOption {
      kCompress,
      /*!< compress data (when buffer) */
      kNoCompress,
      /*!< do not compress data (default) */
      kKeepSource,
      /*!< keep source format in buffer */
      kNoKeepSource,
      /*!< do not keep source in buffer (default) */
      kDirectAccess,
      /*!< read direct data (look for branch ClassName.) */
      kNoDirectAccess,
      /*!< do not use direct data */
      kReaderAccess,
      /*!< use reader acces (look for branch HalEvent.) */
      kNoReaderAccess,
      /*!< */
      kDisableChecking,
      /*!< dusable format checking*/
      kStandardAccess /**!< standard acces */
    };
    /**
     * basic constructor
     */
    EventAna() : EventAna(ECutUpdate::kEvent) { SetFormatOption(EFormatOption::kReaderAccess); };
    /**
     * copy constructor
     * @param ana
     */
    EventAna(const EventAna& ana);
    /**
     * assignement operator
     * @param other
     * @return
     */
    EventAna& operator=(const EventAna& other);
    /**
     * add tags
     * @param tag tags seperated by space
     */
    virtual void AddTags(TString tag);
    /**
     * add cut object
     * @param cut
     * @param opt  can be "fast", with multiplying factor "{AxB}" (where A is
     * number of added cuts, and B is jump in collection value), like "{A}" (then
     * cut is added with collection number A), also mixed versions like
     * "fast+{AxB}" and "fast+{A}" can be used. For two track analysis option
     * "sig" add cut only for signal pairs and "bckg" only for background pairs.
     * It's also possible to add in this way pure real or pure imaginary cuts. For
     * example by adding TrackXCut with "im" option
     * TrackComplexCut(TrackVirtualCut, TrackXCut) is added.
     */
    virtual void AddCut(const Cut& cut, Option_t* opt = "");
    /**
     * add cut monitor object
     * @param mon
     * @param opt option of adding cut monitor. Following options are allowed:
     * "bckg" to add only to background, "sig" to add only in singal (both flags
     * are valid only for analysis where signal and background is present). Other
     * options are like "{AxB}" where A is number of cut monitors, and B is jumb
     * in collection number, eg. {2x3} mean adding cut with collection N, N+3.
     * User might also set option like "{A}" where A is collection number.
     * Combination of those options are allowed. Its's also possible to monitor
     * pure "real"  cuts or pure imaginary cuts but setting option "+re" or "+im"
     * respectively
     *
     */
    virtual void AddCutMonitor(const CutMonitor& mon, Option_t* opt = "");
    /**
     * add collection of predefined cuts and cut monitors
     * @param monCuts
     */
    virtual void AddCutsAndMonitors(const CutsAndMonitors& monCuts);
    /**
     * called for each event
     * @param opt not used
     */
    virtual void Exec(Option_t* opt);
    /**
     * for setting options, not used in EventAna
     * @param opt
     */
    virtual void SetOption(Option_t* opt);
    /**
     * set format option
     *  - kCompress - compress buffered events
     *  - kDirectAcces - read preprocessed event from tree (that inherits from
     * Event)
     *  - kKeepSource - keep original structure in memory
     * @param option
     */
    void SetFormatOption(EFormatOption option);
    /**
     * add owno comment
     * @param comment comment
     */
    virtual void SetComment(TString comment);
    /**
     * set format
     * @param format any class that inherit from NIcaEventAna and ican be read
     * from currently processed file
     * @param depth - format type - use buffered to setup different format for
     * buffered events
     */
    virtual void SetFormat(Event* format, EFormatDepth depth = EFormatDepth::kAll);
    /**
     * called at the end of analysis save resutls
     */
    virtual void FinishTask();
    /**
     * default dtor
     */
    virtual ~EventAna();
    ClassDef(EventAna, 1)
  };
}  // namespace Hal
#endif /* HALEVENTANA_H_ */
