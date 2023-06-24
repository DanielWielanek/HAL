/*
 * HalHtmlTable.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HtmlTable.h"

#include <TString.h>
#include <iostream>
namespace Hal {
  HtmlTable::HtmlTable() {}

  HtmlTable::HtmlTable(TString id, TString className, TString style) : HtmlObject(id, className, style) {}
  void HtmlTable::AddContent(const HtmlObject& obj) {
    if (obj.InheritsFrom("Hal::HtmlRow")) { HtmlObject::AddContent(obj); }
  }

  HtmlTable::~HtmlTable() {}

  TString HtmlTable::ToString() const { return GetDefaultString("table"); }

  HtmlRow::HtmlRow() {}

  HtmlRow::HtmlRow(TString id, TString className, TString style) : HtmlObject(id, className, style) {}

  void HtmlRow::AddContent(const HtmlObject& obj) {
    if (obj.InheritsFrom("Hal::HtmlCell")) { HtmlObject::AddContent(obj); }
  }

  TString HtmlRow::ToString() const { return GetDefaultString("tr"); }

  HtmlRow::~HtmlRow() {}

  HtmlCell::HtmlCell() : fColSpan(0), fRowSpan(0) {}

  void HtmlCell::AddContent(const HtmlObject& obj) { HtmlObject::AddContent(obj); }

  TString HtmlCell::ToString() const {
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

  HtmlCell::~HtmlCell() {}

  HtmlCell::HtmlCell(TString content) : HtmlCell() { SetStringContent(content); }

  HtmlCell::HtmlCell(TString id, TString className, TString style) : HtmlObject(id, className, style), fColSpan(0), fRowSpan(0) {}

  HtmlCellCol::HtmlCellCol() {}

  HtmlCellCol::HtmlCellCol(TString name, Int_t colspan) : HtmlCell(name) { SetColSpan(colspan); }

  HtmlCellCol::~HtmlCellCol() {}

  HtmlCellRow::HtmlCellRow() {}

  HtmlCellRow::HtmlCellRow(TString name, Int_t colspan) : HtmlCell(name) { SetRowSpan(colspan); }

  HtmlCellRow::~HtmlCellRow() {}

  namespace HtmlTableRowClass {
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
  }  // namespace HtmlTableRowClass

  void HtmlRow::AddSimpleCells(std::initializer_list<TString> cels) {
    for (auto str : cels) {
      this->AddContent(HtmlCell(str));
    }
  }

}  // namespace Hal
