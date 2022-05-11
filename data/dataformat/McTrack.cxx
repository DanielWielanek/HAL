/*
 * TrackMC.cxx
 *
 *  Created on: 1 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DataFormat.h"
#include "McTrack.h"
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
      case DataFieldID::EMcTrack::kXfr: return fFreez.X(); break;
      case DataFieldID::EMcTrack::kYfr: return fFreez.Y(); break;
      case DataFieldID::EMcTrack::kZfr: return fFreez.Z(); break;
      case DataFieldID::EMcTrack::kTfr: return fFreez.T(); break;
      case DataFieldID::EMcTrack::kXstar: return fStart.X(); break;

      case DataFieldID::EMcTrack::kYstart: return fStart.Y(); break;
      case DataFieldID::EMcTrack::kZstart: return fStart.Z(); break;
      case DataFieldID::EMcTrack::kTstart: return fStart.T(); break;
      case DataFieldID::EMcTrack::kPdg: return GetPdg(); break;
      case DataFieldID::EMcTrack::kXYstart: return fStart.Pt(); break;

      case DataFieldID::EMcTrack::kXYfr: return fFreez.Pt(); break;
    }
    return Track::GetFieldVal(fieldID);
  }

  TString McTrack::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::EMcTrack::kXfr: return "X_{fr} [fm]"; break;
      case DataFieldID::EMcTrack::kYfr: return "Y_{fr} [fm]"; break;
      case DataFieldID::EMcTrack::kZfr: return "Z_{fr} [fm]"; break;
      case DataFieldID::EMcTrack::kTfr: return "T_{fr} [fm/c]"; break;
      case DataFieldID::EMcTrack::kXstar: return "X_{start} [cm]"; break;

      case DataFieldID::EMcTrack::kYstart: return "T_{start} [cm]"; break;
      case DataFieldID::EMcTrack::kZstart: return "Z_{start} [cm]"; break;
      case DataFieldID::EMcTrack::kTstart: return "T_{start} [ns]"; break;
      case DataFieldID::EMcTrack::kPdg: return "Pdg [AU]"; break;
      case DataFieldID::EMcTrack::kXYstart: return "XY_{start} [cm]"; break;

      case DataFieldID::EMcTrack::kXYfr: return "XY_{fr} [fm]"; break;
    }
    return Track::GetFieldName(fieldID);
  }
}  // namespace Hal
