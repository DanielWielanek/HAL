/*
 * FemtoBasicAna2.cxx
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoBasicAna.h"

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
#include "HalStd.h"
#include "MemoryMapManager.h"
#include "ObjectMatrix.h"
#include "Package.h"
#include "Parameter.h"

#include <FairLogger.h>
#include <TClonesArray.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TString.h>


namespace Hal {
  FemtoBasicAna::FemtoBasicAna() :
    TwoTrackAna(),
    fPdg1(0),
    fPdg2(0),
    fUseImgMomenta(kFALSE),
    fIgnoreSign(kFALSE),
    fFsiWeight(0.0),
    fEventCollectionCF(0),
    fFemtoPair(nullptr),
    fCalc(nullptr),
    fFreezoutGenerator(nullptr),
    fCFs(nullptr),
    fCFTemp(nullptr) {
    fMixSize        = 2;
    fTiers          = ECutUpdate::kTwoTrackBackgroundUpdate;
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
    fEventCollectionCF(ana.fEventCollectionCF),
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
    Task::EInitFlag prev = TwoTrackAna::Init();
    if (fCutContainer->GetTwoTrackCollectionsNo() != fCutContainer->GetTwoTrackCollectionsBackgroundNo()) {
      LOG(WARNING) << "Two track collectionsNo in signal and background are "
                      "different - this might result in crash";
    }
    const Event* ev = DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kBuffered);
    if (!ev->InheritsFrom("ComplexEvent") && fUseImgMomenta == kTRUE) {
      LOG(WARNING) << "Can't use fake momenta with current format !";
      fUseImgMomenta = kFALSE;
    }
    if (fCFTemp == nullptr) {
      LOG(ERROR) << "No correlation function in analysis";
      return Task::EInitFlag::kFATAL;
    }
    DividedHisto1D* dummy    = fCFTemp->GetCF(0);
    Femto::EKinematics kinem = Femto::EKinematics::kLCMS;
    if (dummy->GetLabelsNo() > 0) {
      TString label = dummy->GetLabel(0);
      kinem         = Femto::LabelToKinematics(label);
    }
    fFemtoPair = Femto::MakePair(kinem, fUseImgMomenta);
    if (fFemtoPair == nullptr) { LOG(FATAL) << "Something wrong is with FemtoPair it cannot be created"; }
    if (fPdg1 == 0 || fPdg2 == 0) {
      LOG(WARNING) << "Assumed wrong PID = 0, pion PID will be assumed";
      fPdg1 = fPdg2 = 211;
    }
    if (fPdg1 != fPdg2) { EnableNonIdentical(); }
    fFemtoPair->SetPdg1(fPdg1);
    fFemtoPair->SetPdg2(fPdg2);
    fFemtoPair->Init(GetTaskID());
    if (fIgnoreSign) fFemtoPair->UseAbs();
    AddTags(fFemtoPair->GetTags());
    if (fUseImgMomenta == kFALSE) { LOG(INFO) << "Only true momenta will be used"; }
    if (prev != Task::EInitFlag::kSUCCESS) {
      LOG(FATAL) << "Fatal FemtoBasicAna::TwoTrackAna::Init";
      return Task::EInitFlag::kFATAL;
    }
    // check weights
    if (fCalc == nullptr) {
      LOG(WARNING) << "Weight algorithm not configured, autoconfiguration";
      fCalc = new FemtoWeightGenerator();
    }
    if (fCalc->Init(GetTaskID(), fFemtoPair) == kFALSE) { LOG(WARNING) << "Weight algorithm not initialized correctly"; }
    // inilitalize array of histograms
    if (InitArray() == kFALSE) return Task::EInitFlag::kFATAL;
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
      LOG(WARNING) << "This analysis must have background !";
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
    ((FemtoCorrFunc*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID))->FillNum(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Perfect() {
    fFemtoPair->Compute();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID))->FillDenPerfect(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID))->FillDenRotated(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID))->FillDenHemisphere(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute_Mixed();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID))->FillDenMixed(fFemtoPair);
  }

  void FemtoBasicAna::ProcessFemtoPair_Charged() {
    fFemtoPair->Compute_Charged();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID))->FillDenCharged(fFemtoPair);
  }

  void FemtoBasicAna::CheckCutContainerCollections() {
    TwoTrackAna::CheckCutContainerCollections();
    Int_t two_track_signal     = fCutContainer->GetTwoTrackCollectionsNo();
    Int_t two_track_background = fCutContainer->GetTwoTrackCollectionsBackgroundNo();
    if (fBackgroundMode != kNoBackgroundID && fBackgroundMode != kNoBackgroundNID) {
      if (two_track_signal != two_track_background) {
        LOG(DEBUG) << "Different number of two-track collections in signal and "
                      "background, fixing";
      }
      if (two_track_signal > two_track_background) {
        for (int i = two_track_background; i < two_track_signal; i++) {
          fCutContainer->RemoveCollection(ECutUpdate::kTwoTrackUpdate, i);
        }
      } else if (two_track_background > two_track_signal) {
        for (int i = two_track_signal; i < two_track_background; i++) {
          fCutContainer->RemoveCollection(ECutUpdate::kTwoTrackBackgroundUpdate, i);
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
    LOG(DEBUG3) << "Initialization MemoryMap";
    fMemoryMap->Init(1, GetTaskID(), fKeepSource, fCompressEvents, fDirectAcces);
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
    fEventCollectionCF = fCurrentEventCollectionID;
    fMemoryMap->BufferEvent(fCurrentEventCollectionID);
    fCurrentTrackCollectionID = 0;
    if (IdenticalParticles()) {
      LOG(DEBUG) << "Finish identical event with "
                 << fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID) << " tracks";
      FinishEventIdentical();
    } else {
      LOG(DEBUG) << "Finish non-identical event with " << fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 0) << " "
                 << fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 1) << " tracks";
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
      fEventCollectionCF = other.fEventCollectionCF;
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
