\page femto_analysis Femto Analysis

Example of code for running HBT analysis

\code{.C}

void addCuts(Hal::FemtoBasicAna* hbt) {
  Hal::TrackPtCut pt;
  Hal::TrackEtaCut eta;
  hbt->AddCut(pt);
  hbt->AddCut(eta);

  Hal::CutMonitorXY mon(pt.CutName(), 0, eta.CutName("re"), 0);
  mon.SetXaxis(100, 0, 1);
  mon.SetYaxis(100, -1, 1);

  hbt->AddCutMonitor(mon);
}

void otf() {
  Hal::AnalysisManager* run = new Hal::AnalysisManager();
  HalOTF::Source* source    = new HalOTF::Source(2200);
  run->SetSource(source);
  run->SetOutput("output.root");

  TH2D* spec = new TH2D("spec", "spec", 2, -1, 1, 2, 0, 2);
  for (int i = 1; i < 3; i++) {
    for (int j = 1; j < 3; j++) {
      spec->SetBinContent(i, j, 1);
    }
  }
  Hal::Cout::SetVerboseMode(Hal::EInfo::kDebugInfo);

  HalOTF::Reader* read = new HalOTF::Reader();  
  read->SetSpiecies(*spec, 211, 200);
  run->AddTask(read);

  Hal::FemtoBasicAna* hbt = new Hal::FemtoBasicAna();
  Hal::FemtoCorrFuncKt kt(Hal::FemtoSHCF("cf", 2, 100, 0, 0.5, Hal::Femto::EKinematics::kPRF));
  hbt->SetCorrFctn(kt);
  
  Hal::FemtoWeightGeneratorLednicky lednickyFWG;
  lednickyFWG.SetPairType(Hal::Femto::EPairType::kPionPlusPionPlus);
  lednickyFWG.SetQuantumOn();
  lednickyFWG.SetCoulOff();
  lednickyFWG.SetStrongOff();
  hbt->SetWeight(lednickyFWG);
  
  addCuts(hbt);

  Hal::FemtoFreezoutGeneratorPRF m;
  Hal::FemtoSourceModelGauss3D g;
  g.SetOutRadius(2);
  g.SetSideRadius(3);
  g.SetLongRadius(4);
  m.SetSourceModel(g);
  hbt->SetFreezoutGenerator(m);
  
  hbt->SetFormat(new HalOTF::McEvent());
  run->AddTask(hbt); 

  run->Init();
  run->Run(0, 1000);
}
\endcode

# Description of macro

First part of code is a function that creates two cuts (on pt and eta), then these cuts are added. Later the cut monitor is created. Cut monitor contains
the pt on X-axis, and eta on Y-axis.

\code{.C}
void addCuts(Hal::FemtoBasicAna* hbt) {
  Hal::TrackPtCut pt;
  Hal::TrackEtaCut eta;
  hbt->AddCut(pt);
  hbt->AddCut(eta);

  Hal::CutMonitorXY mon(pt.CutName(), 0, eta.CutName("re"), 0);
  mon.SetXaxis(100, 0, 1);
  mon.SetYaxis(100, -1, 1);

  hbt->AddCutMonitor(mon);
}

\endcode

Second part is creation of analysis manager. This is a class that contains all tasks used for analysis. AnalysisManager is also responsible for reading/writing
files. Below there is a code that create simple AnalysisManager. This manager read HalOTF::Source - this is an "on the fly source" that generates data during analysis.
There are different types of sources (e.g. for root files).
The source of data and path/name of the output file is set.
\code{.C}
  Hal::AnalysisManager* run = new Hal::AnalysisManager();
  HalOTF::Source* source    = new HalOTF::Source(2200);
  run->SetSource(source);
  run->SetOutput("output.root");
\endcode

The lines below are just creating a histogram with distrubution of pt and rapidity, this histogram will be used for generation of data on the fly. The last line is 
enable high verbose of code (you can comment out this line).
\code{.C}
  TH2D* spec = new TH2D("spec", "spec", 2, -1, 1, 2, 0, 2);
  for (int i = 1; i < 3; i++) {
    for (int j = 1; j < 3; j++) {
      spec->SetBinContent(i, j, 1);
    }
  }
  Hal::Cout::SetVerboseMode(Hal::EInfo::kDebugInfo);
\endcode

Then we have to create a reader. Reader is a first task in analysis. This task convert data in source that can be in any form into data format that base on Hal::Event.
This is required by framework. Because we are using "on the fly data" we have to provide the plot that contains the distribution of particles, we also set the
PDG code of generated particles and number of particles per event. At the end we have to add reader to analysis manager.

\code{.C}
  HalOTF::Reader* read = new HalOTF::Reader();  
  read->SetSpiecies(*spec, 211, 200);
  run->AddTask(read);
\endcode

Line below create basic HBT analysis class, and set the type of analysis - by setting the correlation function. In this case the correlation function is calculated
as a function of Kt, for each Kt bin the Spherical Harmonics function is calculated up to L=2, the SH function have 100 bins from 0 to 0.5 GeV/c. SH frame is PRF.
\code{.C}
  Hal::FemtoBasicAna* hbt = new Hal::FemtoBasicAna();
  Hal::FemtoCorrFuncKt kt(Hal::FemtoSHCF("cf", 2, 100, 0, 0.5, Hal::Femto::EKinematics::kPRF));
  hbt->SetCorrFctn(kt);
\endcode

Then we have to setup the weight algorithm - this algorithm is needed for MC data to include correlations (MC data usually does not cotain correlations so we have
to add them by using weight algorithm. In last line the cuts are added.

\code{.C}
  Hal::FemtoWeightGeneratorLednicky lednickyFWG;
  lednickyFWG.SetPairType(Hal::Femto::EPairType::kPionPlusPionPlus);
  lednickyFWG.SetQuantumOn();
  lednickyFWG.SetCoulOff();
  lednickyFWG.SetStrongOff();
  hbt->SetWeight(lednickyFWG);
  
  addCuts(hbt);
\endcode

OTF data does not provide freezout information. So we have to set the freezoug generator. In principle to generate freezouts we need "freezout generator" and "source model".
"Source model" generates the coordinates in laboratory frame, the "Freezout generator" boos them to given frame (in this case PRF).

\code{.C}
  Hal::FemtoFreezoutGeneratorPRF m;
  Hal::FemtoSourceModelGauss3D g;
  g.SetOutRadius(2);
  g.SetSideRadius(3);
  g.SetLongRadius(4);
  m.SetSourceModel(g);
  hbt->SetFreezoutGenerator(m);
\endcode

At the end we are setting the format for our hbt task. Then our HBT task is configured.

\code{.C}
  hbt->SetFormat(new HalOTF::McEvent());
  run->AddTask(hbt); 
\endcode

We now have to call Init function - this function checks if settings of our analysis are good. The Run method perform simulation in this case from event 0 to event 2000.

\code{.C}
  run->Init();
  run->Run(0, 1000);
\endcode

