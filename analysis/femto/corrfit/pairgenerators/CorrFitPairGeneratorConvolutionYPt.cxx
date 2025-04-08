/*
 * CorrFitPairGeneratorConvolutionYPt.cxx
 *
 *  Created on: 20 sty 2025
 *      Author: daniel
 */

#include "CorrFitPairGeneratorConvolutionYPt.h"

#include "CorrFitPairFile.h"
#include "Cout.h"
#include "FemtoCorrFuncKt.h"
#include "FemtoMiniPair.h"
#include "StdHist.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH3D.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TTree.h>


namespace Hal {

  void CorrFitPairGeneratorConvolutionYPt::GenerateEvent() {
    fBinCFZ                  = fOneDimBin % fModuloZ;
    Int_t binY               = ((fOneDimBin - fBinCFZ) / fModuloZ) % fModuloY;
    Int_t binX               = ((fOneDimBin - fBinCFZ - binY * fModuloY) / fModuloZ) % fModuloY;
    Double_t q_out           = fCentersX[binX];
    Double_t q_side          = fCentersY[binY];
    Double_t q_long          = fCentersZ[fBinCFZ];
    Double_t q_outLimits[2]  = {q_out - fHalfWidthX, q_out + fHalfWidthX};
    Double_t q_sideLimits[2] = {q_side - fHalfWidthY, q_side + fHalfWidthY};
    Double_t q_longLimits[2] = {q_long - fHalfWidthZ, q_long + fHalfWidthZ};

    // fOut  = 0.11;
    // fSide = 0.07;
    // fLong = 0.15;

    fOneDimBin++;
    if (fGenerateSwap && fBinSmearFactor > 0) {  // smear and swap
      for (int i = 0; i < fBinSmearFactor; i++) {
        for (int j = 0; j < 8; j++) {
          q_out  = gRandom->Uniform(q_outLimits[0], q_outLimits[1]);
          q_side = gRandom->Uniform(q_sideLimits[0], q_sideLimits[1]);
          q_long = gRandom->Uniform(q_longLimits[0], q_longLimits[1]);

          fOut[i * 8 + j]  = (j & 1) ? -q_out : q_out;
          fSide[i * 8 + j] = (j & 2) ? -q_side : q_side;
          fLong[i * 8 + j] = (j & 4) ? -q_long : q_long;
        }
      }
    } else if (fGenerateSwap) {  // only swap
      for (int i = 0; i < 8; i++) {
        fOut[i]  = (i & 1) ? -q_out : q_out;
        fSide[i] = (i & 2) ? -q_side : q_side;
        fLong[i] = (i & 4) ? -q_long : q_long;
      }
    } else if (fBinSmearFactor > 0) {  // only smear
      for (int i = 0; i < fBinSmearFactor; i++) {
        fOut[i]  = gRandom->Uniform(q_outLimits[0], q_outLimits[1]);
        fSide[i] = gRandom->Uniform(q_sideLimits[0], q_sideLimits[1]);
        fLong[i] = gRandom->Uniform(q_longLimits[0], q_longLimits[1]);
      }
    } else {  // simples case
      fOut[0]  = q_out;
      fSide[0] = q_side;
      fLong[0] = q_long;
    }
    for (int i = 0; i < fOut.size(); i++) {
      auto array = fPairFile->GetSignal(fBinCFZ);
      fX         = fOut[i];
      fY         = fSide[i];
      fZ         = fLong[i];
      GeneratePairEvent();
    }
  }

  CorrFitPairGeneratorConvolutionYPt::CorrFitPairGeneratorConvolutionYPt() {}

  void CorrFitPairGeneratorConvolutionYPt::SetHist(const TH2D& hist1, const TH2D& hist2) {
    fConvolution.SetData(hist1, fPid1, hist2, fPid2);
  }

  void CorrFitPairGeneratorConvolutionYPt::GeneratePairEvent() {
    fConvolution.Recalculate(fX, fY, fZ);
    auto array = fPairFile->GetSignal(fBinCFZ);
    TLorentzVector p1, p2;
    for (int i = 0; i < fPairsPerBin; i++) {
      auto pair = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
      fConvolution.FillPair(*pair);
    }
  }

  Bool_t CorrFitPairGeneratorConvolutionYPt::Init() {
    bool val = CorrFitPairGenerator::Init();
    if (fGroupingFlag == EGrouping::kOneDim) { return false; }
    if (!val) return val;
    auto kt = dynamic_cast<Hal::FemtoCorrFuncKt*>(fCF);
    if (kt) {
      auto ktArr = kt->GetRange();
      fConvolution.SetKt(ktArr[0], ktArr[ktArr.GetSize() - 1]);
      fConvolution.SetFrame(fFrame);
    }
    if (!fConvolution.Init()) return kFALSE;

    fOut.resize(1);
    fSide.resize(1);
    fLong.resize(1);
    if (fGenerateSwap) {
      fOut.resize(8);
      fSide.resize(8);
      fLong.resize(8);
    }
    if (fBinSmearFactor >= 1) {
      fOut.resize(fOut.size() * fBinSmearFactor);
      fSide.resize(fSide.size() * fBinSmearFactor);
      fLong.resize(fLong.size() * fBinSmearFactor);
    }
    fModuloX = fXaxis.GetNBins();
    fModuloY = fYaxis.GetNBins();
    fModuloZ = fZaxis.GetNBins();
    return kTRUE;
  }

  void CorrFitPairGeneratorConvolutionYPt::SwapSignRandom(Double_t& x, Double_t& y, Double_t& z) const {
    Int_t se = TMath::Floor(gRandom->Uniform(0, 8));
    switch (se) {
      case 0: {
      } break;
      case 1: {
        x = -x;
      } break;
      case 2: {
        y = -y;
      } break;
      case 3: {
        x = -x;
        y = -y;
      } break;
      case 4: {
        z = -z;
      } break;
      case 5: {
        x = -x;
        z = -z;
      } break;
      case 6: {
        y = -y;
        z = -z;
      } break;
      case 7: {
        x = -x;
        y = -y;
        z = -z;
      } break;
    }
  }

  void CorrFitPairGeneratorConvolutionYPt::RunFraction(Int_t entries, Int_t nSamples, Int_t nNo) {
    if (!fInited || !fPairFile) return;

    Int_t nTotalBins  = fXaxis.GetNBins() * fYaxis.GetNBins() * fZaxis.GetNBins();
    Int_t step        = nTotalBins / nSamples;
    Int_t nEffEntries = step;
    int percent       = nEffEntries / 100;
    if (percent == 0) percent = 1;
    for (int i = 0; i < nEffEntries; i++) {
      if (i % percent == 0) { Cout::ProgressBar(i, nEffEntries); }
      fPairFile->ClearData();
      int iStep  = i + nEffEntries * nNo;
      fOneDimBin = iStep;
      GenerateEvent();
      fPairFile->Fill();
    }
    CleanUpFiles();
  }

} /* namespace Hal */
