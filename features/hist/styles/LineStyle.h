/*
 * LineStyle.h
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_STYLES_LINESTYLE_H_
#define HAL_FEATURES_HIST_STYLES_LINESTYLE_H_

#include "Style.h"

namespace Hal {
  /**
   * contains information about line style
   */
  class LineStyle : public Style {


  public:
    static const unsigned short int kColor;
    static const unsigned short int kWidth;
    static const unsigned short int kStyle;
    LineStyle() {};

    void SetColor(Int_t val);

    void SetWidth(Int_t val);

    void SetStyle(Int_t val);

    Int_t GetColor() const;

    Int_t GetWidth() const;

    Int_t GetStyle() const;

    void ExportToXML(XMLNode* node) const;

    void ImportFromXML(XMLNode* node);

    template<class T>
    void Apply(T& obj) const;

    virtual ~LineStyle() {};
    ClassDef(LineStyle, 1);
  };

} /* namespace Hal */

template<class T>
inline void Hal::LineStyle::Apply(T& obj) const {
  if (Find(kWidth)) obj.SetLineWidth(GetI(kWidth));
  if (Find(kColor)) obj.SetLineColor(GetI(kColor));
  if (Find(kStyle)) obj.SetLineStyle(GetI(kStyle));
}

#endif /* HAL_FEATURES_HIST_STYLES_LINESTYLE_H_ */
