/*
 * FemtoWeightGeneratorResidual.cxx
 *
 *  Created on: 30 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoWeightGeneratorResidual.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cout.h"
#include "DataFormatManager.h"
#include "ExpEvent.h"
#include "FemtoPair.h"
#include "FemtoWeightGeneratorLednicky.h"
#include "McTrack.h"
#include "Track.h"

namespace Hal {
  FemtoWeightGeneratorResidual::FemtoWeightGeneratorResidual() :
    fMainWeight(nullptr), fResPair(nullptr), fWeightType(eWeightType::kOther), fComplexFormat(kFALSE) {}

  FemtoWeightGeneratorResidual::FemtoWeightGeneratorResidual(const FemtoWeightGenerator& w) :
    fResPair(nullptr), fWeightType(eWeightType::kOther), fComplexFormat(kFALSE) {
    fMainWeight = w.MakeCopy();
  }

  FemtoWeightGeneratorResidual::FemtoWeightGeneratorResidual(const FemtoWeightGeneratorResidual& other) :
    FemtoWeightGenerator(other),
    fMainWeight(nullptr),
    fResPair(nullptr),
    fWeightType(eWeightType::kOther),
    fComplexFormat(kFALSE) {}

  Double_t FemtoWeightGeneratorResidual::GenerateWeight(FemtoPair* pair) {
    McTrack *tr1, *tr2;
    if (fComplexFormat) {
      ComplexTrack* z1 = (ComplexTrack*) pair->GetTrack1();
      ComplexTrack* z2 = (ComplexTrack*) pair->GetTrack2();
      tr1              = (McTrack*) z1->GetImgTrack();
      tr2              = (McTrack*) z2->GetImgTrack();
      if (tr1 == nullptr || tr2 == nullptr) return 1;  // no MC tracks weight should be 1
    } else {
      tr1 = (McTrack*) pair->GetTrack1();
      tr2 = (McTrack*) pair->GetTrack2();
    }

    Int_t prim = tr1->IsGoodSecondary() + 2 * tr2->IsGoodSecondary();
    switch (prim) {
      case 0: {  // no primary particles
                 // do nothing
        SetPairData(tr1, tr2);
      } break;
      case 1: {  // first is seondary
        while (tr1->IsGoodSecondary()) {
          tr1 = (McTrack*) tr1->GetEvent()->GetTrack(tr1->GetMotherIndex());
        }
        SetPairData(tr1, tr2);
      } break;
      case 2: {  // second is secondary
        while (tr2->IsGoodSecondary()) {
          tr2 = (McTrack*) tr2->GetEvent()->GetTrack(tr2->GetMotherIndex());
        }
        SetPairData(tr1, tr2);
      } break;
      case 3: {  // both are secondary
        while (tr1->IsGoodSecondary()) {
          tr1 = (McTrack*) tr1->GetEvent()->GetTrack(tr1->GetMotherIndex());
        }
        while (tr2->IsGoodSecondary()) {
          tr2 = (McTrack*) tr2->GetEvent()->GetTrack(tr2->GetMotherIndex());
        }
        SetPairData(tr1, tr2);
      } break;
    }
    switch (fWeightType) {
      case eWeightType::kLednicky: {
        (static_cast<FemtoWeightGeneratorLednicky*>(fMainWeight))->SetPairTypeFromPairAndConfigureFSI(fResPair);
      } break;
      default: {

      } break;
    }
    return fMainWeight->GenerateWeight(fResPair);
  }

  Bool_t FemtoWeightGeneratorResidual::Init(Int_t task_id, FemtoPair* pair) {
    if (fMainWeight == nullptr) return kFALSE;
    fMainWeight->Init(task_id, pair);
    fResPair                = pair->MakeCopy();
    DataFormatManager* mngr = DataFormatManager::Instance();
    const Event* event      = mngr->GetFormat(task_id, EFormatDepth::kBuffered);
    if (dynamic_cast<const ComplexEvent*>(event)) fComplexFormat = kTRUE;
    if (dynamic_cast<const ExpEvent*>(event)) {
      Cout::PrintInfo("Experimental format cannot be used for residual weights", EInfo::kLowWarning);
      return kFALSE;
    }
    if (dynamic_cast<FemtoWeightGeneratorLednicky*>(fMainWeight)) { fWeightType = eWeightType::kLednicky; }
    return kTRUE;
  }

  Package* FemtoWeightGeneratorResidual::Report() const {
    Package* report = new Package(this);
    report->AddObject(fMainWeight->Report());
    return report;
  }

  void FemtoWeightGeneratorResidual::SetPairData(McTrack* track1, McTrack* track2) {
    const TLorentzVector& pt1 = track1->GetMomentum();
    const TLorentzVector& pt2 = track2->GetMomentum();
    fResPair->SetPdg1(track1->GetPdg());
    fResPair->SetPdg2(track2->GetPdg());

    const TLorentzVector& x1 = track1->GetFreezoutPosition();
    const TLorentzVector& x2 = track2->GetFreezoutPosition();
    fResPair->SetFreezoutCoord1(x1.X(), x1.Y(), x1.Z(), x1.T());
    fResPair->SetFreezoutCoord2(x2.X(), x2.Y(), x2.Z(), x2.T());
    fResPair->SetTrueMomenta1(pt1.Px(), pt1.Py(), pt1.Pz(), pt1.E());
    fResPair->SetTrueMomenta2(pt2.Px(), pt2.Py(), pt2.Pz(), pt2.E());
    // fResPair->SetMomenta1(p1.Px(), p1.Py(), p1.Pz()); // currently not necessary
    // fResPair->SetMomenta2(p2.Px(), p2.Py(), p2.Pz());
  }

  FemtoWeightGeneratorResidual::~FemtoWeightGeneratorResidual() {
    if (fMainWeight) delete fMainWeight;
    if (fResPair) delete fResPair;
  }

  FemtoWeightGeneratorResidual& FemtoWeightGeneratorResidual::operator=(const FemtoWeightGeneratorResidual& other) {
    if (this == &other) return *this;
    if (fMainWeight) {
      delete fMainWeight;
      fMainWeight = nullptr;
    }
    if (fResPair) {
      delete fResPair;
      fResPair = nullptr;
    }
    if (other.fMainWeight) { fMainWeight = other.fMainWeight->MakeCopy(); }
    if (other.fResPair) { fResPair = other.fResPair->MakeCopy(); }
    fWeightType    = other.fWeightType;
    fComplexFormat = other.fComplexFormat;
    return *this;
  }
}  // namespace Hal
