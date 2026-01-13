/*
 * TextStyle.h
 *
 *  Created on: 13 sty 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_STYLES_TEXTSTYLE_H_
#define HAL_FEATURES_HIST_STYLES_TEXTSTYLE_H_

#include "Style.h"
#include "TAttText.h"

namespace Hal {

  class TextStyle : public Style {
  public:
    static const unsigned short int kColor;
    static const unsigned short int kAngle;
    static const unsigned short int kFont;
    static const unsigned short int kSize;
    static const unsigned short int kAling;
    TextStyle();
    void Apply(TAttText& st) const;
    void SetAlign(Int_t align);
    void SetAngle(Float_t angle);
    void SetColor(Int_t color);
    void SetSize(Float_t size);
    void SetFont(Float_t font);
    Int_t GetAlign() const;
    Int_t GetColor() const;
    Float_t GetAngle() const;
    Float_t GetSize() const;
    Float_t GetFont() const;
    void ExportToXML(XMLNode* node) const;
    void ImportFromXML(XMLNode* node);
    void Import(const TAttText& text);
    virtual ~TextStyle();
    ClassDef(TextStyle, 1);
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_STYLES_TEXTSTYLE_H_ */
