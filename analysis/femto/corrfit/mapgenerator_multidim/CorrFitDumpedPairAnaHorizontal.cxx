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
#include "CorrFitPairFile.h"
#include "CorrFitParamsSetup.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoConst.h"
#include "FemtoCorrFunc.h"
#include "FemtoCorrFuncSimple.h"
#include "FemtoDumpPairAna.h"
#include "FemtoFreezeoutGenerator.h"
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
      obj             = fCF[iJob]->GetCF(0);
      Bool_t exported = SaveAsRawArray(obj, iJob);
      if (!exported) Cout::Text("Not supported CF", "L", kRed);
    }
    if (fJobId == 0) {
      obj               = fCF[0]->GetCF(0);
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
      info->SetPairFile(fPairFile->GetFileName());
      info->SetCf(obj);
      auto source = fGenerator[0]->GetSourceModel();
      source->Print();
      CorrFitParamsSetup* setup = new CorrFitParamsSetup("corrfit_conf.xml");
      info->SetSetup(*setup);
      info->Write();
      file->Close();
    }
  }

  void CorrFitDumpedPairAnaHorizontal::RunSignalPair() {
    Int_t no = fPairFile->GetSignalLength();
    for (int i = 0; i < no; i++) {
      auto clones = fPairFile->GetSignal(i);
      for (int jPair = 0; jPair < clones->GetEntriesFast(); jPair++) {
        auto MiniPair = (FemtoMicroPair*) clones->UncheckedAt(jPair);
        *fPair        = *MiniPair;
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessPair();
          fPair->Compute();
          fPairsProcessed++;
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            for (int weightMulti = 0; weightMulti < fMultiplyWeight; weightMulti++) {
              fGenerator[nJobs]->GenerateFreezeoutCooordinates(fPair);
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

  void CorrFitDumpedPairAnaHorizontal::RunSignalBackgroundPair() {
    Int_t no = fPairFile->GetSignalLength();
    for (int i = 0; i < no; i++) {
      auto clones = fPairFile->GetSignal(i);
      for (int jSig = 0; jSig < clones->GetEntriesFast(); jSig++) {
        auto MiniPair = (FemtoMicroPair*) clones->UncheckedAt(jSig);
        *fPair        = *MiniPair;
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessPair();
          fPair->Compute();
          fPairsProcessed++;
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            for (int weightPair = 0; weightPair < fMultiplyWeight; weightPair++) {
              fGenerator[nJobs]->GenerateFreezeoutCooordinates(fPair);
              fPair->SetWeight(fWeight->GenerateWeight(fPair));
              fCF[nJobs]->FillNum(fPair);
            }
          }
        }
      }
    }
    no = fPairFile->GetBackgroundLength();
    for (int i = 0; i < no; i++) {
      auto clones = fPairFile->GetBackground(i);
      for (int jSig = 0; jSig < clones->GetEntriesFast(); jSig++) {
        auto MiniPair = (FemtoMicroPair*) clones->UncheckedAt(jSig);
        *fPair        = *MiniPair;
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessPair();
          fPair->Compute();
          fPairsProcessed++;
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            fPair->SetWeight(1 * fMultiplyWeight);
            fCF[nJobs]->FillNum(fPair);
          }
        }
      }
    }
  }

  void CorrFitDumpedPairAnaHorizontal::RunBackgroundPair() {
    Int_t no = fPairFile->GetBackgroundLength();
    for (int i = 0; i < no; i++) {
      auto clones = fPairFile->GetBackground(i);
      for (int jMix = 0; jMix < clones->GetEntriesFast(); jMix++) {
        auto MiniPair = (FemtoMicroPair*) clones->UncheckedAt(jMix);
        *fPair        = *MiniPair;
        for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
          PreprocessPair();
          fPair->Compute();
          fPairsProcessed++;
          for (int nJobs = 0; nJobs < fMultiplyJobs; nJobs++) {
            for (int weightPair = 0; weightPair < fMultiplyWeight; weightPair++) {
              fGenerator[nJobs]->GenerateFreezeoutCooordinates(fPair);
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

  Bool_t CorrFitDumpedPairAnaHorizontal::InitGenerators(const std::vector<int>& dims,
                                                        XMLNode* parameters,
                                                        const CorrFitParamsSetup& setup) {
    fGenerator.clear();
    for (int j = 0; j < fMultiplyJobs; j++) {
      fGenerator.push_back(fTempGenerator->MakeCopy());
      FemtoSourceModel* freez = fGenerator[j]->GetSourceModel();
      std::vector<int> arPos  = Hal::Std::OneToMultiDimIndex(dims, fJobId * fMultiplyJobs + j);
      for (int i = 0; i < parameters->GetNChildren(); i++) {
        // XMLNode* parameter = parameters->GetChild(i);
        Double_t val = setup.GetMin(i) + setup.GetStepSize(i) * ((Double_t) arPos[i]);
        freez->SetParameterByName(setup.GetParName(i), val);
      }
      fGenerator[j]->Init();
    }
    return kTRUE;
  }

  Bool_t CorrFitDumpedPairAnaHorizontal::InitPairFile() {
    Int_t bins;
    Double_t min, max;
    Hal::Std::GetAxisPar(*fCF[0]->GetCF(0)->GetNum(), bins, min, max, "x");
    auto grouping = fPairFile->GetConfig();
    auto vec      = grouping->GetBranchesByValue(min, max);
    switch (fMode) {
      case eDumpCalcMode::kSignalPairs: {
        fPairFile->Init(vec.first, vec.second, kTRUE, kFALSE);
      } break;
      case eDumpCalcMode::kBackgroundPairsOnly: {
        fPairFile->Init(vec.first, vec.second, kFALSE, kTRUE);
      } break;
      case eDumpCalcMode::kSignalBackgroundPairs: {
        fPairFile->Init(vec.first, vec.second, kTRUE, kTRUE);
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
    for (auto cf : fCF) {
      delete cf;
    }
  }

} /* namespace Hal */
