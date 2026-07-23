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
    static constexpr uint32_t fgReFlagId        = {0};
    static constexpr uint32_t fgImFlagId        = {1};
    static constexpr uint32_t fgSigFlagId       = {2};
    static constexpr uint32_t fgBckgFlagId      = {3};
    static constexpr uint32_t fgFastFlagId      = {4};
    static constexpr uint32_t fgAccNullFlagId   = {5};
    static constexpr uint32_t fgAccDoubleFlagId = {6};
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
     * make copy of cut
     * @param cu cut to copy
     * @param flag re for real, other create img flag
     * @param acceptNulls for img if true than accept nulls
     * @return
     */
    Hal::Cut* MakeCutCopy(const Hal::Cut& cut, TString flag, Bool_t acceptNulls = kTRUE) const;
    /**
     * return cut update ratio name
     * @param upd
     * @return
     */
    TString GetCutUpdateRatioName(Hal::ECutUpdate upd) const;

  public:
    /**
     * base constructor
     * @param opt option
     * @param defCol default collection number
     */
    CutOptions(TString opt = "", Int_t defCol = -1);
    /**
     * clear given flag
     * @param flag
     */
    void ClearFlag(TString flag);
    /**
     *
     * @return true if real option is set
     */
    Bool_t Re() const { return TESTBIT(fFlag, fgReFlagId); }
    /**
     *
     * @return true if imaginary option is set
     */
    Bool_t Im() const { return TESTBIT(fFlag, fgImFlagId); }
    /**
     *
     * @return true if signal option is set
     */
    Bool_t Sig() const { return TESTBIT(fFlag, fgSigFlagId); }
    /**
     *
     * @return true if background option is set
     */
    Bool_t Bckg() const { return TESTBIT(fFlag, fgBckgFlagId); }
    /**
     *
     * @return true if fast option is set
     */
    Bool_t Fast() const { return TESTBIT(fFlag, fgFastFlagId); }
    /**
     *
     * @return true if accept null flags is set
     */
    Bool_t Null() const { return TESTBIT(fFlag, fgAccNullFlagId); }
    /**
     *
     * @return true if keep double option is set
     */
    Bool_t KeepDouble() const { return TESTBIT(fFlag, fgAccDoubleFlagId); }
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
    virtual ~CutOptions() {};
    ClassDef(CutOptions, 1)
  };


} /* namespace Hal */

#endif /* HAL_CUTS_CUTOPTIONS_H_ */
