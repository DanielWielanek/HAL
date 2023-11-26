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
#include "HistogramManager.h"
#include "QAPlotAxis.h"
#include "StdHist.h"
#include "Track.h"

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TVector3.h>

namespace Hal {

  QAPlot::QAPlot() : fUpdate(ECutUpdate::kNo), fReport(nullptr) {}

  QAPlot::QAPlot(ECutUpdate upd) : QAPlot("", upd) {}

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

  QAPlot::QAPlot(TString name, ECutUpdate upd) : fUpdate(upd), fReport(nullptr) { SetName(name); }

  Bool_t QAPlot::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (ev == nullptr) return kFALSE;
    switch (fUpdate) {
      case ECutUpdate::kEvent: {
        for (int j = 0; j < 3; j++) {
          for (unsigned int i = 0; i < fSettings[j].size(); i++) {
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
      case ECutUpdate::kTrack: {
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
          for (unsigned int i = 0; i < fSettings[j].size(); i++) {
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
      default: break;
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
      case ECutUpdate::kEvent: {
        FillEvent(static_cast<Event*>(obj));
      } break;
      case ECutUpdate::kTrack: {
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

  Int_t QAPlot::AddTH1(TString name, const QAPlotAxis& x, TString flag) {
    Int_t old_size = fSettings[0].size();
    fSettings[0].push_back(QAHistoSettings(1));
    fSettings[0][old_size].SetName(name);
    fSettings[0][old_size].SetTitle(name);
    fSettings[0][old_size].SetFlag(flag);
    fSettings[0][old_size].SetFillFlagX(x.GetFlag());
    fSettings[0][old_size].SetXaxis(x.GetNBins(), x.GetMin(), x.GetMax());
    return old_size;
  }

  Int_t QAPlot::AddTH2(TString name, const QAPlotAxis& x, const QAPlotAxis& y, TString flag) {
    Int_t old_size = fSettings[1].size();
    fSettings[1].push_back(QAHistoSettings(2));
    fSettings[1][old_size].SetName(name);
    fSettings[1][old_size].SetTitle(name);
    fSettings[1][old_size].SetFlag(flag);
    fSettings[1][old_size].SetFillFlagX(x.GetFlag());
    fSettings[1][old_size].SetFillFlagY(y.GetFlag());
    fSettings[1][old_size].SetXaxis(x.GetNBins(), x.GetMin(), x.GetMax());
    fSettings[1][old_size].SetYaxis(y.GetNBins(), y.GetMin(), y.GetMax());
    return old_size;
  }

  Int_t QAPlot::AddTH3(TString name, const QAPlotAxis& x, const QAPlotAxis& y, const QAPlotAxis& z, TString flag) {
    Int_t old_size = fSettings[2].size();
    fSettings[2].push_back(QAHistoSettings(3));
    fSettings[2][old_size].SetName(name);
    fSettings[2][old_size].SetTitle(name);
    fSettings[2][old_size].SetFlag(flag);
    fSettings[2][old_size].SetFillFlagX(x.GetFlag());
    fSettings[2][old_size].SetFillFlagY(y.GetFlag());
    fSettings[2][old_size].SetFillFlagZ(z.GetFlag());
    fSettings[2][old_size].SetXaxis(x.GetNBins(), x.GetMin(), x.GetMax());
    fSettings[2][old_size].SetYaxis(y.GetNBins(), y.GetMin(), y.GetMax());
    fSettings[2][old_size].SetZaxis(z.GetNBins(), z.GetMin(), z.GetMax());
    return old_size;
  }

  void QAPlot::FillTrack(Track* track) {
    for (unsigned int i = 0; i < fSettings[0].size(); i++) {
      if (fSettings[0][i].IsCustom()) {
        FillTrackCustom1D(track, (TH1D*) Get1D(i), i);
      } else {
        Get1D(i)->Fill(track->GetFieldVal(fSettings[0][i].GetFillFlagX()));
      }
    }
    for (unsigned int i = 0; i < fSettings[1].size(); i++) {
      if (fSettings[1][i].IsCustom()) {
        FillTrackCustom2D(track, (TH2D*) Get2D(i), i);
      } else {
        Get2D(i)->Fill(track->GetFieldVal(fSettings[1][i].GetFillFlagX()), track->GetFieldVal(fSettings[1][i].GetFillFlagY()));
      }
    }
    for (unsigned int i = 0; i < fSettings[2].size(); i++) {
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

  void QAPlot::Print(Option_t* /*opt*/) const {
    for (int i = 0; i < 3; i++) {
      for (unsigned int j = 0; j < fSettings[i].size(); j++) {
        fSettings[i][j].Print();
      }
    }
  }

}  // namespace Hal
