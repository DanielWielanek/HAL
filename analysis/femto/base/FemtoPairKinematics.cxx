/*
 * FemtoPair.cxx
 *
 *  Created on: 30-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoPairKinematics.h"

#include "Track.h"

#include <TLorentzVector.h>
#include <TMathBase.h>
#include <TVector3.h>


//----------------HAL FEMTO KINEMATICS
namespace Hal {
  void FemtoPairPRF::Compute() {
    Double_t tPx = fpx1 + fpx2;
    Double_t tPy = fpy1 + fpy2;
    Double_t tPz = fpz1 + fpz2;
    Double_t tE  = fe1 + fe2;
    Double_t tPt = tPx * tPx + tPy * tPy;
    Double_t tMt = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM  = TMath::Sqrt(tMt - tPt);
    tMt          = TMath::Sqrt(tMt);
    tPt          = TMath::Sqrt(tPt);

    //  mBetat = tPt/tMt;

    //   if ((mBetat > btmin) && (mBetat < btmax))
    //     {
    // Boost to LCMS
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    fZ              = tGamma * (fpz1 - tBeta * fe1);
    Double_t tE1L   = tGamma * (fe1 - tBeta * fpz1);

    // Transform to LCMS

    // Double_t particle1lcms_pz = tGamma * (FalsePz1() - tBeta * FalseE1());
    // Double_t particle1prf_pz = particle1lcms_pz;

    // Double_t particle1lcms_e  = tGamma * (FalseE1()  - tBeta * FalsePz1());
    // Double_t particle2lcms_pz = tGamma * (FalsePz2() - tBeta * FalseE2());
    // Double_t particle2prf_pz = particle2lcms_pz;
    // Double_t particle2lcms_e  = tGamma * (FalseE2()  - tBeta * FalsePz2());

    // Rotate in transverse plane
    fX = (fpx1 * tPx + fpy1 * tPy) / tPt;
    fY = (-fpx1 * tPy + fpy1 * tPx) / tPt;

    // Double_t particle2lcms_px = (FalsePx2()*tPx + FalsePy2()*tPy)/tPt;
    // Double_t particle2lcms_py = (FalsePy2()*tPx - FalsePx2()*tPy)/tPt;;

    // save the rotated coordinates in LCMS variables

    // Boost to pair cms
    fX = tMt / tM * (fX - tPt / tMt * tE1L);

    // Double_t tBetat = tPt/tMt;
    // Double_t tGammat = 1.0/TMath::Sqrt(1.0-tBetat*tBetat);

    fT = fX > 0. ? 1. : -1.;
    fT *= TMath::Sqrt(fY * fY + fX * fX + fZ * fZ);
  }

  void FemtoPairPRF::Compute_Rotated() {
    Double_t tPx = fpx1 - fpx2;  // reversed
    Double_t tPy = fpy1 - fpy2;  // reversed
    Double_t tPz = fpz1 + fpz2;
    Double_t tE  = fe1 + fe2;
    Double_t tPt = tPx * tPx + tPy * tPy;
    Double_t tMt = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM  = TMath::Sqrt(tMt - tPt);
    tMt          = TMath::Sqrt(tMt);
    tPt          = TMath::Sqrt(tPt);

    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    fZ              = tGamma * (fpz1 - tBeta * fe1);
    Double_t tE1L   = tGamma * (fe1 - tBeta * fpz1);

    // Rotate in transverse plane
    fX = (fpx1 * tPx + fpy1 * tPy) / tPt;
    fY = (-fpx1 * tPy + fpy1 * tPx) / tPt;
    fX = tMt / tM * (fX - tPt / tMt * tE1L);
    fT = fX > 0. ? 1. : -1.;
    fT *= TMath::Sqrt(fY * fY + fX * fX + fZ * fZ);
  }

  void FemtoPairPRF::Compute_Hemisphere() {
    Double_t tPx = fpx1 - fpx2;  // reversed
    Double_t tPy = fpy1 - fpy2;  // reversed
    Double_t tPz = fpz1 - fpz2;  // reversed
    Double_t tE  = fe1 + fe2;
    Double_t tPt = tPx * tPx + tPy * tPy;
    Double_t tMt = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM  = TMath::Sqrt(tMt - tPt);
    tMt          = TMath::Sqrt(tMt);
    tPt          = TMath::Sqrt(tPt);

    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    fZ              = tGamma * (fpz1 - tBeta * fe1);
    Double_t tE1L   = tGamma * (fe1 - tBeta * fpz1);

    // Rotate in transverse plane
    fX = (fpx1 * tPx + fpy1 * tPy) / tPt;
    fY = (-fpx1 * tPy + fpy1 * tPx) / tPt;
    fX = tMt / tM * (fX - tPt / tMt * tE1L);
    fT = fX > 0. ? 1. : -1.;
    fT *= TMath::Sqrt(fY * fY + fX * fX + fZ * fZ);
  }

  void FemtoPairLCMS::Compute() {
    Double_t tPx    = fpx1 + fpx2;
    Double_t tPy    = fpy1 + fpy2;
    Double_t tPz    = fpz1 + fpz2;
    Double_t tE     = fe1 + fe2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;

    // Transform to LCMS

    Double_t particle1lcms_pz = tGamma * (fpz1 - tBeta * fe1);
    Double_t particle1lcms_e  = tGamma * (fe1 - tBeta * fpz1);
    Double_t particle2lcms_pz = tGamma * (fpz2 - tBeta * fe2);
    Double_t particle2lcms_e  = tGamma * (fe2 - tBeta * fpz2);

    // Rotate in transverse plane

    Double_t particle1lcms_px = (fpx1 * tPx + fpy1 * tPy) / tPt;
    Double_t particle1lcms_py = (-fpx1 * tPy + fpy1 * tPx) / tPt;

    Double_t particle2lcms_px = (fpx2 * tPx + fpy2 * tPy) / tPt;
    Double_t particle2lcms_py = (-fpx2 * tPy + fpy2 * tPx) / tPt;

    fX           = particle1lcms_px - particle2lcms_px;
    fY           = particle1lcms_py - particle2lcms_py;
    fZ           = particle1lcms_pz - particle2lcms_pz;
    Double_t mDE = particle1lcms_e - particle2lcms_e;
    fT           = TMath::Sqrt(TMath::Abs(fX * fX + fY * fY + fZ * fZ - mDE * mDE));
    //  Z = fpz1-fpz2;
  }

  void FemtoPairLCMS::Compute_Rotated() {
    Double_t tPx    = fpx1 - fpx2;  // reversed
    Double_t tPy    = fpy1 - fpy2;  // reversed
    Double_t tPz    = fpz1 + fpz2;
    Double_t tE     = fe1 + fe2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;

    // Transform to LCMS

    Double_t particle1lcms_pz = tGamma * (fpz1 - tBeta * fe1);
    Double_t particle1lcms_e  = tGamma * (fe1 - tBeta * fpz1);
    Double_t particle2lcms_pz = tGamma * (fpz2 - tBeta * fe2);
    Double_t particle2lcms_e  = tGamma * (fe2 - tBeta * fpz2);

    // Rotate in transverse plane

    Double_t particle1lcms_px = (fpx1 * tPx + fpy1 * tPy) / tPt;
    Double_t particle1lcms_py = (-fpx1 * tPy + fpy1 * tPx) / tPt;

    Double_t particle2lcms_px = (-fpx2 * tPx - fpy2 * tPy) / tPt;  // reversed
    Double_t particle2lcms_py = (+fpx2 * tPy - fpy2 * tPx) / tPt;
    ;  // reversed

    fX = particle1lcms_px - particle2lcms_px;
    fY = particle1lcms_py - particle2lcms_py;
    fZ = particle1lcms_pz - particle2lcms_pz;
    //  Z = fpz1-fpz2;
    Double_t mDE = particle1lcms_e - particle2lcms_e;
    fT           = TMath::Sqrt(TMath::Abs(fX * fX + fY * fY + fZ * fZ - mDE * mDE));
  }

  void FemtoPairLCMS::Compute_Hemisphere() {
    Double_t tPx    = fpx1 - fpx2;  // reversed
    Double_t tPy    = fpy1 - fpy2;  // reversed
    Double_t tPz    = fpz1 - fpz2;  // reversed
    Double_t tE     = fe1 + fe2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;

    // Transform to LCMS

    Double_t particle1lcms_pz = tGamma * (fpz1 - tBeta * fe1);
    Double_t particle1lcms_e  = tGamma * (fe1 - tBeta * fpz1);
    Double_t particle2lcms_pz = tGamma * (-fpz2 - tBeta * fe2);  // reversed
    Double_t particle2lcms_e  = tGamma * (fe2 + tBeta * fpz2);   // reversed

    // Rotate in transverse plane

    Double_t particle1lcms_px = (fpx1 * tPx + fpy1 * tPy) / tPt;
    Double_t particle1lcms_py = (-fpx1 * tPy + fpy1 * tPx) / tPt;

    Double_t particle2lcms_px = (-fpx2 * tPx - fpy2 * tPy) / tPt;  // reversed
    Double_t particle2lcms_py = (+fpx2 * tPy - fpy2 * tPx) / tPt;
    ;  // reversed

    fX = particle1lcms_px - particle2lcms_px;
    fY = particle1lcms_py - particle2lcms_py;
    fZ = particle1lcms_pz - particle2lcms_pz;
    //  Z = fpz1-fpz2;
    Double_t mDE = particle1lcms_e - particle2lcms_e;
    fT           = TMath::Sqrt(TMath::Abs(fX * fX + fY * fY + fZ * fZ - mDE * mDE));
  }

  void FemtoPairPRF_SH::Compute() {
    Double_t tPx = fpx1 + fpx2;
    Double_t tPy = fpy1 + fpy2;
    Double_t tPz = fpz1 + fpz2;
    Double_t tE  = fe1 + fe2;
    Double_t tPt = tPx * tPx + tPy * tPy;
    Double_t tMt = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM  = TMath::Sqrt(tMt - tPt);
    tMt          = TMath::Sqrt(tMt);
    tPt          = TMath::Sqrt(tPt);

    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    fZ              = tGamma * (fpz1 - tBeta * fe1);
    Double_t tE1L   = tGamma * (fe1 - tBeta * fpz1);

    fX = (fpx1 * tPx + fpy1 * tPy) / tPt;
    fY = (-fpx1 * tPy + fpy1 * tPx) / tPt;
    fX = tMt / tM * (fX - tPt / tMt * tE1L);
    //	  fT = fX>0.? 1. : -1.;
    //	  fT *= TMath::Sqrt(fY*fY + fX*fX + fZ*fZ);
    ConvertToSh();
  }

  void FemtoPairPRF_SH::Compute_Rotated() {
    Double_t tPx = fpx1 - fpx2;  // reversed
    Double_t tPy = fpy1 - fpy2;  // reversed
    Double_t tPz = fpz1 + fpz2;
    Double_t tE  = fe1 + fe2;
    Double_t tPt = tPx * tPx + tPy * tPy;
    Double_t tMt = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM  = TMath::Sqrt(tMt - tPt);
    tMt          = TMath::Sqrt(tMt);
    tPt          = TMath::Sqrt(tPt);

    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    fZ              = tGamma * (fpz1 - tBeta * fe1);
    Double_t tE1L   = tGamma * (fe1 - tBeta * fpz1);

    // Rotate in transverse plane
    fX = (fpx1 * tPx + fpy1 * tPy) / tPt;
    fY = (-fpx1 * tPy + fpy1 * tPx) / tPt;
    fX = tMt / tM * (fX - tPt / tMt * tE1L);
    fT = fX > 0. ? 1. : -1.;
    fT *= TMath::Sqrt(fY * fY + fX * fX + fZ * fZ);
    ConvertToSh();
  }

  void FemtoPairPRF_SH::Compute_Hemisphere() {
    Double_t tPx = fpx1 - fpx2;  // reversed
    Double_t tPy = fpy1 - fpy2;  // reversed
    Double_t tPz = fpz1 - fpz2;  // reversed
    Double_t tE  = fe1 + fe2;
    Double_t tPt = tPx * tPx + tPy * tPy;
    Double_t tMt = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM  = TMath::Sqrt(tMt - tPt);
    tMt          = TMath::Sqrt(tMt);
    tPt          = TMath::Sqrt(tPt);

    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    fZ              = tGamma * (fpz1 - tBeta * fe1);
    Double_t tE1L   = tGamma * (fe1 - tBeta * fpz1);

    // Rotate in transverse plane
    fX = (fpx1 * tPx + fpy1 * tPy) / tPt;
    fY = (-fpx1 * tPy + fpy1 * tPx) / tPt;
    fX = tMt / tM * (fX - tPt / tMt * tE1L);
    fT = fX > 0. ? 1. : -1.;
    fT *= TMath::Sqrt(fY * fY + fX * fX + fZ * fZ);
    ConvertToSh();
  }

  void FemtoPairLCMS_SH::Compute() {
    Double_t tPx    = fpx1 + fpx2;
    Double_t tPy    = fpy1 + fpy2;
    Double_t tPz    = fpz1 + fpz2;
    Double_t tE     = fe1 + fe2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;

    // Transform to LCMS

    Double_t particle1lcms_pz = tGamma * (fpz1 - tBeta * fe1);
    Double_t particle2lcms_pz = tGamma * (fpz2 - tBeta * fe2);

    // Rotate in transverse plane

    Double_t particle1lcms_px = (fpx1 * tPx + fpy1 * tPy) / tPt;
    Double_t particle1lcms_py = (-fpx1 * tPy + fpy1 * tPx) / tPt;

    Double_t particle2lcms_px = (fpx2 * tPx + fpy2 * tPy) / tPt;
    Double_t particle2lcms_py = (-fpx2 * tPy + fpy2 * tPx) / tPt;
    ;
    fX = particle1lcms_px - particle2lcms_px;
    fY = particle1lcms_py - particle2lcms_py;
    fZ = particle1lcms_pz - particle2lcms_pz;
    //  Z = fpz1-fpz2;
    // fT=TMath::Sqrt(TMath::Abs(fX*fX+fY*fY+fZ*fZ-mDE*mDE));
    ConvertToSh();
  }

  void FemtoPairLCMS_SH::Compute_Rotated() {
    Double_t tPx    = fpx1 - fpx2;  // reversed
    Double_t tPy    = fpy1 - fpy2;  // reversed
    Double_t tPz    = fpz1 + fpz2;
    Double_t tE     = fe1 + fe2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;

    // Transform to LCMS

    Double_t particle1lcms_pz = tGamma * (fpz1 - tBeta * fe1);
    Double_t particle2lcms_pz = tGamma * (fpz2 - tBeta * fe2);

    // Rotate in transverse plane

    Double_t particle1lcms_px = (fpx1 * tPx + fpy1 * tPy) / tPt;
    Double_t particle1lcms_py = (-fpx1 * tPy + fpy1 * tPx) / tPt;

    Double_t particle2lcms_px = (-fpx2 * tPx - fpy2 * tPy) / tPt;  // reversed
    Double_t particle2lcms_py = (+fpx2 * tPy - fpy2 * tPx) / tPt;
    ;  // reversed

    fX = particle1lcms_px - particle2lcms_px;
    fY = particle1lcms_py - particle2lcms_py;
    fZ = particle1lcms_pz - particle2lcms_pz;
    ConvertToSh();
  }

  void FemtoPairLCMS_SH::Compute_Hemisphere() {
    Double_t tPx    = fpx1 - fpx2;  // reversed
    Double_t tPy    = fpy1 - fpy2;  // reversed
    Double_t tPz    = fpz1 - fpz2;  // reversed
    Double_t tE     = fe1 + fe2;
    Double_t tPt    = tPx * tPx + tPy * tPy;
    Double_t tMt    = tE * tE - tPz * tPz;  // mCVK;
    tMt             = TMath::Sqrt(tMt);
    tPt             = TMath::Sqrt(tPt);
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;

    // Transform to LCMS

    Double_t particle1lcms_pz = tGamma * (fpz1 - tBeta * fe1);
    Double_t particle2lcms_pz = tGamma * (-fpz2 - tBeta * fe2);  // reversed

    // Rotate in transverse plane

    Double_t particle1lcms_px = (fpx1 * tPx + fpy1 * tPy) / tPt;
    Double_t particle1lcms_py = (-fpx1 * tPy + fpy1 * tPx) / tPt;

    Double_t particle2lcms_px = (-fpx2 * tPx - fpy2 * tPy) / tPt;  // reversed
    Double_t particle2lcms_py = (+fpx2 * tPy - fpy2 * tPx) / tPt;
    ;  // reversed

    fX = particle1lcms_px - particle2lcms_px;
    fY = particle1lcms_py - particle2lcms_py;
    fZ = particle1lcms_pz - particle2lcms_pz;
    ConvertToSh();
  }

  void FemtoPairDPhiDEta::Compute() {
    fX = GetPhi1() - GetPhi2();
    fY = GetTrack1()->GetMomentum().Eta() - GetTrack2()->GetMomentum().Eta();
    while (fX < fPhiMin)
      fX += fPI2;
    while (fX > fPhiMax)
      fX -= fPI2;
  }

  void FemtoPairDPhiDEta::Compute_Rotated() {
    fX = GetPhi1() - GetPhi2() + TMath::Pi();
    fY = GetTrack1()->GetMomentum().Eta() - GetTrack2()->GetMomentum().Eta();
    while (fX < fPhiMin)
      fX += fPI2;
    while (fX > fPhiMax)
      fX -= fPI2;
  }

  void FemtoPairDPhiDEta::Compute_Hemisphere() {
    fX = GetPhi1() - GetPhi2() + TMath::Pi();
    fY = GetTrack1()->GetMomentum().Eta() + GetTrack2()->GetMomentum().Eta();
    while (fX < fPhiMin)
      fX += fPI2;
    while (fX > fPhiMax)
      fX -= fPI2;
  }

  FemtoPairDPhiDEta::FemtoPairDPhiDEta(Bool_t use_mc, Double_t phi_min) : FemtoPair(use_mc) {
    if ((phi_min > (-TMath::Pi() * 2.0)) && (phi_min < TMath::Pi() * 2.0)) {
      fPhiMin = phi_min;
      fPhiMax = fPhiMin + 2.0 * TMath::Pi();
    } else {
      fPhiMin = 0;
      fPhiMax = TMath::Pi() * 2.0;
    }
    fPI2 = TMath::Pi() * 2.0;
  }

  void FemtoPair_SH::ConvertToSh() {
    TVector3 vec;
    vec.SetXYZ(fX, fY, fZ);
    fX = vec.Mag();
    fY = vec.Phi();
    fZ = vec.Theta();
  }

  void FemtoPairPRFL::Compute() {
    TLorentzVector p1, sum;
    p1.SetXYZT(fpx1, fpy1, fpz1, fe1);
    sum.SetXYZT(fpx2, fpy2, fpz2, fe2);
    sum += p1;
    TVector3 boost = sum.BoostVector();
    Double_t phi   = sum.Phi();
    p1.Boost(-boost);
    p1.RotateZ(-phi);
    fX = p1.Px();
    fY = p1.Py();
    fZ = p1.Pz();
    fT = p1.Rho();
  }

  void FemtoPairPRFL::Compute_Rotated() {
    TLorentzVector p1, sum;
    p1.SetXYZT(fpx1, fpy1, fpz1, fe1);
    sum.SetXYZT(-fpx2, -fpy2, fpz2, fe2);
    sum += p1;
    TVector3 boost = sum.BoostVector();
    Double_t phi   = sum.Phi();
    p1.Boost(-boost);
    p1.RotateZ(-phi);
    fX = p1.Px();
    fY = p1.Py();
    fZ = p1.Pz();
    fT = p1.Rho();
  }

  void FemtoPairPRFL::Compute_Hemisphere() {
    TLorentzVector p1, sum;
    p1.SetXYZT(fpx1, fpy1, fpz1, fe1);
    sum.SetXYZT(-fpx2, -fpy2, -fpz2, fe2);
    sum += p1;
    TVector3 boost = sum.BoostVector();
    Double_t phi   = sum.Phi();
    p1.Boost(-boost);
    p1.RotateZ(-phi);
    fX = p1.Px();
    fY = p1.Py();
    fZ = p1.Pz();
    fT = p1.Rho();
  }
}  // namespace Hal
//---------------- HAL FEMTO KINEMATICS LCMS  - COMPUTE LCMS WITHOUTH WEIGHT
