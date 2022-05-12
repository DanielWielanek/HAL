/*
 * QAPlot.cxx
 *
 *  Created on: 11 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "QAPlot.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "HalStdHist.h"
#include "HistogramManager.h"
#include "Track.h"

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TVector3.h>

namespace Hal {

  QAPlot::QAPlot() : fUpdate(ECutUpdate::kNoUpdate), fReport(nullptr) {}

  QAPlot::QAPlot(ECutUpdate upd) : QAPlot("", 0, 0, 0, upd) {};

  QAPlot::~QAPlot() {
    if (fReport) delete fReport;
  }

  QAPlot::QAPlot(const QAPlot& other) : TNamed(other), fReport(nullptr) {
    if (other.fReport) fReport = new QAPlotReport(*other.fReport);
    fUpdate = other.fUpdate;
    for (int i = 0; i < 3; i++) {
      fSettings[i] = other.fSettings[i];
    }
  }

  QAPlot::QAPlot(TString name, Int_t oneDimHist, Int_t twoDimHist, Int_t threeDimHist, ECutUpdate upd) :
    fReport(nullptr), fUpdate(upd) {
    SetName(name);
    fSettings[0].resize(oneDimHist, QAHistoSettings(1));
    fSettings[1].resize(twoDimHist, QAHistoSettings(2));
    fSettings[2].resize(threeDimHist, QAHistoSettings(3));
  }

  void QAPlot::SetAxis1D(Int_t no, Int_t nbinsX, Double_t minX, Double_t maxX) {
    if (no >= fSettings[0].size()) return;
    fSettings[0][no].SetXaxis(nbinsX, minX, maxX);
  }

  void QAPlot::SetAxis2D(Int_t no, Int_t nbinsX, Double_t minX, Double_t maxX, Int_t nbinsY, Double_t minY, Double_t maxY) {
    if (no >= fSettings[1].size()) return;
    fSettings[1][no].SetXaxis(nbinsX, minX, maxX);
    fSettings[1][no].SetYaxis(nbinsY, minY, maxY);
  }

  void QAPlot::SetAxis3D(Int_t no,
                         Int_t nbinsX,
                         Double_t minX,
                         Double_t maxX,
                         Int_t nbinsY,
                         Double_t minY,
                         Double_t maxY,
                         Int_t nbinsZ,
                         Double_t minZ,
                         Double_t maxZ) {
    if (no >= fSettings[2].size()) return;
    fSettings[2][no].SetXaxis(nbinsX, minX, maxX);
    fSettings[2][no].SetYaxis(nbinsY, minY, maxY);
    fSettings[2][no].SetZaxis(nbinsZ, minZ, maxZ);
  }

  Bool_t QAPlot::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (ev == nullptr) return kFALSE;
    switch (fUpdate) {
      case ECutUpdate::kEventUpdate: {
        for (int j = 0; j < 3; j++) {
          for (int i = 0; i < fSettings[j].size(); i++) {
            if (fSettings[j][i].IsCustom()) continue;
            Int_t fieldIDx = fSettings[j][i].GetFillFlagX();

            TString axisNameX = ev->GetFieldName(fieldIDx);
            fSettings[j][i].SetXaxisName(axisNameX);

            if (j > 0) {
              Int_t fieldIDy    = fSettings[j][i].GetFillFlagY();
              TString axisNameY = ev->GetFieldName(fieldIDy);
              fSettings[j][i].SetYaxisName(axisNameY);
            }
            if (j > 1) {
              Int_t fieldIDz    = fSettings[j][i].GetFillFlagZ();
              TString axisNameZ = ev->GetFieldName(fieldIDz);
              fSettings[j][i].SetZaxisName(axisNameZ);
            }
          }
        }
      } break;
      case ECutUpdate::kTrackUpdate: {
        Track* track         = ev->GetNewTrack();
        ComplexEvent* zEvent = static_cast<ComplexEvent*>(ev->GetNewEvent());
        track->SetEvent(zEvent);
        Track* re_track         = nullptr;
        Track* im_track         = nullptr;
        const ComplexEvent* zev = dynamic_cast<const ComplexEvent*>(ev);
        if (zev) {
          ComplexTrack* z_track = (ComplexTrack*) track;
          re_track              = zev->GetNewRealTrack();
          re_track->SetEvent(zEvent->GetRealEvent());
          im_track = zev->GetNewImgTrack();
          im_track->SetEvent(zEvent->GetImgEvent());
          z_track->SetImgTrack(im_track);
          z_track->SetRealTrack(re_track);
        }

        for (int j = 0; j < 3; j++) {
          for (int i = 0; i < fSettings[j].size(); i++) {
            if (fSettings[j][i].IsCustom()) continue;
            Int_t fieldIDx = fSettings[j][i].GetFillFlagX();

            TString axisNameX = track->GetFieldName(fieldIDx);
            fSettings[j][i].SetXaxisName(axisNameX);

            if (j > 0) {
              Int_t fieldIDy    = fSettings[j][i].GetFillFlagY();
              TString axisNameY = track->GetFieldName(fieldIDy);
              fSettings[j][i].SetYaxisName(axisNameY);
            }
            if (j > 1) {
              Int_t fieldIDz    = fSettings[j][i].GetFillFlagZ();
              TString axisNameZ = track->GetFieldName(fieldIDz);
              fSettings[j][i].SetZaxisName(axisNameZ);
            }
          }
        }
        delete track;
        delete zEvent;
        if (re_track) delete re_track;
        if (im_track) delete im_track;
      } break;
    }

    fReport = new QAPlotReport(GetName(), GetSize1D(), GetSize2D(), GetSize3D());
    for (int j = 0; j < 3; j++) {
      for (unsigned int i = 0; i < fSettings[j].size(); i++) {
        fReport->SetFlag(i, fSettings[j][i].GetFlag(), j + 1);
      }
    }
    if (GetSize1D() > 0) {
      fReport->f1dHistos->Init(GetSize1D(), 1, 0, 1);
      for (int i = 0; i < GetSize1D(); i++) {
        fReport->f1dHistos->OverwriteAt(((TH1D*) fSettings[0][i].MakeHisto()), i);
      }
    }
    if (GetSize2D() > 0) {
      fReport->f2dHistos->Init(GetSize2D(), 1, 0, 1, 1, 0, 1);
      for (int i = 0; i < GetSize2D(); i++) {
        fReport->f2dHistos->OverwriteAt(((TH2D*) fSettings[1][i].MakeHisto()), i);
      }
    }
    if (GetSize3D() > 0) {
      fReport->f3dHistos->Init(GetSize3D(), 1, 0, 1, 1, 0, 1, 1, 0, 1);
      for (int i = 0; i < GetSize3D(); i++)
        fReport->f3dHistos->OverwriteAt(((TH3D*) fSettings[2][i].MakeHisto()), i);
    }
    return kTRUE;
  }

  void QAPlot::Fill(TObject* obj) {
    switch (fUpdate) {
      case ECutUpdate::kEventUpdate: {
        FillEvent(static_cast<Event*>(obj));
      } break;
      case ECutUpdate::kTrackUpdate: {
        FillTrack(static_cast<Track*>(obj));
      } break;
      default: break;
    }
  }

  QAPlot& QAPlot::operator=(const QAPlot& other) {
    if (&other == this) return *this;
    if (fReport) delete fReport;
    fReport = nullptr;
    if (other.fReport) fReport = new QAPlotReport(*other.fReport);
    TNamed::operator=(other);
    fUpdate         = other.fUpdate;
    for (int i = 0; i < 3; i++) {
      fSettings[i] = other.fSettings[i];
    }
    return *this;
  }

  QAPlotReport* QAPlot::GetReport() const {
    QAPlotReport* rep = new QAPlotReport(*this->fReport);
    rep->SetName(this->GetName());
    rep->SetOriginClass(this->ClassName());
    return rep;
  }

  Int_t QAPlot::AddTH1(TString name, Int_t flagIDx, TString flag) {
    Int_t old_size = fSettings[0].size();
    fSettings[0].push_back(QAHistoSettings(1));
    fSettings[0][old_size].SetName(name);
    fSettings[0][old_size].SetTitle(name);
    fSettings[0][old_size].SetFlag(flag);
    fSettings[0][old_size].SetFillFlagX(flagIDx);
    return old_size;
  }

  Int_t QAPlot::AddTH2(TString name, Int_t flagIDx, Int_t flagIDy, TString flag) {
    Int_t old_size = fSettings[1].size();
    fSettings[1].push_back(QAHistoSettings(2));
    fSettings[1][old_size].SetName(name);
    fSettings[1][old_size].SetTitle(name);
    fSettings[1][old_size].SetFlag(flag);
    fSettings[1][old_size].SetFillFlagX(flagIDx);
    fSettings[1][old_size].SetFillFlagY(flagIDy);
    return old_size;
  }

  Int_t QAPlot::AddTH3(TString name, Int_t flagIDx, Int_t flagIDy, Int_t flagIDz, TString flag) {
    Int_t old_size = fSettings[2].size();
    fSettings[2].push_back(QAHistoSettings(3));
    fSettings[2][old_size].SetName(name);
    fSettings[2][old_size].SetTitle(name);
    fSettings[2][old_size].SetFlag(flag);
    fSettings[2][old_size].SetFillFlagX(flagIDx);
    fSettings[2][old_size].SetFillFlagY(flagIDy);
    fSettings[2][old_size].SetFillFlagZ(flagIDz);
    return old_size;
  }

  void QAPlot::FillTrack(Track* track) {
    for (int i = 0; i < fSettings[0].size(); i++) {
      if (fSettings[0][i].IsCustom()) {
        FillTrackCustom1D(track, (TH1D*) Get1D(i), i);
      } else {
        Get1D(i)->Fill(track->GetFieldVal(fSettings[0][i].GetFillFlagX()));
      }
    }
    for (int i = 0; i < fSettings[1].size(); i++) {
      if (fSettings[1][i].IsCustom()) {
        FillTrackCustom2D(track, (TH2D*) Get2D(i), i);
      } else {
        Get2D(i)->Fill(track->GetFieldVal(fSettings[1][i].GetFillFlagX()), track->GetFieldVal(fSettings[1][i].GetFillFlagY()));
      }
    }
    for (int i = 0; i < fSettings[2].size(); i++) {
      if (fSettings[2][i].IsCustom()) {
        FillTrackCustom3D(track, (TH3D*) Get3D(i), i);
      } else {
        Get3D(i)->Fill(track->GetFieldVal(fSettings[2][i].GetFillFlagX()),
                       track->GetFieldVal(fSettings[2][i].GetFillFlagY()),
                       track->GetFieldVal(fSettings[2][i].GetFillFlagZ()));
      }
    }
  }

  void QAPlot::FillEvent(Event* event) {
    for (int i = 0; i < GetSize1D(); i++) {
      if (fSettings[0][i].IsCustom()) {
        FillEventCustom1D(event, (TH1D*) Get1D(i), i);
      } else {
        Get1D(i)->Fill(event->GetFieldVal(fSettings[0][i].GetFillFlagX()));
      }
    }
    for (int i = 0; i < GetSize2D(); i++) {
      if (fSettings[1][i].IsCustom()) {
        FillEventCustom2D(event, (TH2D*) Get2D(i), i);
      } else {
        Get2D(i)->Fill(event->GetFieldVal(fSettings[1][i].GetFillFlagX()), event->GetFieldVal(fSettings[1][i].GetFillFlagY()));
      }
    }
    for (int i = 0; i < GetSize3D(); i++) {
      if (fSettings[2][i].IsCustom()) {
        FillEventCustom3D(event, (TH3D*) Get3D(i), i);
      } else {
        Get3D(i)->Fill(event->GetFieldVal(fSettings[2][i].GetFillFlagX()),
                       event->GetFieldVal(fSettings[2][i].GetFillFlagY()),
                       event->GetFieldVal(fSettings[2][i].GetFillFlagZ()));
      }
    }
  }

  void QAPlot::Print(Option_t* opt) const {
    for (int i = 0; i < 3; i++) {
      for (unsigned int j = 0; j < fSettings[i].size(); j++) {
        fSettings[i][j].Print();
      }
    }
  }

  Int_t QAPlot::AddTH1(TString name, Int_t flagIDx, Int_t nbinsX, Double_t min, Double_t max, TString flag) {
    Int_t no = AddTH1(name, flagIDx, flag);
    SetAxis1D(no, nbinsX, min, max);
    return no;
  }

  Int_t QAPlot::AddTH2(TString name,
                       Int_t flagIDx,
                       Int_t flagIDy,
                       Int_t nbinsX,
                       Double_t minX,
                       Double_t maxX,
                       Int_t nbinsY,
                       Double_t minY,
                       Double_t maxY,
                       TString flag) {
    Int_t no = AddTH2(name, flagIDx, flagIDy, flag);
    SetAxis2D(no, nbinsX, minX, maxX, nbinsY, minY, maxY);
    return no;
  }

  Int_t QAPlot::AddTH3(TString name,
                       Int_t flagIDx,
                       Int_t flagIDy,
                       Int_t flagIDz,
                       Int_t nbinsX,
                       Double_t minX,
                       Double_t maxX,
                       Int_t nbinsY,
                       Double_t minY,
                       Double_t maxY,
                       Int_t nbinsZ,
                       Double_t minZ,
                       Double_t maxZ,
                       TString flag) {
    Int_t no = AddTH3(name, flagIDx, flagIDy, flagIDz, flag);
    SetAxis3D(no, nbinsX, minX, maxX, nbinsY, minY, maxY, nbinsZ, minZ, maxZ);
    return no;
  }
}  // namespace Hal
