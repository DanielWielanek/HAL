/*
 * TrackToFMass2Cut.cxx
 *
 *  Created on: 10 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackToFMass2Cut.h"


#include <TLorentzVector.h>
#include <TMatrixDfwd.h>
#include <TMatrixT.h>
#include <iostream>

#include "Cut.h"
#include "DataFormat.h"
#include "ExpTrack.h"
#include "Package.h"
#include "Parameter.h"
#include "ToFTrack.h"
#include "Track.h"

namespace Hal {
  TrackToFMass2Cut::TrackToFMass2Cut() : TrackExpCut(1) {
    SetUnitName("ToF M^{2} A [GeV/c^2]", 0);
    fLowLine[0]  = -1E+2;
    fHighLine[0] = 1E+2;
    for (int i = 1; i < 3; i++) {
      fLowLine[i]  = 0;
      fHighLine[i] = 0;
    }
  }

  TrackToFMass2Cut::TrackToFMass2Cut(Int_t nParams) : TrackExpCut(nParams) {
    SetUnitName("ToF M^{2} A [GeV/c^2]", 0);
    fLowLine[0]  = -1E+2;
    fHighLine[0] = 1E+2;
    for (int i = 1; i < 3; i++) {
      fLowLine[i]  = 0;
      fHighLine[i] = 0;
    }
  }

  void TrackToFMass2Cut::SetPolyLineDown(Double_t a, Double_t b, Double_t c) {
    fLowLine[0] = a;
    fLowLine[1] = b;
    fLowLine[2] = c;
  }

  void TrackToFMass2Cut::SetPolyLineUp(Double_t a, Double_t b, Double_t c) {
    fHighLine[0] = a;
    fHighLine[1] = b;
    fHighLine[2] = c;
  }

  Bool_t TrackToFMass2Cut::Init(Int_t formad_id) {
    Bool_t stat = TrackExpCut::Init(formad_id);
    if (stat == kFALSE) return kFALSE;
    if (ToFAvaiable(formad_id)) return kTRUE;
    return kFALSE;
  }

  Bool_t TrackToFMass2Cut::Pass(Track* tr) {
    ToFTrack* tof = (ToFTrack*) ((ExpTrack*) tr)->GetDetTrack(DetectorID::kTOF);
    Double_t m2   = tof->GetMass2();
    Double_t p    = tr->GetMomentum().P();
    SetValue(m2, 0);
    Double_t min = fLowLine[0] + fLowLine[1] * p + fLowLine[2] * p * p;
    Double_t max = fHighLine[0] + fHighLine[1] * p + fHighLine[2] * p * p;
    if (m2 > max || m2 < min) { return ForcedUpdate(kFALSE); }
    return Validate();
  }

  TrackToFMass2Cut::~TrackToFMass2Cut() {}

  Package* TrackToFMass2Cut::Report() const {
    Package* rep = TrackExpCut::Report();
    rep->AddObject(new ParameterDouble("ToF m^{2} a_{min} ", fLowLine[0]));
    rep->AddObject(new ParameterDouble("ToF m^{2} a_{max} ", fHighLine[0]));
    rep->AddObject(new ParameterDouble("ToF m^{2} b_{min} ", fLowLine[1]));
    rep->AddObject(new ParameterDouble("ToF m^{2} b_{max} ", fHighLine[0]));
    rep->AddObject(new ParameterDouble("ToF m^{2} c_{min}", fLowLine[2]));
    rep->AddObject(new ParameterDouble("ToF m^{2} c_{max} ", fHighLine[0]));
    return rep;
  }

  void TrackToFMass2Cut::GetPolyUp(Double_t& a, Double_t& b, Double_t& c) const {
    a = fHighLine[0];
    b = fHighLine[1];
    c = fHighLine[2];
  }

  void TrackToFMass2Cut::GetPolyDown(Double_t& a, Double_t& b, Double_t& c) const {
    a = fLowLine[0];
    b = fLowLine[1];
    c = fLowLine[2];
  }

  TrackToFMass2Cut::TrackToFMass2Cut(const TrackToFMass2Cut& other) : TrackExpCut(other) {
    for (int i = 0; i < 3; i++) {
      fLowLine[i]  = other.fLowLine[i];
      fHighLine[i] = other.fHighLine[i];
    }
  }

  void TrackToFMass2Cut::SetUpPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t x3, Double_t y3) {
    GetPoly(fHighLine[0], fHighLine[1], fHighLine[2], x1, y1, x2, y2, x3, y3);
  }

  void TrackToFMass2Cut::GetPoly(Double_t& a,
                                 Double_t& b,
                                 Double_t& c,
                                 Double_t x1,
                                 Double_t y1,
                                 Double_t x2,
                                 Double_t y2,
                                 Double_t x3,
                                 Double_t y3) {
    TMatrixD A(3, 3);
    A[0][0] = x1 * x1;
    A[0][1] = x1;
    A[0][2] = 1.0;
    A[1][0] = x2 * x2;
    A[1][1] = x2;
    A[1][2] = 1.0;
    A[2][0] = x3 * x3;
    A[2][1] = x3;
    A[2][2] = 1.0;
    TMatrixD B(3, 1);
    B[0][0] = y1;
    B[1][0] = y2;
    B[2][0] = y3;
    A.Invert();
    TMatrixD C = A * B;
    c          = C[0][0];
    b          = C[1][0];
    a          = C[2][0];
    std::cout << a << " " << b << " " << c << std::endl;
  }

  void TrackToFMass2Cut::SetDownPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t x3, Double_t y3) {
    GetPoly(fLowLine[0], fLowLine[1], fLowLine[2], x1, y1, x2, y2, x3, y3);
  }

  Bool_t TrackTofMass2CutNoZeros::Pass(Track* tr) {
    ToFTrack* tof = (ToFTrack*) ((ExpTrack*) tr)->GetDetTrack(DetectorID::kTOF);
    Double_t m2   = tof->GetMass2();
    Double_t p    = tr->GetMomentum().P();
    SetValue(m2, 0);
    if (m2 == 0) return ForcedUpdate(kFALSE);
    Double_t min = fLowLine[0] + fLowLine[1] * p + fLowLine[2] * p * p;
    Double_t max = fHighLine[0] + fHighLine[1] * p + fHighLine[2] * p * p;
    if (m2 > max || m2 < min) { return ForcedUpdate(kFALSE); }
    return Validate();
  }

  TrackTofMass2CutNoZeros::TrackTofMass2CutNoZeros(const TrackToFMass2Cut& other) : TrackToFMass2Cut(other) {}
}  // namespace Hal
