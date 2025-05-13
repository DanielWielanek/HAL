/*
 * TrackTPCToFCut.cxx
 *
 *  Created on: 28 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackTpcToFCut.h"

#include <TMath.h>
#include <TString.h>

#include "Cout.h"
#include "Cut.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "Track.h"

namespace Hal {
  TrackTpcToFCut::TrackTpcToFCut(TrackTpcCut* tpc, TrackToFMass2Cut* tof) : TrackExpCut(8) {
    if (tof)
      fToF = new TrackToFMass2Cut(*tof);
    else
      fToF = new TrackToFMass2Cut();
    if (tpc)
      fTpc = new TrackTpcCut(*tpc);
    else
      fTpc = new TrackTpcCut();
    fThreshold = 0.5;
    fTofOpt    = kFALSE;
    for (int i = 0; i < fTpc->GetCutSize(); i++) {
      SetUnitName(fTpc->GetUnit(i), i);
    }
    SetUnitName(fToF->GetUnit(0), fTpc->GetCutSize());
    for (int i = 0; i < 7; i++) {
      SetMinMax(fTpc->GetMin(i), fTpc->GetMax(i), i);
    }
    SetMinMax(fToF->GetMin(0), fToF->GetMax(0), 7);
  }

  Bool_t TrackTpcToFCut::Init(Int_t format_id) {
    if (fTpc->Init(format_id) == kFALSE) {
      Cout::PrintInfo(Form("Cannot init %s becase TPC cut", ClassName()), EInfo::kWarning);
      return kFALSE;
    }
    if (fToF->Init(format_id) == kFALSE) {
      Cout::PrintInfo(Form("Cannot init %s because ToF cut", ClassName()), EInfo::kWarning);
      return kFALSE;
    }
    for (int i = 0; i < 7; i++) {
      SetMinMax(fTpc->GetMin(i), fTpc->GetMax(i), i);
    }
    SetMinMax(fToF->GetMin(0), fToF->GetMax(0), 7);
    return kTRUE;
  }

  Bool_t TrackTpcToFCut::Pass(Track* tr) {
    Double_t px   = tr->GetPx();
    Double_t py   = tr->GetPy();
    Double_t pz   = tr->GetPz();
    Double_t p    = TMath::Sqrt(px * px + py * py + pz * pz);
    Bool_t good   = fTpc->Pass(tr);
    Bool_t tof_ok = fToF->Pass(tr);
    for (int i = 0; i < fTpc->GetCutSize(); i++) {
      SetValue(fTpc->GetValue(i), i);
    }
    SetValue(fToF->GetValue(), 7);
    if (p > fThreshold) {
      if (tof_ok == kFALSE) good = kFALSE;
    } else if (fTofOpt) {
      Double_t m2 = fToF->GetValue(0);
      if (m2 != 0.0 && m2 > -1) {  // ToF data avaiable
        if (tof_ok == kFALSE) good = kFALSE;
      }
    }
    return ForcedUpdate(good);
  }

  Package* TrackTpcToFCut::Report() const {
    Package* pack        = TrackExpCut::Report();
    Package* reportTpc   = fTpc->Report();
    TString active_sigma = ((ParameterString*) reportTpc->GetObjectByName("ActiveSigma"))->GetValue();
    TString mode         = ((ParameterString*) reportTpc->GetObjectByName("Mode"))->GetValue();
    delete reportTpc;
    pack->AddObject(new ParameterString("ActiveSigma", active_sigma));
    pack->AddObject(new ParameterString("Mode", mode));
    Double_t LowLine[3];
    Double_t HighLine[3];
    fToF->GetPolyDown(LowLine[0], LowLine[1], LowLine[2]);
    fToF->GetPolyUp(HighLine[0], HighLine[1], HighLine[2]);
    pack->AddObject(new ParameterDouble("ToF m^{2} a_{min} ", LowLine[0]));
    pack->AddObject(new ParameterDouble("ToF m^{2} a_{max} ", HighLine[0]));
    pack->AddObject(new ParameterDouble("ToF m^{2} b_{min} ", LowLine[1]));
    pack->AddObject(new ParameterDouble("ToF m^{2} b_{max} ", HighLine[1]));
    pack->AddObject(new ParameterDouble("ToF m^{2} c_{min}", LowLine[2]));
    pack->AddObject(new ParameterDouble("ToF m^{2} c_{max} ", HighLine[2]));
    pack->AddObject(new ParameterBool("ToF optional", fTofOpt));
    pack->AddObject(new ParameterDouble("ToF threshold [GeV/c]", fThreshold));
    return pack;
  }

  TrackTpcToFCut::TrackTpcToFCut(const TrackTpcToFCut& other) :
    TrackExpCut(other), fThreshold(other.fThreshold), fTofOpt(other.fTofOpt) {
    TString tof_class = other.fToF->ClassName();
    if (tof_class == "Hal::TrackToFMass2Cut") {
      fToF = new TrackToFMass2Cut(*other.fToF);
    } else if (tof_class == "Hal::TrackTofMass2CutNoZeros") {
      fToF = new TrackTofMass2CutNoZeros(*other.fToF);
    }
    fTpc = new TrackTpcCut(*other.fTpc);
  }

  TrackTpcToFCut::~TrackTpcToFCut() {
    delete fTpc;
    delete fToF;
  }
}  // namespace Hal
