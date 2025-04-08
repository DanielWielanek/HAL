/*
 * FemtoNDimMap.cxx
 *
 *  Created on: 24 paź 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitDumpedPairAna.h"


#include <TApplication.h>
#include <TBranch.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TCollection.h>
#include <TDirectory.h>
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
#include "FemtoSerializationInterface.h"
#include "FemtoSourceModel.h"
#include "FemtoWeightGenerator.h"
#include "FemtoWeightGeneratorLednicky.h"
#include "XMLNode.h"

namespace Hal {
  CorrFitDumpedPairAna::CorrFitDumpedPairAna(Int_t jobid, Int_t maps_perAna) :
    TObject(), fJobId(jobid), fMultiplyJobs(maps_perAna), fMode(eDumpCalcMode::kSignalPairs) {}

  void CorrFitDumpedPairAna::SetCorrFunc(const FemtoCorrFunc& func) {
    if (fTempCF) delete fTempCF;
    fTempCF = (FemtoCorrFunc*) func.Clone();
  }

  void CorrFitDumpedPairAna::SetWeightGenerator(const FemtoWeightGenerator& weight) {
    if (fWeight) delete fWeight;
    fWeight = weight.MakeCopy();
  }

  Bool_t CorrFitDumpedPairAna::Init() {
    if (!ConfigureFromXML()) {
      Hal::Cout::PrintInfo("Cannot configure XML input", EInfo::kError);
      return kFALSE;
    }
    Bool_t fileOk = CorrFitParamsSetup::TestMapFile(fJobId * fMultiplyJobs);
    if (fileOk) {
      Hal::Cout::PrintInfo("Test file found no need to calculate map", EInfo::kError);
      exit(0);  // force exit to prevent crash
      return kFALSE;
    }
    if (!fWeight) {
      Hal::Cout::PrintInfo("Cannot find weight", EInfo::kError);
      return kFALSE;
    }
    if (!fTempCF) {
      Hal::Cout::PrintInfo("Cannot find CF", EInfo::kError);
      return kFALSE;
    }
    if (fTempCF->GetEntries() > 1) {
      Hal::Cout::PrintInfo("To many CF", EInfo::kError);
      return kFALSE;
    }
    InitPairFile();
    if (!fPairFile) {
      Hal::Cout::PrintInfo("Cannot find pair data", EInfo::kError);
      return kFALSE;
    }
    if (fJobId == -1) {
      Hal::Cout::PrintInfo("Wrong job iD", EInfo::kError);
      return kFALSE;
    }
    DividedHisto1D* dummy    = fTempCF->GetCF(0);
    Femto::EKinematics kinem = Femto::EKinematics::kLCMS;
    if (dummy->GetLabelsNo() > 0) {
      TString label = dummy->GetLabel(0);
      kinem         = Femto::LabelToKinematics(label);
    }
    fPair = Femto::MakePair(kinem, fImgMom);

    if (fIgnoreSing) {
      if (dummy->InheritsFrom("Hal::FemtoSHCF")) {
        Cout::PrintInfo("Cannot ignore sign with SH correlation function!", EInfo::kError);
      } else {
        fPair->UseAbs();
      }
    }
    return kTRUE;
  }

  void CorrFitDumpedPairAna::Run(Int_t events) {
    Int_t nEvents = 0;
    if (events > 0) {
      nEvents = events;
    } else {
      nEvents = fPairFile->GetEntries();
    }
    switch (fMode) {
      case eDumpCalcMode::kSignalBackgroundPairs: {
        Int_t step = nEvents / 100;
        for (int iEvent = 0; iEvent < nEvents; iEvent++) {
          fPairFile->GetEntry(iEvent);
          if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }
          RunSignalBackgroundPair();
          if (fPairThreshold > 0 && fPairThreshold < fPairsProcessed) break;
        }

      } break;
      case eDumpCalcMode::kSignalPairs: {
        Int_t step = nEvents / 100;
        for (int iEvent = 0; iEvent < nEvents; iEvent++) {
          fPairFile->GetEntry(iEvent);
          if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }
          RunSignalPair();
          if (fPairThreshold > 0 && fPairThreshold < fPairsProcessed) break;
        }
      } break;
      case eDumpCalcMode::kBackgroundPairsOnly: {
        Int_t step = nEvents / 100;
        for (int iEvent = 0; iEvent < nEvents; iEvent++) {
          fPairFile->GetEntry(iEvent);
          if (iEvent % step == 0) { Cout::ProgressBar(iEvent, nEvents); }
          RunBackgroundPair();
          if (fPairThreshold > 0 && fPairThreshold < fPairsProcessed) break;
        }
      } break;
    }
  }

  Bool_t CorrFitDumpedPairAna::SaveAsRawArray(TObject* cf, Int_t step) {
    TFile* file        = new TFile(Form("files/corrfit_map_%i.root", GetSimStepNo() + step), "recreate");
    auto cfx           = (Hal::DividedHisto1D*) cf;
    auto serialization = (FemtoSerializationInterface*) cfx->GetSpecial("serialization");
    serialization->BindCFs(cf);
    Array_1<Float_t>* Data = new Array_1<Float_t>();
    serialization->BindArray(Data);
    serialization->Init();
    serialization->Serialize();
    if (!Data) return kFALSE;
    TTree* tree = new TTree("map", "map");
    tree->Branch("data", &Data);
    tree->Fill();
    tree->Write();
    file->Close();
    delete serialization;
    return kTRUE;
  }

  Bool_t CorrFitDumpedPairAna::ConfigureFromXML() {
    auto printErr = [](TString str) {
      Hal::Cout::PrintInfo(Form("Cannot find node: %s in XML", str.Data()), EInfo::kError);
      return kFALSE;
    };
    XMLFile file("corrfit_conf.xml");
    CorrFitParamsSetup setup("corrfit_conf.xml");
    fTotalNumberOfPoints = 1;
    int parNo            = setup.GetNParams();
    for (int i = 0; i < parNo; i++) {
      fTotalNumberOfPoints = fTotalNumberOfPoints * setup.GetNPoints(i);
    }
    XMLNode* root        = file.GetRootNode();
    TString pairFileName = root->GetChild("PairFile")->GetValue();
    fPairFile            = new Hal::CorrFitPairFile(pairFileName, "read");
    XMLNode* parameters  = root->GetChild("Parameters");
    XMLNode* dumpAna     = root->GetChild("DumpAnalysisConf");
    if (!dumpAna) return printErr("DumpAnalysisConf");

    if (!parameters) return printErr("Parameters");

    XMLNode* freezXml  = dumpAna->GetChild("FreezeoutGenerator");
    XMLNode* sourceXml = dumpAna->GetChild("SourceModel");
    if (!freezXml) return printErr("FreezeoutGenerator");

    if (!sourceXml) return printErr("SourceModel");
    TClass* freezoutClass              = TClass::GetClass(freezXml->GetValue(), 1, 0);
    TClass* sourceClass                = TClass::GetClass(sourceXml->GetValue(), 1, 0);
    FemtoFreezeoutGenerator* generator = static_cast<FemtoFreezeoutGenerator*>(freezoutClass->New());
    FemtoSourceModel* source           = static_cast<FemtoSourceModel*>(sourceClass->New());
    if (source && generator) {
      generator->SetSourceModel(*source);
      SetGenerator(*generator);
    } else {
      Hal::Cout::PrintInfo("Cannot create source or generator class", EInfo::kError);
      return kFALSE;
    }
    if (source) delete source;
    if (generator) delete generator;
    XMLNode* calcOpts = dumpAna->GetChild("CalcOptions");
    if (calcOpts) {
      XMLNode* pairCut = calcOpts->GetChild("NoPairCut");
      if (pairCut) { fPairThreshold = pairCut->GetValue().Atoi(); }
      Hal::Cout::PrintInfo("Pair threshold detected", EInfo::kInfo);
    }
    XMLNode* multiJobs = calcOpts->GetChild("JobMultiplyFactor");
    if (multiJobs) {
      if (multiJobs->GetValue().Length() > 0 && fMultiplyJobs <= 0) { fMultiplyJobs = multiJobs->GetValue().Atoi(); }
    }
    if (fMultiplyJobs <= 0) fMultiplyJobs = 1;
    std::vector<int> dims = setup.GetDimensions();
    if (!InitGenerators(dims, parameters, setup)) {
      Cout::PrintInfo("Cannot init generators !", EInfo::kError);
      return kFALSE;
    }
    if (calcOpts) {
      XMLNode* multiplyXmlWeight = calcOpts->GetChild("WeightMultiplyFactor");
      if (multiplyXmlWeight) SetMultiplyFactorWeight(multiplyXmlWeight->GetValue().Atoi());

      XMLNode* multiplyXmlPreproc = calcOpts->GetChild("PreprocessMultiplyFactor");
      if (multiplyXmlPreproc) SetMultiplyFactorPreprocess(multiplyXmlPreproc->GetValue().Atoi());

      XMLNode* calcXml = calcOpts->GetChild("CalcMode");
      if (calcXml) {
        TString opt = calcXml->GetValue();
        if (opt == "S") fMode = eDumpCalcMode::kSignalPairs;
        if (opt == "S+B") fMode = eDumpCalcMode::kSignalBackgroundPairs;  // UseMixed
        if (opt == "B") fMode = eDumpCalcMode::kBackgroundPairsOnly;      // UseMixedOnly
      }
      XMLNode* ignore = calcOpts->GetChild("IgnoreSign");
      if (ignore) {
        if (ignore->GetValue() == "kTRUE") this->IgnoreSign();
      }
    }

    XMLNode* weight = dumpAna->GetChild("WeightConf");
    if (weight) {
      auto w = Hal::Femto::GetWeightGeneratorFromXLM(weight);
      if (!w) {
        Cout::PrintInfo("Problem with weight configuration !", EInfo::kError);
        return kFALSE;
      }
      this->SetWeightGenerator(*w);
      delete w;
    }
    XMLNode* cfXml     = dumpAna->GetChild("CorrelationFunction");
    DividedHisto1D* cf = Femto::GetHistoFromXML(cfXml);
    if (cf) {
      FemtoCorrFuncSimple corrFunc(*cf);
      delete cf;
      this->SetCorrFunc(corrFunc);
      if (!InitCFs()) { Cout::PrintInfo("Cannot init CF !", EInfo::kError); }
    } else {
      Cout::PrintInfo("Cannot find CF in XML !", EInfo::kError);
      return kFALSE;
    }
    return kTRUE;
  }

  CorrFitDumpedPairAna::~CorrFitDumpedPairAna() {
    if (fTempCF) delete fTempCF;
    if (fTempGenerator) delete fTempGenerator;
    for (auto i : fGenerator) {
      delete i;
    }
  }
  void CorrFitDumpedPairAna::SetPairFile(TString pairFile) {
    if (fPairFile) delete fPairFile;
    fPairFile = new Hal::CorrFitPairFile(pairFile, "read");
  }

  void CorrFitDumpedPairAna::Print(Option_t* /*option*/) const {
    Cout::Text("CorrFitDumpedPairAna Info", "M");
    Cout::Text(Form("JobID: %i", fJobId), "L");
    Cout::Text(Form("Multiply factor jobs: %i", fMultiplyJobs), "L");
    Cout::Text(Form("Multiply factor weight: %i", fMultiplyWeight), "L");
    Cout::Text(Form("Multiply factor preprocess: %i", fMultiplyPreprocess), "L");
    Cout::Text(Form("IgnoreSign: %i", fIgnoreSing), "L");
    Cout::Text(Form("Pair file: %s", fPairFile->GetFileName().Data()), "L");
    switch (fMode) {
      case eDumpCalcMode::kBackgroundPairsOnly: {
        Cout::Text("Use only background branch", "L");
      } break;
      case eDumpCalcMode::kSignalBackgroundPairs: {
        Cout::Text("Use signal & background branches", "L");
      } break;
      case eDumpCalcMode::kSignalPairs: {
        Cout::Text("Use only signal branch", "L");
      } break;
    }
    Cout::Text("CF Info", "M");
    if (fTempCF) fTempCF->Print();
    Cout::Text("Generator Info", "M");

    for (auto& gen : fGenerator) {
      if (gen) gen->Print();
    }
    Cout::Text("Weight Info", "M");
    if (fWeight) fWeight->Print();
  }
}  // namespace Hal
