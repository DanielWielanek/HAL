/*
 * WielanekMapGenerator.cxx
 *
 *  Created on: 28 sty 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoWielanekMapGenerator.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "Femto1DMapGenerator.h"

#include <TAxis.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TVector3.h>
#include <cstdlib>

#include "FemtoPair.h"
#include "FemtoPairKinematics.h"
#include "Std.h"

namespace Hal {
  FemtoWielanekMapGenerator::FemtoWielanekMapGenerator() :
    Femto1DMapGenerator(kFALSE), fPairsNo(100), fWeightArray(nullptr), fAngleArray(nullptr) {}

  void FemtoWielanekMapGenerator::Exec(Int_t pairs_per_bin) {
    fPairsNo           = pairs_per_bin;
    FemtoPairPRF* pair = new FemtoPairPRF();
    fWeightArray       = new Double_t[fPairsNo];
    fAngleArray        = new Double_t[fPairsNo];
    Double_t step      = TMath::Pi() / ((Double_t) fPairsNo);
    Double_t sum       = 0;
    Int_t count        = 0;
    // calculate angular weight
    for (Double_t i = 0; i < TMath::Pi(); i += step) {
      fWeightArray[count] = TMath::Cos(i - 0.5 * step) - TMath::Cos(i + 0.5 * step);
      fAngleArray[count]  = i + 0.5 * step;
      sum += fWeightArray[count];
      count++;
    }
    // normalizing weight
    for (int i = 0; i < fPairsNo; i++) {
      fWeightArray[i] = fWeightArray[i] / sum;
    }
    for (int i = 1; i <= fMap->GetNum()->GetNbinsX(); i++) {
      Double_t k = fMap->GetNum()->GetXaxis()->GetBinCenter(i);
      for (int j = 1; j <= fMap->GetNum()->GetNbinsY(); j++) {
        Double_t r = fMap->GetNum()->GetYaxis()->GetBinCenter(j);
        fMap->GetNum()->SetBinContent(i, j, GeneratePairs(pair, r, k));
        // fMap->GetDen()->SetBinContent(i, j, 1);
      }
    }
    delete[] fWeightArray;
    delete[] fAngleArray;
  }

  Bool_t FemtoWielanekMapGenerator::Init() {
    Bool_t res = Femto1DMapGenerator::Init();
    fMap->GetNum()->GetXaxis()->SetTitle("K*[GeV/c]");
    fMap->GetNum()->GetYaxis()->SetTitle("R*[fm]");
    return res;
  }

  Double_t FemtoWielanekMapGenerator::GeneratePairs(FemtoPair* pair, Double_t R, Double_t K) {
    Double_t sum = 0;
    Double_t px1 = K;
    Double_t e1  = TMath::Sqrt(px1 * px1 + 0.02 + fMass12);
    Double_t e2  = TMath::Sqrt(px1 * px1 + 0.02 + fMass22);
    TLorentzVector p, x;
    p.SetPxPyPzE(0.2, 0, 0.2, e1 + e2);
    TVector3 boost = p.BoostVector();
    pair->SetTrueMomenta1(0.1, px1, 0.1, e1);
    pair->SetTrueMomenta2(0.1, -px1, 0.1, e2);
    for (int i = 0; i < fPairsNo; i++) {
      x.SetXYZT(R * TMath::Sin(fAngleArray[i]), R * TMath::Cos(fAngleArray[i]), 0, 0);
      x.Boost(boost);
      pair->SetX1(x.X());
      pair->SetY1(x.Y());
      Double_t v = fWeight->GenerateWeight(pair);
      sum += fWeightArray[i] * v;
    }
    return sum;
  }

  FemtoWielanekMapGenerator::~FemtoWielanekMapGenerator() {}
}  // namespace Hal
