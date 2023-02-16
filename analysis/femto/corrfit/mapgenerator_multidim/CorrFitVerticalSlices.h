/*
 * CorrFitSlices.h
 *
 *  Created on: 2 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITVERTICALSLICES_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITVERTICALSLICES_H_

#include <TObject.h>
#include <vector>

#include "FemtoYlmIndexes.h"
#include "FemtoYlmMath.h"

namespace Hal {
  class FemtoPair;
  class CorrFitVerticalSlices : public TObject {
  protected:
    std::vector<Double_t> fNum;
    std::vector<Double_t> fDen;
    Int_t fBinNo;

  public:
    CorrFitVerticalSlices(Int_t paramSpace = 0, Int_t binNo = 0);
    virtual void FillNum(Int_t bin, FemtoPair* pair) = 0;
    virtual void FillDen(Int_t bin, FemtoPair* pair) = 0;
    Double_t GetValNum(Int_t bin) const { return fNum[bin]; };
    Double_t GetValDen(Int_t bin) const { return fDen[bin]; };
    virtual ~CorrFitVerticalSlices() {};
    ClassDef(CorrFitVerticalSlices, 1)
  };

  class CorrFitVerticalSlices1D : public CorrFitVerticalSlices {
  public:
    CorrFitVerticalSlices1D(Int_t paramSpace = 0, Int_t binNo = 0) : CorrFitVerticalSlices(paramSpace, binNo) {};
    void FillNum(Int_t bin, FemtoPair* pair);
    void FillDen(Int_t bin, FemtoPair* pair);
    virtual ~CorrFitVerticalSlices1D() {};
    ClassDef(CorrFitVerticalSlices1D, 1)
  };

  class CorrFitVerticalSlices3D : public CorrFitVerticalSlices {
  public:
    CorrFitVerticalSlices3D(Int_t paramSpace = 0, Int_t binNo = 0) : CorrFitVerticalSlices(paramSpace, binNo) {};
    void FillNum(Int_t bin, FemtoPair* pair);
    void FillDen(Int_t bin, FemtoPair* pair);
    virtual ~CorrFitVerticalSlices3D() {};
    ClassDef(CorrFitVerticalSlices3D, 1)
  };

  class CorrFitVerticalSlicesSH : public CorrFitVerticalSlices {
    FemtoYlmIndexes fLmVals;
    FemtoYlmMath fLmMath;
    const Int_t fMaxJM;
    std::vector<std::vector<Double_t>> fShNumReal;
    std::vector<std::vector<Double_t>> fShDenReal;
    std::vector<std::vector<Double_t>> fShNumImag;
    std::vector<std::vector<Double_t>> fShDenImag;

  public:
    CorrFitVerticalSlicesSH(Int_t paramSpace = 0, Int_t binNo = 0, Int_t lmax = 5);
    void FillNum(Int_t bin, FemtoPair* pair);
    void FillDen(Int_t bin, FemtoPair* pair);
    const std::vector<Double_t>& GetNumReal(Int_t bin) const { return fShNumReal[bin]; };
    const std::vector<Double_t>& GetNumImag(Int_t bin) const { return fShNumImag[bin]; };
    const std::vector<Double_t>& GetDenReal(Int_t bin) const { return fShDenReal[bin]; };
    const std::vector<Double_t>& GetDenImag(Int_t bin) const { return fShDenImag[bin]; };
    virtual ~CorrFitVerticalSlicesSH() {};
    ClassDef(CorrFitVerticalSlicesSH, 1)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITVERTICALSLICES_H_ */
