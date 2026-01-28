/*
 * CorrFitHDFunc3D.h
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_HD_FUNC_CORRFITHDFUNC3D_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_HD_FUNC_CORRFITHDFUNC3D_H_

#include "Array.h"
#include "CorrFitHDFunc.h"
#include "CorrFitMask.h"

#include <RtypesCore.h>


namespace Hal {

  class CorrFitHDFunc3D : public CorrFitHDFunc {
    Array_3<Double_t> fDenominatorHD;
    Array_3<Double_t> fDenominatorSum;
    Array_3<Double_t> fMapHD;
    Array_1<Int_t> fBinsX;
    Array_1<Int_t> fBinsY;
    Array_1<Int_t> fBinsZ;
    Array_1<Int_t> fBinsHDX;
    Array_1<Int_t> fBinsHDY;
    Array_1<Int_t> fBinsHDZ;
    void CalculateBinsArrays(const CorrFitMask3D& mask, Bool_t hd);

  public:
    CorrFitHDFunc3D();
    void SetMask(const CorrFitMask& mask, TH1* denominator, Bool_t hd);
    /**
     *
     * @param binX
     * @param binY
     * @param binZ
     * @param extrapolated
     * @return value of CF for given bins
     */
    Double_t GetBinCFVal(Int_t binX, Int_t binY, Int_t binZ, Bool_t extrapolated) const;
    /**
     * converts active bin number into bin number
     * @param i
     * @return
     */
    inline Int_t GetXBin(Int_t i) const { return fBinsX.Get(i); };
    /**
     * converts active bin number into bin number
     * @param i
     * @return
     */
    inline Int_t GetYBin(Int_t i) const { return fBinsY.Get(i); };
    /**
     * converts active bin number into bin number
     * @param i
     * @return
     */
    inline Int_t GetZBin(Int_t i) const { return fBinsZ.Get(i); };
    inline Array_1<Int_t>& GetBinsHDX() { return fBinsHDX; };
    inline Array_1<Int_t>& GetBinsHDY() { return fBinsHDY; };
    inline Array_1<Int_t>& GetBinsHDZ() { return fBinsHDZ; };
    inline Array_3<Double_t>& CFMapHD() { return fMapHD; }
    inline Array_3<Double_t>& GetDenominatorHD() { return fDenominatorHD; };
    inline Array_3<Double_t>& GetDenominatorSum() { return fDenominatorSum; };
    inline Int_t GetNBins() const { return fBinsX.GetSize(); };
    inline Double_t EvalHDX(Double_t hdBin) const { return fMins[0] + fSteps[0] * hdBin; }
    inline Double_t EvalHDY(Double_t hdBin) const { return fMins[1] + fSteps[1] * hdBin; }
    inline Double_t EvalHDZ(Double_t hdBin) const { return fMins[2] + fSteps[2] * hdBin; }
    virtual ~CorrFitHDFunc3D() {};
    ClassDef(CorrFitHDFunc3D, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_HD_FUNC_CORRFITHDFUNC3D_H_ */
