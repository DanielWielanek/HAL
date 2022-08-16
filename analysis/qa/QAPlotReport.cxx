/*
 * QAPlotReport.cxx
 *
 *  Created on: 12 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "QAPlotReport.h"

#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TRegexp.h>
#include <TSystem.h>
#include <iostream>
#include <vector>

#include "Cout.h"
#include "HtmlCore.h"
#include "HtmlDiv.h"
#include "HtmlFile.h"
#include "HtmlTable.h"
#include "StdString.h"
namespace Hal {
  QAPlotReport::QAPlotReport() :
    fExtraInfo(nullptr), fOriginName("OriginName", ""), f1dHistos(nullptr), f2dHistos(nullptr), f3dHistos(nullptr) {}

  QAPlotReport::~QAPlotReport() {
    if (f1dHistos) delete f1dHistos;
    if (f2dHistos) delete f2dHistos;
    if (f3dHistos) delete f3dHistos;
    if (fExtraInfo) delete fExtraInfo;
  }

  QAPlotReport::QAPlotReport(const QAPlotReport& other) :
    Object(other),
    f1dFlags(other.f1dFlags),
    f2dFlags(other.f2dFlags),
    f3dFlags(other.f3dFlags),
    fExtraInfo(nullptr),
    f1dHistos(nullptr),
    f2dHistos(nullptr),
    f3dHistos(nullptr) {
    fOriginName = other.fOriginName;
    if (other.f1dHistos) f1dHistos = new HistogramManager_1_1D<TH1D>(*other.f1dHistos);
    if (other.f2dHistos) f2dHistos = new HistogramManager_1_2D<TH2D>(*other.f2dHistos);
    if (other.f3dHistos) f3dHistos = new HistogramManager_1_3D<TH3D>(*other.f3dHistos);
    if (other.fExtraInfo) { fExtraInfo = (Object*) other.fExtraInfo->Clone(); }
  }

  QAPlotReport& QAPlotReport::operator=(const QAPlotReport& other) {
    if (this == &other) return *this;
    if (f1dHistos) {
      delete f1dHistos;
      f1dHistos = nullptr;
    }
    if (f2dHistos) {
      delete f2dHistos;
      f2dHistos = nullptr;
    }
    if (f3dHistos) {
      delete f3dHistos;
      f3dHistos = nullptr;
    }
    if (fExtraInfo) {
      delete fExtraInfo;
      fExtraInfo = nullptr;
    }

    Object::operator=(other);
    fOriginName     = other.fOriginName;
    if (other.f1dHistos) f1dHistos = new HistogramManager_1_1D<TH1D>(*other.f1dHistos);
    if (other.f2dHistos) f2dHistos = new HistogramManager_1_2D<TH2D>(*other.f2dHistos);
    if (other.f3dHistos) f3dHistos = new HistogramManager_1_3D<TH3D>(*other.f3dHistos);
    f1dFlags = other.f1dFlags;
    f2dFlags = other.f2dFlags;
    f3dFlags = other.f3dFlags;
    if (other.fExtraInfo) { fExtraInfo = (Object*) other.fExtraInfo->Clone(); }
    return *this;
  }

  void QAPlotReport::TranslateFlag(TString flag, Int_t& hist1, Int_t& hist2, Char_t& op) const {
    if (flag.Contains("/")) { op = '/'; }
    if (flag.Contains("+")) { op = '+'; }
    if (flag.Contains("-")) { op = '-'; }
    std::vector<TString> arr = Hal::Std::ExplodeString(flag, op);
    hist1                    = arr[0].Atoi();
    hist2                    = arr[1].Atoi();
  }

  void QAPlotReport::Operate(TH1* h0, TH1* h1, TH1* h2, Char_t op) const {
    TString name  = h0->GetName();
    TString title = h0->GetTitle();
    TString xAxis = h0->GetXaxis()->GetTitle();
    TString yAxis = h0->GetYaxis()->GetTitle();
    TString zAxis = h0->GetXaxis()->GetTitle();
    //  h0->Reset();
    h1->Copy(*h0);
    /** overwrite title if old title present **/
    if (xAxis.Length() > 0) h0->GetXaxis()->SetTitle(xAxis);
    if (yAxis.Length() > 0) h0->GetYaxis()->SetTitle(yAxis);
    if (zAxis.Length() > 0) h0->GetZaxis()->SetTitle(zAxis);
    h0->SetTitle(title);
    h0->SetName(name);
    switch (op) {
      case '-': {
        h0->Add(h2, -1);
      } break;
      case '+': {
        h0->Add(h2);
      } break;
      case '/': {
        h0->Divide(h2);
      } break;
      default: break;
    }
  }

  void QAPlotReport::Add(const Object* pack) {
    QAPlotReport* qa = (QAPlotReport*) pack;
    if (fExtraInfo) fExtraInfo->Add(qa->fExtraInfo);
    for (int i = 0; i < GetSize1D(); i++) {
      f1dHistos->At(i)->Add(qa->f1dHistos->At(i));
    }
    for (int i = 0; i < GetSize2D(); i++) {
      f2dHistos->At(i)->Add(qa->f2dHistos->At(i));
    }
    for (int i = 0; i < GetSize3D(); i++) {
      f3dHistos->At(i)->Add(qa->f3dHistos->At(i));
    }
    Recalculate();
  }

  Long64_t QAPlotReport::Merge(TCollection* collection) {
    Object::Merge(collection);
    return 1;
  }

  void QAPlotReport::Recalculate() {
    Int_t h1, h2;
    Char_t oper;
    for (int i = 0; i < GetSize1D(); i++) {
      if (f1dFlags[i].Length() < 3) continue;
      TranslateFlag(f1dFlags[i], h1, h2, oper);
      TH1* H0 = f1dHistos->At(i);
      TH1* H1 = f1dHistos->At(h1);
      TH1* H2 = f1dHistos->At(h2);
      Operate(H0, H1, H2, oper);
    }
    for (int i = 0; i < GetSize2D(); i++) {
      if (f2dFlags[i].Length() < 3) continue;
      TranslateFlag(f2dFlags[i], h1, h2, oper);
      TH1* H0 = f2dHistos->At(i);
      TH1* H1 = f2dHistos->At(h1);
      TH1* H2 = f2dHistos->At(h2);
      Operate(H0, H1, H2, oper);
    }
    for (int i = 0; i < GetSize3D(); i++) {
      if (f3dFlags[i].Length() < 3) continue;
      TranslateFlag(f3dFlags[i], h1, h2, oper);
      TH1* H0 = f3dHistos->At(i);
      TH1* H1 = f3dHistos->At(h1);
      TH1* H2 = f3dHistos->At(h2);
      Operate(H0, H1, H2, oper);
    }
  }

  QAPlotReport::QAPlotReport(TString name, Int_t oneDim, Int_t twoDim, Int_t threeDim) :
    Object(), fExtraInfo(nullptr), fOriginName("OriginName", name), f1dHistos(nullptr), f2dHistos(nullptr), f3dHistos(nullptr) {
    SetName(name);
    f1dFlags.resize(oneDim);
    f2dFlags.resize(twoDim);
    f3dFlags.resize(threeDim);

    if (f1dFlags.size() > 0) { f1dHistos = new HistogramManager_1_1D<TH1D>(); }
    if (f2dFlags.size() > 0) { f2dHistos = new HistogramManager_1_2D<TH2D>(); }
    if (f3dFlags.size() > 0) { f3dHistos = new HistogramManager_1_3D<TH3D>(); }
  }

  void QAPlotReport::SetExtraInfo(Object* p) {
    if (fExtraInfo) delete fExtraInfo;
    fExtraInfo = p;
  }

  void QAPlotReport::SetFlag1D(Int_t no, Char_t c, Int_t h1, Int_t h2) {
    if (CheckFlag(no, c, h1, h2, 1)) f1dFlags[no] = Form("%i%c%i", h1, c, h2);
  }

  void QAPlotReport::SetFlag2D(Int_t no, Char_t c, Int_t h1, Int_t h2) {
    if (CheckFlag(no, c, h1, h2, 2)) f2dFlags[no] = Form("%i%c%i", h1, c, h2);
  }

  void QAPlotReport::SetFlag3D(Int_t no, Char_t c, Int_t h1, Int_t h2) {
    if (CheckFlag(no, c, h1, h2, 3)) f3dFlags[no] = Form("%i%c%i", h1, c, h2);
  }

  Bool_t QAPlotReport::CheckFlag(Int_t no, Char_t c, Int_t h1, Int_t h2, Int_t dim) const {
    Int_t size = 0;
    switch (dim) {
      case 1: size = GetSize1D(); break;
      case 2: size = GetSize2D(); break;
      case 3: size = GetSize3D(); break;
    }
    if (no >= size) return kFALSE;
    if (no < 0) return kFALSE;
    if (h1 >= size) return kFALSE;
    if (h1 < 0) return kFALSE;
    if (h2 >= size) return kFALSE;
    if (h2 < 0) return kFALSE;
    if (h1 == h2) return kFALSE;
    if (no == h1) return kFALSE;
    if (no == h2) return kFALSE;
    if (c != '/' && c != '+' && c != '-') return kFALSE;
    return kTRUE;
  }

  TString QAPlotReport::ExportHistogramToFile(TString path, Int_t count, TH1* h) const {
    return ExportHistogramToFile(path, "", count, h);
  }

  TString QAPlotReport::ExportHistogramToFile(TString path, TString dir, Int_t count, TH1* h) const {
    Int_t dim        = 1;
    TString draw_opt = "";
    if (h->InheritsFrom("TH2")) {
      dim      = 2;
      draw_opt = "colz";
    } else if (h->InheritsFrom("TH3")) {
      dim = 3;
    }
    HtmlCore::FixAddress(dir);
    HtmlCore::FixAddress(path);
    TString rootPathFull  = Form("%shist_%id_%i.root", path.Data(), dim, count);
    TString rootPathShort = Form("hist_%id_%i.root", dim, count);
    TString htmlPathFull  = Form("%shist_%id_%i.html", path.Data(), dim, count);
    TString htmlPathShort = dir + Form("hist_%id_%i.html", dim, count);

    TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
    h->DrawClone(draw_opt);
    c1->SaveAs(rootPathFull);
    delete c1;
    HtmlFile file(htmlPathFull, kFALSE);
    file.AddStringContent(HtmlCore::GetJsDiv(rootPathShort, "canvas;1", draw_opt));
    file.Save();
    return HtmlCore::GetUrl(htmlPathShort, h->GetTitle());
  }

  TString QAPlotReport::HTMLExtract(Int_t no, TString dir) const {
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    TString path = Form("%s/qa_plot_%i/", dir.Data(), no);
    HtmlCore::FixAddress(path);
    gSystem->mkdir(path);
    TString filename = Form("%sqa_plot.html", path.Data());
    TString filePath = Form("qa_plot_%i", no);
    HtmlFile file(filename);

    HtmlDiv div;

    Int_t max = TMath::Max(GetSize1D(), TMath::Max(GetSize2D(), GetSize3D()));
    HtmlTable table1("table_1d", "haltable", "");

    HtmlRow rowParent("", "green_", "");
    rowParent.AddContent(HtmlCell("Origin"));
    rowParent.AddContent(HtmlCellCol(fOriginName.GetValue(), 3));
    HtmlRow extra("", "green_", "");
    extra.AddContent(HtmlCellCol("Extra pack", 2));
    if (fExtraInfo == nullptr) {
      extra.AddContent(HtmlCellCol("Null", 2));
    } else {
      TString url = fExtraInfo->HTMLExtract(0, Form("%s/qa_plot_%i/", path.Data(), no));
      extra.AddContent(HtmlCellCol(url, 2));
    }

    table1.AddContent(rowParent);
    table1.AddContent(extra);
    HtmlRow rowLeg1("", "dark_blue", "");
    rowLeg1.AddContent(HtmlCell("No"));
    rowLeg1.AddContent(HtmlCell("1D histograms"));
    rowLeg1.AddContent(HtmlCell("2D histograms"));
    rowLeg1.AddContent(HtmlCell("3D histograms"));
    table1.AddContent(rowLeg1);
    for (int i = 0; i < max; i++) {
      HtmlRow rowEnt("", "light_blue", "");
      rowEnt.AddContent(HtmlCell(Form("%i", i)));
      TString cell1 = "", cell2 = "", cell3;
      if (i < GetSize1D()) { cell1 = ExportHistogramToFile(path, i, f1dHistos->At(i)); }
      if (i < GetSize2D()) { cell2 = ExportHistogramToFile(path, i, f2dHistos->At(i)); }
      if (i < GetSize3D()) { cell3 = ExportHistogramToFile(path, i, f3dHistos->At(i)); }
      rowEnt.AddContent(HtmlCell(cell1));
      rowEnt.AddContent(HtmlCell(cell2));
      rowEnt.AddContent(HtmlCell(cell3));
      table1.AddContent(rowEnt);
    }

    div.AddContent(table1);
    file.AddContent(div);
    file.Save();
    gROOT->SetBatch(batch);
    TString relPath = Form("qa_plot_%i/qa_plot.html", no);
    return HtmlCore::GetUrl(relPath, Form("QAPlot [%s]", GetName()));
  }

  void QAPlotReport::HTMLExtractIntoTable(Int_t no, HtmlTable& table, TString dir, TString rel_dir) const {
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    TString path   = Form("%s/qa_plot_%i/", dir.Data(), no);
    TString subdir = Form("qa_plot_%i", no);
    if (rel_dir.Length() > 0) subdir = Form("%s/qa_plot_%i", rel_dir.Data(), no);
    HtmlCore::FixAddress(path);
    HtmlCore::FixAddress(subdir);
    gSystem->mkdir(path);

    // Int_t max = TMath::Max(GetSize1D(), TMath::Max(GetSize2D(), GetSize3D()));

    TString className = Form("med_blue qa_%i", no);

    HtmlRow row;
    row.SetClass("light_blue");
    HtmlCell cell1(Form("%i", no));
    cell1.SetColSpan(2);
    row.AddContent(cell1);
    row.AddContent(HtmlCell("QAPlotReport"));
    row.AddContent(HtmlCell(this->GetName()));
    TString rowButton = HtmlCore::GetHideButtonRow(Form("qa_%i", no), "Show/Hide");
    row.AddContent(HtmlCell(rowButton));
    table.AddContent(row);
    HtmlRow rowPaint("", className, "display:none");
    TString buttonDraw = Form("<button onclick=\"qaPopup('%i')\">Plot Many</button>", no);
    rowPaint.AddContent(HtmlCellCol(buttonDraw, 5));
    table.AddContent(rowPaint);
    gSystem->mkdir(path);
    //  if (path_url != path_data) list_dir2 = Form("%s/list_%i/", path_url.Data(), no);
    struct constRowData {
      TString className;
      TString subDir;
      TString path;
      HtmlTable* table;
      QAPlotReport* rep;
      Int_t no;
      Int_t dim;
    };
    constRowData data;
    data.className = className;
    data.subDir    = subdir;
    data.path      = path;
    data.no        = no;
    data.table     = &table;

    auto makeRow = [](Int_t nu, TH1* h, constRowData dat) {
      HtmlRow rowElement2("", dat.className, "display:none");
      TString url      = dat.rep->ExportHistogramToFile(dat.path, dat.subDir, nu, h);
      TString boxname  = Form("cb_%i_%i_%i", dat.no, dat.dim, nu);
      TString urlBox   = Form("<input type=\"checkbox\" name = \"%s\" >", boxname.Data());
      HtmlCellCol cell = HtmlCellCol(urlBox, 1);
      cell.AddAtrib("onclick", Form("chageCheckBoxCell('%s')", boxname.Data()));
      rowElement2.AddContent(cell);
      rowElement2.AddContent(HtmlCellCol(Form("%i", nu), 1));
      rowElement2.AddContent(HtmlCellCol(url, 3));
      dat.table->AddContent(rowElement2);
    };
    HtmlRow rowElement("", className, "display:none");
    rowElement.AddContent(HtmlCellCol(" ", 1));
    if (GetSize1D() > 0) {
      data.dim   = 1;
      HtmlRow rE = rowElement;
      rE.AddContent(HtmlCellCol("Histograms 1D", 4));
      table.AddContent(rE);
      for (int i = 0; i < GetSize1D(); i++) {
        makeRow(i, f1dHistos->At(i), data);
      }
    }
    if (GetSize2D() > 0) {
      data.dim   = 2;
      HtmlRow rE = rowElement;
      rE.AddContent(HtmlCellCol("Histograms 2D", 4));
      table.AddContent(rE);
      for (int i = 0; i < GetSize2D(); i++) {
        makeRow(i, f2dHistos->At(i), data);
      }
    }
    if (GetSize3D() > 0) {
      data.dim   = 3;
      HtmlRow rE = rowElement;
      rE.AddContent(HtmlCellCol("Histograms 3D", 4));
      table.AddContent(rE);
      for (int i = 0; i < GetSize3D(); i++) {
        makeRow(i, f3dHistos->At(i), data);
      }
    }
    /*
      HtmlRow rowParent("", className, "display:none");
      rowParent.AddContent(HtmlCell("Origin"));
      rowParent.AddContent(HtmlCellCol(fOriginName.GetValue(), 4));
      table.AddContent(rowParent);
      */

    if (fExtraInfo == nullptr) {
      /*
      HtmlRow extra("", className, "display:none");
      extra.AddContent(HtmlCellCol("Extra pack", 3));
    extra.AddContent(HtmlCellCol("Null", 2));
      table.AddContent(extra);

    */
    } else {
      HtmlRow extra("", className, "display:none");
      extra.AddContent(HtmlCellCol("Extra pack", 3));
      TString url = fExtraInfo->HTMLExtract(0, Form("%s/qa_plot_%i/", path.Data(), no));
      extra.AddContent(HtmlCellCol(url, 4));
      table.AddContent(extra);
    }


    gROOT->SetBatch(batch);
  }

  void QAPlotReport::Browse(TBrowser* b) {
    b->Add(&fOriginName);
    for (int i = 0; i < GetSize1D(); i++) {
      b->Add(f1dHistos->At(i));
    }
    for (int i = 0; i < GetSize2D(); i++) {
      b->Add(f2dHistos->At(i));
    }
    for (int i = 0; i < GetSize3D(); i++) {
      b->Add(f3dHistos->At(i));
    }
  }

  void QAPlotReport::Print(Option_t* /*option*/) const {
    Cout::Database(3, "ID", "Name", "Flags");
    Cout::InStars("1D", kWhite);
    for (int i = 0; i < GetSize1D(); i++) {
      TString name = f1dHistos->At(i)->GetName();
      TString flag = f1dFlags[i];
      Cout::Database(3, Form("%i", i), name.Data(), flag.Data());
    }
    Cout::InStars("2D", kWhite);
    for (int i = 0; i < GetSize2D(); i++) {
      TString name = f2dHistos->At(i)->GetName();
      TString flag = f2dFlags[i];
      Cout::Database(3, Form("%i", i), name.Data(), flag.Data());
    }
    Cout::InStars("3D", kWhite);
    for (int i = 0; i < GetSize3D(); i++) {
      TString name = f3dHistos->At(i)->GetName();
      TString flag = f3dFlags[i];
      Cout::Database(3, Form("%i", i), name.Data(), flag.Data());
    }
  }

  void QAPlotReport::SetFlag(Int_t no, TString flag, Int_t dim) {
    switch (dim) {
      case 1: f1dFlags[no] = flag; break;
      case 2: f2dFlags[no] = flag; break;
      case 3: f3dFlags[no] = flag; break;
    }
  }
}  // namespace Hal
