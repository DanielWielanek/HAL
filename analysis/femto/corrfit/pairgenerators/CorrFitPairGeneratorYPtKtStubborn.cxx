/*
 * CorrFitPairGeneratorYPtStubbornSubborn.cxx
 *
 *  Created on: 29 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitPairGeneratorYPtKtStubborn.h"

#include "CorrFitPairFile.h"
#include "FemtoCorrFuncKt.h"
#include "FemtoMiniPair.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TTree.h>

namespace Hal {
  CorrFitPairGeneratorYPtKtStubborn::CorrFitPairGeneratorYPtKtStubborn() {}

  CorrFitPairGeneratorYPtKtStubborn::~CorrFitPairGeneratorYPtKtStubborn() {}

  void CorrFitPairGeneratorYPtKtStubborn::GenerateEvent() {
    switch (fGroupingFlag) {
      case EGrouping::kOneDim: {
        int size1d   = fLimitsN.GetSize() - 1;
        int bin      = -1;
        int counted  = 0;
        int startBin = fLimitsN[1];
        do {
          bin = GeneratePairOneDim();
          if (bin < 1) continue;
          if (fLimitsN[bin] == startBin) {
            auto array = fPairFile->GetSignal(bin);
            fLimitsN.IncrementAfter(bin);
            auto pair = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
            *pair     = fPair;
            counted++;
          }
        } while (counted != size1d);
      } break;
      case EGrouping::kThreeDim: {
        int size1d    = fLimits3D.GetSize() - 1;
        int size2d    = fLimits3D[1].GetSize() - 1;
        int size3d    = fLimits3D[1][1].GetSize() - 1;
        int totalSize = size1d * size2d * size3d - 1;
        int counted   = 0;
        int startBin  = fLimits3D[1][1][1];
        int nPairs    = 0;
        do {
          auto bin = GeneratePairThreeDim();
          nPairs++;
          if (bin.x < 1) continue;
          if (bin.y < 1) continue;
          if (bin.z < 1) continue;
          if (fLimits3D[bin.x][bin.y][bin.z] == startBin) {
            auto array = fPairFile->GetSignal(bin.z - 1);
            auto pair  = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
            fLimits3D.IncrementAfter(bin.x, bin.y, bin.z);
            *pair = fPair;
            counted++;
          }
        } while (counted != totalSize);
        for (int i = 0; i < totalSize; i++) {
          int bins = fPairFile->GetConfig()->GetNbins();
          for (int ii = 0; ii < bins; ii++) {
            auto pair = (FemtoMicroPair*) fPairFile->GetSignal(ii)->ConstructedAt(0);
            *fHbtPair = *pair;
            fHbtPair->Compute();
          }
        }
      } break;
    }
  }

}  // namespace Hal
