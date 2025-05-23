/*
 * FemtoFreezeoutsAna.cxx
 *
 *  Created on: 15-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoFreezeoutsAna.h"

#include "Cout.h"
#include "Cut.h"
#include "FemtoConst.h"
#include "FemtoFastCut.h"
#include "FemtoFreezeoutGenerator.h"
#include "FemtoPair.h"
#include "HistogramManager.h"
#include "Package.h"
#include "Parameter.h"

#include <TCollection.h>
#include <TDatabasePDG.h>
#include <TH3.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TMathBase.h>
#include <TParticlePDG.h>
#include <TString.h>
#include <TVector3.h>


namespace Hal {
  FemtoFreezeoutsAna::FemtoFreezeoutsAna() :
    TwoTrackAna(kFALSE),
    fPdg1(211),
    fPdg2(211),
    fX(0),
    fY(0),
    fZ(0),
    fT(0),
    fCut(0.15),
    fFastCut(nullptr),
    fIgnoreSign(kFALSE),
    fUseFakeMomenta(kFALSE),
    fFemtoPair(nullptr),
    fFreezeoutGenerator(nullptr),
    fHistograms1d(nullptr),
    fHistograms3d(nullptr) {
    fBackgroundMode = kNoBackground;
    fMixSize        = 1;
    fKinematicsMode = EMode::kPRF;
    for (int i = 0; i < 3; i++) {
      fBins[i]     = 100;
      fHistoMin[i] = -100;
      fHistoMax[i] = 100;
    }
    AddTags("freezeouts");
  }

  FemtoFreezeoutsAna::FemtoFreezeoutsAna(const FemtoFreezeoutsAna& ana) :
    TwoTrackAna(ana),
    fPdg1(ana.fPdg1),
    fPdg2(ana.fPdg2),
    fX(ana.fX),
    fY(ana.fY),
    fZ(ana.fZ),
    fT(ana.fT),
    fCut(ana.fCut),
    fFastCut(nullptr),
    fKinematicsMode(ana.fKinematicsMode),
    fIgnoreSign(ana.fIgnoreSign),
    fUseFakeMomenta(ana.fUseFakeMomenta),
    fFemtoPair(nullptr),
    fFreezeoutGenerator(nullptr),
    fHistograms1d(nullptr),
    fHistograms3d(nullptr) {
    if (ana.fFastCut) { fFastCut = (FemtoFastCut*) ana.fFastCut->Clone(); }
    for (int i = 0; i < 3; i++) {
      fBins[i]     = ana.fBins[i];
      fHistoMin[i] = ana.fHistoMin[i];
      fHistoMax[i] = ana.fHistoMax[i];
    }
    if (ana.fFemtoPair) {
      switch (ana.fKinematicsMode) {
        case EMode::kPRF: {
          fFemtoPair = Femto::MakePair(Femto::EKinematics::kPRF, ana.fUseFakeMomenta);
        } break;
        default: {
          fFemtoPair = Femto::MakePair(Femto::EKinematics::kLCMS, ana.fUseFakeMomenta);
        } break;
      }
    }
    if (ana.fFreezeoutGenerator) { fFreezeoutGenerator = ana.fFreezeoutGenerator->MakeCopy(); }
  }

  void FemtoFreezeoutsAna::ComputePRF() {
    Int_t bin = fFastCut->GetBin();
    if (bin < 0) return;
    TLorentzVector p1, p2, x1, x2;
    p1.SetPxPyPzE(fFemtoPair->TruePx1(), fFemtoPair->TruePy1(), fFemtoPair->TruePz1(), fFemtoPair->TrueE1());
    p2.SetPxPyPzE(fFemtoPair->TruePx2(), fFemtoPair->TruePy2(), fFemtoPair->TruePz2(), fFemtoPair->TrueE2());
    TLorentzVector P = p1 + p2;
    x1.SetPxPyPzE(fFemtoPair->GetX1(), fFemtoPair->GetY1(), fFemtoPair->GetZ1(), fFemtoPair->GetT1());
    x2.SetPxPyPzE(fFemtoPair->GetX2(), fFemtoPair->GetY2(), fFemtoPair->GetZ2(), fFemtoPair->GetT2());
    Double_t vz = P.Pz() / P.E();
    P.Boost(0, 0, -vz);  //
    Double_t phi = P.Phi();
    // bost in out direction
    Double_t beta = P.Pt() / P.E();
    p1.Boost(0, 0, -vz);
    p1.RotateZ(-phi);
    p1.Boost(-beta, 0, 0);
    if (fCut < p1.P()) return;
    // bost in out direction
    x1.Boost(0, 0, -vz);
    x2.Boost(0, 0, -vz);
    // rotation
    x1.RotateZ(-phi);
    x2.RotateZ(-phi);
    x1.Boost(-beta, 0, 0);
    x2.Boost(-beta, 0, 0);
    fX         = x1.X() - x2.X();
    fY         = x1.Y() - x2.Y();
    fZ         = x1.Z() - x2.Z();
    Double_t R = TMath::Sqrt(fX * fX + fY * fY + fZ * fZ);
    if (fX < 0) R = -R;
    if (fIgnoreSign) {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(R));
      fHistograms3d->Fill(
        fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(fX), TMath::Abs(fY), TMath::Abs(fZ));
    } else {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, R);
      fHistograms3d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, fX, fY, fZ);
    }
  }

  void FemtoFreezeoutsAna::ComputeLCMS() {
    Int_t bin = fFastCut->GetBin();
    if (bin < 0) return;
    TLorentzVector p1, p2, x1, x2;
    p1.SetPxPyPzE(fFemtoPair->TruePx1(), fFemtoPair->TruePy1(), fFemtoPair->TruePz1(), fFemtoPair->TrueE1());
    p2.SetPxPyPzE(fFemtoPair->TruePx2(), fFemtoPair->TruePy2(), fFemtoPair->TruePz2(), fFemtoPair->TrueE2());
    TLorentzVector P = p1 + p2;
    TLorentzVector M = p1 - p2;
    fT               = TMath::Abs(M.M());
    if (fT > fCut) return;
    x1.SetPxPyPzE(fFemtoPair->GetX1(), fFemtoPair->GetY1(), fFemtoPair->GetZ1(), fFemtoPair->GetT1());
    x2.SetPxPyPzE(fFemtoPair->GetX2(), fFemtoPair->GetY2(), fFemtoPair->GetZ2(), fFemtoPair->GetT2());
    // boost along Z axis
    x1.Boost(0, 0, -P.Pz() / P.E());
    x2.Boost(0, 0, -P.Pz() / P.E());
    P.Boost(0, 0, -P.Pz() / P.E());  //
                                     // rotation
    Double_t phi = P.Phi();
    x1.RotateZ(-phi);
    x2.RotateZ(-phi);
    fX = x1.X() - x2.X();
    fY = x1.Y() - x2.Y();
    fZ = x1.Z() - x2.Z();

    Double_t R = TMath::Sqrt(fX * fX + fY * fY + fZ * fZ);
    if (fX < 0) R = -R;
    if (fIgnoreSign) {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(R));
      fHistograms3d->Fill(
        fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(fX), TMath::Abs(fY), TMath::Abs(fZ));
    } else {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, R);
      fHistograms3d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, fX, fY, fZ);
    }
  }

  void FemtoFreezeoutsAna::ComputePRFL() {
    Int_t bin = fFastCut->GetBin();
    if (bin < 0) return;
    TLorentzVector p1, p2, x1, x2;
    p1.SetPxPyPzE(fFemtoPair->TruePx1(), fFemtoPair->TruePy1(), fFemtoPair->TruePz1(), fFemtoPair->TrueE1());
    p2.SetPxPyPzE(fFemtoPair->TruePx2(), fFemtoPair->TruePy2(), fFemtoPair->TruePz2(), fFemtoPair->TrueE2());
    TLorentzVector P = p1 + p2;
    TVector3 v       = P.BoostVector();
    p1.Boost(-v);
    if (p1.P() > fCut) return;
    x1.SetPxPyPzE(fFemtoPair->GetX1(), fFemtoPair->GetY1(), fFemtoPair->GetZ1(), fFemtoPair->GetT1());
    x2.SetPxPyPzE(fFemtoPair->GetX2(), fFemtoPair->GetY2(), fFemtoPair->GetZ2(), fFemtoPair->GetT2());
    TLorentzVector X = x2 - x1;
    X.Boost(-v);
    // X.RotateZ(-P.Phi());
    fX         = X.X();
    fY         = X.Y();
    fZ         = X.Z();
    fT         = X.Rho();
    Double_t R = TMath::Sqrt(fX * fX + fY * fY + fZ * fZ);
    if (fX < 0) R = -R;
    if (fIgnoreSign) {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(R));
      fHistograms3d->Fill(
        fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(fX), TMath::Abs(fY), TMath::Abs(fZ));
    } else {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, R);
      fHistograms3d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, fX, fY, fZ);
    }
  }

  void FemtoFreezeoutsAna::ComputeLCMSGamma() {
    Int_t bin = fFastCut->GetBin();
    if (bin < 0) return;
    TLorentzVector p1, p2, x1, x2;
    p1.SetPxPyPzE(fFemtoPair->TruePx1(), fFemtoPair->TruePy1(), fFemtoPair->TruePz1(), fFemtoPair->TrueE1());
    p2.SetPxPyPzE(fFemtoPair->TruePx2(), fFemtoPair->TruePy2(), fFemtoPair->TruePz2(), fFemtoPair->TrueE2());
    TLorentzVector P      = p1 + p2;
    TVector3 v            = P.BoostVector();
    TLorentzVector pFirst = p1;
    pFirst.Boost(-v);
    if (pFirst.P() > fCut) return;
    x1.SetPxPyPzE(fFemtoPair->GetX1(), fFemtoPair->GetY1(), fFemtoPair->GetZ1(), fFemtoPair->GetT1());
    x2.SetPxPyPzE(fFemtoPair->GetX2(), fFemtoPair->GetY2(), fFemtoPair->GetZ2(), fFemtoPair->GetT2());

    Double_t vz = P.Pz() / P.E();
    x1.Boost(0, 0, -vz);
    x2.Boost(0, 0, -vz);
    P.Boost(0, 0, -vz);  //
    Double_t phi = P.Phi();
    x1.RotateZ(-phi);
    x2.RotateZ(-phi);
    fX = x1.X() - x2.X();
    fY = x1.Y() - x2.Y();
    fZ = x1.Z() - x2.Z();

    p1.Boost(0, 0, -vz);
    p1.RotateZ(-phi);
    p2.Boost(0, 0, -vz);
    p2.RotateZ(-phi);
    Double_t qout = p1.X() - p2.X();


    Double_t beta = P.Pt() / P.E();
    x1.Boost(-beta, 0, 0);
    x2.Boost(-beta, 0, 0);
    p1.Boost(-beta, 0, 0);
    Double_t kstarO = p1.X();
    Double_t rstarO = TMath::Abs(x1.X() - x2.X());
    fX              = rstarO * kstarO / qout * 2.0;

    Double_t R = TMath::Sqrt(fX * fX + fY * fY + fZ * fZ);
    if (fX < 0) R = -R;
    if (fIgnoreSign) {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(R));
      fHistograms3d->Fill(
        fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(fX), TMath::Abs(fY), TMath::Abs(fZ));
    } else {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, R);
      fHistograms3d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, fX, fY, fZ);
    }
  }

  void FemtoFreezeoutsAna::ComputeRaw() {
    Int_t bin = fFastCut->GetBin();
    if (bin < 0) return;
    TLorentzVector p1, p2, x1, x2;
    p1.SetPxPyPzE(fFemtoPair->TruePx1(), fFemtoPair->TruePy1(), fFemtoPair->TruePz1(), fFemtoPair->TrueE1());
    p2.SetPxPyPzE(fFemtoPair->TruePx2(), fFemtoPair->TruePy2(), fFemtoPair->TruePz2(), fFemtoPair->TrueE2());
    TLorentzVector P = p1 + p2;
    x1.SetPxPyPzE(fFemtoPair->GetX1(), fFemtoPair->GetY1(), fFemtoPair->GetZ1(), fFemtoPair->GetT1());
    x2.SetPxPyPzE(fFemtoPair->GetX2(), fFemtoPair->GetY2(), fFemtoPair->GetZ2(), fFemtoPair->GetT2());
    Double_t vz = P.Pz() / P.E();
    P.Boost(0, 0, -vz);  //
    Double_t phi = P.Phi();
    // bost in out direction
    Double_t beta = P.Pt() / P.E();
    p1.Boost(0, 0, -vz);
    p1.RotateZ(-phi);
    p1.Boost(-beta, 0, 0);
    if (fCut < p1.P()) return;
    fX         = x1.X() - x2.X();
    fY         = x1.Y() - x2.Y();
    fZ         = x1.Z() - x2.Z();
    Double_t R = TMath::Sqrt(fX * fX + fY * fY + fZ * fZ);
    if (fX < 0) R = -R;
    if (fIgnoreSign) {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(R));
      fHistograms3d->Fill(
        fCurrentEventCollectionID, fCurrentPairCollectionID, bin, TMath::Abs(fX), TMath::Abs(fY), TMath::Abs(fZ));
    } else {
      fHistograms1d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, R);
      fHistograms3d->Fill(fCurrentEventCollectionID, fCurrentPairCollectionID, bin, fX, fY, fZ);
    }
  }

  void FemtoFreezeoutsAna::ProcessFemtoPair() {
    switch (fKinematicsMode) {
      case EMode::kPRF: ComputePRF(); break;
      case EMode::kLCMS: ComputeLCMS(); break;
      case EMode::kGammaLCMS: ComputeLCMSGamma(); break;
      case EMode::kRaw: ComputeRaw(); break;
      default: break;
    }
  }

  void FemtoFreezeoutsAna::SetMomentumCut(Double_t cut) {
    if (fCut <= 0) {
      Cout::PrintInfo("KSTar cut shouldn't be <=0", EInfo::kLowWarning);
      return;
    }
    fCut = cut;
  }

  void FemtoFreezeoutsAna::SetOption(Option_t* opt) {
    TString option = opt;
    if (option.BeginsWith("background:")) {
      Cout::PrintInfo(Form("%s backgrounds are not supported", this->ClassName()), EInfo::kDebugInfo);
      return;
    } else if (option.EqualTo("prf")) {
      fKinematicsMode = EMode::kPRF;
    } else if (option.EqualTo("lcms")) {
      fKinematicsMode = EMode::kLCMS;
    } else if (option.EqualTo("lcmsG")) {
      fKinematicsMode = EMode::kGammaLCMS;
    } else if (option.EqualTo("use_true_momenta")) {
      fUseFakeMomenta = kFALSE;
    } else {
      TwoTrackAna::SetOption(opt);
    }
  }

  void FemtoFreezeoutsAna::AddCut(const Cut& cut, Option_t* opt) {
    if (cut.GetCollectionID() != 0 && cut.GetUpdateRatio() == ECutUpdate::kEvent) {
      Cout::PrintInfo("This class not work with more than 1 event cut collection", EInfo::kLowWarning);
      return;
    }
    if (cut.GetCollectionID() >= 2 && cut.GetUpdateRatio() == ECutUpdate::kTrack) {
      Cout::PrintInfo("This class not work with more than 1 track collections (2 "
                      "in case of non idental HBT",
                      EInfo::kLowWarning);
      return;
    }
    if (cut.GetUpdateRatio() == ECutUpdate::kTwoTrack) {
      if (cut.GetCollectionID() != 0) {
        Cout::PrintInfo("You are trying to add two track collection with tirgger "
                        ">0, it's not supported now, try add FemtoFastCut",
                        EInfo::kLowWarning);
      }
    }
    TwoTrackAna::AddCut(cut, opt);
  }

  Task::EInitFlag FemtoFreezeoutsAna::Init() {
    Task::EInitFlag prev = TwoTrackAna::Init();
    if (prev == Task::EInitFlag::kFATAL) return prev;
    fHistograms1d = new HistogramManager_3_1D<TH1D>();
    fHistograms3d = new HistogramManager_3_3D<TH3D>();
    std::vector<HistogramAxisConf> axis(3);
    if (fFastCut == NULL) { fFastCut = new FemtoFastCutVirtual(); }
    switch (fKinematicsMode) {
      case EMode::kPRF: {
        fFemtoPair = Femto::MakePair(Femto::EKinematics::kPRF, fUseFakeMomenta);
      } break;
      default: {
        fFemtoPair = Femto::MakePair(Femto::EKinematics::kLCMS, fUseFakeMomenta);
      } break;
    }
    TParticlePDG* pid1 = TDatabasePDG::Instance()->GetParticle(fPdg1);
    TParticlePDG* pid2 = TDatabasePDG::Instance()->GetParticle(fPdg2);
    if (pid1) { fFemtoPair->SetMass(pid1->Mass(), pid1->Mass()); }
    if (pid2) { fFemtoPair->SetMass(pid1->Mass(), pid2->Mass()); }
    fFemtoPair->Init(GetTaskID());
    fFastCut->Init(fFemtoPair);
    TString titles[5];
    switch (fKinematicsMode) {
      case EMode::kPRF: {
        titles[0] = "r^{*}_{out} [fm]";
        titles[1] = "r^{*}_{side}[fm]";
        titles[2] = "r^{*}_{long} [fm]";
        titles[3] = "r^{*} [fm]";
      } break;
      case EMode::kLCMS: {
        titles[0] = "r_{out} [fm]";
        titles[1] = "r_{side}[fm]";
        titles[2] = "r_{long} [fm]";
        titles[3] = "r [fm]";
      } break;
      case EMode::kGammaLCMS: {
        titles[0] = "r_{out #gamma} [fm]";
        titles[1] = "r_{side}[fm]";
        titles[2] = "r_{long} [fm]";
        titles[3] = "r [fm]";
      } break;
      case EMode::kRaw: {
        titles[0] = "#DeltaX [fm]";
        titles[1] = "#DeltaY[fm]";
        titles[2] = "#DeltaZ [fm]";
        titles[3] = "#DeltaR [fm]";
      } break;
    }
    titles[4] = "N_{pairs}";
    for (int i = 0; i < 3; i++)
      axis[i] = HistogramAxisConf(titles[i], fBins[i], fHistoMin[i], fHistoMax[i]);
    std::vector<HistogramAxisConf> axis2(2);
    axis2[0] = HistogramAxisConf(titles[3], fBins[0], fHistoMin[0], fHistoMax[0]);
    axis2[1] = HistogramAxisConf(titles[4]);


    fHistograms1d->Init(fEventCollectionsNo, fTwoTrackCollectionsNo, fFastCut->GetNBins(), axis2, "Freezeouts1d", kFALSE);
    fHistograms3d->Init(fEventCollectionsNo, fTwoTrackCollectionsNo, fFastCut->GetNBins(), axis, "Freezeouts3d", kFALSE);
    return Task::EInitFlag::kSUCCESS;
  }

  Package* FemtoFreezeoutsAna::Report() const {
    Package* pack = TwoTrackAna::Report();
    if (fKinematicsMode == EMode::kPRF) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "PRF"));
    } else if (fKinematicsMode == EMode::kLCMS) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "LCMS"));
    } else if (fKinematicsMode == EMode::kGammaLCMS) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "LCMS+Gamma"));
    }
    if (fFreezeoutGenerator) {
      AddToAnaMetadata(pack, new ParameterString("FreezeoutGenerator", "Enabled"));
      AddToAnaMetadata(pack, fFreezeoutGenerator->Report());
    } else {
      AddToAnaMetadata(pack, new ParameterString("FreezeoutGenerator", "Disabled"));
    }
    if (fPdg1 != 0) {
      AddToAnaMetadata(pack, new ParameterInt("Assumed pdg_{1}", fPdg1));
      AddToAnaMetadata(pack, new ParameterInt("Assumed pdg_{2}", fPdg2));
    }
    AddToAnaMetadata(pack, new ParameterBool("IgnoreSign", fIgnoreSign));
    TList* list = fHistograms1d->GetFlatList();
    for (int i = 0; i < list->GetEntries(); i++) {
      pack->AddObject(list->At(i));
    }
    delete list;
    list = fHistograms3d->GetFlatList();
    for (int i = 0; i < list->GetEntries(); i++) {
      pack->AddObject(list->At(i));
    }
    delete list;
    AddToAnaMetadata(pack, fFastCut->Report());
    AddToAnaMetadata(pack, new ParameterDouble("freez_cut", fCut));
    return pack;
  }

  void FemtoFreezeoutsAna::ProcessPair() {
    fFemtoPair->Build(fCurrentSignalPair);
    PreprocessFemtoPair();
    ProcessFemtoPair();
  }

  void FemtoFreezeoutsAna::PreprocessFemtoPair() {
    if (fFreezeoutGenerator) fFreezeoutGenerator->GenerateFreezeoutCooordinates(fFemtoPair);
  }

  FemtoFreezeoutsAna::~FemtoFreezeoutsAna() {
    delete fHistograms1d;
    delete fHistograms3d;
    delete fFastCut;
  }

  void FemtoFreezeoutsAna::SetAxes(Int_t bins, Double_t min, Double_t max) {
    for (int i = 0; i < 3; i++) {
      fBins[i]     = bins;
      fHistoMin[i] = min;
      fHistoMax[i] = max;
    }
  }

  void FemtoFreezeoutsAna::SetOutAxis(Int_t bins, Double_t min, Double_t max) {
    const Int_t i = 0;
    fBins[i]      = bins;
    fHistoMin[i]  = min;
    fHistoMax[i]  = max;
  }

  void FemtoFreezeoutsAna::SetSideAxis(Int_t bins, Double_t min, Double_t max) {
    const Int_t i = 1;
    fBins[i]      = bins;
    fHistoMin[i]  = min;
    fHistoMax[i]  = max;
  }

  void FemtoFreezeoutsAna::SetLongAxis(Int_t bins, Double_t min, Double_t max) {
    const Int_t i = 2;
    fBins[i]      = bins;
    fHistoMin[i]  = min;
    fHistoMax[i]  = max;
  }
}  // namespace Hal
