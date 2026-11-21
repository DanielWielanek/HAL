/*
 * PaveStyle.h
 *
 *  Created on: 19 lis 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_STYLES_PAVESTYLE_H_
#define HAL_FEATURES_HIST_STYLES_PAVESTYLE_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "FillStyle.h"
#include "LineStyle.h"
#include "TextStyle.h"
#include "XMLNode.h"


class TPave;
class TPaveText;
namespace Hal {

  class PaveStyle : public Style {
    LineStyle fLine;
    FillStyle fFill;
    TextStyle fText;

  public:
    enum class EBitFlag : Int_t { kBorderSize, kCornerRadius, kShadowColor };
    PaveStyle(TString = "");
    LineStyle& GetLineStyle() { return fLine; }
    FillStyle& GetFillStyle() { return fFill; }
    void SetBorderSize(Int_t size);
    void SetCornerRadis(Double_t size);
    void SetShadowColor(Int_t color);
    Int_t GetBorderSize() const;
    Int_t GetShadowColor() const;
    Double_t GetCornerRadius() const;
    void ExportToXML(XMLNode& node) const;
    void ImportFromXML(const XMLNode& node);
    void Apply(TPave& leg) const;
    virtual void SetGlobal(TString opt = "") const;
    virtual ~PaveStyle() {};
    ClassDef(PaveStyle, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_STYLES_PAVESTYLE_H_ */
