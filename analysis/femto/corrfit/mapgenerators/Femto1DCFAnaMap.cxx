/*
 * Femto1DCFAnaMap.cpp
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto1DCFAnaMap.h"

#include <TAxis.h>
#include <TH1.h>
#include <TMath.h>
#include <TString.h>

#include "Array.h"
#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "FemtoCorrFuncKt.h"
#include "FemtoFreezoutGenerator.h"
#include "FemtoPair.h"
#include "FemtoSourceModel.h"
#include "FemtoWeightGenerator.h"
#include "Std.h"
#include "ObjectMatrix.h"


namespace Hal {
  FemtoCorrFuncKtMap1D::FemtoCorrFuncKtMap1D(FemtoCorrFuncKt* cfkt,
                                             Int_t Rbins,
                                             Double_t Rmin,
                                             Double_t Rmax,
                                             Femto::EKinematics kin) :
    FemtoCorrFunc1D(NULL, cfkt->GetLabel(), cfkt->GetEntries()), fR(0) {
    fArray             = new ObjectMatrix_1();
    fRange             = cfkt->GetRange();
    DividedHisto1D* cf = (DividedHisto1D*) cfkt->GetCF(0);
    Int_t bins         = cf->GetNum()->GetNbinsX();
    Double_t min       = cf->GetNum()->GetXaxis()->GetBinLowEdge(1);
    Double_t max       = cf->GetNum()->GetXaxis()->GetBinUpEdge(bins);
    TString XTitle, YTitle, ZTitle;
    if (kin == Femto::EKinematics::kPRF) {
      XTitle = "K* [GeV/c]";
      YTitle = "R_{source}";
      ZTitle = "dN_{pairs}/dK*";
    } else if (kin == Femto::EKinematics::kLCMS) {
      XTitle = " Q_{inv} [GeV/c]";
      YTitle = "R_{source}";
      ZTitle = "dN_{pairs}/dQ_{inv}";
    }
    DividedHisto2D* map = new DividedHisto2D("Map", bins, min, max, Rbins, Rmin, Rmax, 'D');
    map->GetNum()->GetXaxis()->SetTitle(XTitle);
    map->GetDen()->GetXaxis()->SetTitle(XTitle);
    map->GetNum()->GetYaxis()->SetTitle(YTitle);
    map->GetDen()->GetYaxis()->SetTitle(YTitle);
    map->GetNum()->GetZaxis()->SetTitle(ZTitle);
    map->GetDen()->GetZaxis()->SetTitle(ZTitle);
    fArray->Init(fRange.GetSize() - 1, map);
  }

  void FemtoCorrFuncKtMap1D::FillNum(FemtoPair* pair) {
    Double_t px      = pair->GetPx1() + pair->GetPx2();
    Double_t py      = pair->GetPy1() + pair->GetPy2();
    Double_t kt      = TMath::Sqrt(px * px + py * py) * 0.5;
    const Int_t size = fRange.GetSize();

    if (kt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (kt < fRange.Get(i)) {
        ((DividedHisto2D*) fArray->At(i - 1))->FillNum(pair->GetT(), fR, pair->GetWeight());
        return;
      }
    }
  }

  void FemtoCorrFuncKtMap1D::FillDenRotated(FemtoPair* pair) {
    Double_t px      = pair->GetPx1() - pair->GetPx2();
    Double_t py      = pair->GetPy1() - pair->GetPy2();
    Double_t kt      = TMath::Sqrt(px * px + py * py) * 0.5;
    const Int_t size = fRange.GetSize();
    if (kt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (kt < fRange.Get(i)) {
        ((DividedHisto2D*) fArray->At(i - 1))->FillDen(pair->GetT(), fR, pair->GetWeight());
        return;
      }
    }
  }

  void FemtoCorrFuncKtMap1D::FillDenMixed(FemtoPair* pair) {
    Double_t px      = pair->GetPx1() + pair->GetPx2();
    Double_t py      = pair->GetPy1() + pair->GetPy2();
    Double_t kt      = TMath::Sqrt(px * px + py * py) * 0.5;
    const Int_t size = fRange.GetSize();
    if (kt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (kt < fRange.Get(i)) {
        ((DividedHisto2D*) fArray->At(i - 1))->FillDen(pair->GetT(), fR, pair->GetWeight());
        return;
      }
    }
  }

  CorrFitMapKstarRstar* FemtoCorrFuncKtMap1D::GetKstarMap(Int_t kt_bin) {
    DividedHisto2D* hist   = (DividedHisto2D*) fArray->At(kt_bin);
    TH2D* h                = (TH2D*) hist->GetHist(kFALSE);
    Femto::EKinematics kin = Femto::EKinematics::kPRF;
    TString tit            = h->GetXaxis()->GetTitle();
    if (tit.Contains("inv")) kin = Femto::EKinematics::kLCMS;
    CorrFitMapKstarRstar* map = new CorrFitMapKstarRstar(*h, kin);
    delete h;
    map->SetName(Form("kt_%i", kt_bin));
    return map;
  }

  FemtoCorrFuncKtMap1D::~FemtoCorrFuncKtMap1D() {
    // TODO Auto-generated destructor stub
  }

  /*******************************************************************************/

  Femto1DCFAnaMap::Femto1DCFAnaMap() : FemtoBasicAna(), fRbins(0), fRMinEff(0), fRStep(0), fRmin(1), fRmax(10) {}

  void Femto1DCFAnaMap::ProcessFemtoPair() {
    fFemtoPair->Compute();
    FemtoCorrFuncKtMap1D* map = ((FemtoCorrFuncKtMap1D*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID));
    for (Double_t i = 0; i < fRbins; i++) {
      Double_t R = fRMinEff + i * fRStep;
      map->SetR(R);
      fFreezoutGenerator->GetSourceModel()->SetRadius(R);
      fFreezoutGenerator->GenerateFreezoutCooordinates(fFemtoPair);
      Double_t weight = fCalc->GenerateWeight(fFemtoPair);
      fFemtoPair->SetWeight(weight);
      map->FillNum(fFemtoPair);
    }
  }

  void Femto1DCFAnaMap::ProcessFemtoPair_Perfect() {
    fFemtoPair->Compute();
    FemtoCorrFuncKtMap1D* map = ((FemtoCorrFuncKtMap1D*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID));
    fFemtoPair->SetWeight(1.0);
    for (Double_t i = 0; i < fRbins; i++) {
      Double_t R = fRMinEff + i * fRStep;
      map->SetR(R);
      map->FillDenPerfect(fFemtoPair);
    }
  }

  void Femto1DCFAnaMap::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    FemtoCorrFuncKtMap1D* map = ((FemtoCorrFuncKtMap1D*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID));
    for (Double_t i = 0; i < fRbins; i++) {
      Double_t R = fRMinEff + i * fRStep;
      map->SetR(R);
      fFreezoutGenerator->GetSourceModel()->SetRadius(R);
      fFreezoutGenerator->GenerateFreezoutCooordinates(fFemtoPair);
      Double_t weight = fCalc->GenerateWeight(fFemtoPair);
      fFemtoPair->SetWeight(weight);
      map->FillDenRotated(fFemtoPair);
    }
  }

  void Femto1DCFAnaMap::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    FemtoCorrFuncKtMap1D* map = ((FemtoCorrFuncKtMap1D*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID));
    for (Double_t i = 0; i < fRbins; i++) {
      Double_t R = fRMinEff + i * fRStep;
      map->SetR(R);
      fFreezoutGenerator->GetSourceModel()->SetRadius(R);
      fFreezoutGenerator->GenerateFreezoutCooordinates(fFemtoPair);
      Double_t weight = fCalc->GenerateWeight(fFemtoPair);
      fFemtoPair->SetWeight(weight);
      map->FillDenHemisphere(fFemtoPair);
    }
  }

  void Femto1DCFAnaMap::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute_Mixed();
    FemtoCorrFuncKtMap1D* map = ((FemtoCorrFuncKtMap1D*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID));
    fFemtoPair->SetWeight(1.0);
    for (Double_t i = 0; i < fRbins; i++) {
      Double_t R = fRMinEff + i * fRStep;
      map->SetR(R);
      map->FillDenMixed(fFemtoPair);
    }
  }

  void Femto1DCFAnaMap::ProcessFemtoPair_Charged() {
    fFemtoPair->Compute_Charged();
    FemtoCorrFuncKtMap1D* map = ((FemtoCorrFuncKtMap1D*) fCFs->At(fEventCollectionCF, fCurrentPairCollectionID));
    for (Double_t i = 0; i < fRbins; i++) {
      Double_t R = fRMinEff + i * fRStep;
      map->SetR(R);
      fFreezoutGenerator->GetSourceModel()->SetRadius(R);
      fFreezoutGenerator->GenerateFreezoutCooordinates(fFemtoPair);
      Double_t weight = fCalc->GenerateWeight(fFemtoPair);
      fFemtoPair->SetWeight(weight);
      map->FillDenCharged(fFemtoPair);
    }
  }

  Task::EInitFlag Femto1DCFAnaMap::Init() {
    if (fRbins == 0) {
      fRbins = 22;
      fRmin  = 0.5;
      fRmax  = 10.5;
    }
    fRStep               = (fRmax - fRmin) / ((Double_t) fRbins);
    fRMinEff             = fRmin + 0.5 * fRStep;
    Task::EInitFlag stat = FemtoBasicAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return Task::EInitFlag::kFATAL;

    if (fFreezoutGenerator == NULL) {
      Cout::PrintInfo("No freezoug generator", EInfo::kLessError);
      return Task::EInitFlag::kFATAL;
    }
    return stat;
  }

  Package* Femto1DCFAnaMap::Report() const {
    Package* pack = FemtoBasicAna::Report();
    return pack;
  }

  void Femto1DCFAnaMap::SetRadiusRange(Int_t r_bins, Double_t r_min, Double_t r_max) {
    fRbins = r_bins;
    fRmin  = r_min;
    fRmax  = r_max;
  }

  void Femto1DCFAnaMap::CheckCutContainerCollections() { FemtoBasicAna::CheckCutContainerCollections(); }

  Femto1DCFAnaMap::~Femto1DCFAnaMap() {
    // TODO Auto-generated destructor stub
  }

  Bool_t Femto1DCFAnaMap::InitArray() {
    fCFs = new ObjectMatrix_2();
    FemtoCorrFuncKtMap1D* map_cf =
      new FemtoCorrFuncKtMap1D(dynamic_cast<FemtoCorrFuncKt*>(fCFTemp), fRbins, fRmin, fRmax, fFemtoPair->GetFrame());
    fCFs->Init(fEventCollectionsNo, fTwoTrackCollectionsNo, map_cf);
    delete map_cf;
    for (int i = 0; i < fEventCollectionsNo; i++) {
      for (int j = 0; j < fTwoTrackCollectionsNo; j++) {
        FemtoCorrFuncKtMap1D* corrfunc = (FemtoCorrFuncKtMap1D*) fCFs->At(i, j);
        corrfunc->SetEventCollID(i);
        corrfunc->SetPairCollID(j);
        corrfunc->Check();
      }
    }
    return kTRUE;
  }
}  // namespace Hal
