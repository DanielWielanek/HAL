/*
 * TwoTrackDphiDetaCut.cxx
 *
 *  Created on: 19 kwi 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackDphiDetaCut.h"

#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackDphiDetaCut::TwoTrackDphiDetaCut() : TwoTrackCut(2) {
    SetUnitName("#Delta#phi", 0);
    SetUnitName("#Delta#eta", 1);
  }

  Bool_t TwoTrackDphiDetaCut::Pass(TwoTrack* pair) {
    Double_t px1 = pair->GetTrack1()->GetPx();
    Double_t py1 = pair->GetTrack1()->GetPy();
    Double_t px2 = pair->GetTrack2()->GetPx();
    Double_t py2 = pair->GetTrack2()->GetPy();
    SetValue(TMath::ATan2(py1, px1) - TMath::ATan2(py2, px2));
    SetValue(pair->GetTrack1()->GetMomentum().Eta() - pair->GetTrack2()->GetMomentum().Eta(), 1);
    return AntiValidate();
  }

  TwoTrackDphiDetaCut::~TwoTrackDphiDetaCut() {}

  TwoTrackDphistarDetaCut::TwoTrackDphistarDetaCut() :
    TwoTrackCut(2),
    fMinRad(0.27),
    fMaxRad(1.4),
    fPDG(NULL),
    fMagSign(1.0),
    fMagField(0.5),
    fMagScale(0.0),
    fMagConst(0.150200415) {
    SetUnitName("#Delta#phi^{*}", 0);
    SetUnitName("#Delta#eta", 1);
  }

  void TwoTrackDphistarDetaCut::SetMagSing(Double_t sign) { fMagSign = sign; }

  void TwoTrackDphistarDetaCut::SetMagField(Double_t field) { fMagField = field; }

  Bool_t TwoTrackDphistarDetaCut::Pass(TwoTrack* pair) {
    Double_t px1  = pair->GetTrack1()->GetPx();
    Double_t px2  = pair->GetTrack2()->GetPx();
    Double_t py1  = pair->GetTrack1()->GetPy();
    Double_t py2  = pair->GetTrack2()->GetPy();
    Double_t ptv1 = TMath::Sqrt(px1 * px1 + py1 * py1);
    Double_t ptv2 = TMath::Sqrt(px2 * px2 + py2 * py2);
    Double_t phi1 = TMath::ATan2(py1, px1);
    Double_t phi2 = TMath::ATan2(py2, px2);
    Double_t chg1 = pair->GetTrack1()->GetCharge();
    Double_t chg2 = pair->GetTrack2()->GetCharge();
    Double_t rad  = fMinRad;
    // fMagScale = 0.075 (0.15*0.5T) *fMagSign
    Double_t afsi0b = fMagScale * chg1 * rad / ptv1;
    Double_t afsi1b = fMagScale * chg2 * rad / ptv2;

    /*  if (fabs(afsi0b) >=1.) {
     return kTRUE;
     }
     if (fabs(afsi1b) >=1.) return kTRUE;
     */
    Double_t phistar = phi2 - phi1 + TMath::ASin(afsi1b) - TMath::ASin(afsi0b);
    phistar          = TVector2::Phi_mpi_pi(phistar);
    SetValue(phistar, 0);
    SetValue(pair->GetTrack1()->GetMomentum().Eta() - pair->GetTrack2()->GetMomentum().Eta(), 1);
    return AntiValidate();
  }

  Bool_t TwoTrackDphistarDetaCut::Init(Int_t task_id) {
    Bool_t stat = TwoTrackCut::Init(task_id);
    fPDG        = TDatabasePDG::Instance();
    fMagScale   = fMagConst * fMagField * fMagSign;
    return stat;
  }

  Package* TwoTrackDphistarDetaCut::Report() const {
    Package* pack = TwoTrackCut::Report();
    pack->AddObject(new ParameterDouble("Magnetic field (T)", fMagField));
    pack->AddObject(new ParameterDouble("Magnetic field sign", fMagSign));
    pack->AddObject(new ParameterDouble("Min TPC radii (m)", fMinRad));
    return pack;
  }

  TwoTrackDphistarDetaCut::~TwoTrackDphistarDetaCut() {}
}  // namespace Hal
