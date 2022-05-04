/*
 * HalHtmlTable.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_HTML_TABLE_H_
#define HAL_FEATURES_IO_HTML_TABLE_H_
#include "HalHtmlObject.h"

namespace HalHtmlTableRowStyles {
  TString LightBlue();
  TString MedBlue();
  TString DarkBlue();
  TString LightGreen();
  TString Green();  // doesn't exist yet
  TString DarkGreen();
  TString Yellow();
  TString Grey();
  TString Red();
  TString Violet();

  TString DefStyle();
  TString LegendStyle();
  TString TitleStyle();
  TString ExtraStyle();
  TString TaskStyle();
  TString ExpandableStyle();
  TString SummaryStyle();
};  // namespace HalHtmlTableRowStyles

class HalHtmlTable : public HalHtmlObject {
public:
  HalHtmlTable();
  HalHtmlTable(TString id, TString className, TString style);
  void SetDefaultClass() { SetClass("haltable"); };
  TString ToString() const;
  virtual void AddContent(const HalHtmlObject& obj);
  virtual ~HalHtmlTable();
  ClassDef(HalHtmlTable, 1)
};

class HalHtmlRow : public HalHtmlObject {
public:
  HalHtmlRow();
  HalHtmlRow(TString id, TString className, TString style);
  virtual void AddContent(const HalHtmlObject& obj);
  TString ToString() const;
  virtual ~HalHtmlRow();
  ClassDef(HalHtmlRow, 1)
};

class HalHtmlCell : public HalHtmlObject {
protected:
  Int_t fColSpan;
  Int_t fRowSpan;

public:
  HalHtmlCell();
  HalHtmlCell(TString id, TString className, TString style);
  HalHtmlCell(TString content);
  void SetColSpan(Int_t span) { fColSpan = span; };
  void SetRowSpan(Int_t span) { fRowSpan = span; };
  virtual void AddContent(const HalHtmlObject& obj);
  TString ToString() const;
  virtual ~HalHtmlCell();
  ClassDef(HalHtmlCell, 1)
};

class HalHtmlCellCol : public HalHtmlCell {
public:
  HalHtmlCellCol();
  HalHtmlCellCol(TString name, Int_t colspan);
  virtual ~HalHtmlCellCol();
  ClassDef(HalHtmlCellCol, 1)
};

class HalHtmlCellRow : public HalHtmlCell {
public:
  HalHtmlCellRow();
  HalHtmlCellRow(TString name, Int_t colspan);
  virtual ~HalHtmlCellRow();
  ClassDef(HalHtmlCellRow, 1)
};

#endif /* HAL_FEATURES_IO_HTML_TABLE_H_ */
