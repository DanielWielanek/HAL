/*
 * CorrFitMap1D.cxx
 *
 *  Created on: 18 paź 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitMapKstarRstar.h"

#include "Cout.h"
#include "DividedHisto.h"
#include "Splines.h"

namespace Hal {
  CorrFitMapKstarRstar::CorrFitMapKstarRstar(TString name) :
    fHisto2d(nullptr), fSplined(nullptr), fFrameScale(1), fFrame(Femto::EKinematics::kPRF) {
    SetName(name);
  }

  CorrFitMapKstarRstar::CorrFitMapKstarRstar(const TH2D& h, Femto::EKinematics kin) :
    fSplined(nullptr), fFrameScale(1), fFrame(kin) {
    fHisto2d = (TH2D*) h.Clone();
    fHisto2d->SetDirectory(nullptr);
  }

  CorrFitMapKstarRstar::CorrFitMapKstarRstar(const CorrFitMapKstarRstar& other) :
    Object(other), fHisto2d(nullptr), fSplined(nullptr), fFrameScale(other.fFrameScale), fFrame(other.fFrame) {
    if (other.fHisto2d) fHisto2d = (TH2D*) other.fHisto2d->Clone();
    // if (other.fSplined) fSplined = (Spline2D*) other.fSplined->Clone();
  }

  Double_t CorrFitMapKstarRstar::Eval(Double_t q, Double_t R) const {
    q = q * fFrameScale;
    return fSplined->Eval(q, R);
  }

  CorrFitMapKstarRstar::CorrFitMapKstarRstar(TString name,
                                             Int_t binsKstar,
                                             Double_t minKstar,
                                             Double_t maxKstar,
                                             Int_t binsR,
                                             Double_t minR,
                                             Double_t maxR,
                                             Femto::EKinematics kin) :
    fSplined(nullptr), fFrameScale(1.0), fFrame(kin) {
    fHisto2d = new TH2D("CFMap", "CFMap", binsKstar, minKstar, maxKstar, binsR, minR, maxR);
    SetName(name);
  }

  void CorrFitMapKstarRstar::Recalc(TString extrOpt) {
    if (fSplined != nullptr) {
      Cout::Text("Splinned already exist in CorrFitMap1D::BuildSpline");
      delete fSplined;
      fSplined = nullptr;
    }
    fSplined = new Spline2D(fHisto2d, extrOpt);
    fSplined->Refit();
  }

  void CorrFitMapKstarRstar::SetFrameScale(Femto::EKinematics kin) {
    if (kin == fFrame) fFrameScale = 1.0;
    if (kin == Femto::EKinematics::kLCMS && fFrame == Femto::EKinematics::kPRF) { fFrameScale = 0.5; }
    if (kin == Femto::EKinematics::kPRF && fFrame == Femto::EKinematics::kLCMS) { fFrameScale = 2.0; }
  }

  Double_t CorrFitMapKstarRstar::EvalNumError(Double_t q, Double_t R) const {
    q = q * fFrameScale;
    return fSplined->GetError(q, R) * 1.5;
  }

  CorrFitMapKstarRstar::~CorrFitMapKstarRstar() {
    if (fHisto2d) delete fHisto2d;
    if (fSplined) delete fSplined;
  }

  CorrFitMapKstarRstar& CorrFitMapKstarRstar::operator=(const CorrFitMapKstarRstar& other) {
    if (this == &other) return *this;
    TNamed::operator=(other);
    if (fHisto2d) delete fHisto2d;
    if (fSplined) delete fSplined;
    fHisto2d = nullptr;
    fSplined = nullptr;
    if (other.fHisto2d) { fHisto2d = (TH2D*) other.fHisto2d->Clone(); }
    if (other.fSplined) { fSplined = (Spline2D*) fSplined->Clone(); }
    fFrame      = other.fFrame;
    fFrameScale = other.fFrameScale;
    return *this;
  }

  void CorrFitMapKstarRstar::Add(const Object* pack) {
    CorrFitMapKstarRstar* other = (CorrFitMapKstarRstar*) pack;
    if (fHisto2d) fHisto2d->Add(other->fHisto2d);
  }

  //===============================================================================
  CorrFitMapKstarRstarDiv::CorrFitMapKstarRstarDiv(TString name) : CorrFitMapKstarRstar(name), fHistoDiv2d(nullptr) {}

  CorrFitMapKstarRstarDiv::CorrFitMapKstarRstarDiv(const CorrFitMapKstarRstarDiv& other) : CorrFitMapKstarRstar(other) {
    fHistoDiv2d = (DividedHisto2D*) other.fHistoDiv2d->Clone();
  }

  CorrFitMapKstarRstarDiv::CorrFitMapKstarRstarDiv(const DividedHisto2D& other, Femto::EKinematics kin) : CorrFitMapKstarRstar() {
    fFrame      = kin;
    fHistoDiv2d = (DividedHisto2D*) other.Clone();
    fHisto2d    = (TH2D*) fHistoDiv2d->GetHist(kFALSE);
  }

  CorrFitMapKstarRstarDiv& CorrFitMapKstarRstarDiv::operator=(const CorrFitMapKstarRstarDiv& other) {
    if (this == &other) return *this;
    CorrFitMapKstarRstar::operator=(other);
    if (fHistoDiv2d) {
      delete fHistoDiv2d;
      fHistoDiv2d = nullptr;
    }
    fHistoDiv2d = (DividedHisto2D*) other.fHistoDiv2d->Clone();
    return *this;
  }

  void CorrFitMapKstarRstarDiv::Add(const Object* pack) {
    CorrFitMapKstarRstar::Add(pack);
    CorrFitMapKstarRstarDiv* other = (CorrFitMapKstarRstarDiv*) pack;
    fHistoDiv2d->Add(other->fHistoDiv2d);
  }

  void CorrFitMapKstarRstarDiv::Recalc(TString opt) {
    if (fHisto2d) delete fHisto2d;
    if (fSplined) delete fSplined;
    fSplined = nullptr;
    fHisto2d = (TH2D*) fHistoDiv2d->GetHist(kFALSE);
    CorrFitMapKstarRstar::Recalc(opt);
  }

  CorrFitMapKstarRstarDiv::~CorrFitMapKstarRstarDiv() {
    if (fHistoDiv2d) delete fHistoDiv2d;
  }
}  // namespace Hal
