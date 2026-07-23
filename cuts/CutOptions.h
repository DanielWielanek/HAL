/*
 * CutOptions.h
 *
 *  Created on: 14 sty 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTOPTIONS_H_
#define HAL_CUTS_CUTOPTIONS_H_

#include <Rtypes.h>
#include <TObject.h>
#include <TString.h>
#include <vector>

#include "Std.h"

namespace Hal {
  class Cut;
  class CutMonitor;

  class CutOptions : public TObject {
    std::vector<Int_t> fCollections;
    TString fExtra;
    static constexpr uint32_t fgReFlagId        = {0};
    static constexpr uint32_t fgImFlagId        = {1};
    static constexpr uint32_t fgSigFlagId       = {2};
    static constexpr uint32_t fgBckgFlagId      = {3};
    static constexpr uint32_t fgFastFlagId      = {4};
    static constexpr uint32_t fgAccNullFlagId   = {5};
    static constexpr uint32_t fgAccDoubleFlagId = {6};
    static constexpr uint32_t fgStartColSet     = {7};
    Int_t fFlag                                 = {0};
    Int_t fDefCol                               = {-1};
    /**
     * find number in exression like *{A}*
     * @param expression
     * @param val value found
     * @param remove remove from expression
     * @return true if expression found
     */
    Bool_t FindExpressionSingleValue(TString& expression, Int_t& val, Bool_t remove) const;
    /**
     * find pair of numbers in expression like *{AxB}*
     * @param expression
     * @param val1 first found value
     * @param va2 second found value
     * @param remove remove from expression
     * @return true if expression found
     */
    Bool_t FindExpressionTwoValues(TString& expression, Int_t& val1, Int_t& val2, Bool_t remove) const;

  protected:
    /**
     * calculate collection Id's from expression
     * @param option
     * @return
     */
    std::vector<Int_t> GetCollectionsFlags(Int_t startCol, TString option) const;
    /**
     * return cut update ratio name
     * @param upd
     * @return
     */
    TString GetCutUpdateRatioName(Hal::ECutUpdate upd) const;

    void SetFlag(Bool_t state, Int_t flag) {
      if (state) {
        SETBIT(fFlag, flag);
      } else {
        CLRBIT(fFlag, flag);
      }
    }

  public:
    /**
     * base constructor
     * @param opt option
     * @param defCol default collection number
     */
    CutOptions(TString opt = "", Int_t defCol = -2);
    CutOptions(const CutOptions& other) = default;
    /**
     * clear given flag
     * @param flag
     */
    void ClearFlag(TString flag);
    /**
     *
     * @return true if real option is set
     */
    Bool_t IsRe() const { return TESTBIT(fFlag, fgReFlagId); }
    /**
     *
     * @return true if imaginary option is set
     */
    Bool_t IsIm() const { return TESTBIT(fFlag, fgImFlagId); }
    /**
     *
     * @return true if signal option is set
     */
    Bool_t IsSig() const { return TESTBIT(fFlag, fgSigFlagId); }
    /**
     *
     * @return true if background option is set
     */
    Bool_t IsBckg() const { return TESTBIT(fFlag, fgBckgFlagId); }
    /**
     *
     * @return true if fast option is set
     */
    Bool_t IsFast() const { return TESTBIT(fFlag, fgFastFlagId); }
    /**
     *
     * @return true if accept null flags is set
     */
    Bool_t IsNull() const { return TESTBIT(fFlag, fgAccNullFlagId); }
    /**
     *
     * @return true if keep double option is set
     */
    Bool_t IsKeepDouble() const { return TESTBIT(fFlag, fgAccDoubleFlagId); }
    /**
     *
     * @return true if start colission was set
     */
    Bool_t IsColIdSet() const { return TESTBIT(fFlag, fgStartColSet); };
    /**
     *
     * @param set status of real flag
     */
    void SetRe(Bool_t stat = kTRUE) { SetFlag(stat, fgReFlagId); }
    /**
     *
     * @param set status of imaginary flag
     */
    void SetIm(Bool_t stat = kTRUE) { SetFlag(stat, fgImFlagId); }
    /**
     *
     * @param set status of signal flag
     */
    void SetSig(Bool_t stat = kTRUE) { SetFlag(stat, fgSigFlagId); }
    /**
     *
     * @return @param set status of background flag
     */
    void SetBckg(Bool_t stat = kTRUE) { SetFlag(stat, fgBckgFlagId); }
    /**
     *
     * @return @param set status of fast flag
     */
    void SetFast(Bool_t stat = kTRUE) { SetFlag(stat, fgFastFlagId); }
    /**
     *
     * @return @param set status of null flag
     */
    void SetNull(Bool_t stat = kTRUE) { SetFlag(stat, fgAccNullFlagId); }
    /**
     *
     * @return @param set status of double flag
     */
    void SetKeepDouble(Bool_t stat = kTRUE) { SetFlag(stat, fgAccDoubleFlagId); }
    /**
     *
     * @return collection id's
     */
    std::vector<Int_t> GetCollectionsIds() const { return fCollections; };
    /**
     * make copy of cut according to those options
     * @param x
     * @return
     */
    Hal::Cut* MakeCutCopy(const Hal::Cut& x) const;
    /**
     * make cut monitor copy accordint to those options
     * @param x
     * @return
     */
    Hal::CutMonitor* MakeMonitorCopy(const Hal::CutMonitor& x) const;
    /**
     * @return extra flags
     */
    TString GetExtra() const { return fExtra; }
    /**
     * set extra flags
     */
    void SetExtra(TString extra) { fExtra = extra; };
    virtual ~CutOptions() {};
    ClassDef(CutOptions, 1)
  };


} /* namespace Hal */

#endif /* HAL_CUTS_CUTOPTIONS_H_ */
