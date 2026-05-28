/*
 * PadStyle.h
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_STYLES_PADSTYLE_H_
#define HAL_FEATURES_HIST_STYLES_PADSTYLE_H_

#include <RtypesCore.h>

#include "Style.h"

namespace Hal {
  /**
   * contains information about pad style
   */
  class PadStyle : public Style {
  public:
    enum class EBitFlag : Int_t {
      kBottomMargin,
      kLeftMargin,
      kTopMargin,
      kRightMargin,
      kLogx,
      kLogy,
      kLogz,
      kGridx,
      kGridy,
      kGridz,
      kTickX,
      kTickY,
      kFillColor,
      kLineColor,
      kFrameLineColor
    };
    PadStyle(TString style = "");
    PadStyle(const PadStyle& other) = default;
    PadStyle(Double_t x1, Double_t y1, Double_t x2, Double_t y2, TString style = "");
    /**
     * apply setting to pad if pad is null apply to gPad
     */
    void SetBottomMargin(Float_t val);

    void SetLeftMargin(Float_t val);

    void SetTopMargin(Float_t val);

    void SetRightMargin(Float_t val);

    void SetLogx(Int_t val);

    void SetLogy(Int_t val);

    void SetLogz(Int_t val);

    void SetGridx(Int_t val);

    void SetGridy(Int_t val);

    void SetGridz(Int_t val);

    void SetTickx(Int_t val);

    void SetTicky(Int_t val);

    void SetLineColor(Int_t col);

    void SetFillColor(Int_t col);

    void SetFrameLineColor(Int_t col);

    Float_t GetBottomMargin() const;

    Float_t GetLeftMargin() const;

    Float_t GetTopMargin() const;

    Float_t GetRightMargin() const;

    Int_t GetLogx() const;

    Int_t GetLogy() const;

    Int_t GetLogz() const;

    Int_t GetGridx() const;

    Int_t GetGridy() const;

    Int_t GetGridz() const;

    Int_t GetTickx() const;

    Int_t GetTicky() const;

    Int_t GetLineColor() const;

    Int_t GetFillColor() const;

    Int_t GetFrameLineColor() const;

    void ExportToXML(XMLNode& node) const;
    void ImportFromXML(const XMLNode& node);
    void Import(TVirtualPad& pad);
    void Apply(TVirtualPad* pad = nullptr);
    virtual ~PadStyle() {}
    ClassDef(PadStyle, 1);
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_STYLES_PADSTYLE_H_ */
