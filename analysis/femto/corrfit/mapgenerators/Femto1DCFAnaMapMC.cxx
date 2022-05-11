/*
 * Femto1DCFAnaMapMC.cxx
 *
 *  Created on: 2 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto1DCFAnaMapMC.h"


#include <TAxis.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TVector3.h>

#include "DividedHisto.h"
#include "Femto1DCF.h"
#include "FemtoPair.h"
#include "FemtoSourceModel.h"

namespace Hal {
  Femto1DCFAnaMapMC::Femto1DCFAnaMapMC() : Femto1DMapGenerator() {}

  Femto1DCFAnaMapMC::~Femto1DCFAnaMapMC() {}

  void Femto1DCFAnaMapMC::Run(Int_t pairs_per_bin) {
    Double_t* kstar = new Double_t[fMap->GetNum()->GetNbinsX() + 1];
    Double_t* kfill = new Double_t[fMap->GetNum()->GetNbinsX() + 1];
    for (int i = 1; i <= fMap->GetNum()->GetNbinsX(); i++) {
      kstar[i] = fMap->GetNum()->GetXaxis()->GetBinCenter(i);
      kfill[i] = kstar[i];
      if (fKinematics == Femto::EKinematics::kLCMS) { kstar[i] = kstar[i] * 0.5; }
    }
    TVector3 boost(0.1, 0.1, 0.1);

    for (int ikst = 1; ikst <= fMap->GetNum()->GetNbinsX(); ikst++) {
      Double_t E1 = TMath::Sqrt(fMass12 + kstar[ikst] * kstar[ikst]);
      Double_t E2 = TMath::Sqrt(fMass22 + kstar[ikst] * kstar[ikst]);
      Double_t px, py, pz;
      gRandom->Sphere(px, py, pz, kstar[ikst]);
      TLorentzVector p1(px, py, pz, E1);
      TLorentzVector p2(-px, -py, -pz, E2);
      p1.Boost(boost);
      p2.Boost(boost);
      fPair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.T());
      fPair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.T());
      for (int r_bin = 0; r_bin < fRBins; r_bin++) {
        Double_t R = fRadiiBins[r_bin];
        fGenerator->GetSourceModel()->SetRadius(R);
        for (int i = 0; i < pairs_per_bin; i++) {
          fGenerator->GenerateFreezoutCooordinates(fPair);
          Double_t weight = fWeight->GenerateWeight(fPair);
          ((TH2*) fMap->GetNum())->Fill(kfill[ikst], R, weight);
          ((TH2*) fMap->GetDen())->Fill(kfill[ikst], R, 1);
        }
      }
    }
    delete[] kfill;
    delete[] kstar;
  }

  Bool_t Femto1DCFAnaMapMC::Init() {
    Bool_t res = Femto1DMapGenerator::Init();
    return res;
  }
}  // namespace Hal
