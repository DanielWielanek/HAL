/*
 * CorrFitSmearingMatrixCalculator.cxx
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#include "CorrFitSmearingMath2DYield.h"

#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <memory>

#include "Cout.h"
#include "FemtoConst.h"
#include "FemtoPair.h"
#include "LorentzSmearing.h"
#include "StdOpenMP.h"
#include "StdPhys.h"
#include "StdTypes.h"

namespace Hal {

  CorrFitSmearingMath2DYield::CorrFitSmearingMath2DYield(Int_t tracks) : CorrFitSmearingMath1DCF(tracks) {}

  void CorrFitSmearingMath2DYield::GenerateTracks(TLorentzVector* tracks, Int_t pid) {
    Double_t m = 0;
    TH2D* h    = nullptr;
    switch (pid) {
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
      case EMode::kEtaPt: {
        for (int i = 0; i < fNtracks; i++) {
          Double_t pt, eta;
          h->GetRandom2(eta, pt);
          tracks[i] = Hal::Std::LorentzGenerator::PtEtaM(pt, eta, m);
        }
      } break;
      case EMode::kYPt: {
        for (int i = 0; i < fNtracks; i++) {
          Double_t pt, eta;
          h->GetRandom2(eta, pt);
          tracks[i] = Hal::Std::LorentzGenerator::PtYM(pt, eta, m);
        }
      } break;
    }
  }

  Bool_t CorrFitSmearingMath2DYield::Init() {
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

  void CorrFitSmearingMath2DYield::SetYield(TH2D& yield) { fYield1 = (TH2D*) yield.Clone(); }

  void CorrFitSmearingMath2DYield::SetYields(TH2D& yield1, TH2D& yield2) {
    fId     = false;
    fYield1 = (TH2D*) yield1.Clone();
    fYield2 = (TH2D*) yield2.Clone();
  }

  CorrFitSmearingMath2DYield::~CorrFitSmearingMath2DYield() {
    if (fYield1) delete fYield1;
    if (fYield2) delete fYield2;
  }

} /* namespace Hal */
