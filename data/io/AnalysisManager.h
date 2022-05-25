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
    void AddTask(Task* ana) { fTasks.push_back(ana); };
    void Init();
    void SetOutTreeName(TString name) { fOutTreeName = name; }
    void Run(Int_t start, Int_t end);
    void SetSource(Source* source) { fSource = source; };
    void SetOutput(TString name) { fOutputFile = name; };
    MagField* GetField() const { return fField; }
    void SetField(MagField* field) { fField = field; }
    virtual ~AnalysisManager();
    ClassDef(AnalysisManager, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_BASE_HALRUNANA_H_ */
