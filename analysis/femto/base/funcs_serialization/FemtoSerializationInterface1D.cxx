/*
 * FemtoSerialization1D.cxx
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSerializationInterface1D.h"
#include "Femto1DCF.h"

namespace Hal {

  Bool_t FemtoSerializationInterface1D::GetBinsNo() {
    if (!fCF) return kFALSE;
    fBinStart = 1;
    fBinStop  = fCF->GetNum()->GetNbinsX();
    return kTRUE;
  }


  Bool_t GetBinsNo() { return kTRUE; };
  void FemtoSerializationInterface1D::ConvertCF(ECopyDir dir) {
    ExpandArrayIfSerialize(fCF->GetNum()->GetNbinsX() * 2, dir);
    std::vector<TH1*> cfs;
    cfs.push_back(fCF->GetNum());
    cfs.push_back(fCF->GetDen());
    for (int i = fBinLow; i <= fBinHi; i++) {
      CopyData1D(cfs, i, dir);
    }
  }
  void FemtoSerializationInterface1D::ConvertSlice(ECopyDir dir) {
    ExpandArrayIfSerialize(2, dir);
    CopyDataSingle(fSlice->fNum[fBinLow], dir);
    CopyDataSingle(fSlice->fDen[fBinLow], dir);
  }
  void FemtoSerializationInterface1D::ConvertCFSimple(ECopyDir dir) {
    ExpandArrayIfSerialize(fCF->GetNum()->GetNbinsX(), dir);
    std::vector<TH1*> nums, dens;
    nums.push_back(fCF->GetNum());
    dens.push_back(fCF->GetDen());
    for (int i = fBinStart; i <= fBinStop; i++) {
      CopyData1D2(nums, dens, i, dir);
    }
  }

  void FemtoSerializationInterface1D::ConvertSliceSimple(ECopyDir dir) {
    ExpandArrayIfSerialize(2, dir);
    CopyDataPair(fSlice->fNum[fBinLow], fSlice->fDen[fBinLow], dir);
  }

} /* namespace Hal */
