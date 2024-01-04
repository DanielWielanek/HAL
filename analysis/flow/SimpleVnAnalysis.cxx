/*
 * SimpleVnAnalysis.cxx
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "SimpleVnAnalysis.h"

#include "Cout.h"
#include "CutContainer.h"
#include "DividedHisto.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "Track.h"

#include <TMath.h>
#include <TString.h>
#include <iostream>


namespace Hal {
  SimpleVnAnalysis::SimpleVnAnalysis(Double_t n) :
    TrackAna(),
    fBinsX(1),
    fBinsY(1),
    fVarX(NULL),
    fVarY(NULL),
    fMinX(0),
    fMinY(0),
    fMaxX(1),
    fMaxY(0),
    fN(n),
    fPhi(0.0),
    fNDim(2) {}

  void SimpleVnAnalysis::ProcessTrack() {
    Double_t px     = fCurrentTrack->GetPx();
    Double_t py     = fCurrentTrack->GetPy();
    Double_t phi    = TMath::ATan2(py, px);
    Double_t Phi    = GetPhi();
    Double_t v2N    = TMath::Cos(fN * (phi - Phi));
    Double_t valueX = fVarX->GetVariable(fCurrentTrack);

    auto div = fHistos[fCurrentTrackCollectionID];
    switch (fNDim) {
      case 1: {
        div->FillNum(valueX, v2N);
        div->FillDen(valueX, 1);
      } break;
      case 2: {
        Double_t valueY = fVarY->GetVariable(fCurrentTrack);
        ((DividedHisto2D*) div)->FillNum(valueX, valueY, v2N);
        ((DividedHisto2D*) div)->FillDen(valueX, valueY, 1);
      } break;
      default: break;
    }
  }

  void SimpleVnAnalysis::SetAxis(Int_t nbins, Double_t min, Double_t max, Char_t axis) {
    if (min == max) {
      min = 0;
      max = 1;
    }
    if (nbins < 0) { nbins = 100; }

    switch (axis) {
      case 'x': {
        fMinX  = min;
        fMaxX  = max;
        fBinsX = nbins;
      } break;
      case 'y': {
        fMinY  = min;
        fMaxY  = max;
        fBinsY = nbins;
      } break;

      default:
#ifdef HAL_DEBUG
        Cout::PrintInfo(Form("Wrong SetAxis option in %s", this->ClassName()), EInfo::kLowWarning);
#endif
        ;
        break;
    }
  }

  Task::EInitFlag SimpleVnAnalysis::Init() {
    if (TrackAna::Init() == Task::EInitFlag::kSUCCESS) {
      // checking "fake collections"
      if (!fVarX) {
        Cout::PrintInfo("No FlowVariable on X", EInfo::kLowWarning);
        return Task::EInitFlag::kFATAL;
      }
      if (!fVarY) {
        Cout::PrintInfo("No FlowVariable on Y, it will be empty", EInfo::kLowWarning);
        fVarY = new FlowVirtualVariable();
        fNDim = 1;
        SetAxis(1, 0, 1, 1);
      }
      if (!fVarX->Init(GetTaskID())) { return Task::EInitFlag::kERROR; }
      if (!fVarY->Init(GetTaskID()) && fNDim == 2) { return Task::EInitFlag::kERROR; }
      HistogramAxisConf axX(fVarX->GetAxisName(), fBinsX, fMinX, fMaxX);
      fHistos = new DividedHisto1D*[fTrackCollectionsNo];
      switch (fNDim) {
        case 1: {
          HistogramAxisConf axY(Form("dN/%s", fVarX->GetAxisUnit().Data()), 0, 0, 1);
          for (int i = 0; i < fTrackCollectionsNo; i++) {
            fHistos[i] = new DividedHisto1D(Form("V_{%i}[%i]", (int) fN, i), axX, 'D');
            fHistos[i]->GetNum()->GetYaxis()->SetTitle(axY.GetTitle());
            fHistos[i]->GetDen()->GetYaxis()->SetTitle(axY.GetTitle());
            fHistos[i]->SetAxisName(Form("v_{%i RP}", (int) fN));
          }
        } break;
        case 2: {
          HistogramAxisConf axY(fVarY->GetAxisName(), fBinsY, fMinY, fMaxY);
          HistogramAxisConf axZ(Form("dN/%s%s", fVarX->GetAxisUnit().Data(), fVarY->GetAxisUnit().Data()), 0, 0, 1);
          for (int i = 0; i < fTrackCollectionsNo; i++) {
            fHistos[i] = new DividedHisto2D(Form("V_{%i}[%i]", (int) fN, i), axX, axY, 'D');
            fHistos[i]->GetNum()->GetZaxis()->SetTitle(axZ.GetTitle());
            fHistos[i]->GetDen()->GetZaxis()->SetTitle(axZ.GetTitle());
            fHistos[i]->SetAxisName(Form("v_{%i RP}", (int) fN));
          };
          break;
        }
        default: {
        } break;
      }
      return Task::EInitFlag::kSUCCESS;
    } else {
      return Task::EInitFlag::kFATAL;
    }
  }

  Package* SimpleVnAnalysis::Report() const {
    Package* pack = TrackAna::Report();
    AddToAnaMetadata(pack, new ParameterString("Variable_class on X ", fVarX->ClassName()));
    if (fNDim == 2) AddToAnaMetadata(pack, new ParameterString("Variable_class on Y", fVarY->ClassName()));
    for (int i = 0; i < fTrackCollectionsNo; i++) {
      pack->AddObject(fHistos[i]->Clone());
    }
    return pack;
  }

  void SimpleVnAnalysis::LinkCollections() {
    for (int i = 0; i < this->fCutContainer->GetTrackCollectionsNo(); i++) {
      this->fCutContainer->LinkCollections(ECutUpdate::kEvent, 0, ECutUpdate::kTrack, i);
    }
  }

  Double_t SimpleVnAnalysis::GetPhi() { return fCurrentEvent->GetPhi(); }

  void SimpleVnAnalysis::CheckCutContainerCollections() {
    TrackAna::CheckCutContainerCollections();
    if (fCutContainer->GetEventCollectionsNo() > 1) {
      for (int i = 1; i < fCutContainer->GetEventCollectionsNo(); i++)
        fCutContainer->RemoveCollection(ECutUpdate::kEvent, i);
      Cout::PrintInfo("Only 1 event cut container allowed for SimpleVnAnalysis", EInfo::kError);
    }
  }

  SimpleVnAnalysis::SimpleVnAnalysis(const SimpleVnAnalysis& ana) : TrackAna(ana) {
    fBinsX = ana.fBinsX;
    fBinsY = ana.fBinsY;
    fMinX  = ana.fMinX;
    fMinY  = ana.fMinY;
    fMaxX  = ana.fMaxX;
    fMaxY  = ana.fMaxY;
    fN     = ana.fN;
    fPhi   = ana.fPhi;
    fNDim  = ana.fNDim;
    if (ana.fHistos) {
      fHistos = new DividedHisto1D*[fTrackCollectionsNo];
      for (int i = 0; i < ana.fTrackCollectionsNo; i++) {
        fHistos[i] = (DividedHisto1D*) ana.fHistos[i]->Clone();
      }
    }
    if (ana.fVarX) fVarX = ana.fVarX->MakeCopy();
    if (ana.fVarY) fVarY = ana.fVarY->MakeCopy();
  }

  SimpleVnAnalysis::~SimpleVnAnalysis() {
    for (int i = 0; i < fTrackCollectionsNo; i++) {
      delete fHistos[i];
    }
    delete[] fHistos;
  }

  void SimpleVnAnalysis::SetFlowVariableX(const FlowVariable& var) { fVarX = var.MakeCopy(); }

  void SimpleVnAnalysis::SetFlowVariableY(const FlowVariable& var) { fVarY = var.MakeCopy(); }
}  // namespace Hal
