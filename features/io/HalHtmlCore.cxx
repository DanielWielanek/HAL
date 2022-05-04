#include "HalHtmlCore.h"
#include "HalHtmlFile.h"
#include "HalHtmlTable.h"
#include "HalObject.h"
#include "HalPackage.h"
#include "HalPackage2HTML.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TList.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TVector2.h>
#include <iostream>
#include <vector>

#include "HalCout.h"
#include "HalStd.h"
#include "HalStdString.h"

HalHtmlCore* HalHtmlCore::fgInstance = NULL;
TString HalHtmlCore::fMainDir        = "";
Bool_t HalHtmlCore::fgOnline         = kFALSE;

void HalHtmlCore::CopyCss(TString path, TString css_name) {
  TString workdir    = HalStd::GetHalrootPlus();
  TString filetocopy = Form("%s/html/%s", workdir.Data(), css_name.Data());
  TString outPath    = Form("%s/table.css", path.Data());
  if (path.Length() == 0) outPath = "table.css";  // to copy into this dir
  gSystem->CopyFile(filetocopy, outPath, kTRUE);
}

void HalHtmlCore::CopyJs(TString path) {
  TString workdir    = HalStd::GetHalrootPlus();
  TString filetocopy = Form("%s/html/hal_js.js", workdir.Data());
  if (path.Length() != 0) {
    gSystem->CopyFile(filetocopy, Form("%s/hal_js.js", path.Data()), kTRUE);
  } else {
    gSystem->CopyFile(filetocopy, "hal_js.js", kTRUE);
  }

  if (fgOnline) return;
  filetocopy = HalStd::GetJsRoot();
  if (path.Length() != 0) {
    gSystem->mkdir(Form("%s/hal_js", path.Data()));
    gSystem->mkdir(Form("%s/hal_js/img", path.Data()));
    gSystem->mkdir(Form("%s/hal_js/scripts", path.Data()));
    gSystem->mkdir(Form("%s/hal_js/style", path.Data()));
    HalStd::CopyFiles(Form("%s/img", filetocopy.Data()), Form("%s/hal_js/img", path.Data()), kFALSE);
    HalStd::CopyFiles(Form("%s/scripts", filetocopy.Data()), Form("%s/hal_js/scripts", path.Data()), kFALSE);
    HalStd::CopyFiles(Form("%s/style", filetocopy.Data()), Form("%s/hal_js/style", path.Data()), kFALSE);
  } else {
    gSystem->mkdir("hal_js");
    gSystem->mkdir("hal_js/img");
    gSystem->mkdir("hal_js/scripts");
    gSystem->mkdir("hal_js/style");
    HalStd::CopyFiles(Form("%s/img", filetocopy.Data()), "hal_js/img", kFALSE);
    HalStd::CopyFiles(Form("%s/scripts", filetocopy.Data()), "hal_js/scripts", kFALSE);
    HalStd::CopyFiles(Form("%s/style", filetocopy.Data()), "hal_js/style", kFALSE);
  }
}

HalHtmlCore::HalHtmlCore() {
  if (fgInstance != NULL) {
    HalCout::PrintInfo("Overwriting HalHTML !", Hal::EInfo::kLessError);
    return;
  } else {
    fMainDir   = "";
    fgInstance = this;
  }
}

HalHtmlCore* HalHtmlCore::Instance() {
  if (fgInstance != NULL) {
  } else {
    fgInstance = new HalHtmlCore();
  }
  return fgInstance;
}

void HalHtmlCore::SetMainDir(TString name) {
  if (name.Length() == 0) { HalCout::PrintInfo("SetMainDir as null", Hal::EInfo::kLessWarning); }
  fMainDir = name;
  FixAddress(fMainDir);
}

void HalHtmlCore::SetMainDirAndExtract(TString name) {
  SetMainDir(name);
  CopyJs(name);
  CopyCss(name);
}

void HalHtmlCore::FixAddress(TString& address) {
  address.ReplaceAll("//", "/");
  if (address.EndsWith(".js") || address.EndsWith(".html") || address.EndsWith(".htm")) return;
  if (address.Length() > 0) {
    if (!address.EndsWith("/")) { address = address + "/"; }
  }
}

