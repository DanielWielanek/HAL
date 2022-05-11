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
     * calculates the total number of non-zero bins
     * @param h
     * @return
     */
    Int_t CalcBinNonZero(TH1* h) const;
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
    virtual void SetMask(TH1* mask, TH1* denominator, Bool_t hd) = 0;
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
    void CalculateBinsArrays(TH1* mask, Bool_t hd);

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
    void SetMask(TH1* mask, TH1* denominator, Bool_t hd);
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
    void CalculateBinsArrays(TH1* mask, Bool_t hd);

  public:
    CorrFitHDFunc3D();
    void SetMask(TH1* mask, TH1* denominator, Bool_t hd);
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

#endif /* HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITHDFUNC_H_ */
