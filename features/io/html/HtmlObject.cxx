/*
 * HalHTMLObject.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HtmlObject.h"

#include <TString.h>
namespace Hal {
  HtmlObject::HtmlObject() : TObject(), fClass(""), fID(""), fStyle(""), fContent("") {}

  HtmlObject::~HtmlObject() {}

  HtmlObject::HtmlObject(const HtmlObject& other) :
    TObject(other), fClass(other.fClass), fID(other.fID), fStyle(other.fStyle), fContent(other.fContent) {}

  void HtmlObject::AddContent(const HtmlObject& obj) { fContent = fContent + obj.ToString(); }

  HtmlObject::HtmlObject(TString id, TString className, TString style) : fClass(className), fID(id), fStyle(style) {}

  HtmlObject& HtmlObject::operator=(const HtmlObject& other) {
    if (this == &other) return *this;
    TObject::operator=(other);
    fClass           = other.fClass;
    fID              = other.fID;
    fStyle           = other.fStyle;
    fContent         = other.fContent;
    return *this;
  }

  TString HtmlObject::GetProperties() const {
    TString properties;
    if (GetStyle().Length() > 0) { properties = properties + " style=\"" + GetStyle() + "\" "; }
    if (GetClass().Length() > 0) { properties = properties + " class=\"" + GetClass() + "\" "; }
    if (GetId().Length() > 0) { properties = properties + " id=\"" + GetId() + "\" "; }
    return properties;
  }

  TString HtmlObject::GetDefaultString(TString tag) const {
    TString res = Form("<%s", tag.Data());
    res         = res + GetProperties();
    res         = res + ">\n" + GetContent() + "</" + tag + ">\n";
    return res;
  }
}  // namespace Hal
