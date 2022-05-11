/*
 * TwoTrackRadialDistanceCut.cxx
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackRadialDistanceCut.h"

#include "ExpTrack.h"
#include "Helix.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackRadialDistanceCut::TwoTrackRadialDistanceCut() : TwoTrackCut(1), fPDG(NULL) {
    SetUnitName("#pi^{*}_{min}");
    fMagSign  = 1.0;
    fMagField = 0.5;
    fMaxRad   = 1.33;
    fMinRad   = 0.34;
    fMagConst = 0.15;
  }

  Bool_t TwoTrackRadialDistanceCut::Pass(TwoTrack* pair) {
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

    Double_t phistar_min = 10E+6;
    const Helix& hel1    = ((ExpTrackHelix*) pair->GetTrack1())->GetHelix();
    const Helix& hel2    = ((ExpTrackHelix*) pair->GetTrack2())->GetHelix();
    for (rad = fMinRad; rad < fMaxRad; rad += 0.01) {
      Double_t dps = (phi2 - phi1 + (TMath::ASin(-0.075 * chg2 * fMagSign * rad / ptv2))
                      - (TMath::ASin(-0.075 * chg1 * fMagSign * rad / ptv1)));
      Double_t s1, s2;
      Double_t S1, S2;
      hel1.PathLength(rad * 100, s1, s2);
      S1 = TMath::Min(s1, s2);
      hel2.PathLength(rad * 100, s1, s2);
      S2            = TMath::Min(s1, s2);
      TVector3 pos1 = hel1.EvalPos(S1);
      TVector3 pos2 = hel2.EvalPos(S2);
      Double_t dps2 = pos1.Phi() - pos2.Phi();
      dps2          = TVector2::Phi_mpi_pi(dps2) / TMath::DegToRad();
      dps           = TVector2::Phi_mpi_pi(dps) / TMath::DegToRad();
      if (TMath::Abs(dps) < TMath::Abs(phistar_min)) { phistar_min = dps; }
    }
    SetValue(phistar_min);
    if (phistar_min <= GetMin(0) || phistar_min >= GetMax(0)) {
      ForcedUpdate(kTRUE);
      return kTRUE;
    } else {
      ForcedUpdate(kFALSE);
      return kFALSE;
    }
  }

  void TwoTrackRadialDistanceCut::SetRadius(Double_t min, Double_t max) {
    fMinRad = min;
    fMaxRad = max;
  }

  void TwoTrackRadialDistanceCut::SetMagFiled(Double_t field) { fMagField = field; }

  void TwoTrackRadialDistanceCut::SetMagFieldSign(Double_t sign) { fMagSign = sign; }

  Bool_t TwoTrackRadialDistanceCut::Init(Int_t task_id) {
    Bool_t stat = TwoTrackCut::Init(task_id);
    fPDG        = TDatabasePDG::Instance();
    fMagConst   = fMagConst * fMagField;
    return stat;
  }

  TwoTrackRadialDistanceCut::~TwoTrackRadialDistanceCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
