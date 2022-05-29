/*
 * CutMonitor.cpp
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#include "CutMonitor.h"

#include "Cout.h"
#include "Cut.h"
#include "Package.h"
#include "Parameter.h"
#include "StdString.h"

#include <TClass.h>
#include <TH1.h>


namespace Hal {
  void CutMonitor::AddForcedCut(Cut* cut, Int_t no) { fCut[no] = cut; }

  void CutMonitor::SetAxis(Int_t bins, Double_t min, Double_t max, Int_t opt) {
    if (opt < fAxisNo) {
      fAxisMin[opt]  = min;
      fAxisMax[opt]  = max;
      fAxisBins[opt] = bins;
    } else {
      Cout::PrintInfo(Form("You cant configure more than %i axis in %s", fAxisNo + 1, this->ClassName()), EInfo::kLowWarning);
    }
  }

  void CutMonitor::SetCollectionID(Int_t i) { fCollectionID = i; }

  Cut* CutMonitor::GetCut(Int_t i) const { return fCut[i]; }

  Int_t CutMonitor::GetCutParameter(Int_t i) const {
    if (i >= fAxisNo) return -1;
    if (i < 0) return -1;
    return fOptionAxis[i];
  }

  CutMonitor::CutMonitor(Int_t size) :
    TObject(),
    fAxisNo(size),
    fCuts(0),
    fCollectionID(-1),
    fAxisBins(NULL),
    fOptionAxis(NULL),
    fHistoPassed(NULL),
    fHistoFailed(NULL),
    fAxisMin(NULL),
    fAxisMax(NULL),
    fCut(NULL),
    fCutNames(NULL) {
    fInit     = kFALSE;
    fExUpdate = kFALSE;
    fCuts     = 0;
    if (fAxisNo > 0) {
      fUpdateRatio = ECutUpdate::kNo;
      fCut         = new Cut*[fAxisNo];
      fCutNames    = new TString[fAxisNo];
      fAxisBins    = new Int_t[fAxisNo];
      fOptionAxis  = new Int_t[fAxisNo];
      fAxisMin     = new Double_t[fAxisNo];
      fAxisMax     = new Double_t[fAxisNo];
      for (int i = 0; i < fAxisNo; i++) {
        fCut[i]        = NULL;
        fAxisMin[i]    = 0;
        fAxisMax[i]    = 1.0;
        fOptionAxis[i] = 0;
        fAxisBins[i]   = 100;
      }
    }
  }

  CutMonitor::CutMonitor(const CutMonitor& other) :
    TObject(other), fAxisNo(other.fAxisNo), fHistoPassed(NULL), fHistoFailed(NULL) {
    fInit         = kFALSE;
    fExUpdate     = other.fExUpdate;
    fCuts         = other.fCuts;
    fUpdateRatio  = other.fUpdateRatio;
    fCut          = new Cut*[fAxisNo];
    fCutNames     = new TString[fAxisNo];
    fAxisBins     = new Int_t[fAxisNo];
    fOptionAxis   = new Int_t[fAxisNo];
    fAxisMin      = new Double_t[fAxisNo];
    fAxisMax      = new Double_t[fAxisNo];
    fCollectionID = other.fCollectionID;
    for (int i = 0; i < fAxisNo; i++) {
      fCut[i]        = NULL;  // this will be set by CutContainer
      fCutNames[i]   = other.fCutNames[i];
      fAxisMin[i]    = other.fAxisMin[i];
      fAxisMax[i]    = other.fAxisMax[i];
      fAxisBins[i]   = other.fAxisBins[i];
      fOptionAxis[i] = other.fOptionAxis[i];
    }
  }

  Bool_t CutMonitor::Init(Int_t /*task_id*/) {
    if (fInit) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("%s is initialized ", this->ClassName()), EInfo::kDebugInfo);
