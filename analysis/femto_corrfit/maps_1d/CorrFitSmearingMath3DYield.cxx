/*
 * CorrFitSmearingMath3DYield.cxx
 *
 *  Created on: 11 wrz 2026
 *      Author: daniel
 */

#include "CorrFitSmearingMath3DYield.h"

#include <TH3.h>
#include <TLorentzVector.h>

#include "Cout.h"
#include "LorentzSmearing.h"
#include "StdPhys.h"
#include "StdTypes.h"

namespace Hal {

  CorrFitSmearingMath3DYield::CorrFitSmearingMath3DYield(Int_t tracks) : CorrFitSmearingMath1DCF(tracks) {}

  void CorrFitSmearingMath3DYield::GenerateTracks(TLorentzVector* tracks, Int_t type) {
    Double_t m = 0;
    TH3D* h    = nullptr;
    switch (type) {
      case 0: {
        m = fSmearing1->GetMass();
        h = fYield1;
      } break;
      case 1: {
        m = fSmearing2->GetMass();
        h = fYield2;
      } break;
    }
    switch (fMode) {
      case EMode::kEtaPtPhi: {
        for (int i = 0; i < fNtracks; i++) {
          Double_t pt, eta, phi;
          h->GetRandom3(eta, pt, phi);
          tracks[i] = Hal::Std::LorentzGenerator::PtEtaM(pt, eta, m);
          tracks[i].RotateZ(phi);
        }
      } break;
      case EMode::kYPtPhi: {
        for (int i = 0; i < fNtracks; i++) {
          Double_t pt, eta, phi;
          h->GetRandom3(eta, pt, phi);
          tracks[i] = Hal::Std::LorentzGenerator::PtYM(pt, eta, m);
          tracks[i].RotateZ(phi);
        }
      } break;
    }
  }

  Bool_t CorrFitSmearingMath3DYield::Init() {
    if (!CorrFitSmearingMath1DCF::Init()) return false;
    if (!fYield1) {
      Hal::Cout::PrintInfo("CorrFitSmearingMath no yield for first particle !", EInfo::kError);
      return false;
    }
    if (!fId) {
      if (!fYield2) {
        Hal::Cout::PrintInfo("CorrFitSmearingMath no yield for second particle !", EInfo::kError);
        return false;
      }
    }
    return true;
  }

  void CorrFitSmearingMath3DYield::SetYield(TH3D& yield) { fYield1 = (TH3D*) yield.Clone(); }

  void CorrFitSmearingMath3DYield::SetYields(TH3D& yield1, TH3D& yield2) {
    fId     = false;
    fYield1 = (TH3D*) yield1.Clone();
    fYield2 = (TH3D*) yield2.Clone();
  }

  CorrFitSmearingMath3DYield::~CorrFitSmearingMath3DYield() {
    if (fYield1) delete fYield1;
    if (fYield2) delete fYield2;
  }


} /* namespace Hal */
