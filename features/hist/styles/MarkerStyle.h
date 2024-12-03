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
    static const unsigned short int kColor;
    static const unsigned short int kStyle;
    static const unsigned short int kLineWidth;
    static const unsigned short int kSize;

    MarkerStyle() {};

    void SetColor(Int_t val);

    void SetStyle(Int_t val);

    //  void SetLineWidth(Int_t val);

    void SetSize(Int_t val);

    Int_t GetColor() const;

    Int_t GetStyle() const;

    // Int_t GetLineWidth() const;

    Int_t GetSize() const;

    void ExportToXML(XMLNode* node) const;

    void ImportFromXML(XMLNode* node);

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
  if (Find(kColor)) obj.SetMarkerColor(GetI(kColor));
  if (Find(kSize)) obj.SetMarkerSize(GetI(kSize));
  // if (Find(kLineWidth)) obj.SetMarkerLineWidth(GetI(kLineWidth));
  if (Find(kStyle)) obj.SetMarkerStyle(GetI(kStyle));
}

template<class T>
inline void Hal::MarkerStyle::Import(const T& obj) {
  SetColor(obj.GetMarkerColor());
  SetSize(obj.GetMarkerSize());
  SetStyle(obj.GetMarkerStyle());
}

#endif /* HAL_FEATURES_HIST_STYLES_MARKERSTYLE_H_ */
