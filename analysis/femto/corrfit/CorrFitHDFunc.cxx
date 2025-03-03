/*
 * CorrFitTempMaps.cxx
 *
 *  Created on: 14 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitHDFunc.h"

#include "Cout.h"

#include <iostream>

// #define DEBUG_FUNCHD
namespace Hal {
  CorrFitHDFunc::CorrFitHDFunc(Int_t dim) : fDummyVal(-100.01), fDim(dim) {
    fParams = new Double_t[fDim];
    fMins   = new Double_t[fDim];
    fSteps  = new Double_t[fDim];
  }

  CorrFitHDFunc::~CorrFitHDFunc() {
    delete[] fParams;
    delete[] fMins;
    delete[] fSteps;
  }

  void CorrFitHDFunc::RecalcHDBin(Int_t newSize) {
    newSize     = newSize * 2 + 1;
    Int_t count = 1;
    if (fHDToBin.GetSize() < newSize) fHDToBin.MakeBigger(newSize);
    fHDToBin[0] = 0;
    for (int i = 1; i < fHDToBin.GetSize(); i++) {
      fHDToBin[i] = count;
      if (i % 2 == 0) count++;
    }
  }

}  // namespace Hal
