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
    static const unsigned short int kBottomMargin;
    static const unsigned short int kLeftMargin;
    static const unsigned short int kTopMargin;
    static const unsigned short int kRightMargin;
    static const unsigned short int kLogx;
    static const unsigned short int kLogy;
    static const unsigned short int kLogz;
    static const unsigned short int kGridx;
    static const unsigned short int kGridy;
    static const unsigned short int kGridz;
    static const unsigned short int kTickX;
    static const unsigned short int kTickY;

    PadStyle();
    PadStyle(const PadStyle& other) = default;
    PadStyle(Double_t x1, Double_t y1, Double_t x2, Double_t y2);
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

    Float_t GetBottomMargin() const;

    Float_t GetLeftMargin() const;

    Float_t GetTopMargin() const;

    Float_t GetRightMargin() const;

    Int_t GetLogx(Int_t val) const;

    Int_t GetLogy(Int_t val) const;

    Int_t GetLogz(Int_t val) const;

    Int_t GetGridx(Int_t val) const;

    Int_t GetGridy(Int_t val) const;

    Int_t GetGridz(Int_t val) const;

    Int_t GetTickx(Int_t val) const;

    Int_t GetTicky(Int_t val) const;

    void ExportToXML(XMLNode* node) const;
    void ImportFromXML(XMLNode* node);
    void Import(TVirtualPad& pad);
    void Apply(TVirtualPad* pad = nullptr);
    virtual ~PadStyle() {}
    ClassDef(PadStyle, 1);
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_STYLES_PADSTYLE_H_ */