#endif
      return kFALSE;
    }
    for (int i = 0; i < fAxisNo; i++) {
      if (fCut[i] == NULL) {
        Cout::PrintInfo(Form("Missed cut %i %s in %s", i, fCutNames[i].Data(), this->ClassName()), EInfo::kError);
        return kFALSE;
      }
    }
    for (int i = 0; i < fAxisNo; i++) {
      for (int j = i + 1; j < fAxisNo; j++) {
        if (fCut[i]->GetUpdateRatio() != fCut[j]->GetUpdateRatio()) {
          Cout::PrintInfo("Not compatible update ratios this might cause wrong "
                          "results in CutMonitor",
                          EInfo::kLowWarning);
        }
      }
    }
    TH1::AddDirectory(kFALSE);
    CreateHistograms();
    TH1::AddDirectory(kTRUE);
    fInit = kTRUE;
    return kTRUE;
  }

  void CutMonitor::Reset() {
    fHistoPassed->Reset();
    fHistoFailed->Reset();
  }

  ECutUpdate CutMonitor::GetUpdateRatio() const { return fUpdateRatio; }

  void CutMonitor::Update(Bool_t /*passed*/, TObject* /*obj*/) {}

  void CutMonitor::TrueUpdate(Bool_t /*passed*/) {}

  void CutMonitor::EnableExclusiveUpdate() { fExUpdate = kTRUE; }

  void CutMonitor::CreateHistograms() {}

  void CutMonitor::AddCut(TString cut, Int_t parameter_no) {
    if (Hal::Std::FindParam(cut, "Cloned")) {
      Cout::PrintInfo("You can't add Cloned Cuts to CutMonitor", EInfo::kLowWarning);
      return;
    }
    TClass* classdata = NULL;
    if (cut.Contains("(")) {  // complex cut?
      TString name(cut(0, cut.First('(')));
      classdata = TClass::GetClass(name);
    } else {
      classdata = TClass::GetClass(cut);
    }
    ECutUpdate newUpd = ECutUpdate::kNo;

    if (classdata == nullptr) {
      Cout::PrintInfo(Form("Cannot find class %s", cut.Data()), EInfo::kLowWarning);
    } else {
      if (classdata->InheritsFrom("Hal::EventCut")) newUpd = ECutUpdate::kEvent;
      if (classdata->InheritsFrom("Hal::TrackCut")) newUpd = ECutUpdate::kTrack;
      if (classdata->InheritsFrom("Hal::TwoTrackCut")) newUpd = ECutUpdate::kTwoTrack;
    }

    if (fCuts == 0) {
      fUpdateRatio = newUpd;
    } else {
      if (fUpdateRatio < newUpd) { fUpdateRatio = newUpd; }
    }
    if (fCuts < fAxisNo) {
      fCutNames[fCuts]   = cut;
      fOptionAxis[fCuts] = parameter_no;
      fCuts++;
      if (fUpdateRatio < newUpd) { fUpdateRatio = newUpd; }
    }
  }

  void CutMonitor::SetXaxis(Int_t bins, Double_t min, Double_t max) {
    if (fAxisNo > 0) {
      SetAxis(bins, min, max, 0);
    } else {
      Cout::PrintInfo("CutMonitor::X axis not found", EInfo::kLowWarning);
    }
  }

  void CutMonitor::SetYaxis(Int_t bins, Double_t min, Double_t max) {
    if (fAxisNo > 1) {
      SetAxis(bins, min, max, 1);
    } else {
      Cout::PrintInfo("CutMonitor::Y axis not found", EInfo::kLowWarning);
    }
  }

  void CutMonitor::SetZaxis(Int_t bins, Double_t min, Double_t max) {
    if (fAxisNo > 2) {
      SetAxis(bins, min, max, 2);
    } else {
      Cout::PrintInfo("CutMonitor::Z axis not found", EInfo::kLowWarning);
    }
  }

  void CutMonitor::CopySettings(const CutMonitor* source) {
    for (int i = 0; i < fAxisNo; i++) {
      SetAxis(source->fAxisBins[i], source->fAxisMin[i], source->fAxisMax[i], i);
    }
  }

  CutMonitor* CutMonitor::MakeCopy() const { return new CutMonitor(*this); }

  CutMonitor::~CutMonitor() {
    delete[] fCut;
    if (fHistoFailed) delete fHistoFailed;
    if (fHistoPassed) delete fHistoPassed;
    delete[] fAxisBins;
    delete[] fOptionAxis;
    delete[] fAxisMin;
    delete[] fAxisMax;
    delete[] fCutNames;
  }

  CutMonitor& CutMonitor::operator=(const CutMonitor& other) {
    if (this != &other) {
      fCuts         = other.fCuts;
      fCollectionID = other.fCollectionID;
      if (fAxisNo != other.fAxisNo) { Cout::PrintInfo("Copy incompatible cut monitors", EInfo::kLowWarning); }
      for (int i = 0; i < fAxisNo; i++) {
        fAxisBins[i]   = other.fAxisBins[i];
        fOptionAxis[i] = other.fOptionAxis[i];
        fAxisMin[i]    = other.fAxisMin[i];
        fAxisMax[i]    = other.fAxisMax[i];
        fCut[i]        = other.fCut[i];
        fCutNames[i]   = other.fCutNames[i];
      }
      if (fHistoFailed) delete fHistoFailed;
      fHistoFailed = nullptr;
      if (fHistoPassed) delete fHistoPassed;
      fHistoPassed = nullptr;
      if (other.fInit) {
        fHistoPassed = (TH1*) other.fHistoPassed->Clone();
        fHistoFailed = (TH1*) other.fHistoFailed->Clone();
      }
      fInit        = other.fInit;
      fExUpdate    = other.fExUpdate;
      fUpdateRatio = other.fUpdateRatio;
    }
    return *this;
  }

  Package* CutMonitor::Report() const {
    Package* pack = new Package(this);
    pack->AddObject(new ParameterInt("Cuts", fCuts));
    pack->AddObject(new ParameterInt("CollectionID", fCollectionID));
    pack->AddObject(new ParameterInt("UpdateRatio", (Int_t) fUpdateRatio));
    pack->AddObject(new ParameterInt("ExclusiveUpdate", (Int_t) fExUpdate));

    if (!ObjMonitor()) {
      TH1* hP = (TH1*) fHistoPassed->Clone();
      TH1* hF = (TH1*) fHistoFailed->Clone();
      pack->AddObject(hP);
      pack->AddObject(hF);
      if (fAxisNo >= 1) {
        std::vector<std::pair<TString, Double_t>> labels = fCut[0]->GetBinLabels(fOptionAxis[0]);
        if (labels.size() > 0) {
          for (auto iLabel : labels) {
            Int_t bin = hP->GetXaxis()->FindBin(iLabel.second);
            if (bin != 0) {
              hP->GetXaxis()->SetBinLabel(bin, iLabel.first);
              hF->GetXaxis()->SetBinLabel(bin, iLabel.first);
            }
          }
        }
        pack->AddObject(new ParameterString("AxisX", fCut[0]->GetUnit(fOptionAxis[0])));
        pack->AddObject(new ParameterString("CutXName", fCut[0]->CutName()));
        pack->AddObject(new ParameterInt("CutXAxis", fOptionAxis[0]));
        pack->AddObject(new ParameterDouble("CutXMin", fCut[0]->GetMin(fOptionAxis[0])));
        pack->AddObject(new ParameterDouble("CutXMax", fCut[0]->GetMax(fOptionAxis[0])));
        pack->AddObject(new ParameterInt("CutXCollection", fCut[0]->GetCollectionID()));
      }
      if (fAxisNo >= 2) {
        std::vector<std::pair<TString, Double_t>> labels = fCut[1]->GetBinLabels(fOptionAxis[1]);
        if (labels.size() > 0) {
          for (auto iLabel : labels) {
            Int_t bin = hP->GetYaxis()->FindBin(iLabel.second);
            if (bin != 0) {
              hP->GetYaxis()->SetBinLabel(bin, iLabel.first);
              hF->GetYaxis()->SetBinLabel(bin, iLabel.first);
            }
          }
        }
        pack->AddObject(new ParameterString("AxisY", fCut[1]->GetUnit(fOptionAxis[1])));
        pack->AddObject(new ParameterString("CutYName", fCut[1]->CutName()));
        pack->AddObject(new ParameterInt("CutYAxis", fOptionAxis[1]));
        pack->AddObject(new ParameterDouble("CutYMin", fCut[1]->GetMin(fOptionAxis[1])));
        pack->AddObject(new ParameterDouble("CutYMax", fCut[1]->GetMax(fOptionAxis[1])));
        pack->AddObject(new ParameterInt("CutYCollection", fCut[1]->GetCollectionID()));
      }
      if (fAxisNo >= 3) {
        std::vector<std::pair<TString, Double_t>> labels = fCut[2]->GetBinLabels(fOptionAxis[2]);
        if (labels.size() > 0) {
          for (auto iLabel : labels) {
            Int_t bin = hP->GetZaxis()->FindBin(iLabel.second);
            if (bin != 0) {
              hP->GetZaxis()->SetBinLabel(bin, iLabel.first);
              hF->GetZaxis()->SetBinLabel(bin, iLabel.first);
            }
          }
        }
        pack->AddObject(new ParameterString("AxisZ", fCut[2]->GetUnit(fOptionAxis[2])));
        pack->AddObject(new ParameterString("CutZName", fCut[2]->CutName()));
        pack->AddObject(new ParameterInt("CutZAxis", fOptionAxis[2]));
        pack->AddObject(new ParameterDouble("CutZMin", fCut[2]->GetMin(fOptionAxis[2])));
        pack->AddObject(new ParameterDouble("CutZMax", fCut[2]->GetMax(fOptionAxis[2])));
        pack->AddObject(new ParameterInt("CutZCollection", fCut[2]->GetCollectionID()));
      }
    } else {
      pack->AddObject(fHistoPassed->Clone());
      pack->AddObject(fHistoFailed->Clone());
    }
    pack->SetComment(this->ClassName());
    return pack;
  }

  Bool_t CutMonitor::AreSimilar(CutMonitor* other) const {
    if (this->fAxisNo != other->fAxisNo) return kFALSE;
    for (int i = 0; i < fAxisNo; i++) {
      if (!this->fCutNames[i].EqualTo(other->fCutNames[i])) { return kFALSE; }
    }
    return kTRUE;
  }
}  // namespace Hal
