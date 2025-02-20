/*
 * LegendStyle.h
 *
 *  Created on: 20 gru 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_STYLES_LEGENDSTYLE_H_
#define HAL_FEATURES_HIST_STYLES_LEGENDSTYLE_H_

#include <Rtypes.h>
#include <RtypesCore.h>

#include "FillStyle.h"
#include "LineStyle.h"

class TLegend;

namespace Hal {

  class LegendStyle : public Style {
    FillStyle fFill;
    LineStyle fLine;
    static const unsigned short int kFontSize;
    static const unsigned short int kFontStyle;

  public:
    LegendStyle() {};
    static LegendStyle GetStyle(TString opt);
    void SetFontSize(Float_t size);
    void SetFont(Int_t style);
    void Import(const TLegend& x);
    Float_t GetFontSize() const;
    Int_t GetFont() const;
    FillStyle& GetFillStyle() { return fFill; }
    LineStyle& GetLineStyle() { return fLine; }
    void Apply(TLegend& leg) const;
    void ExportToXML(XMLNode* node) const;
    void ImportFromXML(XMLNode* node);
    virtual ~LegendStyle() {};
    ClassDef(LegendStyle, 0);
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_STYLES_LEGENDSTYLE_H_ */
