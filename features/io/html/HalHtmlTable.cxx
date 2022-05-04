/*
 * HalHtmlTable.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HalHtmlTable.h"

#include <TString.h>
#include <iostream>

HalHtmlTable::HalHtmlTable() {}

HalHtmlTable::HalHtmlTable(TString id, TString className, TString style) : HalHtmlObject(id, className, style) {}
void HalHtmlTable::AddContent(const HalHtmlObject& obj) {
  if (obj.InheritsFrom("HalHtmlRow")) { HalHtmlObject::AddContent(obj); }
}

HalHtmlTable::~HalHtmlTable() {}

TString HalHtmlTable::ToString() const { return GetDefaultString("table"); }

HalHtmlRow::HalHtmlRow() {}

HalHtmlRow::HalHtmlRow(TString id, TString className, TString style) : HalHtmlObject(id, className, style) {}

void HalHtmlRow::AddContent(const HalHtmlObject& obj) {
  if (obj.InheritsFrom("HalHtmlCell")) { HalHtmlObject::AddContent(obj); }
}

TString HalHtmlRow::ToString() const { return GetDefaultString("tr"); }

HalHtmlRow::~HalHtmlRow() {}

HalHtmlCell::HalHtmlCell() : fColSpan(0), fRowSpan(0) {}

void HalHtmlCell::AddContent(const HalHtmlObject& obj) { HalHtmlObject::AddContent(obj); }

TString HalHtmlCell::ToString() const {
  TString res = "<td";
  res         = res + GetProperties();
  if (fColSpan > 0) {
    res = res + Form(" colspan=\"%i\">", fColSpan);
  } else if (fRowSpan > 0) {
    res = res + Form(" rowspan=\"%i\">", fRowSpan);
  } else {
    res = res + ">";
  }
  res = res + GetContent();
  return res + "</td>";
}

HalHtmlCell::~HalHtmlCell() {}

HalHtmlCell::HalHtmlCell(TString content) : HalHtmlCell() { SetStringContent(content); }

HalHtmlCell::HalHtmlCell(TString id, TString className, TString style) :
  HalHtmlObject(id, className, style), fColSpan(0), fRowSpan(0) {}

HalHtmlCellCol::HalHtmlCellCol() {}

HalHtmlCellCol::HalHtmlCellCol(TString name, Int_t colspan) : HalHtmlCell(name) { SetColSpan(colspan); }

HalHtmlCellCol::~HalHtmlCellCol() {}

HalHtmlCellRow::HalHtmlCellRow() {}

HalHtmlCellRow::HalHtmlCellRow(TString name, Int_t colspan) : HalHtmlCell(name) { SetRowSpan(colspan); }

HalHtmlCellRow::~HalHtmlCellRow() {}

namespace HalHtmlTableRowStyles {
  TString LightBlue() { return "light_blue"; };
  TString MedBlue() { return "med_blue"; };
  TString DarkBlue() { return "dark_blue"; };
  TString LightGreen() { return "green_"; };
  TString Green() { return "med_green"; };  // not exist!
  TString DarkGreen() { return "dark_green"; };
  TString Yellow() { return "yellow_"; };
  TString Grey() { return "grey_"; };
  TString Red() { return "red_"; };
  TString Violet() { return "violet_"; };

  TString DefStyle() { return LightBlue(); };
  TString LegendStyle() { return LightGreen(); };
  TString TitleStyle() { return DarkBlue(); };
  TString ExtraStyle() { return MedBlue(); };
  TString TaskStyle() { return Red(); };
  TString ExpandableStyle() { return Yellow(); };
  TString SummaryStyle() { return Violet(); };
}  // namespace HalHtmlTableRowStyles
