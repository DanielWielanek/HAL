/*
 * AxisStyle.h
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_AXISSTYLE_H_
#define HAL_FEATURES_HIST_AXISSTYLE_H_

#include "Style.h"
namespace Hal {

  /**
   * contains information about axis style
   */
  class AxisStyle : public Style {

    TString fTitle;
    Bool_t fSkipTitle;

  public:
    static const unsigned short int kTitleOffset;
    static const unsigned short int kLabelOffset;
    static const unsigned short int kTitleSize;
    static const unsigned short int kLabelSize;
    static const unsigned short int kTickLength;
    static const unsigned short int kNdivisions;
    static const unsigned short int kAxisColor;
    static const unsigned short int kLabelColor;
    static const unsigned short int kTitleColor;
    static const unsigned short int kTitleFont;
    static const unsigned short int kCenterTitle;
    static const unsigned short int kTitle;
    static const unsigned short int kRangeMin;
    static const unsigned short int kRangeMax;
    static const unsigned short int kTicksOpt;
    static const unsigned short int kMoreLog;

    static const unsigned short int kRotatedTitle;
    static const unsigned short int kFontStyleLabel;
    static const unsigned short int kDecimal;
    static const unsigned short int kNoExp;

    AxisStyle(Bool_t skipTitle = kTRUE);

    AxisStyle(Double_t titleSize, Double_t labelSize, Double_t titleOffset, Double_t labelOffset);

    void SetTitleOffset(Float_t val);

    void SetLabelOffset(Float_t val);

    void SetTitleSize(Float_t val);

    void SetLabelSize(Float_t val);

    void SetTickLength(Float_t val);

    void SetNdivisions(Int_t val, Bool_t optim);

    void SetAxisColor(Int_t val);

    void SetLabelColor(Int_t val);

    void SetTitleColor(Int_t val);

    void SetTitleFont(Int_t val);

    void SetCenterTitle(Int_t val);

    void SetTitle(TString val);

    void SetRangeUser(Float_t min, Float_t max);

    void SetTicks(TString opt);

    void SetMoreLogLabels(Bool_t val = kTRUE);

    void SetLabelFont(Int_t font);

    void SetRotateTitle(Bool_t rot);

    void SetNoExponent(Bool_t rot);

    void SetDecimals(Bool_t opt);

    Float_t GetTitleOffset() const;

    Float_t GetLabelOffset() const;

    Float_t GetTitleSize() const;

    Float_t GetLabelSize() const;

    Float_t GetTickLength() const;

    Int_t GetNDivisions() const;

    Int_t GetAxisColor() const;

    Int_t GetLabelColor() const;

    Int_t GetTitleColor() const;

    Int_t GetTitleFont() const;

    Int_t GetCenterTitle() const;

    Int_t GetLabelFont() const;

    Bool_t GetRotatedTitle() const;

    Bool_t GetDecimal() const;

    Bool_t GetMoreLogLabels() const;

    Bool_t GetNoExponent() const;

    Bool_t NDivisionsOptimized() const;

    TString GetTicks() const;

    TString GetTitle() const { return fTitle; };

    void ExportToXML(XMLNode* node) const;

    void ImportFromXML(XMLNode* node);

    void Apply(TAxis& x) const;

    void Import(const TAxis& x);

    virtual ~AxisStyle() {};
    ClassDef(AxisStyle, 1);
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_AXISSTYLE_H_ */
