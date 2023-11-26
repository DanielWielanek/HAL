/*
 * FemtoCorrFunc2Db.h
 *
 *  Created on: 26 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC2D_H_
#define HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC2D_H_

#include "Array.h"
#include "DividedHisto.h"
#include "FemtoCorrFunc.h"

#include <iostream>

#include <TString.h>

namespace Hal {
  class FemtoCorrFunc2D : public FemtoCorrFunc {
  protected:
    const TString fLabelX, fLabelY;
    Array_1<Float_t> fRangeX;
    Array_1<Float_t> fRangeY;
    TH2D* fNumProp = {nullptr};
    TH2D* fDenProp = {nullptr};
    Int_t fBinsX   = {100};
    Int_t fBinsY   = {100};

    virtual Bool_t InitPropMon();
    virtual void AddHTMLCode(std::ofstream& /*file*/) const {};
    virtual std::pair<Double_t, Double_t> GetPairValNum(FemtoPair* pair) const    = 0;
    virtual std::pair<Double_t, Double_t> GetPairValDenRot(FemtoPair* pair) const = 0;
    virtual std::pair<Double_t, Double_t> GetPairValDenPerf(FemtoPair* pair) const { return GetPairValNum(pair); };
    virtual std::pair<Double_t, Double_t> GetPairValDenHemi(FemtoPair* pair) const = 0;
    virtual std::pair<Double_t, Double_t> GetPairValDenChar(FemtoPair* pair) const { return GetPairValNum(pair); };
    virtual std::pair<Double_t, Double_t> GetPairValDenMix(FemtoPair* pair) const { return GetPairValNum(pair); };
    Int_t ConvertVal(std::pair<Double_t, Double_t> vals) const;
    FemtoCorrFunc2D(const DividedHisto1D* h, TString labelX = "", TString labelY = "", Int_t sizeX = 0, Int_t sizeY = 0);
    virtual Bool_t Angle(Char_t /*opt*/) const { return kFALSE; }

  public:
    FemtoCorrFunc2D() : FemtoCorrFunc() {};
    FemtoCorrFunc2D(const FemtoCorrFunc2D& other);
    FemtoCorrFunc2D(const DividedHisto1D& h, TString labelX = "", TString labelY = "", Int_t sizeX = 0, Int_t sizeY = 0);
    virtual Bool_t Check();
    virtual TString HTMLExtract(Int_t no, TString dir = "") const;
    TString GetXlabel() const { return fLabelX; };
    TString GetYlabel() const { return fLabelY; };
    void SetNBinsX(Int_t bins) { fBinsX = bins; };
    void SetNBinsY(Int_t bins) { fBinsY = bins; };
    Int_t GetXsize() const { return fRangeX.GetSize() - 1; };
    Int_t GetYsize() const { return fRangeY.GetSize() - 1; };
    const Array_1<Float_t> GetRangeX() const { return fRangeX; };
    const Array_1<Float_t> GetRangeY() const { return fRangeY; };
    DividedHisto1D* GetCF(Int_t i, Int_t j) const;
    TH2D* GetNumMon() const { return fNumProp; }
    TH2D* GetDenMon() const { return fDenProp; };
    void FillNum(FemtoPair* pair);
    void FillDenMixed(FemtoPair* pair);
    void FillDenRotated(FemtoPair* pair);
    void FillDenHemisphere(FemtoPair* pair);
    void FillDenCharged(FemtoPair* pair);
    void FillDenPerfect(FemtoPair* pair);
    virtual ~FemtoCorrFunc2D();
    ClassDef(FemtoCorrFunc2D, 2)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC2D_H_ */
