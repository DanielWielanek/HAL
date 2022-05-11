/*
 * TwoTrack3DCFCut.cxx
 *
 *  Created on: 4 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrack3DCFCut.h"

#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "TwoTrack.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>

namespace Hal {
  TwoTrack3DCFCut::TwoTrack3DCFCut() :
    TwoTrackCut(1), fMap(nullptr), fPID1(211), fPID2(211), fFrame(0), fAbs(kTRUE), fM12(0), fM22(0) {
    SetUnitName("Pair type [AU]");
    SetMinAndMax(1);
  }

  TwoTrack3DCFCut::~TwoTrack3DCFCut() {
    if (fMap) delete fMap;
  }

  TwoTrack3DCFCut::TwoTrack3DCFCut(const TwoTrack3DCFCut& other) : TwoTrackCut(other) {
    if (other.fMap) fMap = (TH3D*) other.fMap->Clone();
    fPID1  = other.fPID1;
    fPID2  = other.fPID2;
    fM12   = other.fM12;
    fM22   = other.fM22;
    fFrame = other.fFrame;
    fAbs   = other.fAbs;
  }

  void TwoTrack3DCFCut::SetHistogram(const TH3D& histo) {
    fMap = (TH3D*) histo.Clone();
    fMap->SetDirectory(0x0);
  }

  Bool_t TwoTrack3DCFCut::Pass(TwoTrack* pair) {
    const TLorentzVector& tr1 = pair->GetTrack1()->GetMomentum();
    const TLorentzVector& tr2 = pair->GetTrack2()->GetMomentum();
    Double_t px1              = tr1.Px();
    Double_t py1              = tr1.Py();
    Double_t pz1              = tr1.Pz();
    Double_t px2, py2, pz2;
    switch (pair->GetPairType()) {
      case TwoTrack::kHemishpere:
        px2 = -tr2.Px();
        py2 = -tr2.Py();
        pz2 = -tr2.Pz();
        break;
      case TwoTrack::kRotated:
        px2 = -tr2.Px();
        py2 = -tr2.Py();
        pz2 = tr2.Pz();
        break;
      default:
        px2 = tr2.Px();
        py2 = tr2.Py();
        pz2 = tr2.Pz();
        break;
    }
    Double_t e1 = TMath::Sqrt(fM12 + px1 * px1 + py1 * py1 + pz1 * pz1);
    Double_t e2 = TMath::Sqrt(fM22 + px2 * px2 + py2 * py2 + pz2 * pz2);

    Double_t X, Y, Z;
    Double_t tPx    = px1 + px2;
    Double_t tPy    = py1 + py2;
    Double_t tPz    = pz1 + pz2;
    Double_t tE     = e1 + e2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM     = TMath::Sqrt(tMt - tPt);
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    if (fFrame == 0) {  // use LCMS
      Double_t particle1lcms_pz = tGamma * (pz1 - tBeta * e1);
      Double_t particle1lcms_e  = tGamma * (e1 - tBeta * pz1);
      Double_t particle2lcms_pz = tGamma * (pz2 - tBeta * e2);
      Double_t particle2lcms_e  = tGamma * (e2 - tBeta * pz2);

      Double_t particle1lcms_px = (px1 * tPx + py1 * tPy) / tPt;
      Double_t particle1lcms_py = (-px1 * tPy + py1 * tPx) / tPt;

      Double_t particle2lcms_px = (px2 * tPx + py2 * tPy) / tPt;
      Double_t particle2lcms_py = (-px2 * tPy + py2 * tPx) / tPt;

      X = particle1lcms_px - particle2lcms_px;
      Y = particle1lcms_py - particle2lcms_py;
      Z = particle1lcms_pz - particle2lcms_pz;
    } else {  // use PRF
      //     {
      // Boost to LCMS
      Z             = tGamma * (pz1 - tBeta * e1);
      Double_t tE1L = tGamma * (e1 - tBeta * pz1);
      X             = (px1 * tPx + py1 * tPy) / tPt;
      Y             = (-px1 * tPy + py1 * tPx) / tPt;
      X             = tMt / tM * (X - tPt / tMt * tE1L);
    }
    if (fAbs) {
      X = TMath::Abs(X);
      Y = TMath::Abs(Y);
      Z = TMath::Abs(Z);
    }
    Int_t binX = fMap->GetXaxis()->FindBin(X);
    Int_t binY = fMap->GetYaxis()->FindBin(Y);
    Int_t binZ = fMap->GetZaxis()->FindBin(Z);
    SetValue(fMap->GetBinContent(binX, binY, binZ));
    return Validate();
  }

  TwoTrack3DCFCut& TwoTrack3DCFCut::operator=(const TwoTrack3DCFCut& other) {
    if (this == &other) return *this;
    TwoTrackCut::operator=(other);
    if (other.fMap) {
      fMap = (TH3D*) other.fMap->Clone();
      fMap->SetDirectory(0x0);
    }
    fPID1  = other.fPID1;
    fPID2  = other.fPID2;
    fM12   = other.fM12;
    fM22   = other.fM22;
    fFrame = other.fFrame;
    fAbs   = other.fAbs;
    return *this;
  }

  Bool_t TwoTrack3DCFCut::Init(Int_t int1) {
    if (fMap == nullptr) return kFALSE;
    TDatabasePDG* db = TDatabasePDG::Instance();
    TParticlePDG* p1 = db->GetParticle(fPID1);
    TParticlePDG* p2 = db->GetParticle(fPID2);
    if (p1 == nullptr || p2 == nullptr) return kFALSE;
    fM12 = p1->Mass() * p1->Mass();
    fM22 = p2->Mass() * p2->Mass();
    return kTRUE;
  }

  Cut* TwoTrack3DCFCut::MakeCopy() const { return new TwoTrack3DCFCut(*this); }

  Package* TwoTrack3DCFCut::Report() const {
    Package* pack = TwoTrackCut::Report();
    if (fFrame == 0) {
      pack->AddObject(new ParameterString("Frame", "LCMS", '='));
    } else {
      pack->AddObject(new ParameterString("Frame", "PRF", '='));
    }
    pack->AddObject(new ParameterBool("Abs", fAbs, '='));
    pack->AddObject(new ParameterInt("PID 1 ", fPID1, '='));
    pack->AddObject(new ParameterInt("PID 2 ", fPID2, '='));
    return pack;
  }
}  // namespace Hal