void HalHtmlCore::HTMLExtractIntoTable(TObject* obj, Int_t no, HalHtmlTable& table, TString dir, TString rel_dir) {
  /**TODO
if (dynamic_cast<TList*>(obj)) {
  TList* list = static_cast<TList*>(obj);
  HalHtmlRow row;
  row.SetClass(HalHtmlTableRowStyles::DefStyle());
  HalHtmlCell cell1(Form("%i", no));
  cell1.SetColSpan(2);
  row.AddContent(cell1);
  row.AddContent(HalHtmlCell("TList"));
  row.AddContent(HalHtmlCell(list->GetName()));
  TString rowButton = HalHtmlCore::GetHideButtonRow(Form("list_%i", no), "Show/Hide");
  row.AddContent(HalHtmlCell(rowButton));
  table.AddContent(row);
  TString list_path = Form("%s/list_%i", dir.Data(), no);
  gSystem->mkdir(list_path);
  // TString list_dir = Form("list_%i/", no);
  //   TString list_dir2 = Form("list_%i/", no);
  auto addToUrl = [](TString add_dir, TString url) {
    if (!url.Contains("href")) { return url; }
    Int_t start_url = url.First("=") + 2;
    TString first   = url(0, start_url);
    TString end     = url(start_url, url.Length());
    TString output  = Form("%s%s%s", first.Data(), add_dir.Data(), end.Data());
    output.ReplaceAll("//", "/");
    return output;
  };
  //  list_dir2 = Form("%s/list_%i/", path_url.Data(), no);
  for (int i = 0; i < list->GetEntries(); i++) {
    TObject* obj         = list->At(i);
    TString name         = obj->GetName();
    TString classname    = obj->ClassName();
    TString temp_classes = Form("%s list_%i", HalHtmlTableRowStyles::DefStyle().Data(), no);
    HalHtmlRow rowElement("", temp_classes, "display:none");
    rowElement.AddContent(HalHtmlCell());
    rowElement.AddContent(HalHtmlCell(Form("%i", i)));
    rowElement.AddContent(HalHtmlCell(obj->ClassName()));
    if (classname == "HalPackage") {
      classname = Form("HalPackage  [%s] ",
                       ((HalPackage*) obj)->GetName());  //</br>
      rowElement.AddContent(HalHtmlCell(classname));
    } else {
      rowElement.AddContent(HalHtmlCell(obj->GetName()));
    }
    rowElement.AddContent(HalHtmlCell(addToUrl(rel_dir, HalHtmlCore::HTMLExtract(obj, i, list_path))));
    table.AddContent(rowElement);
  }
}
**/
  return;
};

Bool_t HalHtmlCore::CanBeTableElement(TObject* obj) { return kFALSE; }

TString HalHtmlCore::GetUrl(TString adress, TString text) { return Form("<a href=\"%s\">%s</a>", adress.Data(), text.Data()); }

TString HalHtmlCore::ClickablePic(TString id_name, TObjArray* strings, Int_t width, Int_t height) {
  TString url = "<img border=\"0\" ";
  if (width > 0) { url = url + Form("width=\"%i\" ", width); }
  if (height > 0) { url = url + Form("height=\"%i\" ", height); }
  for (int i = 0; i < strings->GetEntries(); i++) {
    TObjString* current_name = (TObjString*) strings->UncheckedAt(i);
    TString capt             = current_name->String();
    if (i == 0) {
      url = url + Form("src=\"%s\" ", capt.Data());
      url = url + Form("id=\"%s\" onclick=\"changeImage('%s',['%s'", id_name.Data(), id_name.Data(), capt.Data());
    } else {
      url = url + Form(",'%s'", capt.Data());
    }
  }
  url = url + "])\" />";
  return url;
}

TString HalHtmlCore::GetRelativePath(TString path) {
  FixAddress(path);
  if (path.EqualTo(fMainDir)) { return ""; }
  TString relative_path = "";
  TString temp          = path(fMainDir.Length(), path.Length() - fMainDir.Length());

  if (temp.EndsWith("/")) { temp.Remove(temp.Length() - 1); }

  std::vector<TString> vec = HalStd::ExplodeString(temp, '/');

  if (vec.size() == 0) return "";
  for (unsigned int i = 0; i < vec.size() - 1; i++) {
    relative_path = relative_path + "../";
  }
  if (temp.Length() != 0) { relative_path = relative_path + "../"; }
  return relative_path;
}

