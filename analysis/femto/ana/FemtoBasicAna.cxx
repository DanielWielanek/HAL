/*
 * FemtoBasicAna2.cxx
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoBasicAna.h"

#include "Cout.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "DataFormatManager.h"
#include "DividedHisto.h"
#include "Event.h"
#include "EventAna.h"
#include "EventBinningCut.h"
#include "EventVirtualCut.h"
#include "FemtoConst.h"
#include "FemtoPair.h"
#include "MemoryMapManager.h"
#include "ObjectMatrix.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"

#include <TClonesArray.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TString.h>


namespace Hal {
  FemtoBasicAna::FemtoBasicAna() :
    TwoTrackAna(kTRUE),
    fPdg1(0),
    fPdg2(0),
    fUseImgMomenta(kFALSE),
    fIgnoreSign(kFALSE),
    fFsiWeight(0.0),
    fFemtoPair(nullptr),
    fCalc(nullptr),
    fFreezoutGenerator(nullptr),
    fCFs(nullptr),
    fCFTemp(nullptr) {
    fMixSize        = 2;
    fBackgroundMode = kMixedPairs;
    AddTags("femto hbt");
  }

  FemtoBasicAna::FemtoBasicAna(const FemtoBasicAna& ana) :
    TwoTrackAna(ana),
    fPdg1(ana.fPdg1),
    fPdg2(ana.fPdg2),
    fUseImgMomenta(ana.fUseImgMomenta),
    fIgnoreSign(ana.fIgnoreSign),
    fFsiWeight(ana.fFsiWeight),
    fFemtoPair(nullptr),
    fCalc(ana.fCalc),
    fFreezoutGenerator(nullptr) {
    if (ana.fCFs) { fCFs = (ObjectMatrix_2*) ana.fCFs->Clone(); }
    if (ana.fCFTemp) { fCFTemp = (FemtoCorrFunc*) ana.fCFTemp->Clone(); }
    if (ana.fFreezoutGenerator) { fFreezoutGenerator = ana.fFreezoutGenerator->MakeCopy(); }
    if (ana.fFemtoPair) { fFemtoPair = (FemtoPair*) ana.fFemtoPair->Clone(); }
    if (ana.fCalc) { fCalc = (FemtoWeightGenerator*) ana.fCalc->Clone(); }
  }

  Package* FemtoBasicAna::Report() const {
    Package* pack           = TwoTrackAna::Report();
    Femto::EKinematics mode = Femto::EKinematics::kLCMS;
    mode                    = fFemtoPair->GetFrame();
    if (mode == Femto::EKinematics::kPRF) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "PRF"));
    } else if (mode == Femto::EKinematics::kLCMS) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "LCMS"));
    } else if (mode == Femto::EKinematics::kSH_LCMS) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "LCMS+SH"));
    } else if (mode == Femto::EKinematics::kSH_PRF) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "PRF+SH"));
    } else if (mode == Femto::EKinematics::kPRFL) {
      AddToAnaMetadata(pack, new ParameterString("FemtoKinematics", "PRFL"));
    }
    if (fFreezoutGenerator) {
      AddToAnaMetadata(pack, new ParameterString("FreezoutGenerator", "Enabled"));
      AddToAnaMetadata(pack, fFreezoutGenerator->Report());
    } else {
      AddToAnaMetadata(pack, new ParameterString("FreezoutGenerator", "Disabled"));
    }
    if (fCalc) AddToAnaMetadata(pack, fCalc->Report());
    if (fPdg1 != 0) {
      AddToAnaMetadata(pack, new ParameterInt("Assumed Pdg_{1}", fPdg1));
      AddToAnaMetadata(pack, new ParameterInt("Assumed Pdg_{2}", fPdg2));
    }
    AddToAnaMetadata(pack, new ParameterBool("IgnoreSign", fIgnoreSign));
    if (fCFs) {
      for (int j = 0; j < fCFs->GetSize(); j++) {
        ObjectMatrix_1* cfx_col = fCFs->Get(j);
        for (int k = 0; k < cfx_col->GetSize(); k++) {
          pack->AddObject(cfx_col->At(k));
        }
      }
    }
    return pack;
  }

  Task::EInitFlag FemtoBasicAna::Init() {
    if (fPdg1 == 0 || fPdg2 == 0) {
      Cout::PrintInfo("Assumed wrong PID = 0, pion PID will be assumed", EInfo::kLowWarning);
      fPdg1 = fPdg2 = 211;
    }
    if (fPdg1 != fPdg2) { EnableNonIdentical(); }
    Task::EInitFlag prev = TwoTrackAna::Init();
    if (fCutContainer->GetTwoTrackCollectionsNo() != fCutContainer->GetTwoTrackCollectionsBackgroundNo()) {
      Cout::PrintInfo("Two track collectionsNo in signal and background are different - this might result in crash",
                      EInfo::kWarning);
    }
    const Event* ev = DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kBuffered);
    if (!ev->InheritsFrom("Hal::ComplexEvent") && fUseImgMomenta == kTRUE) {
      Cout::PrintInfo("Can't use fake momenta with current format !", EInfo::kLowWarning);
      fUseImgMomenta = kFALSE;
    }
    if (fCFTemp == nullptr) {
      Cout::PrintInfo("No correlation function in analysis", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    DividedHisto1D* dummy    = fCFTemp->GetCF(0);
    Femto::EKinematics kinem = Femto::EKinematics::kLCMS;
    if (dummy->GetLabelsNo() > 0) {
      TString label = dummy->GetLabel(0);
      kinem         = Femto::LabelToKinematics(label);
    }
    fFemtoPair = Femto::MakePair(kinem, fUseImgMomenta);
    if (fFemtoPair == nullptr) {
      Cout::PrintInfo("Something wrong is with FemtoPair it cannot be created", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }

    fFemtoPair->SetPdg1(fPdg1);
    fFemtoPair->SetPdg2(fPdg2);
    fFemtoPair->Init(GetTaskID());
    if (fIgnoreSign) {
      if (dummy->InheritsFrom("Hal::FemtoSHCF")) {
        Cout::PrintInfo("Cannot ignore sign with SH correlation function!", EInfo::kError);
      } else
        fFemtoPair->UseAbs();  // if spherical harmonics do not use abs
    }
    AddTags(fFemtoPair->GetTags());
    if (fUseImgMomenta == kFALSE) { Cout::PrintInfo("Only true momenta will be used", EInfo::kDebugInfo); }
    if (prev != Task::EInitFlag::kSUCCESS) {
      Cout::PrintInfo("Fatal FemtoBasicAna::TwoTrackAna::Init", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    // check weights
    if (fCalc == nullptr) {
      Cout::PrintInfo("Weight algorithm not configured, autoconfiguration", EInfo::kLowWarning);
      fCalc = new FemtoWeightGenerator();
    }
    if (fCalc->Init(GetTaskID(), fFemtoPair) == kFALSE) {
      Cout::PrintInfo("Weight algorithm not initialized correctly", EInfo::kLowWarning);
    }
    // inilitalize array of histograms
    if (InitArray() == kFALSE) {
      Cout::PrintInfo(Form("Failed to init array in %s", ClassName()), EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    if (fCFTemp) delete fCFTemp;
    fCFTemp = nullptr;
    // check freezout generator
    if (fFreezoutGenerator) {
      fFreezoutGenerator->Init();
      AddTags("freezout");
    }
    // check fast cuts -------------------------------------
    return prev;
  }

  void FemtoBasicAna::FinishTask() { TwoTrackAna::FinishTask(); }

  void FemtoBasicAna::SetOption(Option_t* option) {
    TString opt = option;
    if (opt.EqualTo("ignore_sign")) {
      fIgnoreSign = kTRUE;
    } else if (opt.EqualTo("use_im_momenta")) {
      fUseImgMomenta = kTRUE;
    } else if (opt.EqualTo("background:no")) {
      Cout::PrintInfo("This analysis must have background !", EInfo::kLowWarning);
      return;
    } else {
      TwoTrackAna::SetOption(option);
    }
  }

  void FemtoBasicAna::ProcessPair() {
    fFemtoPair->Build(fCurrentSignalPair);
    PreprocessFemtoPair();
    ProcessFemtoPair();
  }

  void FemtoBasicAna::ProcessPair_Perfect() {
    fFemtoPair->Build(fCurrentBackgroundPair);
    PreprocessFemtoPair_Perfect();
    ProcessFemtoPair_Perfect();
  }

  void FemtoBasicAna::ProcessPair_Rotated() {
    fFemtoPair->Build(fCurrentBackgroundPair);
    PreprocessFemtoPair_Rotated();
    ProcessFemtoPair_Rotated();
  }

  void FemtoBasicAna::ProcessPair_Hemisphere() {
    fFemtoPair->Build(fCurrentBackgroundPair);
    PreprocessFemtoPair_Hemisphere();
    ProcessFemtoPair_Hemisphere();
  }

  void FemtoBasicAna::ProcessPair_Mixed() {
    fFemtoPair->Build(fCurrentBackgroundPair);
    PreprocessFemtoPair_Mixed();
    ProcessFemtoPair_Mixed();
  }

  void FemtoBasicAna::PreprocessFemtoPair() {
    if (fFreezoutGenerator) fFreezoutGenerator->GenerateFreezoutCooordinates(fFemtoPair);
  }

  void FemtoBasicAna::ProcessPair_ChargedId() {
    fFemtoPair->Build(fCurrentBackgroundPair);
    PreprocessFemtoPair_Charged();
    ProcessFemtoPair_Charged();
  }

  void FemtoBasicAna::ProcessFemtoPair() {
    fFemtoPair->Compute();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillNum(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Perfect() {
    fFemtoPair->Compute();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenPerfect(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenRotated(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenHemisphere(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute_Mixed();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenMixed(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Charged() {
    fFemtoPair->Compute_Charged();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenCharged(fFemtoPair);
  }

  void FemtoBasicAna::CheckCutContainerCollections() {
    TwoTrackAna::CheckCutContainerCollections();
    Int_t two_track_signal     = fCutContainer->GetTwoTrackCollectionsNo();
    Int_t two_track_background = fCutContainer->GetTwoTrackCollectionsBackgroundNo();
    if (fBackgroundMode != kNoBackgroundID && fBackgroundMode != kNoBackgroundNID) {
      if (two_track_signal != two_track_background) {
#ifdef HAL_DEBUG
        Cout::PrintInfo("Different number of two-track collections in signal and background, fixing", EInfo::kDebugInfo);
#endif
      }
      if (two_track_signal > two_track_background) {
        for (int i = two_track_background; i < two_track_signal; i++) {
          fCutContainer->RemoveCollection(ECutUpdate::kTwoTrack, i);
        }
      } else if (two_track_background > two_track_signal) {
        for (int i = two_track_signal; i < two_track_background; i++) {
          fCutContainer->RemoveCollection(ECutUpdate::kTwoTrackBackground, i);
        }
      }
    }
    fTwoTrackCollectionsNo           = fCutContainer->GetTwoTrackCollectionsNo();
    fTwoTrackCollectionsNoBackground = fCutContainer->GetTwoTrackCollectionsBackgroundNo();
  }

  void FemtoBasicAna::SetCorrFctn(FemtoCorrFunc* h) {
    if (fCFTemp) delete fCFTemp;
    fCFTemp = (FemtoCorrFunc*) h->Clone();
  }

  void FemtoBasicAna::SetCorrFctn(const FemtoCorrFunc& h) {
    if (fCFTemp) delete fCFTemp;
    fCFTemp = (FemtoCorrFunc*) h.Clone();
  }

  FemtoBasicAna::~FemtoBasicAna() {
    delete fCalc;
    if (fCFs) delete fCFs;
    if (fFreezoutGenerator) delete fFreezoutGenerator;
    delete fFemtoPair;
    if (fCFTemp) delete fCFTemp;
  }

  void FemtoBasicAna::InitMemoryMap() {
    fMemoryMap = new MemoryMapManager(fCutContainer);
    fMemoryMap->SetMixSize(fMixSize);
    std::vector<TString> brName;
    if (TESTBIT(fFormatOption, eBitFormat::kReader)) {
      brName.push_back("HalEvent.");
    } else if (TESTBIT(fFormatOption, eBitFormat::kDirectAcesss)) {
      TString evName = DataFormatManager::Instance()->GetFormat(GetTaskID())->ClassName();
      brName.push_back(Form("%s.", evName.Data()));
      brName.push_back(evName);
    }
    fMemoryMap->Init(1, GetTaskID(), TESTBIT(fFormatOption, eBitFormat::kCompression), brName);
  }

  void FemtoBasicAna::ProcessEvent() {
    fMemoryMap->PrepareMaps(fCurrentEventCollectionID);
    CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
    for (fTrackIndex = 0; fTrackIndex < fMemoryMap->GetTemporaryTotalTracksNo(); fTrackIndex++) {
      fCurrentTrack = fCurrentEvent->GetTrack(fTrackIndex);
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) {
          fMemoryMap->AddTrackToMapTrack(fCurrentEventCollectionID,
                                         fCurrentTrackCollectionID,
                                         fTrackIndex);  // load track into memory map - may be usefull at
                                                        // finish event
          ProcessTrack();
        }
      }
    }
    fMemoryMap->BufferEvent(fCurrentEventCollectionID);
    fCurrentTrackCollectionID = 0;
    if (IdenticalParticles()) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Finish identical event with %i tracks ",
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID)),
                      EInfo::kDebugInfo);
#endif
      FinishEventIdentical();
    } else {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Finish identical event with %i  %itracks ",
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID),
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 1)),
                      EInfo::kDebugInfo);
#endif
      FinishEventNonIdentical();
    }
  }

  Bool_t FemtoBasicAna::InitArray() {
    fCFs              = new ObjectMatrix_2();
    DividedHisto1D* h = ((FemtoCorrFunc*) fCFTemp)->GetCF(0);
    TString name      = h->GetName();
    name.ReplaceAll("[0]", "");
    h->SetName(name);
    fCFs->Init(fEventCollectionsNo, fTwoTrackCollectionsNo, fCFTemp);
    for (int i = 0; i < fEventCollectionsNo; i++) {
      for (int j = 0; j < fTwoTrackCollectionsNo; j++) {
        FemtoCorrFunc* corrfunc = (FemtoCorrFunc*) fCFs->At(i, j);
        corrfunc->SetEventCollID(i);
        corrfunc->SetPairCollID(j);
        corrfunc->Check();
      }
    }
    return kTRUE;
  }

  FemtoBasicAna& FemtoBasicAna::operator=(const FemtoBasicAna& other) {
    if (this != &other) {
      TwoTrackAna::operator=(other);
      if (fFemtoPair) {
        delete fFemtoPair;
        fFemtoPair = nullptr;
      }
      if (fCalc) {
        delete fCalc;
        fCalc = nullptr;
      }
      if (fFreezoutGenerator) {
        delete fFreezoutGenerator;
        fFreezoutGenerator = nullptr;
      }
      if (fCFs) {
        delete fCFs;
        fCFs = nullptr;
      }
      if (fCFTemp) {
        delete fCFTemp;
        fCFTemp = nullptr;
      }
      if (other.fFemtoPair) fFemtoPair = (FemtoPair*) other.fFemtoPair->Clone();
      if (other.fCalc) fCalc = (FemtoWeightGenerator*) other.fCalc->Clone();
      if (other.fFreezoutGenerator) fFreezoutGenerator = (FemtoFreezoutGenerator*) other.fFreezoutGenerator->Clone();
      if (other.fCFTemp) fCFTemp = (FemtoCorrFunc*) other.fCFTemp;
      if (other.fCFs) fCFs = (ObjectMatrix_2*) other.Clone();

      fPdg1          = other.fPdg1;
      fPdg2          = other.fPdg2;
      fUseImgMomenta = other.fUseImgMomenta;
      fIgnoreSign    = other.fIgnoreSign;
      fFsiWeight     = other.fFsiWeight;
    }
    return *this;
  }
}  // namespace Hal
