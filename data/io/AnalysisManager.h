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

  class AnalysisManager : public TObject {
    UInt_t fProcessedEvents;
    MagField* fField;
    Source* fSource;
    TString fOutputFile;
    TString fOutTreeName;
    IOManager* fManager;
    std::vector<Task*> fTasks;
    std::vector<Task*> fActiveTasks;
    std::vector<Task*> fPassiveTasks;
    void Finish();

  public:
    AnalysisManager();
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
