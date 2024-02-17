/*
 * FemtoSerializationInterfaceSH.cxx
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSerializationInterfaceSH.h"

#include "Cout.h"
#include "FemtoSHCF.h"
#include <iostream>

namespace Hal {
  void FemtoSerializationInterfaceSH::PostInit() {
    int maxL = 0;
    if (fCF) {
      maxL = fCF->GetLMax();
    } else {
      maxL = fSlice->GetMaxL();
    }
    fMaxJM = (maxL + 1) * (maxL + 1);
  }

  Bool_t FemtoSerializationInterfaceSH::GetBinsNo() {
    fBinStop  = fCF->GetNum()->GetNbinsX();
    fBinStart = 1;
    return kTRUE;
  }

  void FemtoSerializationInterfaceSH::ConvertCF(ECopyDir dir) {
    unsigned int dim = 2 + fMaxJM * 8 + fMaxJM * fMaxJM * 4;
    int qbins        = fBinHi - fBinLow + 1;
    ExpandArrayIfSerialize(dim * qbins, dir);
    std::vector<TH1*> cfs, cfs2;
    cfs.push_back(fCF->GetNum());
    cfs.push_back(fCF->GetDen());
    for (int i = 0; i < fMaxJM; i++) {
      cfs.push_back(fCF->fNumReal[i]);
      cfs.push_back(fCF->fDenReal[i]);
      cfs.push_back(fCF->fNumImag[i]);
      cfs.push_back(fCF->fDenImag[i]);
      cfs2.push_back(fCF->fNumReal[i]);
      cfs2.push_back(fCF->fDenReal[i]);
      cfs2.push_back(fCF->fNumImag[i]);
      cfs2.push_back(fCF->fDenImag[i]);
    }
    for (int iq = fBinLow; iq <= fBinHi; iq++) {
      CopyData1D(cfs, iq, dir);
      CopyData1D(cfs2, iq, dir, true);
      for (int i = 0; i < fMaxJM * 2; i++) {
        for (int j = 0; j < fMaxJM * 2; j++) {
          CopyDataSingle(fCF->fCovNum[iq - 1][i][j], dir);
        }
      }
    }
  }

  void FemtoSerializationInterfaceSH::ConvertSlice(ECopyDir dir) {
    int count = 0;
    ExpandArrayIfSerialize(2 + fMaxJM * 8 + fMaxJM * fMaxJM * 4, dir);
    CopyDataSingle(fSlice->fNum[fBinLow], dir);
    CopyDataSingle(fSlice->fDen[fBinLow], dir);

    for (int i = 0; i < fMaxJM; i++) {
      CopyDataSingle(fSlice->fShNumReal[fBinLow][i], dir);
      CopyDataSingle(fSlice->fShDenReal[fBinLow][i], dir);
      CopyDataSingle(fSlice->fShNumImag[fBinLow][i], dir);
      CopyDataSingle(fSlice->fShDenImag[fBinLow][i], dir);
    }
    auto sqrSeralize = [&](Double_t& val, ECopyDir dirx) {
      Double_t copy;
      if (dirx == ECopyDir::kSerialize) {
        copy = TMath::Sqrt(val);
        CopyDataSingle(copy, dirx);
      } else {
        CopyDataSingle(copy, dirx);
        val = copy * copy;
      }
    };
    float temp_err = 0;
    for (int i = 0; i < fMaxJM; i++) {  // set errors to zero to be compatible with CF's
      sqrSeralize(fSlice->fShNumRealE[fBinLow][i], dir);
      sqrSeralize(fSlice->fShDenRealE[fBinLow][i], dir);
      sqrSeralize(fSlice->fShNumImagE[fBinLow][i], dir);
      sqrSeralize(fSlice->fShDenImagE[fBinLow][i], dir);
    }
    for (int i = 0; i < fMaxJM * 2; i++) {
      for (int j = 0; j < fMaxJM * 2; j++) {
        CopyDataSingle(fSlice->fCovMatrix[fBinLow][i][j], dir);
      }
    }
  }

  void FemtoSerializationInterfaceSH::ConvertCFSimple(ECopyDir dir) {
    unsigned int dim = 2 + fMaxJM * 2;
    int qbins        = fBinHi - fBinLow + 1;
    ExpandArrayIfSerialize(dim * qbins, dir);
    std::vector<TH1*> cfs;
    cfs.push_back(fCF->GetNum());
    cfs.push_back(fCF->GetDen());
    for (int i = 0; i < fMaxJM; i++) {
      cfs.push_back(fCF->fCFReal[i]);
      cfs.push_back(fCF->fCFImag[i]);
    }
    for (int iq = fBinStart; iq <= fBinStop; iq++) {
      CopyData1D(cfs, iq, dir);
    }
  }

  void FemtoSerializationInterfaceSH::ConvertSliceSimple(ECopyDir dir) {
    ConvertSlice(dir);
    return;
    int count = 0;
    ExpandArrayIfSerialize(2 + fMaxJM * 4 + fMaxJM * fMaxJM * 4, dir);

    CopyDataSingle(fSlice->fNum[fBinLow], dir);
    CopyDataSingle(fSlice->fDen[fBinLow], dir);

    for (int i = 0; i < fMaxJM; i++) {
      CopyDataSingle(fSlice->fShNumReal[fBinLow][i], dir);
      CopyDataSingle(fSlice->fShDenReal[fBinLow][i], dir);
      CopyDataSingle(fSlice->fShNumImag[fBinLow][i], dir);
      CopyDataSingle(fSlice->fShDenImag[fBinLow][i], dir);
    }
    for (int i = 0; i < fMaxJM * 2; i++) {
      for (int j = 0; j < fMaxJM * 2; j++) {
        CopyDataSingle(fSlice->fCovMatrix[fBinLow][i][j], dir);
      }
    }
  }

} /* namespace Hal */
