/*
 * Femto1DCFAnaMapMCRoco.cxx
 *
 *  Created on: 2 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto1DCFAnaMapMCRoco.h"

#include "Const.h"
#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "FastHist.h"
#include "Femto1DCF.h"
#include "FemtoPair.h"
#include "FemtoSourceModel.h"
#include "FemtoSourceModelNumerical1D.h"
#include "StdHist.h"

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
    Femto1DMapGenerator(),
    fRMinEff(0),
    fRStep(0),
    fIntegralScale(1),
    fSourceParams(nullptr),
    fDebugDistribution(kFALSE),
    fGeneratorIntegrated(nullptr),
    fModelType(EModelType::kOther),
    fSampleRandom(nullptr) {}

  Femto1DCFAnaMapMCRoco::~Femto1DCFAnaMapMCRoco() {
    if (fGeneratorIntegrated) delete fGeneratorIntegrated;
    if (fSourceParams) delete fSourceParams;
    if (fSampleRandom) delete fSampleRandom;
  }

  void Femto1DCFAnaMapMCRoco::Exec(Int_t pairs_per_bin, Bool_t autoscale) {
    if (autoscale) pairs_per_bin = (Double_t) pairs_per_bin * fIntegralScale;
    const Int_t pointsQ                    = fMap->GetNum()->GetNbinsX() + 1;
    Double_t* kstar                        = new Double_t[pointsQ];
    Double_t* kfill                        = new Double_t[pointsQ];
    Double_t** parametrizations            = new Double_t*[fRBins];
    FemtoSourceModel* sourceModel          = fGenerator->GetSourceModel();
    FemtoSourceModel* sourceModelntegrated = fGeneratorIntegrated->GetSourceModel();
    Int_t sourceParamsNo                   = sourceModel->GetNpar();
    for (int i = 0; i < fRBins; i++) {
      parametrizations[i] = new Double_t[sourceParamsNo];
    }

    for (int i = 1; i <= fMap->GetNum()->GetNbinsX(); i++) {
      kstar[i] = fMap->GetNum()->GetXaxis()->GetBinCenter(i);
      kfill[i] = kstar[i];
      if (fKinematics == Femto::EKinematics::kLCMS) { kstar[i] = kstar[i] * 0.5; }
    }
    TVector3 boost(0.1, 0.1, 0.1);

    const Double_t scale = 1.0 / TMath::Sqrt(3.0);


    for (int i = 0; i < fRBins; i++) {
      for (int j = 0; j < sourceParamsNo; j++) {
        parametrizations[i][j] = fSourceParams[j];
      }
    }
    switch (fModelType) {
      case EModelType::k1dModel: {
        for (int i = 0; i < fRBins; i++) {
          parametrizations[i][0] = fRadiiBins[i];
        }
      } break;
      case EModelType::k3dModel: {
        for (int i = 0; i < fRBins; i++) {
          parametrizations[i][0] = fRadiiBins[i] * scale;
          parametrizations[i][1] = fRadiiBins[i] * scale;
          parametrizations[i][2] = fRadiiBins[i] * scale;
        }
      } break;
    };
    // Int_t mainBin = fRBins * 0.5;
    // if (refRadius != 0) { mainBin = fMap->GetNum()->GetYaxis()->FindBin(refRadius); }
    //  mainBin = 0;
    //  fGenerator->GetSourceModel()->SetRadius(fRadiiBins[mainBin]);  // generate pairs for only one parametrization

    TH1D* monGaus1 = new TH1D("monG1", "monG", 500, 0, 50);
    TH1D* monGaus2 = new TH1D("monG3", "monG", 500, 0, 50);
    TH1D* monGaus3 = new TH1D("monG6", "monG", 500, 0, 50);
    TH1D* monGaus4 = new TH1D("monG9", "monG", 500, 0, 50);
    TH1D* monGaus5 = new TH1D("monG10", "monG", 500, 0, 50);
    TH1D* monRaw   = new TH1D("monR", "monR", 500, 0, 50);

    TDatabasePDG* pdg  = TDatabasePDG::Instance();
    TParticlePDG* pid1 = pdg->GetParticle(fPid1);
    TParticlePDG* pid2 = pdg->GetParticle(fPid2);
    Double_t m1        = Const::PionPlusMass();
    Double_t m2        = Const::PionPlusMass();
    if (pid1) m1 = pid1->Mass();
    if (pid2) m2 = pid2->Mass();
    m1 = m1 * m1;
    m2 = m2 * m2;
    Int_t nbinsX, nbinsY;
    Double_t minX, maxX, minY, maxY;
    Hal::Std::GetAxisPar(*fMap->GetNum(), nbinsX, minX, maxX, "x");
    Hal::Std::GetAxisPar(*fMap->GetNum(), nbinsY, minY, maxY, "y");


    FastHist2D* num1 = new FastHist2D("2dnum", "2dnum", nbinsX, minX, maxX, nbinsY, minY, maxY);
    FastHist2D* num2 = new FastHist2D("2dden", "2dden", nbinsX, minX, maxX, nbinsY, minY, maxY);
    for (int ikst = 1; ikst <= fMap->GetNum()->GetNbinsX(); ikst++) {
      Double_t E1 = TMath::Sqrt(m1 + kstar[ikst] * kstar[ikst]);
      Double_t E2 = TMath::Sqrt(m2 + kstar[ikst] * kstar[ikst]);
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
        // Double_t RadiusFor1D[3] = {0, 0, 0};
        Double_t Rinv = Radius.Mag();
        // RadiusFor1D[0]          = Rinv * TMath::Sqrt(3);
        Double_t refWeight = 1;  // 1.0 / sourceModelntegrated->GetProbDensity3d(Radius, nullptr);
        refWeight          = 1.0 / sourceModelntegrated->GetProbDensity1d(Rinv, nullptr);
        // std::cout << "\t" << std::endl;
        for (int r_bin = 0; r_bin < fRBins; r_bin++) {
          Double_t R         = fRadiiBins[r_bin];
          Double_t newWeight = 1;  // sourceModel->GetProbDensity3d(Radius, parametrizations[r_bin]);  // why?
          newWeight          = sourceModel->GetProbDensity1d(Rinv, parametrizations[r_bin]);  // why?
          Double_t effWeight = newWeight * refWeight;
          if (fDebugDistribution) {
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
          }
          // std::cout << r_bin << " " << newWeight << " " << refWeight << std::endl;
          Int_t bin = num1->FindBin(kfill[ikst], R);
          num1->IncrementRawBinContent(bin, weight * effWeight);
          num2->IncrementRawBinContent(bin, effWeight);
        }
      }
    }
    for (int i = 0; i <= nbinsX + 1; i++) {
      for (int j = 0; j <= nbinsY + 1; j++) {
        fMap->GetNum()->SetBinContent(i, j, num1->GetBinContent(i, j));
        fMap->GetDen()->SetBinContent(i, j, num2->GetBinContent(i, j));
      }
    }
    delete num1;
    delete num2;
    if (fDebugDistribution) {
      TFile* fx = new TFile("ctrl.root", "recreate");
      monRaw->Write();
      monGaus1->Write();
      monGaus2->Write();
      monGaus3->Write();
      monGaus4->Write();
      monGaus5->Write();
      fx->Close();
    } else {
      delete monRaw;
      delete monGaus1;
      delete monGaus2;
      delete monGaus3;
      delete monGaus4;
      delete monGaus5;
    }
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

    for (int i = 0; i < fRBins; i++) {
      delete[] parametrizations[i];
    }
    delete[] parametrizations;
    delete[] kfill;
    delete[] kstar;
  }

  void Femto1DCFAnaMapMCRoco::SaveMap(TString filename) {
    TFile* file                  = new TFile(filename, "recreate");
    CorrFitMapKstarRstarDiv* map = new CorrFitMapKstarRstarDiv(*fMap, fKinematics);
    map->Write("map");
    file->Close();
  }

  Bool_t Femto1DCFAnaMapMCRoco::Init() {
    if (fGenerator == nullptr) return kFALSE;
    if (fGenerator->GetSourceModel()->GetModelNumProp() != FemtoSourceModel::ENumProperty::kFullyAnalytical) {
      Cout::PrintInfo(" Femto1DCFAnaMapMCRoco::Init - cannot use nonanalytical source emission function", EInfo::kLowWarning);
      return kFALSE;
    }
    FemtoSourceModel* sourceModel = fGenerator->GetSourceModel();
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
                      EInfo::kLowWarning);
      return kFALSE;
    }

    fPair    = Femto::MakePair(fKinematics, kFALSE);
    fMap     = new DividedHisto2D("map", fKStarBins, fKStarMin, fKStarMax, fRBins, fRMin, fRMax, 'D');
    fRStep   = (fRMax - fRMin) / ((Double_t) fRBins);
    fRMinEff = fRMin + 0.5 * fRStep;
    fRadiiBins.MakeBigger(fRBins);
    for (int r_bin = 0; r_bin < fRBins; r_bin++) {
      fRadiiBins[r_bin] = fRMinEff + ((double) r_bin) * fRStep;
    }

    TDatabasePDG* pdg   = TDatabasePDG::Instance();
    Int_t pid1          = fWeight->GetPdg1();
    Int_t pid2          = fWeight->GetPdg2();
    TParticlePDG* part1 = pdg->GetParticle(pid1);
    TParticlePDG* part2 = pdg->GetParticle(pid2);
    if (part1 == nullptr) return kFALSE;
    if (part2 == nullptr) return kFALSE;
    fPair->SetPdg1(pid1);
    fPair->SetPdg2(pid2);
    /*   fM1           = part1->Mass();
       fM2           = part2->Mass();
       fM1           = fM1 * fM1;
       fM2           = fM2 * fM2;*/
    Double_t rmin = 0;
    Double_t rmax = fRadiiBins[fRBins - 1] * 5;
    if (fSampleRandom) {
      delete fSampleRandom;
      fSampleRandom = nullptr;
    }
    fSampleRandom        = new TH1D("samram", "samram", 50, rmin, rmax);
    Double_t params[1]   = {0};
    Double_t minIntegral = 1E+9;
    for (int r_bin = 0; r_bin < fRBins; r_bin++) {
      params[0]              = fRadiiBins[r_bin];
      Double_t localIntegral = 0;
      for (int i = 1; i <= fSampleRandom->GetNbinsX(); i++) {
        Double_t r   = fSampleRandom->GetXaxis()->GetBinCenter(i);
        Double_t val = fGenerator->GetSourceModel()->GetProbDensity1d(r, params);
        localIntegral += val;
        if (val > fSampleRandom->GetBinContent(i)) { fSampleRandom->SetBinContent(i, val); }
      }
      if (localIntegral < minIntegral) { minIntegral = localIntegral; }
    }
    Double_t maxIntegral = 0;
    for (int i = 1; i <= fSampleRandom->GetNbinsX(); i++) {
      maxIntegral += fSampleRandom->GetBinContent(i);
    }
    fIntegralScale = maxIntegral / minIntegral;
    Cout::Text(Form("Integral scale = %4.2f ", fIntegralScale), "L", kYellow);
    fGeneratorIntegrated = fGenerator->MakeCopy();
    FemtoSourceModelNumerical1D model;
    model.SetRadiusDistribution(*fSampleRandom);
    fGeneratorIntegrated->SetSourceModel(model);

    delete fSampleRandom;
    fSampleRandom = nullptr;
    return kTRUE;
  }
}  // namespace Hal
