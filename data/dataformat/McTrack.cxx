/*
 * TrackMC.cxx
 *
 *  Created on: 1 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "McTrack.h"
#include "DataFormat.h"
namespace Hal {
  McTrack::McTrack() : Track(), fPDG(0) {}

  McTrack::~McTrack() {}

  void McTrack::Boost(Double_t x, Double_t y, Double_t z) {
    Track::Boost(x, y, z);
    fFreez.Boost(x, y, z);
  }

  void McTrack::CopyData(Track* other) {
    Track::CopyData(other);
    fFreez = ((McTrack*) other)->fFreez;
    fStart = ((McTrack*) other)->fStart;
    fPDG   = ((McTrack*) other)->fPDG;
  }

  Float_t McTrack::GetFieldVal(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::Track::EMc::kXfr: return fFreez.X(); break;
      case DataFieldID::Track::EMc::kYfr: return fFreez.Y(); break;
      case DataFieldID::Track::EMc::kZfr: return fFreez.Z(); break;
      case DataFieldID::Track::EMc::kTfr: return fFreez.T(); break;
      case DataFieldID::Track::EMc::kXstar: return fStart.X(); break;

      case DataFieldID::Track::EMc::kYstart: return fStart.Y(); break;
      case DataFieldID::Track::EMc::kZstart: return fStart.Z(); break;
      case DataFieldID::Track::EMc::kTstart: return fStart.T(); break;
      case DataFieldID::Track::EMc::kPdg: return GetPdg(); break;
      case DataFieldID::Track::EMc::kXYstart: return fStart.Pt(); break;

      case DataFieldID::Track::EMc::kXYfr: return fFreez.Pt(); break;
    }
    return Track::GetFieldVal(fieldID);
  }

  TString McTrack::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::Track::EMc::kXfr: return "X_{fr} [fm]"; break;
      case DataFieldID::Track::EMc::kYfr: return "Y_{fr} [fm]"; break;
      case DataFieldID::Track::EMc::kZfr: return "Z_{fr} [fm]"; break;
      case DataFieldID::Track::EMc::kTfr: return "T_{fr} [fm/c]"; break;
      case DataFieldID::Track::EMc::kXstar: return "X_{start} [cm]"; break;

      case DataFieldID::Track::EMc::kYstart: return "T_{start} [cm]"; break;
      case DataFieldID::Track::EMc::kZstart: return "Z_{start} [cm]"; break;
      case DataFieldID::Track::EMc::kTstart: return "T_{start} [ns]"; break;
      case DataFieldID::Track::EMc::kPdg: return "Pdg [AU]"; break;
      case DataFieldID::Track::EMc::kXYstart: return "XY_{start} [cm]"; break;

      case DataFieldID::Track::EMc::kXYfr: return "XY_{fr} [fm]"; break;
    }
    return Track::GetFieldName(fieldID);
  }
}  // namespace Hal
