/*
 * CorrFitDumpedPairAnaHorizontal.cxx
 *
 *  Created on: 1 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitDumpedPairAnaHorizontal.h"

#include <TApplication.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include <TCollection.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TKey.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TSystem.h>
#include <TTree.h>
#include <cstdlib>
#include <fstream>
#include <string>

#include "Array.h"
#include "CorrFitInfo.h"
#include "CorrFitParamsSetup.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoConst.h"
#include "FemtoCorrFunc.h"
#include "FemtoCorrFuncSimple.h"
#include "FemtoFreezoutGenerator.h"
#include "FemtoMiniPair.h"
#include "FemtoPair.h"
#include "FemtoSHCF.h"
#include "FemtoSourceModel.h"
#include "FemtoWeightGenerator.h"
#include "FemtoWeightGeneratorLednicky.h"
#include "XMLNode.h"

namespace Hal {

  CorrFitDumpedPairAnaHorizontal::CorrFitDumpedPairAnaHorizontal(Int_t jobid, Int_t maps_perAna) :
    CorrFitDumpedPairAna(jobid, maps_perAna), fSlices(nullptr) {}

  Bool_t CorrFitDumpedPairAnaHorizontal::Connect() {
    TBranch* branchSignal      = fTree->GetBranch("FemtoSignal.");
    TBranch* branchBackkground = fTree->GetBranch("FemtoBackground.");
    switch (fMode) {
      case eDumpCalcMode::kSignalPairs: {
        fPairsSignal = new TClonesArray("Hal::FemtoMicroPair");
        if (branchSignal == nullptr) return kFALSE;
        branchSignal->SetAddress(&fPairsSignal);
      } break;
      case eDumpCalcMode::kBackgroundPairsOnly: {
        fPairsBackground = new TClonesArray("Hal::FemtoMicroPair");
        if (branchBackkground == nullptr) return kFALSE;
        branchBackkground->SetAddress(&fPairsBackground);
      } break;
      case eDumpCalcMode::kSignalBackgroundPairs: {
        fPairsSignal     = new TClonesArray("Hal::FemtoMicroPair");
        fPairsBackground = new TClonesArray("Hal::FemtoMicroPair");
        if (branchSignal == nullptr || branchBackkground == nullptr) return kFALSE;
        branchSignal->SetAddress(&fPairsSignal);
        branchBackkground->SetAddress(&fPairsBackground);
      } break;
    }
    return kTRUE;
  }

  Bool_t CorrFitDumpedPairAnaHorizontal::Init() {
    Bool_t preinit = CorrFitDumpedPairAna::Init();
    fCF            = new FemtoCorrFunc*[fMultiplyJobs];
    for (int i = 0; i < fMultiplyJobs; i++) {
      fCF[i] = fTempCF->Clone();
    }
    delete fTempCF;
    fTempCF = nullptr;

    if (fGenerator) {
      for (int i = 0; i < fMultiplyJobs; i++) {
        fGenerator[i]->Init();
      }
    }
    return kTRUE;
  }

  void CorrFitDumpedPairAnaHorizontal::Finish() {
    TObject* obj = nullptr;

    for (int iJob = 0; iJob < fMultiplyJobs; iJob++) {
      obj = fCF[iJob]->GetCF(0);
      if (obj->InheritsFrom("Hal::Femto1DCF")) {
        RootExport1D(static_cast<Femto1DCF*>(obj), iJob);
      } else if (obj->InheritsFrom("Hal::Femto3DCF")) {
        RootExport3D(static_cast<Femto3DCF*>(obj), iJob);
      } else if (obj->InheritsFrom("Hal::FemtoSHCF")) {
        RootExportSH(static_cast<FemtoSHCF*>(obj), iJob);
      } else {
        Cout::Text("Not supported CF", "L", kRed);
      }
    }
    if (fJobId == 0) {
      TFile* file       = new TFile("files/config.root", "recreate");
      CorrFitInfo* info = new CorrFitInfo();
      // TODO don't forget about vertical flag !
      std::ifstream macro;
      macro.open("ana.C");
      std::string line;
      TString val = "";
      while (std::getline(macro, line)) {
        TString l = line;
        l         = l + "\n";
        val       = val + l;
      }
      macro.close();
      info->SetMacroText(val);
      info->SetPairFile(fPairFile);
      info->SetCf(obj);
      CorrFitParamsSetup* setup = new CorrFitParamsSetup("corrfit_conf.xml");
      info->SetSetup(*setup);
      info->Write();
      file->Close();
    }
  }

  void CorrFitDumpedPairAnaHorizontal::RunSignalPairs(Int_t nEvents) {
    Int_t step = nEvents / 100;
    for (int iEvent = 0; iEvent < nEvents; iEvent++) {
      fTree->GetEntry(iEvent);
      if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }
      for (int jPair = 0; jPair < fPairsSignal->GetEntriesFast(); jPair++) {
        fMiniPair = (FemtoMicroPair*) fPairsSignal->UncheckedAt(jPair);
        *fPair    = *fMiniPair;
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessPair();
          fPair->Compute();
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            for (int weightMulti = 0; weightMulti < fMultiplyWeight; weightMulti++) {
              fGenerator[nJobs]->GenerateFreezoutCooordinates(fPair);
              fPair->SetWeight(fWeight->GenerateWeight(fPair));
              fCF[nJobs]->FillNum(fPair);
              fPair->SetWeight(1.0);
              fCF[nJobs]->FillDenMixed(fPair);
            }
          }
        }
      }
    }
  }

  void CorrFitDumpedPairAnaHorizontal::RunSignalBackgroundPairs(Int_t nEvents) {
    Int_t step = nEvents / 100;
    for (int iEvent = 0; iEvent < nEvents; iEvent++) {
      fTree->GetEntry(iEvent);
      if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }

      for (int jSig = 0; jSig < fPairsSignal->GetEntriesFast(); jSig++) {
        fMiniPair = (FemtoMicroPair*) fPairsSignal->UncheckedAt(jSig);
        *fPair    = *fMiniPair;
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessPair();
          fPair->Compute();
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            for (int weightPair = 0; weightPair < fMultiplyWeight; weightPair++) {
              fGenerator[nJobs]->GenerateFreezoutCooordinates(fPair);
              fPair->SetWeight(fWeight->GenerateWeight(fPair));
              fCF[nJobs]->FillNum(fPair);
            }
          }
        }
      }

      for (int jMix = 0; jMix < fPairsBackground->GetEntriesFast(); jMix++) {
        fMiniPair = (FemtoMicroPair*) fPairsBackground->UncheckedAt(jMix);
        *fPair    = *fMiniPair;
        fPair->SetWeight(1.0);
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessMixedPair();
          fPair->Compute();
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            fCF[nJobs]->FillDenMixed(fPair);
          }
        }
      }
    }
  }

  void CorrFitDumpedPairAnaHorizontal::RunBackgroundPairs(Int_t nEvents) {
    Int_t step = nEvents / 100;
    for (int iEvent = 0; iEvent < nEvents; iEvent++) {
      fTree->GetEntry(iEvent);
      if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }

      for (int jMix = 0; jMix < fPairsBackground->GetEntriesFast(); jMix++) {
        fMiniPair = (FemtoMicroPair*) fPairsBackground->UncheckedAt(jMix);
        *fPair    = *fMiniPair;
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessPair();
          fPair->Compute();
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            for (int weightPair = 0; weightPair < fMultiplyWeight; weightPair++) {
              fGenerator[nJobs]->GenerateFreezoutCooordinates(fPair);
              fPair->SetWeight(fWeight->GenerateWeight(fPair));
              fCF[nJobs]->FillNum(fPair);
            }
          }
          fPair->SetWeight(1.0);
          PreprocessMixedPair();
          fPair->Compute();
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            for (int weightPair = 0; weightPair < fMultiplyWeight; weightPair++) {
              fCF[nJobs]->FillDenMixed(fPair);
            }
          }
        }
      }
    }
  }

  CorrFitDumpedPairAnaHorizontal::~CorrFitDumpedPairAnaHorizontal() {}

} /* namespace Hal */
