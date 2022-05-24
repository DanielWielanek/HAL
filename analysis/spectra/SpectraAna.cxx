/*
 * SpectraAna.cxx
 *
 *  Created on: 21-07-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "SpectraAna.h"

#include "Cout.h"
#include "Cut.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "Std.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "TrackPdgCut.h"

#include <TH2.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>


namespace Hal {
  SpectraAna::SpectraAna() :
    fPtEta(NULL),
    fPtY(NULL),
    fMtEta(NULL),
    fMtY(NULL),
    fPtBins(100),
    fMtBins(100),
    fEtaBins(100),
    fYBins(100),
    fPtMin(0.0),
    fPtMax(2.5),
    fMtMin(0.0),
    fMtMax(2.5),
    fEtaMin(-1.0),
    fEtaMax(1.0),
    fYMin(-1.0),
    fYMax(1.0),
    fEventCollectionsNames(NULL),
    fTrackCollectionsNames(NULL),
    fMass(),
    fUseMass(kFALSE) {
    AddTags("spectra");
  }

  void SpectraAna::ProcessTrack() {
    Double_t px = fCurrentTrack->GetPx();
    Double_t py = fCurrentTrack->GetPy();
    Double_t pz = fCurrentTrack->GetPz();
    Double_t p  = TMath::Sqrt(px * px + py * py + pz * pz);
    Double_t e, m;
    if (fUseMass) {
      m = fMass.at(fCurrentTrackCollectionID);
      e = TMath::Sqrt(p * p + m * m);
    } else {
      e = fCurrentTrack->GetE();
      m = TMath::Sqrt(e * e - p * p);
    }
    Double_t pt = TMath::Sqrt(px * px + py * py);
    Double_t mt = TMath::Sqrt(pt * pt + m * m);
    Double_t y  = 0.5 * TMath::Log((e + pz) / (e - pz));
    Double_t eta;
    if (pt == 0) {
      if (pz == 0)
        eta = 0.0;
      else if (pz > 0)
        eta = kMaxInt;
      else
        eta = -kMaxInt;
    } else {
      eta = 0.5 * TMath::Log((p + pz) / (p - pz));
    }
    fPtY->Fill(fCurrentTrackCollectionID, y, pt);
    fPtEta->Fill(fCurrentTrackCollectionID, eta, pt);
    fMtY->Fill(fCurrentTrackCollectionID, y, mt);
    fMtEta->Fill(fCurrentTrackCollectionID, eta, mt);
  }

  Package* SpectraAna::Report() const {
    Package* package = TrackAna::Report();
    for (int i = 0; i < fTrackCollectionsNo; i++) {
      Package* pack             = new Package();
      Int_t event_collection_no = fCutContainer->GetTrackCollection(i)->GetPrevAddr(0);
      TString title_event       = ((TObjString*) fEventCollectionsNames->UncheckedAt(event_collection_no))->GetString();
      TString title_track       = ((TObjString*) fTrackCollectionsNames->UncheckedAt(i))->GetString();
      TString pack_name         = Form("%s#%s", title_event.Data(), title_track.Data());
      pack->SetName(pack_name);
      pack->AddObject(new ParameterString("EventCollectionName", title_event));
      pack->AddObject(new ParameterString("TrackCollectionname", title_track));
      ULong64_t processed_events = fCutContainer->GetEventCollection(event_collection_no)->GetPassed();
      pack->AddObject(new ParameterULong64("Events Passed", processed_events));
      pack->AddObject(new ParameterBool("UseMass", fUseMass));
      if (fUseMass) pack->AddObject(new ParameterDouble("Mass", fMass.at(i)));
      pack->AddObject(fPtY->At(i));
      pack->AddObject(fPtEta->At(i));
      pack->AddObject(fMtY->At(i));
      pack->AddObject(fMtEta->At(i));
      package->AddObject(pack);
    }
    return package;
  }

  Task::EInitFlag SpectraAna::Init() {
    Task::EInitFlag stat = TrackAna::Init();
    // initialization of histograms
    fPtEta                       = new HistogramManager_1_2D<TH2D>();
    fPtY                         = new HistogramManager_1_2D<TH2D>();
    fMtEta                       = new HistogramManager_1_2D<TH2D>();
    fMtY                         = new HistogramManager_1_2D<TH2D>();
    HistogramAxisConf** axisConf = new HistogramAxisConf*[3];
    HistogramAxisConf* pt_axis   = new HistogramAxisConf("p_{T} [GeV/c]", fPtBins, fPtMin, fPtMax);
    HistogramAxisConf* mt_axis   = new HistogramAxisConf("m_{T} [GeV/c]", fMtBins, fMtMin, fMtMax);
    HistogramAxisConf* eta_axis  = new HistogramAxisConf("#eta", fEtaBins, fEtaMin, fEtaMax);
    HistogramAxisConf* y_axis    = new HistogramAxisConf("y", fYBins, fYMin, fYMax);
    HistogramAxisConf* z_axis    = new HistogramAxisConf("dummy", 0, 0, 0);
    axisConf[2]                  = z_axis;
    z_axis->SetTitle("dN/dP_{T}d#eta");
    axisConf[1] = pt_axis;
    axisConf[0] = eta_axis;
    fPtEta->Init(fTrackCollectionsNo, axisConf, "", kFALSE);
    z_axis->SetTitle("dN/dP_{T}dy");
    axisConf[1] = pt_axis;
    axisConf[0] = y_axis;
    fPtY->Init(fTrackCollectionsNo, axisConf, "", kFALSE);
    z_axis->SetTitle("dN/dm_{T}d#eta");
    axisConf[1] = mt_axis;
    axisConf[0] = eta_axis;
    fMtEta->Init(fTrackCollectionsNo, axisConf, "", kFALSE);
    z_axis->SetTitle("dN/dm_{T}dy");
    axisConf[1] = mt_axis;
    axisConf[0] = y_axis;
    fMtY->Init(fTrackCollectionsNo, axisConf, "", kFALSE);
    delete pt_axis;
    delete mt_axis;
    delete y_axis;
    delete eta_axis;
    delete z_axis;
    delete[] axisConf;
    CheckNames();
    for (int i = 0; i < fTrackCollectionsNo; i++) {
      Int_t event_collection_no = fCutContainer->GetTrackCollection(i)->GetPrevAddr(0);
      TString title             = ((TObjString*) fEventCollectionsNames->UncheckedAt(event_collection_no))->GetString();
      title                     = title + " ";
      title                     = title + ((TObjString*) fTrackCollectionsNames->UncheckedAt(i))->GetString();
      fPtY->At(i)->SetTitle(title);
      fPtEta->At(i)->SetTitle(title);
      fMtY->At(i)->SetTitle(title);
      fMtEta->At(i)->SetTitle(title);
      fPtY->At(i)->SetName(title);
      fPtEta->At(i)->SetName(title);
      fMtY->At(i)->SetName(title);
      fMtEta->At(i)->SetName(title);
    }
    if (fUseMass) {
      if (fMass.size() == 0) {
        Cout::PrintInfo("Mass used but mass is not set", EInfo::kError);
        return Task::EInitFlag::kFATAL;
      } else {
        if ((unsigned int) (fTrackCollectionsNo) < fMass.size()) {
          Cout::PrintInfo("Mass used, but some masses are not set", EInfo::kError);
          fMass.resize(fTrackCollectionsNo);
        }
        for (int i = 0; i < fTrackCollectionsNo; i++) {
          Cout::PrintInfo(Form("Mass at collection %i (%4.2f) is not valid", i, fMass.at(i)), EInfo::kError);
        }
      }
    }
    return stat;
  }

  void SpectraAna::SetPtAxis(Int_t bins, Double_t min, Double_t max) {
    fPtBins = bins;
    fPtMin  = min;
    fPtMax  = max;
  }

  void SpectraAna::SetMtAxis(Int_t bins, Double_t min, Double_t max) {
    fMtBins = bins;
    fMtMin  = min;
    fMtMax  = max;
  }

  void SpectraAna::SetYAxis(Int_t bins, Double_t min, Double_t max) {
    fYBins = bins;
    fYMin  = min;
    fYMax  = max;
  }

  void SpectraAna::SetEtaAxis(Int_t bins, Double_t min, Double_t max) {
    fEtaBins = bins;
    fEtaMin  = min;
    fEtaMax  = max;
  }

  void SpectraAna::SetEventCollectionName(TString name, Int_t event_collection) {
    if (fEventCollectionsNames == NULL) { fEventCollectionsNames = new TClonesArray("TObjString"); }
    TObjString* obj_string = (TObjString*) fEventCollectionsNames->ConstructedAt(event_collection);
    obj_string->SetString(name);
  }

  void SpectraAna::SetTrackCollectionName(TString name, Int_t track_collection) {
    if (fTrackCollectionsNames == NULL) { fTrackCollectionsNames = new TClonesArray("TObjString"); }
    TObjString* obj_string = (TObjString*) fTrackCollectionsNames->ConstructedAt(track_collection);
    obj_string->SetString(name);
  }

  void SpectraAna::CheckNames() {
    if (fTrackCollectionsNames == NULL) { fTrackCollectionsNames = new TClonesArray("TObjString"); }
    for (int i = 0; i < fTrackCollectionsNo; i++) {
      TObjString* obj = (TObjString*) fTrackCollectionsNames->UncheckedAt(i);
      if (obj == NULL) {
        obj = (TObjString*) fTrackCollectionsNames->ConstructedAt(i);
        obj->SetString(Form("track_collection[%i]", i));
      }
    }

    if (fEventCollectionsNames == NULL) { fEventCollectionsNames = new TClonesArray("TObjString"); }
    for (int i = 0; i < fEventCollectionsNo; i++) {
      TObjString* obj = (TObjString*) fEventCollectionsNames->UncheckedAt(i);
      if (obj == NULL) {
        obj = (TObjString*) fEventCollectionsNames->ConstructedAt(i);
        obj->SetString(Form("event_collection[%i]", i));
      }
    }
  }

  void SpectraAna::SetMass(Double_t mass, Int_t track_collection) {
    fUseMass = kTRUE;
    if (fMass.size() < (UInt_t) track_collection) { fMass.resize(track_collection + 1); }
    fMass[track_collection] = mass;
  }

  void SpectraAna::SetOption(Option_t* opt) {
    TString option = opt;
    if (!option.EqualTo("auto")) {
      TrackAna::SetOption(opt);
    } else {
      Int_t event_collections = 0;
      if (fCutContainer) {
        Cout::PrintInfo("You sent auto option without  event cuts, automatic configuration "
                        "will be work only for first even colllection",
                        EInfo::kWarning);
        event_collections = fCutContainer->GetEventCollectionsNo();
      }
      TrackPdgCut pion_p;
      ;
      pion_p.SetMinAndMax(211);
      TrackPdgCut pion_n;
      pion_n.SetMinAndMax(-211);
      TrackPdgCut kaon_p;
      kaon_p.SetMinAndMax(321);
      TrackPdgCut kaon_n;
      kaon_n.SetMinAndMax(-321);
      TrackPdgCut proton_p;
      proton_p.SetMinAndMax(2212);
      TrackPdgCut proton_n;
      proton_n.SetMinAndMax(-2212);
      for (int i = 0; i < event_collections; i++) {
        Int_t count = event_collections * i;
        pion_p.SetCollectionID(0 + count);
        AddCut(pion_p);
        SetTrackCollectionName("positive pions", 0 + count);
        pion_n.SetCollectionID(1 + count);
        AddCut(pion_n);
        SetTrackCollectionName("negative pions", 1 + count);
        kaon_p.SetCollectionID(2 + count);
        AddCut(kaon_p);
        SetTrackCollectionName("positive kaons", 2 + count);
        kaon_n.SetCollectionID(3 + count);
        AddCut(kaon_n);
        SetTrackCollectionName("negative kaons", 3 + count);
        proton_p.SetCollectionID(4 + event_collections * i);
        AddCut(proton_p);
        SetTrackCollectionName("protons", 4 + count);
        proton_n.SetCollectionID(5 + count);
        AddCut(proton_n);
        SetTrackCollectionName("antiprotons", 5 + count);
      }
    }
  }

  SpectraAna::~SpectraAna() {
    /*	if(fPtY){
     delete fPtY;
     delete fPtEta;
     delete fMtY;
     delete fMtEta;
     }*/
    if (fTrackCollectionsNames) delete fTrackCollectionsNames;
    if (fEventCollectionsNames) delete fEventCollectionsNames;
  }
}  // namespace Hal
