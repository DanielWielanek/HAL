/*
 * CorrFitHDFunc1Da.h
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_HD_FUNC_CORRFITHDFUNC1D_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_HD_FUNC_CORRFITHDFUNC1D_H_

#include "Array.h"
#include "CorrFitHDFunc.h"
#include "CorrFitMask1D.h"

#include <RtypesCore.h>

namespace Hal {
  /**
   * representation of 1D CF
   * this holds
   * a) map in HD mode
   * b) map f sum
   * NOTE - there are "active bins" as arguments of CF's
   */
  class CorrFitHDFunc1D : public CorrFitHDFunc {
    Array_1<Double_t> fDenominatorHD;
    Array_1<Double_t> fDenominatorSum;
    Array_1<Double_t> fMapHD;

    Array_1<Int_t> fBins;
    Array_1<Int_t> fBinsHD;
    void CalculateBinsArrays(const CorrFitMask1D& mask, Bool_t hd);

  public:
    CorrFitHDFunc1D();
    /**
     *
     * @return total number of "active bins"
     */
    inline Int_t GetNbins() const { return fBins.GetSize(); };
    /**
     * convert "active bin number to bin nubmer
     * @param no
     * @return
     */
    inline Int_t GetBin(Int_t no) const { return fBins.Get(no); };
    /**
     *
     * @param mask fitting mask
     * @param denominator denomiator of CF
     * @param hd if HD then calculate also values of denominator on the edges of bins
     */
    void SetMask(const CorrFitMask& mask, TH1* denominator, Bool_t hd);
    /**
     * return value of the CF at given bin, note: only active bins have this value calculated
     * @param bin
     * @param extrapolated
     * @return
     */
    Double_t GetBinCFVal(Int_t bin, Bool_t extrapolated) const;
    /**
     * calculate X-value for given "hd bin"
     * @param hdBin
     * @return
     */
    inline Double_t EvalHD(Double_t hdBin) const { return fMins[0] + fSteps[0] * hdBin; }
    inline Array_1<Int_t>& GetBinsHD() { return fBinsHD; };
    inline Array_1<Double_t>& GetCFMapHD() { return fMapHD; }
    inline Array_1<Double_t>& GetDenominatorHD() { return fDenominatorHD; };
    inline Array_1<Double_t>& GetDenominatorSum() { return fDenominatorSum; };
    virtual ~CorrFitHDFunc1D();
    ClassDef(CorrFitHDFunc1D, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_HD_FUNC_CORRFITHDFUNC1DA_H_ */
