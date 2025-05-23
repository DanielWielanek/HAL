/*
 * nica_corrfit_db.cpp
 *
 *  Created on: 4 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "AnaFile.h"
#include "CorrFitConst.h"
#include "CorrFitInfo.h"
#include "CorrFitMapRebin.h"
#include "CorrFitParamsSetup.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"
#include "FemtoSerializationInterface.h"
#include "Package.h"
#include "PackageSql.h"

#include <TChain.h>
#include <TError.h>
#include <TFile.h>
#include <TKey.h>
#include <TString.h>
#include <TSystem.h>
#include <TTree.h>

#include <fstream>

#include <fstream>
#include <iostream>

void merge();
void mergeVertical();
void mergeHorizontal();
void printjobs();
void printjobsbad();
void preparetemplate();
void compress();
int getNFiles();
void rebin(TString opt);
int main(int argc, char* argv[]) {
  if (argc < 2) {
    Hal::Cout::PrintInfo("No arguments! run: hal-corrfit --help to get help", Hal::EInfo::kCriticalError);
    return 0;
  }
  TString arg1 = argv[1];
  if (arg1 == "--help") {
    std::cout << " Usage:" << std::endl;
    std::cout << "--help print help" << std::endl;
    std::cout << "0  - prepare template files and database" << std::endl;
    std::cout << "1 - merge maps" << std::endl;
    std::cout << "2 - compress maps" << std::endl;
    std::cout << "--print - check/print info about executed jobs " << std::endl;
    std::cout << "--print-bad - check/print info about executed jobs list of bad jobs is set to bad_jobs.txt" << std::endl;
    std::cout << "--rebin=n rebin CF new_map.root fille will be created" << std::endl;
    return 1;
  } else if (arg1 == "0") {  // prepare macro & xml files
    preparetemplate();
    return 1;
  } else if (arg1 == "--print") {
    printjobs();
  } else if (arg1 == "--print-bad") {
    printjobsbad();
  } else if (arg1 == "1") {
    printjobs();
    merge();
  } else if (arg1 == "2") {
    compress();
  } else if (arg1.BeginsWith("--rebin=")) {
    rebin(arg1);
  } else {
    std::cout << "Unknown parameter type --help" << std::endl;
    return 1;
  }
  return 0;
}

Bool_t AllJobsCompleted(Int_t jobs) {
  for (int i = 0; i < jobs; i++) {
    if (Hal::CorrFitParamsSetup::TestMapFile(i) == kFALSE) return kFALSE;
  }
  return kTRUE;
}

inline void merge() {
  TFile* info_file      = new TFile("files/config.root");
  Hal::CorrFitInfo* obj = new Hal::CorrFitInfo(*(Hal::CorrFitInfo*) info_file->Get("Info"));
  bool vert             = obj->IsVertical();
  info_file->Close();
  vert = true;
  if (vert) {
    mergeVertical();
  } else {
    mergeHorizontal();
  }
}

void printjobs() {
  Int_t nJobs     = getNFiles();
  Bool_t all_jobs = AllJobsCompleted(nJobs);

  Hal::Cout::Text(Form("CorrFitDB %i jobs", nJobs), "M", kWhite);
  if (all_jobs) {
    Hal::Cout::Text("All jobs completed", "", kWhite);
  } else {
    Hal::Cout::Text("Not all jobs are completed", "", kWhite);
  }
  for (int i = 0; i < nJobs; i++) {
    Bool_t fileOk = Hal::CorrFitParamsSetup::TestMapFile(i);
    if (fileOk) {
      Hal::Cout::FailSucced(Form("job %i", i), "GOOD", kCyan);
    } else {
      Hal::Cout::FailSucced(Form("job %i", i), "BAD ", kRed);
    }
  }
}

void printjobsbad() {
  Hal::CorrFitParamsSetup setup("corrfit_conf.xml");
  Int_t nJobs     = setup.GetNJobs();
  Bool_t all_jobs = AllJobsCompleted(nJobs);
  Hal::Cout::Text(Form("CorrFitDB %i jobs", nJobs), "M", kWhite);
  if (all_jobs) {
    Hal::Cout::Text("All jobs completed", "", kWhite);
  } else {
    Hal::Cout::Text("Not all jobs are completed", "", kWhite);
  }
  std::ofstream bad_jobs;
  bad_jobs.open("bad_jobs.txt");
  bad_jobs << "LIST_OF_BAD_JOBS" << std::endl;
  int bad_job_id = 0;
  for (int i = 0; i < nJobs; i++) {
    Bool_t fileOk = Hal::CorrFitParamsSetup::TestMapFile(i);
    if (!fileOk) {
      Hal::Cout::FailSucced(Form("job %i", i), "BAD ", kRed);
      bad_jobs << "BAD_JOB jobs/job_" << i << std::endl;
      ++bad_job_id;
    }
  }
  bad_jobs.close();
}

void preparetemplate() {

  std::ofstream xml_file;
  xml_file.open("corrfit_conf.xml");
  xml_file << "<CorrfitConfig>" << std::endl;
  xml_file << "<Parameters>" << std::endl;
  xml_file << "\t<Param name=\"R_{out}\" min=\"1\" max=\"10\" step=\"1\"></Param>" << std::endl;
  xml_file << "\t<Param name=\"R_{side}\" min=\"1\" max=\"10\" step=\"1\"></Param>" << std::endl;
  xml_file << "\t<Param name=\"R_{long}\" min=\"1\" max=\"10\" step=\"1\"></Param>" << std::endl;
  xml_file << "</Parameters>" << std::endl;
  xml_file << "<!-- full path to file with pairs-->" << std::endl;
  xml_file << "<PairFile>zz.root</PairFile>" << std::endl;
  xml_file << "<!-- optional part, use to configure dump pair analysis-->" << std::endl;
  xml_file << "<DumpAnalysisConf>" << std::endl;
  xml_file << "\t<CorrelationFunction>" << std::endl;
  xml_file << "\t\t<Name>CF</Name>" << std::endl;
  xml_file << "\t\t<Frame>EKinematics::kLCMS</Frame>" << std::endl;
  xml_file << "\t\t<Type>Femto3DCF</Type>" << std::endl;
  xml_file << "<!-- optional part, used only for spherical harmonics-->" << std::endl;
  xml_file << "\t\t<L>3</L>" << std::endl;
  xml_file << "\t\t<Xaxis bins=\"100\" min=\"0.0\" max=\"1.0\"></Xaxis>" << std::endl;
  xml_file << "\t\t<Yaxis bins=\"100\" min=\"0.0\" max=\"1.0\"></Yaxis>" << std::endl;
  xml_file << "\t\t<Zaxis bins=\"100\" min=\"0.0\" max=\"1.0\"></Zaxis>" << std::endl;
  xml_file << "\t</CorrelationFunction>" << std::endl;
  xml_file << "\t<FreezeoutGenerator>Hal::FemtoFreezeoutGeneratorLCMS</FreezeoutGenerator>" << std::endl;
  xml_file << "\t<SourceModel>Hal::FemtoSourceModelGauss3D</SourceModel>" << std::endl;
  xml_file << "\t<CalcOptions>" << std::endl;
  xml_file << "\t\t<JobMultiplyFactor>1</JobMultiplyFactor>" << std::endl;
  xml_file << "\t\t<WeightMultiplyFactor>1</WeightMultiplyFactor>" << std::endl;
  xml_file << "\t\t<PreprocessMultiplyFactor>1</PreprocessMultiplyFactor>" << std::endl;
  xml_file << "\t\t<!-- S/B/S+B for S(signal) B (background)  B+S (both)-->" << std::endl;
  xml_file << "\t\t<CalcMode>S</CalcMode>" << std::endl;
  xml_file << "\t\t<IgnoreSign>kTRUE</IgnoreSign>" << std::endl;
  xml_file << "\t</CalcOptions>" << std::endl;
  xml_file << "\t<WeightConf>" << std::endl;
  xml_file << "\t\t<Type>Hal::FemtoWeightGeneratorLednicky</Type>" << std::endl;
  xml_file << "\t\t<QuantumOn>kTRUE</QuantumOn>" << std::endl;
  xml_file << "\t\t<StrongOn>kFALSE</StrongOn>" << std::endl;
  xml_file << "\t\t<CoulombOn>kFALSE</CoulombOn>" << std::endl;
  xml_file << "\t\t<PairType>211;211</PairType>" << std::endl;
  xml_file << "\t</WeightConf>" << std::endl;
  xml_file << "</DumpAnalysisConf>" << std::endl;
  xml_file << "</CorrfitConfig>" << std::endl;
  xml_file.close();

  std::ofstream macro;
  macro.open("ana.C");
  macro << "void ana(){" << std::endl;
  macro << "\tTString jobEnv = gSystem->Getenv(\"JOB_ID_HAL\");" << std::endl;
  macro << "\tInt_t job_index = jobEnv.Atoi();" << std::endl;
  macro << "\tHal::CorrFitDumpedPairAna *task = new Hal::CorrFitDumpedPairAna(job_index);" << std::endl;
  macro << "\tif(task->Init()==kFALSE) return;" << std::endl;
  macro << "\ttask->Run();" << std::endl;
  macro << "\ttask->Finish();" << std::endl;
  macro << "}" << std::endl;
  macro.close();

  gSystem->mkdir("jobs");
  gSystem->mkdir("files");
}

void rebin(TString rebin) {

  TString opt  = Hal::Std::RemoveString(rebin, "--rebin=");
  Int_t Nrebin = opt.Atoi();
  if (Nrebin <= 0) {
    std::cout << "invalid number of bins " << std::endl;
    return;
  }
  Hal::CorrFitMapRebin* rebinner = new Hal::CorrFitMapRebin("merged_map.root", Nrebin);
  rebinner->Compress("merged_rebined_map.root");
  delete rebinner;
}
int getNFiles() {
  TFile* info_file      = new TFile("files/config.root");
  Hal::CorrFitInfo* obj = new Hal::CorrFitInfo(*(Hal::CorrFitInfo*) info_file->Get("Info"));
  Hal::CorrFitParamsSetup setup("corrfit_conf.xml");
  int nFiles = setup.GetNJobs();
  if (obj->IsVertical()) { nFiles = obj->GetMainBins(); }
  return nFiles;
}
void mergeHorizontal() {
  TFile* info_file      = new TFile("files/config.root");
  Hal::CorrFitInfo* obj = new Hal::CorrFitInfo(*(Hal::CorrFitInfo*) info_file->Get("Info"));
  Hal::CorrFitParamsSetup setup("corrfit_conf.xml");
  int nJobs = setup.GetNJobs();
  std::cout << "Merging vertical CF's" << nJobs << std::endl;
  if (!AllJobsCompleted(nJobs)) {
    Hal::Cout::PrintInfo("Not all jobs are completed", Hal::EInfo::kError);
    Hal::Cout::Text("Do you really want to continue y/n?", "L");
    TString ans;
    std::cin >> ans;
    if (!(ans == "y" || ans == "Y")) { return; }
  }
  TFile* file   = new TFile("merged_map.root", "recreate");
  TChain* chain = new TChain("map", "");

  for (int i = 0; i < nJobs; i++) {
    chain->AddFile(Form("files/corrfit_map_%i.root", i));
  }

  TTree* tree                     = new TTree("map", "");
  Hal::Array_1<Float_t>* Data_out = new Hal::Array_1<Float_t>();
  Hal::Array_1<Float_t>* Data_in  = nullptr;
  chain->SetBranchAddress("data", &Data_in);
  tree->Branch("data", &Data_out);
  int entries = chain->GetEntries() / nJobs;  // n_files *n_bins /nJobs(nbins) = n_files
  chain->GetEntry(0);
  Data_out->MakeBigger(Data_in->GetSize() * nJobs);
  int single_size = Data_in->GetSize();
  for (int i = 0; i < nJobs; i++) {  // nsamples
    chain->GetEntry(i);
    for (int k = 0; k < single_size; k++) {
      Data_out->Set(k, Data_in->Get(k));
    }
    tree->Fill();
  }

  tree->Write();
  obj->Write();
  file->Close();
  info_file->Close();
}
void mergeVertical() {
  TFile* info_file      = new TFile("files/config.root");
  Hal::CorrFitInfo* obj = new Hal::CorrFitInfo(*(Hal::CorrFitInfo*) info_file->Get("Info"));
  bool vert             = obj->IsVertical();
  auto CF               = obj->GetCf()->Clone();
  std::cout << "CF " << CF->ClassName() << std::endl;
  auto cfType = Hal::Femto::GetCFType(CF);
  int nSlices = Hal::Femto::GetNSlices(CF);


  Hal::CorrFitParamsSetup setup("corrfit_conf.xml");
  int n_params = setup.GetNJobs();
  if (!AllJobsCompleted(nSlices)) {
    Hal::Cout::PrintInfo("Not all nFiles are completed", Hal::EInfo::kError);
    Hal::Cout::Text("Do you really want to continue y/n?", "L");
    TString ans;
    std::cin >> ans;
    if (!(ans == "y" || ans == "Y")) { return; }
  }

  TFile* file   = new TFile("merged_map.root", "recreate");
  TChain* chain = new TChain("map", "");

  for (int i = 0; i < nSlices; i++) {
    chain->AddFile(Form("files/corrfit_map_%i.root", i));
  }
  std::cout << "Merging vertical CF's" << nSlices << " " << n_params << std::endl;
  auto* tree                     = new TTree("map", "");
  auto* Data_out                 = new Hal::Array_1<Float_t>();
  Hal::Array_1<Float_t>* Data_in = nullptr;
  chain->SetBranchAddress("data", &Data_in);
  tree->Branch("data", &Data_out);
  chain->GetEntry(0);
  const int slice_size = Data_in->GetSize();
  std::cout << "CHECK SIZE " << nSlices * n_params << " " << chain->GetEntries() << std::endl;
  std::cout << nSlices << " " << n_params << std::endl;
  Data_out->MakeBigger(slice_size * nSlices);
  for (int iPar = 0; iPar < n_params; iPar++) {
    for (int iBin = 0; iBin < nSlices; iBin++) {
      chain->GetEntry(iPar + n_params * iBin);
      for (int iS = 0; iS < slice_size; iS++) {
        Data_out->Set(iBin * slice_size + iS, Data_in->Get(iS));
      }
    }
    tree->Fill();
  }
  delete CF;
  tree->Write();
  obj->Write();
  file->Close();
  info_file->Close();
}

void compress() {
  TFile* info_file      = new TFile("files/config.root");
  Hal::CorrFitInfo* obj = new Hal::CorrFitInfo(*(Hal::CorrFitInfo*) info_file->Get("Info"));
  auto CF               = (Hal::DividedHisto1D*) obj->GetCf()->Clone();
  auto cfType           = Hal::Femto::GetCFType(CF);
  TFile* fileIn         = new TFile("merged_map.root");
  TTree* treeIn         = (TTree*) fileIn->Get("map");

  TFile* fileOut                  = new TFile("compressed_map.root", "recreate");
  TTree* treeOut                  = new TTree("map", "");
  Hal::Array_1<Float_t>* Data_out = new Hal::Array_1<Float_t>();
  Hal::Array_1<Float_t>* Data_in  = nullptr;
  treeIn->SetBranchAddress("data", &Data_in);
  treeOut->Branch("data", &Data_out);
  auto interfaceFrom = (Hal::FemtoSerializationInterface*) CF->GetSpecial("serialization");
  std::cout << interfaceFrom << std::endl;
  std::cout << interfaceFrom->ClassName() << std::endl;
  interfaceFrom->BindCFs(CF);
  interfaceFrom->BindArray(Data_in);
  interfaceFrom->SetOption(Hal::FemtoSerializationInterface::EOption::kFull);
  interfaceFrom->Init();
  auto interfaceTo = (Hal::FemtoSerializationInterface*) CF->GetSpecial("serialization");
  interfaceTo->BindCFs(CF);
  interfaceTo->BindArray(Data_out);
  interfaceTo->SetOption(Hal::FemtoSerializationInterface::EOption::kSimple);
  interfaceTo->Init();
  const int entries = treeIn->GetEntries();
  obj->Write();
  auto sh = dynamic_cast<Hal::FemtoSHCF*>(CF);
  for (int i = 0; i < entries; i++) {
    treeIn->GetEntry(i);
    interfaceFrom->Deserialize();
    if (i == 0) std::cout << "ARSIZE " << Data_in->GetSize() << " " << Data_out->GetSize() << std::endl;
    if (sh) {
      sh->RecalculateCF();
      sh->RecalculateCF(-1, kTRUE);
      /*std::cout << "NUM " << sh->GetNum()->GetBinContent(1) << std::endl;
      std::cout << "NUM " << sh->GetNum()->GetBinContent(2) << std::endl;
      std::cout << "NUM " << sh->GetNum()->GetBinContent(3) << std::endl;
      TFile* f  = new TFile(Form("dump_%i.root", i), "recreate");
      auto copy = sh->Clone();
      f->cd();
      copy->Write();

      f->Close();*/
    }
    interfaceTo->Serialize();
    treeOut->Fill();
  }
  treeOut->Write();
  fileIn->Close();
  fileOut->Close();
  delete interfaceFrom;
  delete interfaceTo;
}
