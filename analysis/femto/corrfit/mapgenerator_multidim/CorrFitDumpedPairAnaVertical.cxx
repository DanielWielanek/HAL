/*
 * CorrFitDumpedAnaVertical.cxx
 *
 *  Created on: 1 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitDumpedPairAnaVertical.h"

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
#include "CorrFitVerticalSlices.h"
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

  CorrFitDumpedPairAnaVertical::CorrFitDumpedPairAnaVertical(Int_t jobid, Int_t mapsPerAna) :
    CorrFitDumpedPairAna(jobid, mapsPerAna) {
    fMomMin = fMomMax = 0;
  }

  void CorrFitDumpedPairAnaVertical::RunSignalPairs(Int_t nEvents) {
    Int_t step = nEvents / 100;
    for (int iEvent = 0; iEvent < nEvents; iEvent++) {
      fTree->GetEntry(iEvent);
      if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }
      for (int iMulti = 0; iMulti < fMultiplyJobs; iMulti++) {
        for (int jPair = 0; jPair < fPairsSignal[iMulti]->GetEntriesFast(); jPair++) {
          auto MiniPair = (FemtoMicroPair*) fPairsSignal[iMulti]->UncheckedAt(jPair);
          *fPair        = *MiniPair;
          for (int preMulti = 0; preMulti < fMultiplyPreprocess; preMulti++) {
            PreprocessPair();
            fPair->Compute();
            for (int weightMulti = 0; weightMulti < fMultiplyWeight; weightMulti++) {
              fGenerator[0]->GenerateFreezoutCooordinates(fPair);
              fPair->SetWeight(fWeight->GenerateWeight(fPair));


              //     fCF[nJobs]->FillNum(fPair);
              //   fPair->SetWeight(1.0);
              //    fCF[nJobs]->FillDenMixed(fPair);
            }
          }
        }
      }
    }
  }

  void CorrFitDumpedPairAnaVertical::RunSignalBackgroundPairs(Int_t /*nEvents*/) {}

  void CorrFitDumpedPairAnaVertical::RunBackgroundPairs(Int_t /*nEvents*/) {}

  Bool_t CorrFitDumpedPairAnaVertical::Connect() {
    TObjArray* obj = fTree->GetListOfBranches();
    for (int i = 0; i < obj->GetEntries(); i++) {
      TObjString* key = (TObjString*) obj->At(i);
      fTree->GetBranch(key->GetString())->SetStatus(0);
    }
    switch (fMode) {
      case eDumpCalcMode::kSignalPairs: {

      } break;
      case eDumpCalcMode::kBackgroundPairsOnly: {

      } break;
      case eDumpCalcMode::kSignalBackgroundPairs: {

      } break;
    }
    return kTRUE;
  }

  Bool_t CorrFitDumpedPairAnaVertical::Init() {
    Bool_t preinit = CorrFitDumpedPairAna::Init();

    TObject* obj    = fTempCF->GetCF(0);
    Femto1DCF* cf1  = dynamic_cast<Femto1DCF*>(obj);
    Femto3DCF* cf3  = dynamic_cast<Femto3DCF*>(obj);
    FemtoSHCF* cfsh = dynamic_cast<FemtoSHCF*>(obj);
    XMLFile file("corrfit_conf.xml");
    CorrFitParamsSetup setup("corrfit_conf.xml");
    XMLNode* root             = file.GetRootNode();
    XMLNode* parameters       = root->GetChild("Parameters");
    Int_t totalNumberOfParams = 1;
    for (int i = 0; i < parameters->GetNChildren(); i++) {
      totalNumberOfParams *= parameters->GetChild(i)->GetAttrib("points")->GetValue().Atoi();
    }
    fParSet.resize(totalNumberOfParams);
    for (int i = 0; i < parameters->GetNChildren(); i++) {}

    if (cf1) {
      fSlices = new CorrFitVerticalSlices*[fMultiplyJobs];
      for (int i = 0; i < fMultiplyJobs; i++) {
        Int_t parNo = fMultiplyJobs * fJobId + i;
        fSlices[i]  = new CorrFitVerticalSlices1D(totalNumberOfParams, parNo);
      }
    }
    if (cf3) {
      fSlices = new CorrFitVerticalSlices*[fMultiplyJobs];
      for (int i = 0; i < fMultiplyJobs; i++) {
        Int_t parNo = fMultiplyJobs * fJobId + i;
        fSlices[i]  = new CorrFitVerticalSlices3D(totalNumberOfParams, parNo);
      }
    }
    if (cfsh) {
      fSlices = new CorrFitVerticalSlices*[fMultiplyJobs];
      for (int i = 0; i < fMultiplyJobs; i++) {
        Int_t parNo = fMultiplyJobs * fJobId + i;
        fSlices[i]  = new CorrFitVerticalSlicesSH(totalNumberOfParams, parNo, cfsh->GetLMax());
      }
    }
    fTempCF = nullptr;
    //   if (fGenerator) { fGenerator[0]->Init(); }
    return preinit;
  }

  void CorrFitDumpedPairAnaVertical::Finish() {}

  CorrFitDumpedPairAnaVertical::~CorrFitDumpedPairAnaVertical() {
    if (fSlices) {
      for (int i = 0; i < fMultiplyJobs; i++) {
        delete fSlices[i];
      }
      delete[] fSlices;
    }
    // TODO Auto-generated destructor stub
  }

} /* namespace Hal */
