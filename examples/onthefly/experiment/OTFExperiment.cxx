/*
 * OTFExperiment.cxx
 *
 *  Created on: 11 lut 2026
 *      Author: daniel
 */

#include "OTFExperiment.h"

#include <Rtypes.h>
#include <iostream>

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
    TString links;
    for (int i = 0; i <= maxId; i++)
      links = links + Form("%2d ", i);
    legendLine.push_back(links);
    std::cout << Hal::Cout::GetColor(kWhite);
    Hal::Cout::Database(legendLine, {7, 50}, "R");
    int count = 0;
    for (auto i : fSubTasks) {
      auto vec = i->GetGeneratorsId();
      std::vector<TString> line;
      line.push_back(Form("%i", count++));
      line.push_back(i->ClassName());
      TString innerlinks;
      for (int a = 0; a <= maxId; a++) {
        Bool_t active = false;
        for (auto el : vec) {
          if (el == a) {
            active = true;
            break;
          }
        }
        if (active) {
          innerlinks = innerlinks + " + ";
        } else {
          innerlinks = innerlinks + " - ";
        }
      }
      line.push_back(innerlinks);
      Hal::Cout::Database(line, {7, 50}, "R");
    }
    std::cout << Hal::Cout::GetDisableColor();
    return Hal::Task::EInitFlag::kSUCCESS;
  }


  void Experiment::Exec(Option_t* option) {
    for (auto i : fSubTasks) {
      i->Exec(fCurrentEvent);
    }
  }

  void Experiment::FinishTask() {}

}  // namespace HalOTF
