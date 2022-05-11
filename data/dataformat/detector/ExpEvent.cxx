/*
 * DetectedEvent.cxx
 *
 *  Created on: 10 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "ExpEvent.h"

#include "DataFormat.h"

#include <TLorentzVector.h>
#include <TVector3.h>

namespace Hal {
  ExpEvent::ExpEvent() : Event(), fRunInfoId(0), fEventTrigger(0), fNTofTracks(0) {
    fVertexError = new TLorentzVector();
    fMagField    = new TVector3;
  }

  ExpEvent::ExpEvent(const ExpEvent& other) : Event(other) {
    fRunInfoId    = other.fRunInfoId;
    fEventTrigger = other.fEventTrigger;
    fVertexError  = new TLorentzVector(*other.fVertexError);
    fMagField     = new TVector3(*other.fMagField);
    fNTofTracks   = other.fNTofTracks;
  }

  ExpEvent::ExpEvent(TString track_class, TString v0_class) :
    Event(track_class, v0_class), fRunInfoId(0), fEventTrigger(0), fNTofTracks(0) {
    fVertexError = new TLorentzVector();
    fMagField    = new TVector3;
  }

  void ExpEvent::ShallowCopyEvent(Event* event) {
    Event::ShallowCopyEvent(event);
    ExpEvent* ev  = (ExpEvent*) event;
    fRunInfoId    = ev->fRunInfoId;
    fEventTrigger = ev->fEventTrigger;
    *fVertexError = *ev->fVertexError;
    *fMagField    = *ev->fMagField;
    fNTofTracks   = ev->GetNTofTracks();
  }

  Float_t ExpEvent::GetFieldVal(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::EExpEvent::kTofTracksNo: return fNTofTracks; break;
      default: return Event::GetFieldVal(fieldID); break;
    }
  }

  TString ExpEvent::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::EExpEvent::kTofTracksNo: return "N_{TOF tracks} [N]"; break;
      default: return Event::GetFieldName(fieldID); break;
    }
  }

  ExpEvent::~ExpEvent() {
    if (fVertexError) {
      delete fVertexError;
      delete fMagField;
    }
  }

  ExpEventHelix::ExpEventHelix(TString track_class, TString v0_class) : ExpEvent(track_class, v0_class) {}

  ExpEventHelix::ExpEventHelix() : ExpEvent() {}

  ExpEventHelix::ExpEventHelix(const ExpEventHelix& other) : ExpEvent(other) {}

  ExpEventHelix::~ExpEventHelix() {}
}  // namespace Hal
