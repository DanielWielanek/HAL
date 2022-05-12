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
#include "Event.h"

#include <TObjArray.h>
#include <TString.h>

namespace Hal {
  DataFormatManager* DataFormatManager::fgInstance = NULL;

  DataFormatManager::DataFormatManager() :
    fRegisteredFormats(0), fDataFormatBuffered(nullptr), fDataFormatNonBuffered(nullptr), fAutoSupportedFormats(nullptr) {
    if (fgInstance) {
      Cout::PrintInfo("Singleton of DataFormatManager already exist, don't use constructor of DataFormatManager",
                      EInfo::kLessWarning);
      return;
    } else {
      fAutoSupportedFormats = new TObjArray();
      fgInstance            = this;
    }
  }

  Event* DataFormatManager::GetEventFromTree(Int_t task_id) {
    if (fDataFormatNonBuffered[task_id]->GetSource() == NULL) fDataFormatNonBuffered[task_id]->CreateSource();
    fDataFormatNonBuffered[task_id]->LinkWithTree();
    return fDataFormatNonBuffered[task_id];
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
    if (task_id > fRegisteredFormats) { Cout::PrintInfo("Task id too big", EInfo::kImportantError); }
    switch (depth) {
      case EFormatDepth::kBuffered: {
        if (fDataFormatBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLessWarning); }
        fDataFormatBuffered[task_id] = format;
      } break;
      case EFormatDepth::kNonBuffered: {
        if (fDataFormatNonBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLessWarning); }
        fDataFormatNonBuffered[task_id] = format;
      } break;
      case EFormatDepth::kAll: {
        if (fDataFormatBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLessWarning); }
        if (fDataFormatNonBuffered[task_id] != NULL) { Cout::PrintInfo("Overwriting format", EInfo::kLessWarning); }
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

  void DataFormatManager::AddAutoSupportedFormat(Event* event) {
    TString new_classname = event->ClassName();
    for (int i = 0; i < fAutoSupportedFormats->GetEntriesFast(); i++) {
      TObject* temp          = fAutoSupportedFormats->UncheckedAt(i);
      TString temp_classname = temp->ClassName();
      if (temp_classname.EqualTo(new_classname)) {
        Cout::PrintInfo(Form("%s already registered in autosupported formats", new_classname.Data()), EInfo::kImportantInfo);
        return;
      }
    }
    fAutoSupportedFormats->AddLast(event);
  }

  Event* DataFormatManager::FindAutoSupportedFormat() {
    for (int i = 0; i < fAutoSupportedFormats->GetEntriesFast(); i++) {
      Event* event = (Event*) fAutoSupportedFormats->At(i);
      if (event->ExistInTree()) return event;
    }
    return NULL;
  }

  void DataFormatManager::Reset() {
    if (fDataFormatBuffered) delete[] fDataFormatBuffered;
    if (fDataFormatNonBuffered) delete[] fDataFormatNonBuffered;
    fRegisteredFormats = 0;
  }

  DataFormatManager::~DataFormatManager() {
    if (fDataFormatBuffered) delete[] fDataFormatBuffered;
    if (fDataFormatNonBuffered) delete[] fDataFormatNonBuffered;
    delete fAutoSupportedFormats;
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
      }
      if (fDataFormatNonBuffered[task_id] != nullptr) return fDataFormatNonBuffered[task_id];
    }
    return nullptr;
  }
}  // namespace Hal
