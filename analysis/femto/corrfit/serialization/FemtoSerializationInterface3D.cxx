/*
 * FemtoSerializationInterface3D.cxx
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSerializationInterface3D.h"

namespace Hal {

  Bool_t FemtoSerializationInterface3D::GetBinsNo() {
    fBinStart = 1;
    fBinStop  = fCF->GetNum()->GetNbinsZ();
    return kTRUE;
  }

  void FemtoSerializationInterface3D::ConvertCF(ECopyDir dir) {
    TH3D* n = (TH3D*) fCF->GetNum();
    TH3D* d = (TH3D*) fCF->GetDen();
    ExpandArrayIfSerialize(n->GetNbinsX() * n->GetNbinsY() * (fBinHi - fBinLow - 1) * 2, dir);
    int count = 0;
    std::vector<TH3*> cfs;
    cfs.push_back(n);
    cfs.push_back(d);
    for (int i = 1; i <= n->GetNbinsX(); i++) {
      for (int j = 1; j <= n->GetNbinsY(); j++) {
        for (int k = fBinLow; k <= fBinHi; k++) {
          CopyData3D(cfs, i, j, k, dir, false);
        }
      }
    }
  }

  void FemtoSerializationInterface3D::ConvertSlice(ECopyDir dir) {
    ExpandArrayIfSerialize(fSlice->fOutBins * fSlice->fSideBins * 2, dir);
    for (unsigned int iO = 0; iO < fSlice->fOutBins; iO++) {
      for (unsigned int iS = 0; iS < fSlice->fSideBins; iS++) {
        CopyDataSingle(fSlice->fNum[iO][iS][fBinLow], dir);
        CopyDataSingle(fSlice->fDen[iO][iS][fBinLow], dir);
      }
    }
  }

  void FemtoSerializationInterface3D::ConvertCFSimple(ECopyDir dir) {
    TH3D* n = (TH3D*) fCF->GetNum();
    TH3D* d = (TH3D*) fCF->GetDen();
    ExpandArrayIfSerialize(n->GetNbinsX() * n->GetNbinsY() * (fBinHi - fBinLow - 1), dir);
    std::vector<TH3*> cfsN, cfsD;
    cfsN.push_back(n);
    cfsD.push_back(d);
    for (int i = 1; i <= n->GetNbinsX(); i++) {
      for (int j = 1; j <= n->GetNbinsY(); j++) {
        for (int k = 1; k <= n->GetNbinsZ(); k++) {
          CopyData3D2(cfsN, cfsD, i, j, k, dir);
        }
      }
    }
  }

  void FemtoSerializationInterface3D::ConvertSliceSimple(ECopyDir dir) {
    ExpandArrayIfSerialize(fSlice->fOutBins * fSlice->fSideBins, dir);
    for (unsigned int iO = 0; iO < fSlice->fOutBins; iO++) {
      for (unsigned int iS = 0; iS < fSlice->fSideBins; iS++) {
        CopyDataPair(fSlice->fNum[iO][iS][fBinLow], fSlice->fDen[iO][iS][fBinLow], dir);
      }
    }
  }

} /* namespace Hal */
