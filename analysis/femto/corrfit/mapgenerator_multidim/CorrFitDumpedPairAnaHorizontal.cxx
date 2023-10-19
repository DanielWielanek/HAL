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
#include "CorrFitDumpedPairAna.h"
#include "CorrFitInfo.h"
#include "CorrFitParamsSetup.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoConst.h"
#include "FemtoCorrFunc.h"
#include "FemtoCorrFuncSimple.h"
#include "FemtoDumpPairAna.h"
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
    CorrFitDumpedPairAna(jobid, maps_perAna) {}

  Bool_t CorrFitDumpedPairAnaHorizontal::Init() {
    Bool_t preinit = CorrFitDumpedPairAna::Init();
    return preinit;
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
      for (auto clones : fSignalClones) {
        for (int jPair = 0; jPair < clones->GetEntriesFast(); jPair++) {
          fMiniPair = (FemtoMicroPair*) clones->UncheckedAt(jPair);
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
  }

  void CorrFitDumpedPairAnaHorizontal::RunSignalBackgroundPairs(Int_t nEvents) {
    Int_t step = nEvents / 100;
    for (int iEvent = 0; iEvent < nEvents; iEvent++) {
      fTree->GetEntry(iEvent);
      if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }

      for (auto clones : fSignalClones) {
        for (int jSig = 0; jSig < clones->GetEntriesFast(); jSig++) {
          fMiniPair = (FemtoMicroPair*) clones->UncheckedAt(jSig);
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
      }
      for (auto clones : fBackgroundClones) {
        for (int jSig = 0; jSig < clones->GetEntriesFast(); jSig++) {
          fMiniPair = (FemtoMicroPair*) clones->UncheckedAt(jSig);
          *fPair    = *fMiniPair;
          for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
            PreprocessPair();
            fPair->Compute();
            for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
              fPair->SetWeight(1 * fMultiplyWeight);
              fCF[nJobs]->FillNum(fPair);
            }
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
      for (auto clones : fSignalClones) {
        for (int jMix = 0; jMix < clones->GetEntriesFast(); jMix++) {
          fMiniPair = (FemtoMicroPair*) clones->UncheckedAt(jMix);
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
            fPair->SetWeight(1.0 * fMultiplyJobs);
            PreprocessMixedPair();
            fPair->Compute();
            for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
              fCF[nJobs]->FillDenMixed(fPair);
            }
          }
        }
      }
    }
  }

  Bool_t CorrFitDumpedPairAnaHorizontal::InitGenerators(const std::vector<int>& dims,
                                                        XMLNode* parameters,
                                                        const CorrFitParamsSetup& setup) {
    for (int j = 0; j < fMultiplyJobs; j++) {
      fGenerator.push_back(fTempGenerator->MakeCopy());
      FemtoSourceModel* freez = fGenerator[j]->GetSourceModel();
      std::vector<int> arPos  = Hal::Std::OneToMultiDimIndex(dims, fJobId * fMultiplyJobs + j);
      for (int i = 0; i < parameters->GetNChildren(); i++) {
        XMLNode* parameter = parameters->GetChild(i);
        Double_t val       = setup.GetMin(i) + setup.GetStepSize(i) * ((Double_t) arPos[i]);
        freez->SetParameterByName(setup.GetParName(i), val);
        freez->Init();
      }
    }
    return kTRUE;
  }

  Bool_t CorrFitDumpedPairAnaHorizontal::ConnectToData() {
    Int_t bins;
    Double_t min, max;
    Hal::Std::GetAxisPar(*fCF[0]->GetCF(0)->GetNum(), bins, min, max, "x");
    switch (fMode) {
      case eDumpCalcMode::kSignalPairs: {
        auto vec = fGrouping->GetBranches(min, max, true);
        ConnectToSignal(vec);
      } break;
      case eDumpCalcMode::kBackgroundPairsOnly: {
        auto vec = fGrouping->GetBranches(min, max, false);
        ConnectToBackground(vec);
      } break;
      case eDumpCalcMode::kSignalBackgroundPairs: {
        auto vec = fGrouping->GetBranches(min, max, false);
        ConnectToBackground(vec);
        vec = fGrouping->GetBranches(min, max, true);
        ConnectToSignal(vec);
      } break;
    }
    return kTRUE;
  }

  Bool_t CorrFitDumpedPairAnaHorizontal::InitCFs() {
    for (int i = 0; i < fMultiplyJobs; i++) {
      fCF.push_back((FemtoCorrFunc*) fTempCF->Clone());
    }
    if (fCF.size() != 0) return kTRUE;
    return kFALSE;
  }

  CorrFitDumpedPairAnaHorizontal::~CorrFitDumpedPairAnaHorizontal() {
    for (auto cf : fCF)
      delete cf;
  }

} /* namespace Hal */
