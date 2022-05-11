/*
 * TwoTrackLCMSCut.cxx
 *
 *  Created on: 3 kwi 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackLCMSCut.h"

#include "Const.h"
#include "Track.h"
#include "TwoTrack.h"

namespace Hal {
  TwoTrackLCMSCut::TwoTrackLCMSCut() : TwoTrackCut(3) {
    SetUnitName("q_{out} [GeV/c]", Rout());
    SetUnitName("q_{side} [GeV/c]", Rside());
    SetUnitName("q_{long} [GeV/c]", Rlong());
    fM = Const::PionPlusMass() * Const::PionPlusMass();
  }

  Bool_t TwoTrackLCMSCut::Pass(TwoTrack* pair) {
    Track* tr1 = pair->GetTrack1();
    Track* tr2 = pair->GetTrack2();
    Double_t px1, py1, pz1, e1;
    Double_t px2, py2, pz2, e2;
    switch (pair->GetPairType()) {
      case TwoTrack::kRotated:
        px1 = tr1->GetPx();
        py1 = tr1->GetPy();
        pz1 = tr1->GetPz();
        px2 = -tr2->GetPx();
        py2 = -tr2->GetPy();
        pz2 = tr2->GetPz();
        break;
      case TwoTrack::kHemishpere:
        px1 = tr1->GetPx();
        py1 = tr1->GetPy();
        pz1 = tr1->GetPz();
        px2 = -tr2->GetPx();
        py2 = -tr2->GetPy();
        pz2 = -tr2->GetPz();
        break;
      default:
        px1 = tr1->GetPx();
        py1 = tr1->GetPy();
        pz1 = tr1->GetPz();
        px2 = tr2->GetPx();
        py2 = tr2->GetPy();
        pz2 = tr2->GetPz();
        break;
    }
    e1              = TMath::Sqrt(px1 * px1 + py1 * py1 + pz1 * pz1 + fM);
    e2              = TMath::Sqrt(px2 * px2 + py2 * py2 + pz2 * pz2 + fM);
    Double_t tPx    = px1 + px2;
    Double_t tPy    = py1 + py2;
    Double_t tPz    = pz1 + pz2;
    Double_t tE     = e1 + e2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;

    // Transform to LCMS

    Double_t particle1lcms_pz = tGamma * (pz1 - tBeta * e1);
    Double_t particle2lcms_pz = tGamma * (pz2 - tBeta * e2);

    // Rotate in transverse plane

    Double_t particle1lcms_px = (px1 * tPx + py1 * tPy) / tPt;
    Double_t particle1lcms_py = (-px1 * tPy + py1 * tPx) / tPt;

    Double_t particle2lcms_px = (px2 * tPx + py2 * tPy) / tPt;
    Double_t particle2lcms_py = (-px2 * tPy + py2 * tPx) / tPt;
    SetValue(particle1lcms_px - particle2lcms_px, Rout());
    SetValue(particle1lcms_py - particle2lcms_py, Rside());
    SetValue(particle1lcms_pz - particle2lcms_pz, Rlong());
    return Validate();
  }

  TwoTrackLCMSCut::~TwoTrackLCMSCut() {}
}  // namespace Hal
