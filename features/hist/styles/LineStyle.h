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
    enum class EBitFlag : Int_t { kColor, kWidth, kStyle };
    LineStyle() {};

    void SetColor(Int_t val);

    void SetWidth(Int_t val);

    void SetStyle(Int_t val);

    Int_t GetColor() const;

    Int_t GetWidth() const;

    Int_t GetStyle() const;

    void ExportToXML(XMLNode& node) const;

    void ImportFromXML(const XMLNode& node);


    template<class T>
    void Apply(T& obj) const;

    template<class T>
    void Import(const T& obj);
    virtual ~LineStyle() {};
    ClassDef(LineStyle, 1);
  };

} /* namespace Hal */

template<class T>
inline void Hal::LineStyle::Apply(T& obj) const {
  if (Find(EBitFlag::kWidth)) obj.SetLineWidth(GetI(EBitFlag::kWidth));
  if (Find(EBitFlag::kColor)) obj.SetLineColor(GetI(EBitFlag::kColor));
  if (Find(EBitFlag::kStyle)) obj.SetLineStyle(GetI(EBitFlag::kStyle));
}
template<class T>
inline void Hal::LineStyle::Import(const T& obj) {
  SetWidth(obj.GetLineWidth());
  SetColor(obj.GetLineColor());
  SetStyle(obj.GetLineStyle());
}

#endif /* HAL_FEATURES_HIST_STYLES_LINESTYLE_H_ */
