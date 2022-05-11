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
#include "HtmlObject.h"
namespace Hal {
  namespace HtmlTableRowStyles {
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
  };  // namespace HtmlTableRowStyles

  class HtmlTable : public HtmlObject {
  public:
    HtmlTable();
    HtmlTable(TString id, TString className, TString style);
    void SetDefaultClass() { SetClass("haltable"); };
    TString ToString() const;
    virtual void AddContent(const HtmlObject& obj);
    virtual ~HtmlTable();
    ClassDef(HtmlTable, 1)
  };

  class HtmlRow : public HtmlObject {
  public:
    HtmlRow();
    HtmlRow(TString id, TString className, TString style);
    virtual void AddContent(const HtmlObject& obj);
    TString ToString() const;
    virtual ~HtmlRow();
    ClassDef(HtmlRow, 1)
  };

  class HtmlCell : public HtmlObject {
  protected:
    Int_t fColSpan;
    Int_t fRowSpan;

  public:
    HtmlCell();
    HtmlCell(TString id, TString className, TString style);
    HtmlCell(TString content);
    void SetColSpan(Int_t span) { fColSpan = span; };
    void SetRowSpan(Int_t span) { fRowSpan = span; };
    virtual void AddContent(const HtmlObject& obj);
    TString ToString() const;
    virtual ~HtmlCell();
    ClassDef(HtmlCell, 1)
  };

  class HtmlCellCol : public HtmlCell {
  public:
    HtmlCellCol();
    HtmlCellCol(TString name, Int_t colspan);
    virtual ~HtmlCellCol();
    ClassDef(HtmlCellCol, 1)
  };

  class HtmlCellRow : public HtmlCell {
  public:
    HtmlCellRow();
    HtmlCellRow(TString name, Int_t colspan);
    virtual ~HtmlCellRow();
    ClassDef(HtmlCellRow, 1)
  };
}  // namespace Hal
#endif /* HAL_FEATURES_IO_HTML_TABLE_H_ */
