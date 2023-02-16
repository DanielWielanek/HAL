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
  CorrFitDumpedPairAna::CorrFitDumpedPairAna(Int_t jobid, Int_t maps_perAna) :
    TObject(),
    fPairFile(""),
    fFile(nullptr),
    fTree(nullptr),
    fJobId(jobid),
    fMultiplyWeight(1),
    fMultiplyPreprocess(1),
    fMultiplyJobs(maps_perAna),
    fIgnoreSing(kFALSE),
    fImgMom(kFALSE),
    fTempCF(nullptr),
    fPair(nullptr),
    fMiniPair(nullptr),
    fTempGenerator(nullptr),
    fWeight(nullptr),
    fGrouping(nullptr),
    fMode(eDumpCalcMode::kSignalPairs) {}

  void CorrFitDumpedPairAna::SetCorrFunc(const FemtoCorrFunc& func) { fTempCF = (FemtoCorrFunc*) func.Clone(); }

  void CorrFitDumpedPairAna::SetWeightGenerator(const FemtoWeightGenerator& weight) { fWeight = weight.MakeCopy(); }

  Bool_t CorrFitDumpedPairAna::Init() {
    Bool_t fileOk = CorrFitParamsSetup::TestMapFile(fJobId);
    if (fileOk) {
      Hal::Cout::PrintInfo("Test file found no need to calculate map", EInfo::kError);
      return kFALSE;
    }

    if (!ConfigureFromXML()) return kFALSE;
    if (!ConfigureInput()) return kFALSE;
    //--- file with pairs loaded
    if (fWeight == nullptr) return kFALSE;
    if (fTempCF == nullptr) return kFALSE;
    if (fTempCF->GetEntries() > 1) return kFALSE;
    if (!fTree) return kFALSE;
    if (fJobId == -1) return kFALSE;

    DividedHisto1D* dummy    = fTempCF->GetCF(0);
    Femto::EKinematics kinem = Femto::EKinematics::kLCMS;
    if (dummy->GetLabelsNo() > 0) {
      TString label = dummy->GetLabel(0);
      kinem         = Femto::LabelToKinematics(label);
    }
    fPair = Femto::MakePair(kinem, fImgMom);

    if (!ConnectToData()) return kFALSE;

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
      nEvents = fTree->GetEntries();
    }

    switch (fMode) {
      case eDumpCalcMode::kSignalBackgroundPairs: {
        RunSignalBackgroundPairs(nEvents);
      } break;
      case eDumpCalcMode::kSignalPairs: {
        RunSignalPairs(nEvents);
      } break;
      case eDumpCalcMode::kBackgroundPairsOnly: {
        RunBackgroundPairs(nEvents);
      } break;
    }
  }

  void CorrFitDumpedPairAna::RootExport1D(Femto1DCF* cf, Int_t step) {
    TFile* file            = new TFile(Form("files/corrfit_map_%i.root", fJobId * fMultiplyJobs + step), "recreate");
    Array_1<Float_t>* Data = cf->ExportToFlatNum();
    TTree* tree            = new TTree("map", "map");
    tree->Branch("data", &Data);
    tree->Fill();
    tree->Write();
    file->Close();
  }

  void CorrFitDumpedPairAna::RootExport3D(Femto3DCF* cf, Int_t step) {
    TFile* file            = new TFile(Form("files/corrfit_map_%i.root", fJobId * fMultiplyJobs + step), "recreate");
    Array_1<Float_t>* Data = cf->ExportToFlatNum();
    TTree* tree            = new TTree("map", "map");
    tree->Branch("data", &Data);
    tree->Fill();
    tree->Write();
    file->Close();
  }

  void CorrFitDumpedPairAna::RootExportSH(FemtoSHCF* /*cf*/, Int_t /*step*/) {
    // TODO
  }

  Bool_t CorrFitDumpedPairAna::ConfigureInput() {
    fFile     = new TFile(fPairFile);
    fGrouping = dynamic_cast<CorrFitMapGroupConfig*>(fFile->Get("HalInfo/CorrFitMapGroup"));
    if (!fGrouping) {
      Hal::Cout::PrintInfo("Cannot find grouping config", EInfo::kError);
      return kFALSE;
    }
    TList* list = fFile->GetListOfKeys();
    std::cout << "CONFIGURE INPUT " << std::endl;
    FindTree(fFile, list);

    if (fTree == nullptr) return kFALSE;
    return ConnectToData();
  }

  Bool_t CorrFitDumpedPairAna::ConfigureFromXML() {
    XMLFile file("corrfit_conf.xml");
    CorrFitParamsSetup setup("corrfit_conf.xml");
    XMLNode* root       = file.GetRootNode();
    fPairFile           = root->GetChild("PairFile")->GetValue();
    XMLNode* parameters = root->GetChild("Parameters");
    XMLNode* dumpAna    = root->GetChild("DumpAnalysisConf");
    if (dumpAna == nullptr) return kFALSE;
    XMLNode* freezXml  = dumpAna->GetChild("FreezoutGenerator");
    XMLNode* sourceXml = dumpAna->GetChild("SourceModel");
    if (freezXml && sourceXml) {
      TClass* freezoutClass             = TClass::GetClass(freezXml->GetValue(), 1, 0);
      TClass* sourceClass               = TClass::GetClass(sourceXml->GetValue(), 1, 0);
      FemtoFreezoutGenerator* generator = static_cast<FemtoFreezoutGenerator*>(freezoutClass->New());
      FemtoSourceModel* source          = static_cast<FemtoSourceModel*>(sourceClass->New());
      if (source && generator) {
        generator->SetSourceModel(*source);
        SetGenerator(*generator);
      }
    }
    XMLNode* calcOpts = dumpAna->GetChild("CalcOptions");

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

    delete fTempGenerator;
    fTempGenerator = nullptr;

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
      XMLNode* weightType = weight->GetChild("Type");
      if (weightType == nullptr) return kFALSE;
      TClass* weightClass     = TClass::GetClass(weightType->GetValue(), 1, 0);
      FemtoWeightGenerator* w = static_cast<FemtoWeightGenerator*>(weightClass->New());
      if (w == nullptr) return kFALSE;
      XMLNode* pairType        = weight->GetChild("PairType");
      TString val              = pairType->GetValue();
      std::vector<TString> str = Hal::Std::ExplodeString(val, ';');
      if (str.size() == 2) {
        Int_t pid1 = str[0].Atoi();
        Int_t pid2 = str[1].Atoi();
        w->SetPairType(Femto::PidToPairType(pid1, pid2));
      }
      FemtoWeightGeneratorLednicky* lednicky = dynamic_cast<FemtoWeightGeneratorLednicky*>(w);
      if (lednicky) {
        lednicky->SetStrongOff();
        lednicky->SetCoulOff();
        lednicky->SetQuantumOff();

        XMLNode* quantum = weight->GetChild("QuantumOn");
        XMLNode* strong  = weight->GetChild("StrongOn");
        XMLNode* coul    = weight->GetChild("CoulombOn");
        if (quantum && lednicky) {
          if (quantum->GetValue() == "kTRUE") lednicky->SetQuantumOn();
        }
        if (strong && lednicky) {
          if (strong->GetValue() == "kTRUE") lednicky->SetStrongOn();
        }
        if (coul && lednicky) {
          if (coul->GetValue() == "kTRUE") lednicky->SetCoulOn();
        }
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

  void CorrFitDumpedPairAna::Print(Option_t* option) const {
    Cout::Text("CorrFitDumpedPairAna Info", "M");
    Cout::Text(Form("JobID: %i", fJobId), "L");
    Cout::Text(Form("Multiply factor jobs: %i", fMultiplyJobs), "L");
    Cout::Text(Form("Multiply factor weight: %i", fMultiplyWeight), "L");
    Cout::Text(Form("Multiply factor preprocess: %i", fMultiplyPreprocess), "L");
    Cout::Text(Form("IgnoreSign: %i", fIgnoreSing), "L");
    Cout::Text(Form("Pair file: %s", fPairFile.Data()), "L");
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
      gen->Print();
    }
    Cout::Text("Weight Info", "M");
    if (fWeight) fWeight->Print();
  }
  /*
    Bool_t CorrFitDumpedPairAna::ConnectToData() {
      TObjArray* list = fTree->GetListOfBranches();
      // lock not used branches
      for (int i = 0; i < list->GetEntries(); i++) {
        TObjString* str = (TObjString*) list->At(i);
        fTree->SetBranchStatus(str->String(), 0);
      }
      InitBranches();
      return kTRUE;
    }
  */

  void CorrFitDumpedPairAna::ConnectToSignal(const std::vector<TString>& branches) {
    for (auto name : branches) {
      TClonesArray* array = new TClonesArray("Hal::FemtoMicroPair");
      fTree->SetBranchAddress(name, &array);
      fSignalClones.push_back(array);
    }
  }

  void CorrFitDumpedPairAna::ConnectToBackground(const std::vector<TString>& branches) {
    for (auto name : branches) {
      TClonesArray* array = new TClonesArray("Hal::FemtoMicroPair");
      fTree->SetBranchAddress(name, &array);
      fBackgroundClones.push_back(array);
    }
  }

  Bool_t CorrFitDumpedPairAna::FindTree(TDirectory* dir, TList* list) {
    if (fTree) return kTRUE;
    for (int i = 0; i < list->GetEntries(); i++) {
      TKey* key = (TKey*) list->At(i);
      std::cout << "CHECK " << key->GetName() << std::endl;
      TObject* obj = dir->Get(key->GetName());
      if (obj->InheritsFrom("TDirectory")) {
        TDirectory* dir = (TDirectory*) obj;
        FindTree(dir, dir->GetListOfKeys());
      }
      std::cout << "CHECK " << key->GetName() << " " << obj->ClassName() << " " << fTree << std::endl;

      if (obj->InheritsFrom("TTree")) {
        fTree = (TTree*) obj;
        break;
      }
    }
    return kFALSE;
  }

}  // namespace Hal
