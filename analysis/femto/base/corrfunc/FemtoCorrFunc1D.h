/*
 * FemtoCorrFunc1Db.h
 *
 *  Created on: 26 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC1D_H_
#define HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC1D_H_

#include "FemtoCorrFunc.h"
namespace Hal {
  /**
   * basic class for drawing collection of CF's as a function of pair parameter
   */
  class FemtoCorrFunc1D : public FemtoCorrFunc {
  protected:
    /**
     * label that describe value
     */
    const TString fLabel;
    /**
     * numerator distribution of value
     */
    TH1D* fNumProp = {nullptr};
    /**
     * denominator distribution of  label
     */
    TH1D* fDenProp = {nullptr};
    virtual Bool_t Angle() const { return kFALSE; }
    Array_1<Float_t> fRange;
    void SetBins(const std::initializer_list<double>& array);
    virtual void AddHTMLCode(HtmlFile& /*file*/) const {};
    virtual Double_t GetPairValNum(FemtoPair* pair) const    = 0;
    virtual Double_t GetPairValDenRot(FemtoPair* pair) const = 0;
    virtual Double_t GetPairValDenPerf(FemtoPair* pair) const { return GetPairValNum(pair); };
    virtual Double_t GetPairValDenHemi(FemtoPair* pair) const { return GetPairValDenRot(pair); };
    virtual Double_t GetPairValDenChar(FemtoPair* pair) const { return GetPairValNum(pair); };
    virtual Double_t GetPairValDenMix(FemtoPair* pair) const { return GetPairValNum(pair); };
    /**
     * convert value of pair into bit of CF that should be filled
     * @param val
     * @return
     */
    Int_t ConvertVal(Double_t val) const;
    FemtoCorrFunc1D(const DividedHisto1D& h,
                    std::initializer_list<Double_t> cuts,
                    Int_t bins,
                    TString xTitle,
                    TString xAxis,
                    TString yAxis);

  public:
    FemtoCorrFunc1D() : FemtoCorrFunc() {};
    FemtoCorrFunc1D(const DividedHisto1D& h, std::initializer_list<Double_t> cuts, Int_t bins) :
      FemtoCorrFunc1D(h, cuts, bins, "", "", "") {};
    void FillNum(FemtoPair* pair);
    void FillDenPerfect(FemtoPair* pair);
    void FillDenRotated(FemtoPair* pair);
    void FillDenMixed(FemtoPair* pair);
    void FillDenHemisphere(FemtoPair* pair);
    void FillDenCharged(FemtoPair* pair);
    TH1D* GetNumMon() const { return fNumProp; };
    TH1D* GetDenMon() const { return fDenProp; }
    virtual Bool_t Check();
    virtual void Add(const Object* pack);
    virtual TString HTMLExtract(Int_t no, TString dir = "") const;
    TString GetLabel() const { return fLabel; };
    const Array_1<Float_t> GetRange() const { return fRange; };
    virtual ~FemtoCorrFunc1D();
    ClassDef(FemtoCorrFunc1D, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC1DB_H_ */
