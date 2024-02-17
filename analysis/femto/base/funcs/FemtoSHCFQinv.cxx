/*
 * FemtoSHCFQinv.cxx
 *
 *  Created on: 1 lip 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSHCFQinv.h"
#include "FemtoPair.h"

namespace Hal {

  FemtoSHCFQinv::FemtoSHCFQinv() : FemtoSHCF(), fQinvDist(nullptr) {}

  FemtoSHCFQinv::~FemtoSHCFQinv() {
    if (fQinvDist) delete fQinvDist;
  }

  void FemtoSHCFQinv::FastAdd(const FemtoSHCF* obj) {
    FemtoSHCF::FastAdd(obj);
    fQinvDist->Add(((FemtoSHCFQinv*) obj)->fQinvDist);
  }

  FemtoSHCFQinv::FemtoSHCFQinv(TString name, Int_t maxL, Int_t bins, Double_t min, Double_t max, Femto::EKinematics kinematics) :
    FemtoSHCF(name, maxL, bins, min, max, kinematics), fQinvDist(new TH1D("kv", "kv", bins, min, max)) {}

  void FemtoSHCFQinv::FillNumObj(TObject* obj) {

    FemtoPair* pair = (FemtoPair*) obj;
    Double_t kv     = TMath::Sqrt(pair->GetX() * pair->GetX() + pair->GetY() * pair->GetY() + pair->GetZ() * pair->GetZ());
    fQinvDist->Fill(kv, pair->GetT());
    FemtoSHCF::FillNumObj(obj);
  }

  void FemtoSHCFQinv::FillDenObj(TObject* obj) { FemtoSHCF::FillDenObj(obj); }

  void FemtoSHCFQinv::AddScaled(const Hal::DividedHisto1D& other, Double_t scale) {
    FemtoSHCF::AddScaled(other, scale);
    auto shcf = dynamic_cast<Hal::FemtoSHCFQinv&>(other);
    fQinvDist->Add(shcf.fQinvDist, scale);
  }

} /* namespace Hal */
