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
    fCF(nullptr),
    fPair(nullptr),
    fMiniPair(nullptr),
    fTempGenerator(nullptr),
    fGenerator(nullptr),
    fWeight(nullptr),
    fMode(eDumpCalcMode::kSignalPairs) {}

  void CorrFitDumpedPairAna::SetCorrFunc(const FemtoCorrFunc& func) { fTempCF = (FemtoCorrFunc*) func.Clone(); }

  void CorrFitDumpedPairAna::SetWeightGenerator(const FemtoWeightGenerator& weight) { fWeight = weight.MakeCopy(); }

  Bool_t CorrFitDumpedPairAna::Init() {
    Bool_t fileOk = CorrFitParamsSetup::TestMapFile(fJobId);
    if (fileOk) { return kFALSE; }
    if (fJobId == -1) return kFALSE;
    if (ConfigureFromXML() == kFALSE) return kFALSE;
    if (ConfigureInput() == kFALSE) return kFALSE;
    //--- file with pairs loaded
    if (fWeight == nullptr) return kFALSE;
    if (fTempCF == nullptr) return kFALSE;
    if (fTempCF->GetEntries() > 1) return kFALSE;
    DividedHisto1D* dummy    = fTempCF->GetCF(0);
    Femto::EKinematics kinem = Femto::EKinematics::kLCMS;
    if (dummy->GetLabelsNo() > 0) {
      TString label = dummy->GetLabel(0);
      kinem         = Femto::LabelToKinematics(label);
    }
    if (Connect() == kFALSE) return kFALSE;
    fPair = Femto::MakePair(kinem, fImgMom);

    if (fIgnoreSing) fPair->UseAbs();
    if (fGenerator) {
      for (int i = 0; i < fMultiplyJobs; i++) {
        fGenerator[i]->Init();
      }
    } else {
      return kFALSE;
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
    fFile       = new TFile(fPairFile);
    TList* list = fFile->GetListOfKeys();
    for (int i = 0; i < list->GetEntries(); i++) {
      TKey* key    = (TKey*) list->At(i);
      TObject* obj = fFile->Get(key->GetName());
      if (obj->InheritsFrom("TTree")) {
        fTree = (TTree*) obj;
        break;
      }
    }
    if (fTree == nullptr) return kFALSE;

    return kTRUE;
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
    fGenerator            = new FemtoFreezoutGenerator*[fMultiplyJobs];
    for (int j = 0; j < fMultiplyJobs; j++) {
      fGenerator[j]           = fTempGenerator->MakeCopy();
      FemtoSourceModel* freez = fGenerator[j]->GetSourceModel();
      std::vector<int> arPos  = Hal::Std::OneToMultiDimIndex(dims, fJobId * fMultiplyJobs + j);
      for (int i = 0; i < parameters->GetNChildren(); i++) {
        XMLNode* parameter = parameters->GetChild(i);
        Double_t val       = setup.GetMin(i) + setup.GetStepSize(i) * ((Double_t) arPos[i]);
        freez->SetParameterByName(val, setup.GetParName(i));
      }
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
    }
    return kTRUE;
  }

  CorrFitDumpedPairAna::~CorrFitDumpedPairAna() {
    if (fTempCF) delete fTempCF;
    if (fTempGenerator) delete fTempGenerator;
    if (fCF) {
      for (int i = 0; i < fMultiplyJobs; i++)
        delete fCF[i];
      delete[] fCF;
    }
    if (fGenerator) {
      for (int i = 0; i < fMultiplyJobs; i++) {
        delete fGenerator[i];
      }
      delete[] fGenerator;
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
    if (fCF)
      for (int i = 0; i < fMultiplyJobs; i++)
        fCF[i]->Print();
    Cout::Text("Generator Info", "M");
    if (fGenerator)
      for (int i = 0; i < fMultiplyJobs; i++)
        fGenerator[i]->Print();
    Cout::Text("Weight Info", "M");
    if (fWeight) fWeight->Print();
  }
}  // namespace Hal
