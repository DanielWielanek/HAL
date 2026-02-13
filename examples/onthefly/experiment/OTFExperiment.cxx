/*
 * OTFExperiment.cxx
 *
 *  Created on: 11 lut 2026
 *      Author: daniel
 */

#include "OTFExperiment.h"

#include <Rtypes.h>

#include "ComplexEvent.h"
#include "Cout.h"
#include "DataManager.h"

namespace HalOTF {

  Experiment::Experiment() {}

  Experiment::~Experiment() {
    for (auto i : fSubTasks) {
      delete i;
    }
  }

  Hal::Task::EInitFlag Experiment::Init() {
    auto datamanager = Hal::DataManager::Instance();
    fCurrentEvent    = dynamic_cast<Hal::ComplexEvent*>(datamanager->GetObject("HalEvent."));
    if (!fCurrentEvent) return Hal::Task::EInitFlag::kERROR;
    for (auto i : fSubTasks) {
      if (i->Init() == false) return Hal::Task::EInitFlag::kERROR;
    }
    Int_t maxId = 0;
    for (auto i : fSubTasks) {
      auto vec = i->GetGeneratorsId();
      if (vec.size() > 0) maxId = TMath::Max(maxId, *std::max_element(vec.begin(), vec.end()));
    }
    Hal::Cout::PrintInfo("=== OFT experiment initialization ", Hal::EInfo::kInfo);
    std::vector<TString> legendLine;
    legendLine.push_back("TaskNo");
    legendLine.push_back("ClassName");
    for (int i = 0; i < maxId; i++) {
      legendLine.push_back(Form("%i", i));
    }
    Hal::Cout::Database(legendLine);
    int count = 0;
    for (auto i : fSubTasks) {
      auto vec = i->GetGeneratorsId();
      std::vector<TString> line;
      line.push_back(Form("%i", count++));
      line.push_back(i->ClassName());
      for (int a = 0; a < maxId; a++) {
        Bool_t active = false;
        for (auto el : vec) {
          if (el == a) active = true;
          break;
        }
        if (active) {
          line.push_back("Active");
        } else {
          line.push_back("Passive");
        }
      }
      Hal::Cout::Database(line);
    }
    return Hal::Task::EInitFlag::kSUCCESS;
  }


  void Experiment::Exec(Option_t* option) {
    for (auto i : fSubTasks) {
      i->Exec(fCurrentEvent);
    }
  }

  void Experiment::FinishTask() {}

}  // namespace HalOTF
