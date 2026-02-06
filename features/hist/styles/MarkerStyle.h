/*
 * MarkerStyle.h
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_STYLES_MARKERSTYLE_H_
#define HAL_FEATURES_HIST_STYLES_MARKERSTYLE_H_

#include "Style.h"

namespace Hal {
  /**
   * constains informations about marker style
   */
  class MarkerStyle : public Style {
  public:
    enum class EBitFlag : Int_t { kColor, kStyle, kLineWidth, kSize };

    MarkerStyle() {};

    void SetColor(Int_t val);

    void SetStyle(Int_t val);

    //  void SetLineWidth(Int_t val);

    void SetSize(Double_t val);

    Int_t GetColor() const;

    Int_t GetStyle() const;

    // Int_t GetLineWidth() const;

    Double_t GetSize() const;

    void ExportToXML(XMLNode& node) const;

    void ImportFromXML(const XMLNode& node);

    template<class T>
    void Apply(T& obj) const;

    template<class T>
    void Import(const T& obj);

    virtual ~MarkerStyle() {};
    ClassDef(MarkerStyle, 1)
  };

} /* namespace Hal */

template<class T>
inline void Hal::MarkerStyle::Apply(T& obj) const {
  if (Find(EBitFlag::kColor)) obj.SetMarkerColor(GetI(EBitFlag::kColor));
  if (Find(EBitFlag::kSize)) obj.SetMarkerSize(GetF(EBitFlag::kSize));
  // if (Find(EBitFlag::kLineWidth)) obj.SetMarkerLineWidth(GetI(EBitFlag::kLineWidth));
  if (Find(EBitFlag::kStyle)) obj.SetMarkerStyle(GetI(EBitFlag::kStyle));
}

template<class T>
inline void Hal::MarkerStyle::Import(const T& obj) {
  SetColor(obj.GetMarkerColor());
  SetSize(obj.GetMarkerSize());
  SetStyle(obj.GetMarkerStyle());
}

#endif /* HAL_FEATURES_HIST_STYLES_MARKERSTYLE_H_ */
