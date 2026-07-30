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
#include "FemtoSourceDensity.h"
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
  Femto1DCFAnaMapMCRoco::Femto1DCFAnaMapMCRoco() : Femto1DMapGenerator() {}

  Femto1DCFAnaMapMCRoco::~Femto1DCFAnaMapMCRoco() {
    if (fGeneratorIntegrated) delete fGeneratorIntegrated;
  }

  void Femto1DCFAnaMapMCRoco::Run(Int_t pairs_per_bin, Bool_t autoscale) {
    if (autoscale) pairs_per_bin = (Double_t) pairs_per_bin * fIntegralScale;
    TVector3 boost(0.1, 0.1, 0.1);
    if (fDebugDistribution) fDebugData.Init();
    Double_t m1, m2;
    Hal::Const::GetPdgMass(fPid1, m1, Const::PionPlusMass());
    Hal::Const::GetPdgMass(fPid2, m1, Const::PionPlusMass());
    m1 = m1 * m1;
    m2 = m2 * m2;
    Int_t nbinsX, nbinsY;
    Double_t minX, maxX, minY, maxY;
    Hal::Std::GetAxisPar(*fMap->GetNum(), nbinsX, minX, maxX, "x");
    Hal::Std::GetAxisPar(*fMap->GetNum(), nbinsY, minY, maxY, "y");

    FastHist2D* num1 = new FastHist2D("2dnum", "2dnum", nbinsX, minX, maxX, nbinsY, minY, maxY);
    FastHist2D* num2 = new FastHist2D("2dden", "2dden", nbinsX, minX, maxX, nbinsY, minY, maxY);
    for (int ikst = 1; ikst <= fMap->GetNum()->GetNbinsX(); ikst++) {
      Double_t E1 = TMath::Sqrt(m1 + fKStar[ikst] * fKStar[ikst]);
      Double_t E2 = TMath::Sqrt(m2 + fKStar[ikst] * fKStar[ikst]);
      Double_t px, py, pz;
      gRandom->Sphere(px, py, pz, fKStar[ikst]);
      TLorentzVector p1(px, py, pz, E1);
      TLorentzVector p2(-px, -py, -pz, E2);
      p1.Boost(boost);
      p2.Boost(boost);
      fPair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.T());
      fPair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.T());

      for (int i = 0; i < pairs_per_bin; i++) {
        ComputePair(num1, num2, i, ikst);
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
    if (fDebugDistribution) fDebugData.Save();
  }

  void Femto1DCFAnaMapMCRoco::ComputePair(FastHist2D* num1, FastHist2D* num2, int& counter, int ikst) {

    fGeneratorIntegrated->GenerateFreezeoutCooordinates(fPair);
    Double_t weight = fWeight->GenerateWeight(fPair);
    TVector3 Radius(fSourceModelntegrated->GetROut(), fSourceModelntegrated->GetRSide(), fSourceModelntegrated->GetRLong());
    Double_t Rinv      = Radius.Mag();
    Double_t refWeight = 1;  // 1.0 / sourceModelntegrated->GetProbDensity3d(Radius, nullptr);
    refWeight          = 1.0 / fDensityIntegratedModel->GetProbDensity1d(Rinv, nullptr);
    if (TMath::IsNaN(refWeight)) {
      counter--;
      return;
    }
    for (int r_bin = 0; r_bin < fRBins; r_bin++) {
      Double_t R         = fRadiiBins[r_bin];
      Double_t newWeight = 1;  // sourceModel->GetProbDensity3d(Radius, parametrizations[r_bin]);  // why?
      newWeight          = fDensityModel->GetProbDensity1d(Rinv, fParametrizations[r_bin].data());  // why?
      Double_t effWeight = newWeight * refWeight;
      if (fDebugDistribution) {
        if (r_bin == 1) {
          fDebugData.monRaw->Fill(Rinv, 1);
          fDebugData.monGaus1->Fill(Rinv, effWeight);
        }
        if (r_bin == 10) { fDebugData.monGaus2->Fill(Rinv, effWeight); }
        if (r_bin == 25) { fDebugData.monGaus3->Fill(Rinv, effWeight); }
        if (r_bin == 50) { fDebugData.monGaus4->Fill(Rinv, effWeight); }
        if (r_bin == 75) { fDebugData.monGaus5->Fill(Rinv, effWeight); }
      }
      Int_t bin = num1->FindBin(fKFill[ikst], R);
      num1->IncrementRawBinContent(bin, weight * effWeight);
      num2->IncrementRawBinContent(bin, effWeight);
    }
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
    fSourceParams.resize(sourceParamsNo);
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

    fPair = Femto::MakePair(fKinematics, kFALSE);
    RecalcRadii();
    fMap = new DividedHisto2D("map", fKStarBins, fKStarMin, fKStarMax, fRBins, fRMin, fRMax, 'D');
    fMap->SetDirectory(nullptr);

    TDatabasePDG* pdg   = TDatabasePDG::Instance();
    Int_t pid1          = fWeight->GetPdg1();
    Int_t pid2          = fWeight->GetPdg2();
    TParticlePDG* part1 = pdg->GetParticle(pid1);
    TParticlePDG* part2 = pdg->GetParticle(pid2);
    if (part1 == nullptr) return kFALSE;
    if (part2 == nullptr) return kFALSE;
    fPair->SetPdg1(pid1);
    fPair->SetPdg2(pid2);

    CalculateIntegral();

    fSourceModel            = fGenerator->GetSourceModel();
    fSourceModelntegrated   = fGeneratorIntegrated->GetSourceModel();
    fDensityModel           = fSourceModel->GetDensityProb();
    fDensityIntegratedModel = fSourceModelntegrated->GetDensityProb();

    InitParametrizations();

    const Int_t pointsQ = fMap->GetNum()->GetNbinsX() + 1;
    fKStar.resize(pointsQ);
    fKFill.resize(pointsQ);
    for (int i = 1; i <= fMap->GetNum()->GetNbinsX(); i++) {
      fKStar[i] = fMap->GetNum()->GetXaxis()->GetBinCenter(i);
      fKFill[i] = fKStar[i];
      if (fKinematics == Femto::EKinematics::kLCMS) { fKStar[i] = fKStar[i] * 0.5; }
    }
    return kTRUE;
  }

  void Femto1DCFAnaMapMCRoco::debugData::Init() {
    monGaus1 = new TH1D("monG1", "monG", 500, 0, 50);
    monGaus2 = new TH1D("monG3", "monG", 500, 0, 50);
    monGaus3 = new TH1D("monG6", "monG", 500, 0, 50);
    monGaus4 = new TH1D("monG9", "monG", 500, 0, 50);
    monGaus5 = new TH1D("monG10", "monG", 500, 0, 50);
    monRaw   = new TH1D("monR", "monR", 500, 0, 50);
  }

  void Femto1DCFAnaMapMCRoco::debugData::Save() {
    TFile* fx = new TFile("ctrl.root", "recreate");
    monRaw->Write();
    monGaus1->Write();
    monGaus2->Write();
    monGaus3->Write();
    monGaus4->Write();
    monGaus5->Write();
    fx->Close();
  }

  Femto1DCFAnaMapMCRoco::debugData::~debugData() {
    if (monRaw) {
      delete monRaw;
      delete monGaus1;
      delete monGaus2;
      delete monGaus3;
      delete monGaus4;
      delete monGaus5;
    }
  }

  void Femto1DCFAnaMapMCRoco::InitParametrizations() {
    Int_t sourceParamsNo = fSourceModel->GetNpar();
    const Double_t scale = 1.0 / TMath::Sqrt(3.0);
    Hal::Std::ResizeVector2D(fParametrizations, fRBins, sourceParamsNo);
    for (int i = 0; i < fRBins; i++) {
      for (int j = 0; j < sourceParamsNo; j++) {
        fParametrizations[i][j] = fSourceParams[j];
      }
    }
    switch (fModelType) {
      case EModelType::k1dModel: {
        for (int i = 0; i < fRBins; i++) {
          fParametrizations[i][0] = fRadiiBins[i];
        }
      } break;
      case EModelType::k3dModel: {
        for (int i = 0; i < fRBins; i++) {
          fParametrizations[i][0] = fRadiiBins[i] * scale;
          fParametrizations[i][1] = fRadiiBins[i] * scale;
          fParametrizations[i][2] = fRadiiBins[i] * scale;
        }
      } break;
      case EModelType::kOther: {
        // DO NOTHING TODO check
      } break;
    };
  }

  void Femto1DCFAnaMapMCRoco::CalculateIntegral() {
    Double_t rmin = 0;
    Double_t rmax = fRadiiBins[fRBins - 1] * 5;

    auto SampleRandom              = new TH1D("samram", "samram", 50, rmin, rmax);
    Double_t params[1]             = {0};
    Double_t minIntegral           = 1E+9;
    FemtoSourceDensity* sourceBase = fGenerator->GetSourceModel()->GetDensityProb();
    for (int r_bin = 0; r_bin < fRBins; r_bin++) {
      params[0]              = fRadiiBins[r_bin];
      Double_t localIntegral = 0;
      for (int i = 1; i <= SampleRandom->GetNbinsX(); i++) {
        Double_t r   = SampleRandom->GetXaxis()->GetBinCenter(i);
        Double_t val = sourceBase->GetProbDensity1d(r, params);
        localIntegral += val;
        if (val > SampleRandom->GetBinContent(i)) { SampleRandom->SetBinContent(i, val); }
      }
      if (localIntegral < minIntegral) { minIntegral = localIntegral; }
    }
    Double_t maxIntegral = 0;
    for (int i = 1; i <= SampleRandom->GetNbinsX(); i++) {
      maxIntegral += SampleRandom->GetBinContent(i);
    }
    fIntegralScale = maxIntegral / minIntegral;
    Cout::Text(Form("Integral scale = %4.2f ", fIntegralScale), "L", kYellow);
    fGeneratorIntegrated = fGenerator->MakeCopy();
    FemtoSourceModelNumerical1D model;
    model.SetRadiusDistribution(*SampleRandom);
    fGeneratorIntegrated->SetSourceModel(model);
    delete SampleRandom;
  }

}  // namespace Hal
