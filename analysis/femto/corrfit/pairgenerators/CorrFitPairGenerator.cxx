/*
 * CorrFitPairGenerator.cxx
 *
 *  Created on: 27 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitPairGenerator.h"

#include "CorrFitPairFile.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "FemtoCorrFunc.h"
#include "StdHist.h"

#include <vector>

#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1.h>
#include <TList.h>
#include <TParticlePDG.h>
#include <TString.h>
#include <TTree.h>

namespace Hal {

  CorrFitPairGenerator::CorrFitPairGenerator() {}

  Bool_t CorrFitPairGenerator::Init() {
    if (!fCF) return kFALSE;
    DividedHisto1D* dummy = fCF->GetCF(0);
    Int_t bins;
    Double_t min, max;

    if (dummy->GetNum()->InheritsFrom("TH3")) {  // 3d histo group by q-long/ k-long
      fGrouping.SetGroupByKLong();
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "z");
      fXaxis.Recalc(*dummy->GetNum()->GetXaxis());
      fYaxis.Recalc(*dummy->GetNum()->GetYaxis());
      fZaxis.Recalc(*dummy->GetNum()->GetZaxis());
      if (fDebug) fDebugHisto = new TH1D("debug", "debug", bins, min, max);
      auto d3       = (TH3*) dummy->GetNum();
      fOutCut[0]    = d3->GetXaxis()->GetBinLowEdge(1);
      fOutCut[1]    = d3->GetXaxis()->GetBinUpEdge(d3->GetNbinsX());
      fSideCut[0]   = d3->GetYaxis()->GetBinLowEdge(1);
      fSideCut[1]   = d3->GetYaxis()->GetBinUpEdge(d3->GetNbinsY());
      fGroupingFlag = EGrouping::kThreeDim;
      Int_t bins1, bins2, bins3;
      Std::GetAxisPar(*dummy->GetNum(), bins2, min, max, "y");
      Std::GetAxisPar(*dummy->GetNum(), bins1, min, max, "x");
      Std::GetAxisPar(*dummy->GetNum(), bins3, min, max, "z");
      fLimits3D.MakeBigger(bins1 + 1, bins2 + 1, bins3 + 1);

    } else {  // 1d histo group by k* qinv
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "x");
      fLimitsN.MakeBigger(bins + 1);
      fXaxis.Recalc(*dummy->GetNum()->GetXaxis());
      fGrouping.GroupByKStar();
      fGroupingFlag = EGrouping::kOneDim;
      if (fDebug) fDebugHisto = new TH1D("debug", "debug", bins, min, max);
    }
    fXaxis.RoundToMinusOne();
    fYaxis.RoundToMinusOne();
    fZaxis.RoundToMinusOne();
    fHi       = max;
    fLow      = min;
    fOverStep = (max - min) / ((double) bins);
    fOverStep = 1.0 / fOverStep;
    if (fLow == 0) fAbs = kTRUE;  // enable abs, because function starts with zero
    if (!fAbs) fGenerateSwap = kFALSE;
    if (dummy->GetLabelsNo() > 0) {
      TString label = dummy->GetLabel(0);
      fFrame        = Femto::LabelToKinematics(label);
    }
    fHbtPair = Hal::Femto::MakePair(fFrame, true);

    TDatabasePDG* pid  = TDatabasePDG::Instance();
    TParticlePDG* pid1 = pid->GetParticle(fPid1);
    TParticlePDG* pid2 = pid->GetParticle(fPid2);
    fHbtPair->SetPdg1(fPid1);
    fHbtPair->SetPdg2(fPid2);
    fHbtPair->Init(-1);
    if (!pid1) return kFALSE;
    if (!pid2) return kFALSE;
    fM1 = pid1->Mass();
    fM2 = pid2->Mass();
    fGrouping.SetFrame(fFrame);
    fGrouping.SetAxis(bins, min, max);
    fNBins = bins;

    fGrouping.EnableSignal();
    fPairFile->SetConfig(fGrouping);
    fPairFile->Init();


    auto FillCenters = [&](Array_1<Double_t>& array, Int_t binsx, Double_t minx, Double_t maxx) {
      Double_t step = (maxx - minx) / double(binsx);
      array.MakeBigger(binsx);
      for (int i = 0; i < binsx; i++) {
        Double_t val = step * i + step * 0.5 + minx;
        array[i]     = val;
      }
    };


    if (dummy->GetNum()->InheritsFrom("TH3")) {
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "x");
      FillCenters(fCentersX, bins, min, max);
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "y");
      FillCenters(fCentersY, bins, min, max);
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "z");
      FillCenters(fCentersZ, bins, min, max);
      fHalfWidthX = dummy->GetNum()->GetXaxis()->GetBinWidth(1) * 0.5;
      fHalfWidthX = dummy->GetNum()->GetYaxis()->GetBinWidth(1) * 0.5;
      fHalfWidthX = dummy->GetNum()->GetZaxis()->GetBinWidth(1) * 0.5;
    } else {
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "x");
      FillCenters(fCentersX, bins, min, max);
      fHalfWidthX = dummy->GetNum()->GetXaxis()->GetBinWidth(1) * 0.5;
    }
    fInited = kTRUE;
    return kTRUE;
  }

  void CorrFitPairGenerator::SetOutput(TString name) { fPairFile = new CorrFitPairFile(name, "recreate"); }

  void CorrFitPairGenerator::SetCorrFctn(const FemtoCorrFunc& cf) { fCF = (FemtoCorrFunc*) cf.Clone(); }

  void CorrFitPairGenerator::Run(Int_t entries) {
    if (!fInited || !fPairFile) return;

    int percent = entries / 100;
    if (percent == 0) percent = 1;
    for (int i = 0; i < entries; i++) {
      if (i % percent == 0) { Cout::ProgressBar(i, entries); }
      if (fDebug) {
        unsigned int count = 1;
        Int_t bins         = fPairFile->GetConfig()->GetNbins();
        for (int ii = 0; ii < bins; ii++) {
          auto vec = fPairFile->GetSignal(ii);
          fDebugHisto->SetBinContent(count, fDebugHisto->GetBinContent(count) + vec->GetEntriesFast());
          ++count;
        }
      }
      fPairFile->ClearData();
      GenerateEvent();
      fPairFile->Fill();
    }
    delete fPairFile;
    fPairFile = nullptr;
    if (fDebug) {
      TFile* f = new TFile(Form("%s_debug.root", ClassName()), "recreate");
      fDebugHisto->Write();
      fDebugHisto = nullptr;
      f->Close();
      delete f;
    }
  }

  Int_t CorrFitPairGenerator::GetBin(Double_t val) const {
    Int_t bin = 0;
    if (fLow == 0) {  // ignore sign automatically?
      bin = TMath::Abs(val) * fOverStep;
    } else {
      bin = (val - fLow) * fOverStep;
    }

    if (bin >= fNBins) return -1;
    return bin;
  }

  CorrFitPairGenerator::~CorrFitPairGenerator() {
    if (fPairFile) { delete fPairFile; }
    if (fCF) delete fCF;
  }

} /* namespace Hal */
