/*
 * CorrFitPairGeneratorYPtKtCluster.cxx
 *
 *  Created on: 14 paź 2024
 *      Author: daniel
 */

#include "CorrFitPairGeneratorYPtKtCluster.h"

TLorentzVector Hal::CorrFitPairGeneratorYPtKtCluster::GeneratePairPRF(TLorentzVector& p1, TVector3& q) {

  TLorentzVector Ptotal = p1;  // Na razie mamy tylko p1, później dodamy p2

  TLorentzVector p1CMS = p1;
  p1CMS.Boost(-Ptotal.BoostVector());

  TVector3 p2CMS_vec = p1CMS.Vect() + q;
  const double m2    = 0.938;
  // Ustalanie energii cząstki p2 w CMS
  double E2CMS = TMath::Sqrt(p2CMS_vec.Mag2() + m2 * m2);  // m2 to masa cząstki p2
  TLorentzVector p2CMS(p2CMS_vec, E2CMS);

  // Transformacja z powrotem do układu laboratoryjnego
  TLorentzVector p2 = p2CMS;
  p2.Boost(Ptotal.BoostVector());
  return p2;
}

TLorentzVector Hal::CorrFitPairGeneratorYPtKtCluster::GeneratePairLCMS(TLorentzVector& p1, TVector3& vec) { TLorentzVector p2; }