TString HalHtmlCore::GetLastDir(TString name, Int_t i) {
  if (name.EndsWith("/")) name = name(0, name.Length() - 1);
  Int_t cut_pos = -1;
  Int_t count   = 0;
  for (int j = name.Length() - 1; j >= 0; j--) {
    if (name[j] == '/') {
      count++;
      cut_pos = j;
    }
    if (count == i) break;
  }
  name = name(cut_pos + 1, name.Length());
  if (name.Length() > 0) name = name + "/";
  return name;
}

TString HalHtmlCore::GetJsDiv(TString root_file, TString object_name, TString draw_opt, TString draw_div_name) {
  TString res;
  res = "<script type='text/javascript'>\n";
  res = res + Form("var filename = \"%s\";\n", root_file.Data());
  res = res + " JSROOT.OpenFile(filename, function(file) {\n";
  res = res + " JSROOT.gStyle.Palette= 50;\n";
  res = res + Form("file.ReadObject(\"%s\", function(obj) {\n", object_name.Data());
  res = res + Form("JSROOT.draw(\"%s\", obj, \"%s\");\n;", draw_div_name.Data(), draw_opt.Data());
  res = res + "});\n;});\n";
  res = res + "</script>\n<div id=\"" + draw_div_name + "\" style=\"width:90%;height:900px\"></div>\n";
  return res;
}

TString HalHtmlCore::GetHideButtonTable(TString tableName, TString text) {
  return Form("<button onclick=\"setTable('%s')\">%s</button>", tableName.Data(), text.Data());
}

TString HalHtmlCore::GetHideButtonRow(TString listName, TString text) {
  return Form("<button onclick=\"setRow('%s')\">%s</button>", listName.Data(), text.Data());
}

TString HalHtmlCore::GetLinkToHistogram(TH1* h, Int_t no, TString path) {
  TString drawOpt = "";
  if (h->InheritsFrom("TH3")) {
    drawOpt = "box1";
  } else if (h->InheritsFrom("TH2")) {
    drawOpt = "colz";
  }
  Bool_t batch = gROOT->IsBatch();
  gROOT->SetBatch(kTRUE);
  path = Form("%s/th_%i", path.Data(), no);
  gSystem->mkdir(path);
  TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 800, 600);
  TH1* histo  = (TH1*) h;
  histo->Draw(drawOpt);
  c1->SaveAs(Form("%s/histo.root", path.Data()));
  delete c1;
  gROOT->SetBatch(batch);
  HalHtmlFile file(Form("%s/histo.html", path.Data()), kFALSE);
  file.AddStringContent(HalHtmlCore::GetJsDiv("histo.root", "canvas;1"));
  file.Save();
  TString filename = Form("%s/histo.html", path.Data());
  gROOT->SetBatch(batch);
  return HalHtmlCore::GetUrl(Form("th_%i/histo.html", no), h->ClassName());
}

TString HalHtmlCore::HTMLExtract(TObject* obj, Int_t no, TString dir) {
  TString className = obj->ClassName();
  if (className.EqualTo("TObjString")) { return static_cast<TObjString*>(obj)->GetString(); }
  if (className.EqualTo("TVector2")) { return GetLinkVector(static_cast<TVector2*>(obj)); }
  if (className.EqualTo("HalPackage")) {
    HalPackage2HTML* pack = new HalPackage2HTML(static_cast<HalPackage*>(obj), Form("%s/pack_%i", dir.Data(), no), kFALSE);
    delete pack;
    return HalHtmlCore::GetUrl(Form("pack_%i/index.html", no), "HalPackage");
  }
  if (obj->InheritsFrom("HalPack")) { return static_cast<HalObject*>(obj)->HTMLExtract(no, dir); }
  if (obj->InheritsFrom("TList")) { return GetLinkList(static_cast<TList*>(obj), no, dir); };
  if (obj->InheritsFrom("TH1")) { return GetLinkTH1(static_cast<TH1*>(obj), no, dir); }
  if (obj->InheritsFrom("TGraph")) { return GetLinkGraph(static_cast<TGraph*>(obj), no, dir); };
  return "";
}

TString HalHtmlCore::GetLinkVector(TVector2* vect) {
  TString stringA = HalStd::RoundToString(vect->Px(), 1);
  TString stringB = HalStd::RoundToString(vect->Py(), 1);
  stringA         = stringA + " ";
  return stringA + stringB;
}

