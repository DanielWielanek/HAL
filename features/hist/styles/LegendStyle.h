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
#include "TextStyle.h"

class TLegend;

namespace Hal {

  class LegendStyle : public Style {
    FillStyle fFill;
    LineStyle fLine;
    TextStyle fText;

  public:
    enum class EBitFlag : Int_t { kTitle, kMin, kMax };
    /**
     * set legend style
     * @param style allowed style "apollo", "black" "05" and "06"
     */
    LegendStyle(TString style = "");
    void Import(const TLegend& x);
    FillStyle& GetFillStyle() { return fFill; }
    LineStyle& GetLineStyle() { return fLine; }
    TextStyle& GetTextStyle() { return fText; }
    void Apply(TLegend& leg) const;
    void ExportToXML(XMLNode& node) const;
    void ImportFromXML(const XMLNode& node);
    virtual void SetGlobal(TString opt = "") const;
    virtual ~LegendStyle() {};
    ClassDef(LegendStyle, 0);
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_STYLES_LEGENDSTYLE_H_ */
