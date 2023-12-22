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


namespace Hal {
  SimpleVnAnalysis::SimpleVnAnalysis(Double_t n) :
    TrackAna(), fBinsX(1), fBinsY(1), fVarX(NULL), fVarY(NULL), fMinX(0), fMinY(0), fMaxX(1), fMaxY(0), fN(n), fPhi(0.0) {}

  void SimpleVnAnalysis::ProcessTrack() {
    Double_t px     = fCurrentTrack->GetPx();
    Double_t py     = fCurrentTrack->GetPy();
    Double_t phi    = TMath::ATan2(py, px);
    Double_t Phi    = GetPhi();
    Double_t v2N    = TMath::Cos(fN * (phi - Phi));
    Double_t valueX = fVarX->GetVariable(fCurrentTrack);
    Double_t valueY = fVarY->GetVariable(fCurrentTrack);
    auto div        = fHistos[fCurrentTrackCollectionID];
    div->FillNum(valueX, valueY, v2N);
    div->FillDen(valueX, valueY, 1);
  }

  void SimpleVnAnalysis::SetAxis(Int_t nbins, Double_t min, Double_t max, Int_t axis) {
    if (min == max) {
      min = 0;
      max = 1;
    }
    if (nbins < 0) { nbins = 100; }

    switch (axis) {
      case 0: {
        fMinX  = min;
        fMaxX  = max;
        fBinsX = nbins;
      } break;
      case 1: {
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
      if (fVarX == NULL) {
        Cout::PrintInfo("No FlowVariable on X", EInfo::kLowWarning);
        return Task::EInitFlag::kFATAL;
      }
      if (fVarY == NULL) {
        Cout::PrintInfo("No FlowVariable on Y, it will be empty", EInfo::kLowWarning);
        fVarY = new FlowVirtualVariable();
        SetAxis(1, 0, 1, 1);
      }
      HistogramAxisConf** axis = new HistogramAxisConf*[3];
      axis[0]                  = new HistogramAxisConf(fVarX->GetAxisName(), fBinsX, fMinX, fMaxX);
      axis[1]                  = new HistogramAxisConf(fVarY->GetAxisName(), fBinsY, fMinY, fMaxY);
      axis[2] = new HistogramAxisConf(Form("dN/%s%s", fVarX->GetAxisUnit().Data(), fVarY->GetAxisUnit().Data()), 0, 0, 1);
      fHistos = new DividedHisto2D*[fTrackCollectionsNo];
      for (int i = 0; i < fTrackCollectionsNo; i++) {
        Int_t binsX = axis[0]->GetNBins();
        Int_t binsY = axis[1]->GetNBins();
        fHistos[i]  = new DividedHisto2D(Form("V_{%i}[%i]", (int) fN, i),
                                        axis[0]->GetNBins(),
                                        axis[0]->GetMin(),
                                        axis[0]->GetMax(),
                                        axis[1]->GetNBins(),
                                        axis[1]->GetMin(),
                                        axis[1]->GetMax(),
                                        'D');
        fHistos[i]->GetNum()->GetXaxis()->SetTitle(axis[0]->GetTitle());
        fHistos[i]->GetNum()->GetYaxis()->SetTitle(axis[1]->GetTitle());
        fHistos[i]->GetNum()->GetZaxis()->SetTitle(axis[2]->GetTitle());

        fHistos[i]->GetDen()->GetXaxis()->SetTitle(axis[0]->GetTitle());
        fHistos[i]->GetDen()->GetYaxis()->SetTitle(axis[1]->GetTitle());
        fHistos[i]->GetDen()->GetZaxis()->SetTitle(axis[2]->GetTitle());
        fHistos[i]->SetAxisName(Form("v_%i{RP}", (int) fN));
      }
      delete[] axis;
      return Task::EInitFlag::kSUCCESS;
    } else {
      return Task::EInitFlag::kFATAL;
    }
  }

  void SimpleVnAnalysis::FinishTask() { TrackAna::FinishTask(); }

  Package* SimpleVnAnalysis::Report() const {
    Package* pack = TrackAna::Report();
    AddToAnaMetadata(pack, new ParameterString("Variable_class on X ", fVarX->ClassName()));
    AddToAnaMetadata(pack, new ParameterString("Variable_class on Y", fVarY->ClassName()));
    DividedHisto2D** flows = new DividedHisto2D*[fTrackCollectionsNo];
    for (int i = 0; i < fTrackCollectionsNo; i++) {
      pack->AddObject(fHistos[i]);
    }
    delete[] flows;
    return pack;
  }

  void SimpleVnAnalysis::LinkCollections() {
    for (int i = 0; i < this->fCutContainer->GetTrackCollectionsNo(); i++) {
      this->fCutContainer->LinkCollections(ECutUpdate::kEvent, 0, ECutUpdate::kTrack, i);
    }
  }

  Double_t SimpleVnAnalysis::GetPhi() { return fCurrentEvent->GetPhi(); }

  void SimpleVnAnalysis::FinishAllEvents() {}

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
    if (ana.fHistos) {
      fHistos = new DividedHisto2D*[fTrackCollectionsNo];
      for (int i = 0; i < ana.fTrackCollectionsNo; i++) {
        fHistos[i] = (DividedHisto2D*) ana.fHistos[i]->Clone();
      }
    }
    if (ana.fVarX) fVarX = ana.fVarX->MakeCopy();
    if (ana.fVarY) fVarY = ana.fVarY->MakeCopy();
  }

  SimpleVnAnalysis::~SimpleVnAnalysis() {
    // TODO Auto-generated destructor stub
  }

  void SimpleVnAnalysis::SetFlowVariableX(const FlowVariable& var) { fVarX = var.MakeCopy(); }

  void SimpleVnAnalysis::SetFlowVariableY(const FlowVariable& var) { fVarY = var.MakeCopy(); }
}  // namespace Hal
