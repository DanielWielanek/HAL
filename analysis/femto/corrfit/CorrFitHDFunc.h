/*
 * CorrFitTempMaps.h
 *
 *  Created on: 14 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITHDFUNC_H_
#define HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITHDFUNC_H_

#include "Array.h"
#include "CorrFitMask.h"
#include "DividedHisto.h"
#include "Splines.h"

#include <TObject.h>

/**
 * store denominator of CF in "hd mode" (with extrapolated bins inside)
 */
namespace Hal {
  class CorrFitHDFunc : public TObject {
  protected:
    /**
     * dummy val - used to avoid recalculation of bins
     */
    const Int_t fDummyVal;
    const Int_t fDim;
    Array_1<Int_t> fHDToBin;
    Double_t* fParams;  //[fDim]
    Double_t* fMins;    //[fDim]
    Double_t* fSteps;   //[fDim]
    /**
     * recalculate array with conversion from bin to hd bin
     * @param newSize
     */
    void RecalcHDBin(Int_t newSize);
    /**
     *
     * @param bin
     * @return HD bin number for given bin
     */
    inline Int_t GetBinHD(Int_t bin) const { return bin * 2 - 1; };

  public:
    CorrFitHDFunc(Int_t dim = 1);
    /**
     *
     * @param mask
     * @return number of bins
     */
    virtual void SetMask(const CorrFitMask& mask, TH1* denominator, Bool_t hd) = 0;
    /**
     * conver "standard bin number" to "HD bin number"
     * @param hd_bin
     * @return
     */
    inline Int_t HDBinToBin(Int_t hd_bin) const { return fHDToBin.Get(hd_bin); };
    /**
     * return half of the bin width of original function
     * @param no axis
     * @return
     */
    inline Double_t GetStep(Int_t no) const { return fSteps[no]; };
    /**
     * return minimal value of the histogram with CF
     * @param no
     * @return
     */
    inline Double_t GetMin(Int_t no) const { return fMins[no]; };
    virtual ~CorrFitHDFunc();
    ClassDef(CorrFitHDFunc, 1)
  };


}  // namespace Hal

#endif /* HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITHDFUNC_H_ */
