/*
 * FemtoDumpPairAnaMC.cxx
 *
 *  Created on: 15 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoDumpPairAnaMC.h"

#include "Cout.h"
#include "DividedHisto.h"
#include "FemtoConst.h"
#include "FemtoCorrFunc.h"
#include "FemtoMiniPair.h"
#include "StdHist.h"

#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TRandom.h>
#include <TTree.h>

namespace Hal {
  Hal::FemtoDumpPairAnaMC::FemtoDumpPairAnaMC() : fM1(0), fM2(0), fFrame(Femto::EKinematics::kLCMS) { fPid1 = fPid2 = 211; }


  Bool_t FemtoDumpPairAnaMC::Init() {
    if (!fCF) return kFALSE;
    DividedHisto1D* dummy = fCF->GetCF(0);
    Int_t bins;
    Double_t min, max;
    if (dummy->GetNum()->InheritsFrom("TH3")) {  // 3d histo group by q-long/ k-long
      fGrouping.SetGroupByKLong();
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "z");

    } else {  // 1d histo group by k* qinv
      fGrouping.GroupByKStar();
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "x");
    }

    fFrame = Femto::EKinematics::kLCMS;
    if (dummy->GetLabelsNo() > 0) {
      TString label = dummy->GetLabel(0);
      fFrame        = Femto::LabelToKinematics(label);
    }
    TDatabasePDG* pid  = TDatabasePDG::Instance();
    TParticlePDG* pid1 = pid->GetParticle(fPid1);
    TParticlePDG* pid2 = pid->GetParticle(fPid2);
    if (!pid1) return kFALSE;
    if (!pid2) return kFALSE;
    fM1 = pid1->Mass();
    fM2 = pid2->Mass();
    fGrouping.SetFrame(fFrame);
    fGrouping.SetAxis(bins, min, max);
    fLimitsN.MakeBigger(bins);
    fOutFile = new TFile(fFileName, "recreate");
    fOutTree = new TTree("HalTree", "Tree");
    fOutFile->mkdir("HalInfo");
    fOutFile->cd("HalInfo");
    fGrouping.Write();
    fOutFile->cd();

    auto FillCenters = [&](Array_1<Double_t>& array, Int_t binsx, Double_t minx, Double_t maxx) {
      Double_t step = (maxx - minx) / double(binsx);
      array.MakeBigger(binsx);
      for (int i = 0; i < binsx; i++) {
        Double_t val = step * i + step * 0.5;
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
    } else {
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "x");
      FillCenters(fCentersX, bins, min, max);
    }

    auto vec = fGrouping.GetBranches(0, 0, kTRUE);  // 0,0 -> get all branches
    int idx  = 0;
    for (auto branchName : vec) {
      fSignalPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
      fOutTree->Branch(branchName, &*fSignalPairs[idx]);
      idx++;
    }
    fInited = kTRUE;
    return kTRUE;
  }

  void FemtoDumpPairAnaMC::Run(Int_t entries) {
    if (!fInited || !fOutFile) return;
    int percent = entries / 100;
    if (percent == 0) percent = 1;
    for (int i = 0; i < entries; i++) {
      if (i % percent == 0) { Cout::ProgressBar(i, entries); }
      for (auto data : fSignalPairs)
        data->Clear();
      GenerateEvent();
      fOutTree->Fill();
    }
    fOutTree->Write();
    fOutFile->Close();
    delete fOutFile;
    fOutFile = nullptr;
  }

  void FemtoDumpPairAnaMC::SetCorrFctn(const FemtoCorrFunc& cf) { fCF = (FemtoCorrFunc*) cf.Clone(); }

  void FemtoDumpPairAnaMC::GenerateEvent() {
    if (fLimitsN[0] > fBinLimit) return;
    fLimitsN[0] += 100;
    Double_t min   = fGrouping.GetMin();
    Double_t max   = fGrouping.GetMax();
    Double_t scale = 1.0;
    if (fFrame == Femto::EKinematics::kLCMS) { scale = 0.5; }
    min = min * scale;
    max = max * scale;
    TLorentzVector p1, p2;
    Double_t x, y, z;
    FemtoMicroPair* pair = nullptr;
    auto build           = [&]() {
      p1.SetXYZM(x, y, z, fM1);
      p2.SetXYZM(-x, -y, -z, fM2);
      p1.Boost(0.2, 0, 0.0);
      p2.Boost(0.2, 0, 0.0);
      pair->SetMomenta1(p1.X(), p1.Y(), p1.Z(), p1.E());
      pair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.E());
      pair->SetMomenta2(p2.X(), p2.Y(), p2.Z(), p2.E());
      pair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.E());
      pair->SetPdg1(fPid1);
      pair->SetPdg2(fPid2);
    };


    if (fGrouping.GroupByKStar()) {
      const Int_t defSize = 100;
      for (int bin = 0; bin < fGrouping.GetNbins(); bin++) {
        const Double_t kstar = fCentersX[bin] * scale;
        fSignalPairs[bin]->ExpandCreateFast(defSize);
        for (int i = 0; i < defSize; i++) {
          pair = (FemtoMicroPair*) fSignalPairs[bin]->UncheckedAt(i);
          gRandom->Sphere(x, y, z, kstar);
          build();
        }
      }
    } else {
      const Int_t defSize = 1;
      const int binY      = fCentersY.GetSize();
      const int binX      = fCentersX.GetSize();
      for (int bin = 0; bin < fGrouping.GetNbins(); bin++) {
        fSignalPairs[bin]->ExpandCreateFast(defSize * binY * binX);
        int count = 0;
        for (int biny = 0; biny < binY; biny++) {
          for (int binx = 0; binx < binX; binx++) {
            pair = (FemtoMicroPair*) fSignalPairs[bin]->UncheckedAt(count++);
            z    = fCentersZ[bin] * scale;
            y    = fCentersY[biny] * scale;
            x    = fCentersX[binx] * scale;
            if (gRandom->Uniform() < 0.5) x = -x;
            if (gRandom->Uniform() < 0.5) y = -y;
            if (gRandom->Uniform() < 0.5) z = -z;
            build();
          }
        }
      }
    } /* finished 3d generating*/
  }


  FemtoDumpPairAnaMC::~FemtoDumpPairAnaMC() {
    if (fOutFile) { fOutFile->Close(); }
    if (fCF) delete fCF;
  }


}  // namespace Hal
