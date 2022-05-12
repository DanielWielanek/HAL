/*
 * Femto1DCFAnaMapMCRoco.cxx
 *
 *  Created on: 2 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto1DCFAnaMapMCRoco.h"

#include "Cout.h"
#include "DividedHisto.h"
#include "Femto1DCF.h"
#include "FemtoPair.h"
#include "FemtoSourceModel.h"
#include "FemtoSourceModelNumerical1D.h"

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
#include <iostream>


namespace Hal {
  Femto1DCFAnaMapMCRoco::Femto1DCFAnaMapMCRoco() :
    fKStarMin(0),
    fKStarMax(1),
    fRMinEff(0),
    fRStep(0),
    fRmin(0),
    fRmax(10),
    fSourceParams(nullptr),
    fM1(0),
    fM2(0),
    fKStarBins(10),
    fKinematics(Femto::EKinematics::kLCMS),
    fRbins(10),
    fPair(NULL),
    fGeneratorRaw(nullptr),
    fGeneratorIntegrated(nullptr),
    fWeight(nullptr),
    fMap(nullptr),
    fModelType(EModelType::kOther),
    fSampleRandom(nullptr) {}

  Femto1DCFAnaMapMCRoco::~Femto1DCFAnaMapMCRoco() {
    if (fPair) delete fPair;
    if (fGeneratorRaw) delete fGeneratorRaw;
    if (fGeneratorIntegrated) delete fGeneratorIntegrated;
    if (fSourceParams) delete fSourceParams;
    if (fSampleRandom) delete fSampleRandom;
  }

  void Femto1DCFAnaMapMCRoco::SetCorrFctn(const Femto1DCF& cf) {
    fKinematics = cf.GetFrame();
    fKStarMin   = cf.GetNum()->GetXaxis()->GetBinLowEdge(1);
    fKStarMax   = cf.GetNum()->GetXaxis()->GetBinUpEdge(cf.GetNum()->GetNbinsX());
    fKStarBins  = cf.GetNum()->GetNbinsX();
  }

  void Femto1DCFAnaMapMCRoco::SetRadiusRange(Int_t r_bins, Double_t r_min, Double_t r_max) {
    fRbins = r_bins;
    fRmin  = r_min;
    fRmax  = r_max;
  }

  void Femto1DCFAnaMapMCRoco::Exec(Int_t pairs_per_bin, Double_t refRadius) {
    const Int_t pointsQ                    = fMap->GetNum()->GetNbinsX() + 1;
    Double_t* kstar                        = new Double_t[pointsQ];
    Double_t* kfill                        = new Double_t[pointsQ];
    Double_t** parametrizations            = new Double_t*[fRbins];
    FemtoSourceModel* sourceModel          = fGeneratorRaw->GetSourceModel();
    FemtoSourceModel* sourceModelntegrated = fGeneratorIntegrated->GetSourceModel();
    Int_t sourceParamsNo                   = sourceModel->GetNpar();
    for (int i = 0; i < fRbins; i++) {
      parametrizations[i] = new Double_t[sourceParamsNo];
    }

    for (int i = 1; i <= fMap->GetNum()->GetNbinsX(); i++) {
      kstar[i] = fMap->GetNum()->GetXaxis()->GetBinCenter(i);
      kfill[i] = kstar[i];
      if (fKinematics == Femto::EKinematics::kLCMS) { kstar[i] = kstar[i] * 0.5; }
    }
    TVector3 boost(0.1, 0.1, 0.1);

    const Double_t scale = 1.0 / TMath::Sqrt(3.0);


    for (int i = 0; i < fRbins; i++) {
      for (int j = 0; j < sourceParamsNo; j++) {
        parametrizations[i][j] = fSourceParams[j];
      }
    }
    switch (fModelType) {
      case EModelType::k1dModel: {
        for (int i = 0; i < fRbins; i++) {
          parametrizations[i][0] = fRadiiBins[i];
        }
      } break;
      case EModelType::k3dModel: {
        for (int i = 0; i < fRbins; i++) {
          parametrizations[i][0] = fRadiiBins[i] * scale;
          parametrizations[i][1] = fRadiiBins[i] * scale;
          parametrizations[i][2] = fRadiiBins[i] * scale;
        }
      } break;
    };
    Int_t mainBin = fRbins * 0.5;
    if (refRadius != 0) { mainBin = fMap->GetNum()->GetYaxis()->FindBin(refRadius); }
    //  mainBin = 0;
    //  fGenerator->GetSourceModel()->SetRadius(fRadiiBins[mainBin]);  // generate pairs for only one parametrization
    TH1D* monGaus1 = new TH1D("monG1", "monG", 500, 0, 50);
    TH1D* monGaus2 = new TH1D("monG3", "monG", 500, 0, 50);
    TH1D* monGaus3 = new TH1D("monG6", "monG", 500, 0, 50);
    TH1D* monGaus4 = new TH1D("monG9", "monG", 500, 0, 50);
    TH1D* monGaus5 = new TH1D("monG10", "monG", 500, 0, 50);
    TH1D* monRaw   = new TH1D("monR", "monR", 500, 0, 50);

    for (int ikst = 1; ikst <= fMap->GetNum()->GetNbinsX(); ikst++) {
      Double_t E1 = TMath::Sqrt(fM1 + kstar[ikst] * kstar[ikst]);
      Double_t E2 = TMath::Sqrt(fM2 + kstar[ikst] * kstar[ikst]);
      Double_t px, py, pz;
      gRandom->Sphere(px, py, pz, kstar[ikst]);
      TLorentzVector p1(px, py, pz, E1);
      TLorentzVector p2(-px, -py, -pz, E2);
      p1.Boost(boost);
      p2.Boost(boost);
      fPair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.T());
      fPair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.T());

      for (int i = 0; i < pairs_per_bin; i++) {
        fGeneratorIntegrated->GenerateFreezoutCooordinates(fPair);
        Double_t weight = fWeight->GenerateWeight(fPair);
        TVector3 Radius(sourceModelntegrated->GetROut(), sourceModelntegrated->GetRSide(), sourceModelntegrated->GetRLong());
        Double_t RadiusFor1D[3] = {0, 0, 0};
        Double_t Rinv           = Radius.Mag();
        RadiusFor1D[0]          = Rinv * TMath::Sqrt(3);
        Double_t refWeight      = 1.0 / sourceModelntegrated->GetProbDensity3d(Radius, nullptr);
        // std::cout << "\t" << std::endl;
        for (int r_bin = 0; r_bin < fRbins; r_bin++) {
          Double_t R         = fRadiiBins[r_bin];
          Double_t newWeight = sourceModel->GetProbDensity3d(Radius, parametrizations[r_bin]);  // why?
          Double_t effWeight = newWeight * refWeight;
          if (r_bin == 1) {
            monRaw->Fill(Rinv, 1);
            monGaus1->Fill(Rinv, effWeight);
          }
          if (r_bin == 10) {
            //   monRaw->Fill(Rinv, 1);
            monGaus2->Fill(Rinv, effWeight);
          }
          if (r_bin == 25) {
            // monRaw->Fill(Rinv, 1);
            monGaus3->Fill(Rinv, effWeight);
          }
          if (r_bin == 50) {
            // monRaw->Fill(Rinv, 1);
            monGaus4->Fill(Rinv, effWeight);
          }
          if (r_bin == 75) {
            // monRaw->Fill(Rinv, 1);
            monGaus5->Fill(Rinv, effWeight);
          }
          // std::cout << r_bin << " " << newWeight << " " << refWeight << std::endl;
          ((TH2*) fMap->GetNum())->Fill(kfill[ikst], R, weight * effWeight);
          ((TH2*) fMap->GetDen())->Fill(kfill[ikst], R, effWeight);
        }
      }
    }
    TFile* fx = new TFile("ctrl.root", "recreate");
    monRaw->Write();
    monGaus1->Write();
    monGaus2->Write();
    monGaus3->Write();
    monGaus4->Write();
    monGaus5->Write();
    fx->Close();
    /*
      mainBin = fRbins * 0.5;
      fGenerator->GetSourceModel()->SetRadius(fRadiiBins[mainBin]);
      for (int ikst = 1; ikst <= fMap->GetNum()->GetNbinsX(); ikst++) {
        Double_t E1 = TMath::Sqrt(fM1 + kstar[ikst] * kstar[ikst]);
        Double_t E2 = TMath::Sqrt(fM2 + kstar[ikst] * kstar[ikst]);
        Double_t px, py, pz;
        gRandom->Sphere(px, py, pz, kstar[ikst]);
        TLorentzVector p1(px, py, pz, E1);
        TLorentzVector p2(-px, -py, -pz, E2);
        p1.Boost(boost);
        p2.Boost(boost);
        fPair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.T());
        fPair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.T());

        for (int i = 0; i < pairs_per_bin; i++) {
          fGenerator->GenerateFreezoutCooordinates(fPair);
          Double_t weight    = fWeight->GenerateWeight(fPair);
          Double_t Radius[3] = {sourceModel->GetROut(), sourceModel->GetRSide(), sourceModel->GetRLong()};
          Double_t refWeight = 1.0 / sourceModel->GetProbDensity(Radius, parametrizations[mainBin]);
          for (int r_bin = 0; r_bin < fRbins; r_bin++) {
            Double_t R         = fRadiiBins[r_bin];
            Double_t newWeight = sourceModel->GetProbDensity(Radius, parametrizations[r_bin]);
            Double_t effWeight = newWeight * refWeight;
            ((TH2*) fMap->GetNum())->Fill(kfill[ikst], R, weight * effWeight);
            ((TH2*) fMap->GetDen())->Fill(kfill[ikst], R, effWeight);
          }
        }
      }


      mainBin = fRbins * 0.9;
      fGenerator->GetSourceModel()->SetRadius(fRadiiBins[mainBin]);
      for (int ikst = 1; ikst <= fMap->GetNum()->GetNbinsX(); ikst++) {
        Double_t E1 = TMath::Sqrt(fM1 + kstar[ikst] * kstar[ikst]);
        Double_t E2 = TMath::Sqrt(fM2 + kstar[ikst] * kstar[ikst]);
        Double_t px, py, pz;
        gRandom->Sphere(px, py, pz, kstar[ikst]);
        TLorentzVector p1(px, py, pz, E1);
        TLorentzVector p2(-px, -py, -pz, E2);
        p1.Boost(boost);
        p2.Boost(boost);
        fPair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.T());
        fPair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.T());

        for (int i = 0; i < pairs_per_bin; i++) {
          fGenerator->GenerateFreezoutCooordinates(fPair);
          Double_t weight    = fWeight->GenerateWeight(fPair);
          Double_t Radius[3] = {sourceModel->GetROut(), sourceModel->GetRSide(), sourceModel->GetRLong()};
          Double_t refWeight = 1.0 / sourceModel->GetProbDensity(Radius, parametrizations[mainBin]);
          for (int r_bin = 0; r_bin < fRbins; r_bin++) {
            Double_t R         = fRadiiBins[r_bin];
            Double_t newWeight = sourceModel->GetProbDensity(Radius, parametrizations[r_bin]);
            Double_t effWeight = newWeight * refWeight;
            ((TH2*) fMap->GetNum())->Fill(kfill[ikst], R, weight * effWeight);
            ((TH2*) fMap->GetDen())->Fill(kfill[ikst], R, effWeight);
          }
        }
      }
    */

    for (int i = 0; i < fRbins; i++) {
      delete[] parametrizations[i];
    }
    delete[] parametrizations;
    delete[] kfill;
    delete[] kstar;
  }

  void Femto1DCFAnaMapMCRoco::SaveMap(TString filename) {
    TFile* file = new TFile(filename, "recreate");
    fMap->Write("map");
    file->Close();
  }

  Bool_t Femto1DCFAnaMapMCRoco::Init() {
    if (fGeneratorRaw == nullptr) return kFALSE;
    if (fGeneratorRaw->GetSourceModel()->GetModelNumProp() != FemtoSourceModel::ENumProperty::kFullyAnalytical) {
      Cout::PrintInfo(" Femto1DCFAnaMapMCRoco::Init - cannot use nonanalytical source emission function", EInfo::kLessWarning);
      return kFALSE;
    }
    FemtoSourceModel* sourceModel = fGeneratorRaw->GetSourceModel();
    Int_t sourceParamsNo          = sourceModel->GetNpar();
    fSourceParams                 = new Double_t[sourceParamsNo];
    for (int i = 0; i < sourceParamsNo; i++) {
      fSourceParams[i] = sourceModel->GetParameter(i);
    }

    if (dynamic_cast<FemtoSourceModel1D*>(sourceModel)) {
      fModelType = EModelType::k1dModel;
    } else if (dynamic_cast<FemtoSourceModel3D*>(sourceModel)) {
      fModelType = EModelType::k3dModel;
    } else {
      Cout::PrintInfo(" Femto1DCFAnaMapMCRoco::Init - cannot use analytical model that not base from 1d or 3d source model",
                      EInfo::kLessWarning);
      return kFALSE;
    }

    fPair    = Femto::MakePair(fKinematics, kFALSE);
    fMap     = new DividedHisto2D("map", fKStarBins, fKStarMin, fKStarMax, fRbins, fRmin, fRmax, 'D');
    fRStep   = (fRmax - fRmin) / ((Double_t) fRbins);
    fRMinEff = fRmin + 0.5 * fRStep;
    fRadiiBins.MakeBigger(fRbins);
    for (int r_bin = 0; r_bin < fRbins; r_bin++) {
      fRadiiBins[r_bin] = fRMinEff + ((double) r_bin) * fRStep;
    }

    TDatabasePDG* pdg   = TDatabasePDG::Instance();
    Int_t pid1          = fWeight->GetPdg1();
    Int_t pid2          = fWeight->GetPdg2();
    TParticlePDG* part1 = pdg->GetParticle(pid1);
    TParticlePDG* part2 = pdg->GetParticle(pid2);
    if (part1 == NULL) return kFALSE;
    if (part2 == NULL) return kFALSE;
    fPair->SetPdg1(pid1);
    fPair->SetPdg2(pid2);
    fM1           = part1->Mass();
    fM2           = part2->Mass();
    fM1           = fM1 * fM1;
    fM2           = fM2 * fM2;
    Double_t rmin = 0;
    Double_t rmax = fRadiiBins[fRbins - 1] * 5;
    if (fSampleRandom) {
      delete fSampleRandom;
      fSampleRandom = nullptr;
    }
    fSampleRandom      = new TH1D("samram", "samram", 50, rmin, rmax);
    Double_t params[1] = {0};
    for (int r_bin = 0; r_bin < fRbins; r_bin++) {
      params[0] = fRadiiBins[r_bin];
      for (int i = 1; i <= fSampleRandom->GetNbinsX(); i++) {
        Double_t r   = fSampleRandom->GetXaxis()->GetBinCenter(i);
        Double_t val = fGeneratorRaw->GetSourceModel()->GetProbDensity1d(r, params);
        if (val > fSampleRandom->GetBinContent(i)) { fSampleRandom->SetBinContent(i, val); }
      }
    }
    fGeneratorIntegrated = fGeneratorRaw->MakeCopy();
    FemtoSourceModelNumerical1D model;
    model.SetRadiusDistribution(*fSampleRandom);
    fGeneratorIntegrated->SetSourceModel(model);


    return kTRUE;
  }
}  // namespace Hal
