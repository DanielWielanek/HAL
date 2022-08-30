/*
 * EventRunCut.cxx
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventRunCut.h"

#include "Cout.h"
#include "Cut.h"
#include "ExpEvent.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "XMLNode.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <RtypesCore.h>
#include <TString.h>

namespace Hal {
  EventRunCut::EventRunCut() : fMode(EMode::kBad) { SetUnitName("RunId [AU]"); }

  Bool_t EventRunCut::Init(Int_t format_id) {
    std::vector<int> runs;
    if (fListFile.Length() == 0 && fRuns.size() == 0) {
      Cout::PrintInfo(Form("Lack of run list in %s", this->ClassName()), EInfo::kWarning);
      return kFALSE;
    }

    if (fListFile.EndsWith(".xml")) {
      XMLFile xml(fListFile);
      XMLNode* root = xml.GetRootNode();
      if (fLabel.Length() > 0) {
        root = root->GetChild(fLabel);
        if (root == nullptr) {
          Cout::PrintInfo(Form("Lack of runs %s in file %s", fLabel.Data(), fListFile.Data()), EInfo::kWarning);
          return kFALSE;
        }
      }
      TString name = "bad_runs";
      if (fMode == EMode::kGood) { name = "good_runs"; }

      XMLNode* xmlRuns = root->GetChild(name);
      if (xmlRuns == nullptr) {
        Cout::PrintInfo(Form("Lack of run list in file %s", fListFile.Data()), EInfo::kWarning);
        return kFALSE;
      }
      for (int i = 0; i < xmlRuns->GetNChildren(); i++) {
        TString val = xmlRuns->GetChild(i)->GetValue();
        runs.push_back(val.Atoi());
      }

    } else {
      /* flat list */
      std::ifstream file;
      file.open(fListFile);
      int temp;
      while (!file.eof()) {
        file >> temp;
        runs.push_back(temp);
      }
      file.close();
    }
    for (auto i : runs) {
      fRuns.push_back(i);
    }
    return EventExpCut::Init(format_id);
  }

  Bool_t EventRunCut::Pass(Event* event) {
    Int_t runId = ((ExpEvent*) event)->GetRunId();
    switch (fMode) {
      case EMode::kGood: {
        for (auto run : fRuns) {
          if (run == runId) return ForcedUpdate(kTRUE);
        }
        return ForcedUpdate(kFALSE);
      } break;
      case EMode::kBad: {
        for (auto run : fRuns) {
          if (run == runId) return ForcedUpdate(kFALSE);
        }
        return ForcedUpdate(kTRUE);
      } break;
    }
    return ForcedUpdate(kFALSE);
  }

  void EventRunCut::Print(Option_t* /*opt*/) const {
    std::cout << ClassName() << std::endl;
    if (fMode == EMode::kBad) {
      std::cout << "MODE: BAD" << std::endl;
    } else {
      std::cout << "MODE: GOOD" << std::endl;
    }
    std::cout << "RUN ID LIST" << std::endl;
    int count = 0;
    for (auto i : fRuns) {
      std::cout << ++count << "\t" << i << std::endl;
    }
  }
  void EventRunCut::SetRunList(std::initializer_list<int> list) {
    for (auto i : list) {
      fRuns.push_back(i);
    }
  }

  Package* EventRunCut::Report() const {
    Package* pack = EventExpCut::Report();
    switch (fMode) {
      case EMode::kBad: {
        pack->AddObject(new ParameterString("Mode", "Reject bad runs", '='));
      } break;
      case EMode::kGood: {
        pack->AddObject(new ParameterString("Mode", "Accept good runs", '='));
      } break;
    }
    TList* triggers = new TList();
    for (auto i : fRuns) {
      pack->AddObject(new TObjString(Form("%i", i)));
    }
    pack->AddObject(triggers);
    return pack;
  }
}  // namespace Hal