TString HalHtmlCore::GetLinkTH1(TH1* h1, Int_t no, TString path) {
  while (path.Contains("//")) {
    path.ReplaceAll("//", "/");
  }
  TString rootFileName = "histo1d.root";
  TString opt          = "";
  TString pattern      = "th1";
  if (h1->InheritsFrom("TH2")) {
    rootFileName = "histo2d.root";
    opt          = "colz";
    pattern      = "th2";
  }
  if (h1->InheritsFrom("TH3")) {
    rootFileName = "histo2d.root";
    opt          = "box2";
    pattern      = "th3";
  }
  Bool_t batch = gROOT->IsBatch();
  gROOT->SetBatch(kTRUE);
  path = Form("%s/%s_%i", path.Data(), pattern.Data(), no);
  gSystem->mkdir(path);
  TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 800, 600);
  TH1* histo  = (TH1*) h1;
  histo->Draw(opt);
  c1->SaveAs(Form("%s/%s", path.Data(), rootFileName.Data()));
  delete c1;
  gROOT->SetBatch(batch);
  HalHtmlFile file(Form("%s/histo.html", path.Data()), kFALSE);
  file.AddStringContent(HalHtmlCore::GetJsDiv(rootFileName, "canvas;1"));
  file.Save();
  TString filename = Form("%s/histo.html", path.Data());
  gROOT->SetBatch(batch);
  return HalHtmlCore::GetUrl(Form("%s_%i/histo.html", pattern.Data(), no), h1->ClassName());
}

TString HalHtmlCore::GetLinkGraph(TGraph* h, Int_t no, TString path) {
  TString classname = h->ClassName();
  Bool_t batch      = gROOT->IsBatch();
  gROOT->SetBatch(kTRUE);
  path = Form("%s/graph_%i", path.Data(), no);
  gSystem->mkdir(path);
  TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 800, 600);
  h->Draw("EP");
  c1->SaveAs(Form("%s/graph.root", path.Data()));
  delete c1;
  HalHtmlFile F(Form("%s/graph.html", path.Data()), kFALSE);
  F.AddStringContent(HalHtmlCore::GetJsDiv("graph.root", "canvas;1", "EP"));
  F.Save();
  gROOT->SetBatch(batch);
  return HalHtmlCore::GetUrl(Form("graph_%i/graph.html", no), h->ClassName());
}

TString HalHtmlCore::GetLinkList(TList* list, Int_t no, TString path) {
  TString filename = Form("%s/list_%i/list.html", path.Data(), no);
  TString linkname = Form("list_%i.html", no);
  HalHtmlFile file(filename, kFALSE);
  file.AddStringContent(Form("<h1>Name %s </h1>", list->GetName()));
  file.AddStringContent("<div  id=\"main_table\">");
  HalHtmlTable table("", "haltable", "");
  HalHtmlRow row("", HalHtmlTableRowStyles::TitleStyle(), "");
  row.AddContent(HalHtmlCell("No"));
  row.AddContent(HalHtmlCell("ClassName"));
  row.AddContent(HalHtmlCell("Value"));
  row.AddContent(HalHtmlCell("Name"));
  table.AddContent(row);
  TString list_dir = Form("list_%i/", no);
  path             = Form("%s/list_%i/", path.Data(), no);
  auto addToUrl    = [](TString add_dir, TString url) {
    if (!url.Contains("href")) { return url; }
    Int_t start_url = url.First("=") + 2;
    TString first   = url(0, start_url);
    TString end     = url(start_url, url.Length());
    TString output  = Form("%s%s%s", first.Data(), add_dir.Data(), end.Data());
    output.ReplaceAll("//", "/");
    return output;
  };
  for (int i = 0; i < list->GetEntries(); i++) {
    TObject* object        = list->At(i);
    TString name           = object->ClassName();
    TString oryginal_class = object->GetName();
    HalHtmlRow row2("", HalHtmlTableRowStyles::DefStyle(), "");
    table.AddContent(HalHtmlCell(HalStd::RoundToString(i)));
    table.AddContent(HalHtmlCell(object->ClassName()));

    table.AddContent(HalHtmlCell(addToUrl(list_dir, HTMLExtract(object, i, path))));
    if (name == "HalPackage") {
      oryginal_class = ((HalPackage*) object)->GetName();
      oryginal_class = Form("%s </br> [%s]", object->ClassName(), oryginal_class.Data());
      table.AddContent(HalHtmlCell(oryginal_class));
    } else {
      table.AddContent(HalHtmlCell(object->GetName()));
    }
    table.AddContent(row2);
  }
  file.AddContent(table);
  file.Save();
  return HalHtmlCore::GetUrl(linkname, list->ClassName());
}
