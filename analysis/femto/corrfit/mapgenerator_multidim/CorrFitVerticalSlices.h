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
  class FemtoSHCF;
  class Femto1DCF;
  class Femto3DCF;
  class DividedHisto1D;
  class CorrFitVerticalSlices : public TObject {
  protected:
  public:
    CorrFitVerticalSlices() {};
    static CorrFitVerticalSlices* MakeSlice(const Hal::DividedHisto1D& h, Int_t nParams);
    virtual void FillNum(Int_t bin, FemtoPair* pair)                                      = 0;
    virtual void FillDen(Int_t bin, FemtoPair* pair)                                      = 0;
    virtual void ExportToFlat(Array_1<Float_t>* array, Int_t paramId, Bool_t first) const = 0;
    virtual ~CorrFitVerticalSlices() {};
    ClassDef(CorrFitVerticalSlices, 1)
  };

  class CorrFitVerticalSlices1D : public CorrFitVerticalSlices {
    std::vector<Double_t> fNum;
    std::vector<Double_t> fDen;

  public:
    CorrFitVerticalSlices1D() {};  // only for root
    CorrFitVerticalSlices1D(const Hal::Femto1DCF& h, Int_t nSamples);
    virtual void ExportToFlat(Array_1<Float_t>* array, Int_t paramId, Bool_t first) const;
    void FillNum(Int_t bin, FemtoPair* pair);
    void FillDen(Int_t bin, FemtoPair* pair);
    virtual ~CorrFitVerticalSlices1D() {};
    ClassDef(CorrFitVerticalSlices1D, 1)
  };

  class CorrFitVerticalSlices3D : public CorrFitVerticalSlices {
    std::vector<std::vector<std::vector<Double_t>>> fNum;
    std::vector<std::vector<std::vector<Double_t>>> fDen;
    Int_t fOutBins  = {0};
    Int_t fSideBins = {0};

  public:
    CorrFitVerticalSlices3D() {};  // only for root
    CorrFitVerticalSlices3D(const Hal::Femto3DCF& h, Int_t nSamples);
    void FillNum(Int_t bin, FemtoPair* pair);
    void FillDen(Int_t bin, FemtoPair* pair);
    virtual void ExportToFlat(Array_1<Float_t>* array, Int_t paramId, Bool_t first) const;
    virtual ~CorrFitVerticalSlices3D() {};
    ClassDef(CorrFitVerticalSlices3D, 1)
  };

  class CorrFitVerticalSlicesSH : public CorrFitVerticalSlices {
    FemtoYlmIndexes fLmVals;
    FemtoYlmMath fLmMath;
    const Int_t fMaxJM;
    std::vector<Double_t> fNum;
    std::vector<Double_t> fDen;
    std::vector<std::vector<Double_t>> fShNumReal;
    std::vector<std::vector<Double_t>> fShDenReal;
    std::vector<std::vector<Double_t>> fShNumImag;
    std::vector<std::vector<Double_t>> fShDenImag;
    std::vector<std::vector<std::vector<Double_t>>> fCovMatrix;

  public:
    CorrFitVerticalSlicesSH() : fMaxJM(3) {};  // only for root
    CorrFitVerticalSlicesSH(const Hal::FemtoSHCF& h, Int_t nSamples);
    void FillNum(Int_t bin, FemtoPair* pair);
    void FillDen(Int_t bin, FemtoPair* pair);
    void FillNumBuffer(std::complex<double>* shCoord, Double_t weight, Int_t paramBin);
    void FillDenBuffer(std::complex<double>* shCoord, Double_t weight, Int_t paramBin);
    virtual void ExportToFlat(Array_1<Float_t>* array, Int_t paramId, Bool_t first) const;
    const std::vector<Double_t>& GetNumReal(Int_t bin) const { return fShNumReal[bin]; };
    const std::vector<Double_t>& GetNumImag(Int_t bin) const { return fShNumImag[bin]; };
    const std::vector<Double_t>& GetDenReal(Int_t bin) const { return fShDenReal[bin]; };
    const std::vector<Double_t>& GetDenImag(Int_t bin) const { return fShDenImag[bin]; };
    virtual ~CorrFitVerticalSlicesSH() {};
    ClassDef(CorrFitVerticalSlicesSH, 1)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITVERTICALSLICES_H_ */
