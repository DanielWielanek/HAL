/*
 * HalHTMLObject.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HalHtmlObject.h"

#include <TString.h>

HalHtmlObject::HalHtmlObject() : TObject(), fClass(""), fID(""), fStyle(""), fContent("") {}

HalHtmlObject::~HalHtmlObject() {}

HalHtmlObject::HalHtmlObject(const HalHtmlObject& other) :
  TObject(other), fClass(other.fClass), fID(other.fID), fStyle(other.fStyle), fContent(other.fContent) {}

void HalHtmlObject::AddContent(const HalHtmlObject& obj) { fContent = fContent + obj.ToString(); }

HalHtmlObject::HalHtmlObject(TString id, TString className, TString style) : fClass(className), fID(id), fStyle(style) {}

HalHtmlObject& HalHtmlObject::operator=(const HalHtmlObject& other) {
  if (this == &other) return *this;
  TObject::operator=(other);
  fClass           = other.fClass;
  fID              = other.fID;
  fStyle           = other.fStyle;
  fContent         = other.fContent;
  return *this;
}

TString HalHtmlObject::GetProperties() const {
  TString properties;
  if (GetStyle().Length() > 0) { properties = properties + " style=\"" + GetStyle() + "\" "; }
  if (GetClass().Length() > 0) { properties = properties + " class=\"" + GetClass() + "\" "; }
  if (GetId().Length() > 0) { properties = properties + " id=\"" + GetId() + "\" "; }
  return properties;
}

TString HalHtmlObject::GetDefaultString(TString tag) const {
  TString res = Form("<%s", tag.Data());
  res         = res + GetProperties();
  res         = res + ">\n" + GetContent() + "</" + tag + ">\n";
  return res;
}
