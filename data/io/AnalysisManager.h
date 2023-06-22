/*
 * RunAna.h
 *
 *  Created on: 4 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_HALRUNANA_H_
#define HAL_ANALYSIS_BASE_HALRUNANA_H_

#include <TObject.h>
#include <TString.h>

/**
 * represents analysis manager for data processing
 */
namespace Hal {
  class IOManager;
  class Source;
  class Task;
  class MagField;
  class Reader;
  class TriggerTask;
  class Package;
  class AnalysisManager : public TObject {
    UInt_t fProcessedEvents = {0};
    Bool_t fTriggersEnabled = {kFALSE};
    MagField* fField        = {nullptr};
    Source* fSource         = {nullptr};
    TString fOutputFile;
    TString fOutTreeName = {"HalTree"};
    IOManager* fManager  = {nullptr};
    std::vector<TriggerTask*> fTriggers;
    std::vector<TriggerTask*> fActiveTriggers;
    std::vector<TriggerTask*> fPassiveTriggers;
    std::vector<Task*> fTasks;
    std::vector<Task*> fActiveTasks;
    std::vector<Task*> fPassiveTasks;
    void Finish();
    void DoStep(Int_t entry);

  public:
    AnalysisManager();
    /**
     * add reader task - it's guaranteed that reader will be set as a first task after the triggers
     * @param reader
     */
    void AddReader(Reader* reader);
    /**
     * add trigger task, it's guaranteed that will be set before other tasks
     * @param trigger
     */
    void AddTrigger(TriggerTask* trigger);
    /**
     * add task to analysis
     * @param ana
     */
    void AddTask(Task* ana) { fTasks.push_back(ana); };
    /**
     * initalize run
     */
    virtual Bool_t Init();
    /**
     * set name of the tree in output file
     * @param name
     */
    void SetOutTreeName(TString name) { fOutTreeName = name; }
    /**
     * process events from start to end -1
     * @param start
     * @param end
     * if both parameters equal -1 then process entire file
     */
    void Run(Int_t start = -1, Int_t end = -1);
    /**
     * set source with data
     * @param source
     */
    void SetSource(Source* source) { fSource = source; };
    /**
     * set name of the output file
     * @param name
     */
    void SetOutput(TString name) { fOutputFile = name; };
    /**
     * set magnetic field
     * @return
     */
    MagField* GetField() const { return fField; }
    /**
     * get magnetic field
     * @param field
     */
    void SetField(MagField* field) { fField = field; }
    virtual ~AnalysisManager();
    ClassDef(AnalysisManager, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_BASE_HALRUNANA_H_ */
