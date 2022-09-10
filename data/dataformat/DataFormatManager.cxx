/*
 * DataFormatManager.cxx
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "DataFormatManager.h"

#include "Cout.h"
#include "DataManager.h"
#include "Event.h"

#include <TList.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

namespace Hal {
  DataFormatManager* DataFormatManager::fgInstance = NULL;

  DataFormatManager::DataFormatManager() : fRegisteredFormats(0), fDataFormatBuffered(nullptr), fDataFormatNonBuffered(nullptr) {
    if (fgInstance) {
      Cout::PrintInfo("Singleton of DataFormatManager already exist, don't use constructor of DataFormatManager",
                      EInfo::kLowWarning);
      return;
    } else {
      fgInstance = this;
    }
  }

  Event* DataFormatManager::GetNewEvent(Int_t task_id, EFormatDepth format_depth) const {
    if (format_depth == EFormatDepth::kBuffered) {
      return fDataFormatBuffered[task_id]->GetNewEvent();
    } else {
      return fDataFormatNonBuffered[task_id]->GetNewEvent();
    }
  }

  Track* DataFormatManager::GetNewTrack(Int_t task_id, EFormatDepth format_depth) const {
    if (format_depth == EFormatDepth::kBuffered) { return fDataFormatBuffered[task_id]->GetNewTrack(); }
    return fDataFormatNonBuffered[task_id]->GetNewTrack();
  }

  void DataFormatManager::SetFormat(Event* format, Int_t task_id, EFormatDepth depth, Bool_t silent) {
    if (silent) {
      if (depth == EFormatDepth::kBuffered) {
        fDataFormatBuffered[task_id] = format;
      } else if (depth == EFormatDepth::kAll) {
        fDataFormatBuffered[task_id]    = format;
        fDataFormatNonBuffered[task_id] = format;
      } else {
        fDataFormatNonBuffered[task_id] = format;
      }
      return;
    }
    if (task_id > fRegisteredFormats) { Cout::PrintInfo("Task id too big", EInfo::kCriticalError); }
    switch (depth) {
      case EFormatDepth::kBuffered: {
        if (fDataFormatBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLowWarning); }
        fDataFormatBuffered[task_id] = format;
      } break;
      case EFormatDepth::kNonBuffered: {
        if (fDataFormatNonBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLowWarning); }
        fDataFormatNonBuffered[task_id] = format;
      } break;
      case EFormatDepth::kAll: {
        if (fDataFormatBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLowWarning); }
        if (fDataFormatNonBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLowWarning); }
        fDataFormatBuffered[task_id]    = format;
        fDataFormatNonBuffered[task_id] = format;
      } break;
    }
  }

  DataFormatManager* DataFormatManager::Instance() {
    if (fgInstance == NULL) { fgInstance = new DataFormatManager(); }
    return fgInstance;
  }

  TString DataFormatManager::GetFormatName(Int_t task_id, EFormatDepth format_depth) const {
    if (format_depth == EFormatDepth::kBuffered) { return fDataFormatBuffered[task_id]->GetFormatName(); }
    return fDataFormatNonBuffered[task_id]->GetFormatName();
  }

  Event* DataFormatManager::FindReaderFormat() {
    DataManager* datamanager = DataManager::Instance();
    Event* event             = nullptr;
    event                    = dynamic_cast<Hal::Event*>(datamanager->GetObject("HalEvent."));
    if (event != nullptr) return event;
    TList* branchList = datamanager->GetBranchNameList();
    for (int i = 0; i < branchList->GetEntries(); i++) {
      TObjString* name = (TObjString*) branchList->At(i);
      TObject* temp    = datamanager->GetObject(name->GetString());
      if (dynamic_cast<Hal::Event*>(temp)) { event = (Hal::Event*) temp; }
    }
    return event;
  }

  void DataFormatManager::Reset() {
    for (int i = 0; i < fRegisteredFormats; i++) {
      if (fDataFormatNonBuffered[i]) delete fDataFormatBuffered[i];
    }
    if (fDataFormatBuffered) delete[] fDataFormatBuffered;
    if (fDataFormatNonBuffered) delete[] fDataFormatNonBuffered;
    fRegisteredFormats = 0;
  }

  DataFormatManager::~DataFormatManager() {
    for (int i = 0; i < fRegisteredFormats; i++) {
      if (fDataFormatNonBuffered[i]) delete fDataFormatBuffered[i];
    }
    if (fDataFormatBuffered) delete[] fDataFormatBuffered;
    if (fDataFormatNonBuffered) delete[] fDataFormatNonBuffered;
  }

  Bool_t DataFormatManager::FormatExist(Int_t task_id, EFormatDepth format_depth) const {
    if (task_id == -1) {
      return kFALSE;
    } else {
      if (format_depth == EFormatDepth::kBuffered) {
        if (fDataFormatBuffered[task_id] == nullptr) return kFALSE;
      } else {
        if (fDataFormatNonBuffered[task_id] == nullptr) return kFALSE;
      }
    }
    return kTRUE;
  }

  Int_t DataFormatManager::RegisterFormat() {
    Event** temp1          = fDataFormatBuffered;
    Event** temp2          = fDataFormatNonBuffered;
    fDataFormatBuffered    = new Event*[fRegisteredFormats + 1];
    fDataFormatNonBuffered = new Event*[fRegisteredFormats + 1];
    for (int i = 0; i < fRegisteredFormats; i++) {
      fDataFormatBuffered[i]    = temp1[i];
      fDataFormatNonBuffered[i] = temp2[i];
    }
    fDataFormatBuffered[fRegisteredFormats]    = NULL;
    fDataFormatNonBuffered[fRegisteredFormats] = NULL;
    delete[] temp1;
    delete[] temp2;
    return fRegisteredFormats++;
  }

  const Event* DataFormatManager::GetFormat(Int_t task_id, EFormatDepth format_depth) const {
    if (task_id < fRegisteredFormats) {
      if (format_depth == EFormatDepth::kBuffered) {
        if (fDataFormatBuffered[task_id] != nullptr) return fDataFormatBuffered[task_id];
      } else if (fDataFormatNonBuffered[task_id] != nullptr)
        return fDataFormatNonBuffered[task_id];
    }
    return nullptr;
  }
}  // namespace Hal
