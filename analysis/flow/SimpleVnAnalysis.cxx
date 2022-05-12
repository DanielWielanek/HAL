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
#include "HalStd.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"

#include <TMath.h>
#include <TString.h>


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
    fNum(NULL),
    fDen(NULL) {}

  void SimpleVnAnalysis::ProcessTrack() {
    Double_t px     = fCurrentTrack->GetPx();
    Double_t py     = fCurrentTrack->GetPy();
    Double_t phi    = TMath::ATan2(py, px);
    Double_t Phi    = GetPhi();
    Double_t v2N    = TMath::Cos(fN * (phi - Phi));
    Double_t valueX = fVarX->GetVariable(fCurrentTrack);
    Double_t valueY = fVarY->GetVariable(fCurrentTrack);
    fNum->Fill(fCurrentTrackCollectionID, valueX, valueY, v2N);
    fDen->Fill(fCurrentTrackCollectionID, valueX, valueY);
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
        Cout::PrintInfo(Form("Wrong SetAxis option in %s", this->ClassName()), EInfo::kLessWarning);
#endif
        ;
        break;
    }
  }

  Task::EInitFlag SimpleVnAnalysis::Init() {
    if (TrackAna::Init() == Task::EInitFlag::kSUCCESS) {
      // checking "fake collections"
      if (fVarX == NULL) {
        Cout::PrintInfo("No FlowVariable on X", EInfo::kLessWarning);
        return Task::EInitFlag::kFATAL;
      }
      if (fVarY == NULL) {
        Cout::PrintInfo("No FlowVariable on Y, it will be empty", EInfo::kLessWarning);
        fVarY = new FlowVirtualVariable();
        SetAxis(1, 0, 1, 1);
      }
      fNum                     = new HistogramManager_1_2D<TH2D>;
      fDen                     = new HistogramManager_1_2D<TH2D>;
      HistogramAxisConf** axis = new HistogramAxisConf*[3];
      axis[0]                  = new HistogramAxisConf(fVarX->GetAxisName(), fBinsX, fMinX, fMaxX);
      axis[1]                  = new HistogramAxisConf(fVarY->GetAxisName(), fBinsY, fMinY, fMaxY);
      axis[2] = new HistogramAxisConf(Form("dN/%s%s", fVarX->GetAxisUnit().Data(), fVarY->GetAxisUnit().Data()), 0, 0, 1);
      fNum->Init(fTrackCollectionsNo, axis, Form("V_{%i num}", (int) fN), kTRUE);
      fDen->Init(fTrackCollectionsNo, axis, Form("V_{%i den}", (int) fN), kTRUE);
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
      TString name = Form("flow_%i", i);
      flows[i]     = new DividedHisto2D(name);
      flows[i]->SetScale(1.0);
      flows[i]->AddNum(fNum->fArray[i]);
      flows[i]->AddDen(fDen->fArray[i]);
      flows[i]->SetAxisName(Form("v_%i{RP}", (int) fN));
      pack->AddObject(flows[i]);
    }
    delete[] flows;
    return pack;
  }

  void SimpleVnAnalysis::LinkCollections() {
    for (int i = 0; i < this->fCutContainer->GetTrackCollectionsNo(); i++) {
      this->fCutContainer->LinkCollections(ECutUpdate::kEventUpdate, 0, ECutUpdate::kTrackUpdate, i);
    }
  }

  Double_t SimpleVnAnalysis::GetPhi() { return fCurrentEvent->GetPhi(); }

  void SimpleVnAnalysis::FinishAllEvents() {}

  void SimpleVnAnalysis::CheckCutContainerCollections() {
    TrackAna::CheckCutContainerCollections();
    if (fCutContainer->GetEventCollectionsNo() > 1) {
      for (int i = 1; i < fCutContainer->GetEventCollectionsNo(); i++)
        fCutContainer->RemoveCollection(ECutUpdate::kEventUpdate, i);
      Cout::PrintInfo("Only 1 event cut container allowed for SimpleVnAnalysis", EInfo::kLessError);
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
    if (ana.fNum) fNum = new HistogramManager_1_2D<TH2D>(*(ana.fNum));
    if (ana.fDen) fDen = new HistogramManager_1_2D<TH2D>(*(ana.fDen));
    if (ana.fVarX) fVarX = ana.fVarX->MakeCopy();
    if (ana.fVarY) fVarY = ana.fVarY->MakeCopy();
  }

  SimpleVnAnalysis::~SimpleVnAnalysis() {
    // TODO Auto-generated destructor stub
  }

  void SimpleVnAnalysis::SetFlowVariableX(const FlowVariable& var) { fVarX = var.MakeCopy(); }

  void SimpleVnAnalysis::SetFlowVariableY(const FlowVariable& var) { fVarY = var.MakeCopy(); }
}  // namespace Hal
