/*
 * HalPackage2HTML.cxx
 *
 *  Created on: 20-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "html/HalHtmlDiv.h"
#include "html/HalHtmlFile.h"
#include "html/HalHtmlObject.h"
#include "html/HalHtmlTable.h"
#include "HalHtmlCore.h"
#include "HalPackage.h"
#include "HalPackage2HTML.h"
#include <TArc.h>
#include <TAttFill.h>
#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TClass.h>
#include <TCollection.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THStack.h>
#include <TKey.h>
#include <TLatex.h>
#include <TLine.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TPad.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TVector2.h>
#include <TVirtualPad.h>
#include <cstring>
#include <vector>

#include "HalCout.h"
#include "HalParameter.h"
#include "HalStdString.h"
#include <iostream>

HalPackage2HTML::HalPackage2HTML() :
  fHTML(nullptr),
  fDir(""),
  fObjectCounter(0),
  fDynamicTableCounter(0),
  fTaskTableCounter(0),
  fCutContainerPosition(0),
  fPackageID(0),
  fCollectionsNo {0, 0, 0, 0},
  fBasicRadius(40),
  fWindowWidth(800),
  fWIndowHeight(800),
  fCurrentCutContainer(nullptr),
  fListDeep(0),
  fSoftVer(0) {}


HalPackage2HTML::HalPackage2HTML(HalPackage* pack, const TString dir, Bool_t main_dir) : HalPackage2HTML() {
  fDir = dir;
  for (int i = 0; i < 4; i++)
    fCollectionsNo[i] = 0;
  gSystem->MakeDirectory(fDir);
  fHTML = new HalHtmlFile(fDir + "/index.html", main_dir);
  HalHtmlCell cell;
  CreatePackageList(cell, pack, eTableStyle::kStandard, fDir, 1, "");
  fHTML->AddStringContent(cell.GetContent());
  fHTML->Save();
  delete fHTML;
  fHTML = nullptr;
}

HalPackage2HTML::HalPackage2HTML(TString filename, const TString dir) : HalPackage2HTML() {
  fDir = dir;
  for (int i = 0; i < 4; i++)
    fCollectionsNo[i] = 0;
  for (int i = 0; i < 4; i++)
    fCollectionsNo[i] = 0;
  gSystem->MakeDirectory(fDir);
  fHTML = new HalHtmlFile(fDir + "/index.html", kTRUE);
  HalHtmlCell dummyCell;

  TFile* file            = new TFile(filename);
  TDirectory* global_dir = (TDirectory*) file->Get("HalInfo");
  if (global_dir) {
    TString temp_dir             = Form("%s/global_data", fDir.Data());
    HalPackage* pack            = (HalPackage*) file->Get("HalInfo/RunInfo");
    HalParameterString* version = (HalParameterString*) pack->GetObjectByName("Software ver");
    fSoftVer                     = HalStd::VersionId(version->GetValue());
    CreatePackageList(dummyCell, pack, eTableStyle::kMetaData, temp_dir, 2, "drawmerged");
    fHTML->AddStringContent(dummyCell.GetContent());
  }
  if (fSoftVer <= 201612) {
    HalCout::PrintInfo("This file version might be not compatible wtih current version, use "
                        "macro/path/fix_files.C to fix it",
                        Hal::EInfo::kImportantWarning);
  }
  TDirectory* tdir = (TDirectory*) file->Get("HalPhysics");
  //---------------------

  TList* list = tdir->GetListOfKeys();
  for (fPackageID = 0; fPackageID < list->GetEntries(); fPackageID++) {
    TString path       = Form("%s/superpack_%i", fDir.Data(), fPackageID);
    TString key_name   = ((TKey*) (list->At(fPackageID)))->GetName();
    TObject* object    = tdir->Get(key_name);
    TString table_name = Form("task_table_%i", fTaskTableCounter++);
    if (object->InheritsFrom("HalPackage")) {
      HalPackage* package   = (HalPackage*) object;
      HalPackage* meta_data = (HalPackage*) (((HalPackage*) object)->GetObjectByName("HalMetadata"));
      TString analysis_name  = ((HalParameterString*) meta_data->GetObjectByName("Analysis Name"))->GetValue();
      // object inherits form HalPackage - probably result of physical analysis
      HalHtmlDiv div;
      div.SetId(Form("buttontableM_%i", fPackageID));
      HalHtmlTable table;
      table.SetDefaultClass();
      HalHtmlRow firstRow;
      firstRow.SetClass(HalHtmlTableRowStyles::TaskStyle());

      TString button = Form("<button "
                            "onclick=\"setTable('%s')\">Hide/Show Pack %s [Package No. "
                            "%i ]</button>",
                            table_name.Data(),
                            analysis_name.Data(),
                            fPackageID);
      firstRow.AddContent(HalHtmlCell(button));
      table.AddContent(firstRow);
      div.AddContent(table);
      fHTML->AddContent(div);
      HalHtmlDiv div2;
      HalHtmlTable mainTable(table_name, "maintable", "display:none");
      HalHtmlRow rowTable("", HalHtmlTableRowStyles::ExpandableStyle(), "");
      HalHtmlCell yellowCell;
      TryExtractAnaResult(yellowCell, package, path);
      rowTable.AddContent(yellowCell);
      mainTable.AddContent(rowTable);
      div2.AddContent(mainTable);
      fHTML->AddContent(div2);
    } else {
      HalCout::PrintInfo(Form("%s found in Physics but don't inherit from HalPackage", key_name.Data()),
                          Hal::EInfo::kLessWarning);
    }
  }
  // delete list;
  file->Close();
  fHTML->AddStringContent("<script>window.onload=checkTask();</script>\n");
  //----------------------
  fHTML->Save();
  delete fHTML;
  fHTML = nullptr;
  HalHtmlCore::ResetMainDir();
}

HalPackage2HTML::HalPackage2HTML(TString filename, const TString dir, TString packname) : HalPackage2HTML() {
  fDir = dir;
  for (int i = 0; i < 4; i++)
    fCollectionsNo[i] = 0;
  for (int i = 0; i < 4; i++)
    fCollectionsNo[i] = 0;
  fDynamicTableCounter = 0;
  gSystem->MakeDirectory(fDir);
  fHTML            = new HalHtmlFile(fDir + "/index.html", kFALSE);
  TFile* file      = new TFile(filename);
  TDirectory* tdir = (TDirectory*) file->Get("HalPhysics");
  //---------------------
  TDirectory* global_dir = (TDirectory*) file->Get("HalInfo");
  if (global_dir) {
    HalHtmlCell dummyCell;
    TString temp_dir             = Form("%s/global_data", fDir.Data());
    HalPackage* pack            = (HalPackage*) file->Get("HalInfo/RunInfo");
    HalParameterString* version = (HalParameterString*) pack->GetObjectByName("Software ver");
    fSoftVer                     = HalStd::VersionId(version->GetValue());
    CreatePackageList(dummyCell, pack, eTableStyle::kMetaData, temp_dir, 2, "drawmerged");
    fHTML->AddStringContent(dummyCell.GetContent());
  }
  if (fSoftVer <= 201612) {
    HalCout::PrintInfo("This file version might be not compatible wtih current version, use "
                        "macro/path/fix_files.C to fix it",
                        Hal::EInfo::kImportantWarning);
  }
  TObject* object    = tdir->Get(packname);
  TString table_name = Form("task_table_%i", fTaskTableCounter++);
  TString path       = Form("%s/superpack_0/", fDir.Data());
  if (object->InheritsFrom("HalPackage")) {
    HalPackage* package   = (HalPackage*) object;
    HalPackage* meta_data = (HalPackage*) (((HalPackage*) object)->GetObjectByName("HalMetadata"));
    TString analysis_name  = ((HalParameterString*) meta_data->GetObjectByName("Analysis Name"))->GetValue();
    HalHtmlDiv div;
    div.SetId("buttontableM_0");
    HalHtmlTable table;
    table.SetDefaultClass();
    HalHtmlRow firstRow;
    firstRow.SetClass(HalHtmlTableRowStyles::TaskStyle());
    firstRow.AddStringContent(Form("<td><button> %s[%s]</button></td>", packname.Data(), analysis_name.Data()));
    div.AddContent(table);
    fHTML->AddContent(div);
    HalHtmlDiv div2;
    HalHtmlTable table2(table_name, "maintable", "display:table");
    HalHtmlRow row;
    row.SetClass(HalHtmlTableRowStyles::ExpandableStyle());
    HalHtmlCell cell;
    TryExtractAnaResult(cell, package, path);
    row.AddContent(cell);
    table2.AddContent(row);
    div2.AddContent(table2);
    fHTML->AddContent(div2);
  } else {
    HalCout::PrintInfo(Form("%s found in HalPhysics but don't inherit from HalPackage", packname.Data()),
                        Hal::EInfo::kLessWarning);
  }
  // delete list;
  file->Close();
  fHTML->AddStringContent("<script>window.onload=checkTask();</script>\n");
  //----------------------
  fHTML->Save();
  delete fHTML;
  fHTML = nullptr;
}

HalPackage2HTML::HalPackage2HTML(HalPackage* pack_ana, HalPackage* pack_meta, const TString dir, Int_t task_id) :
  HalPackage2HTML() {
  fDir = dir;
  for (int i = 0; i < 4; i++)
    fCollectionsNo[i] = 0;
  gSystem->MakeDirectory(fDir);
  fHTML = new HalHtmlFile(fDir + "/index.html", kFALSE);
  if (pack_meta) {
    HalHtmlCell dummyCell;
    TString temp_dir = Form("%s/global_data/", fDir.Data());
    CreatePackageList(dummyCell, pack_meta, eTableStyle::kMetaData, temp_dir, 2, "drawmerged");
    fHTML->AddStringContent(dummyCell.GetContent());
  }
  Int_t pack_counter = 0;
  TString table_name = Form("task_table_%i", fTaskTableCounter++);
  TString path       = fDir + "/superpack_0";
  if (pack_ana->InheritsFrom("HalPackage")) {
    HalPackage* package   = (HalPackage*) pack_ana;
    HalPackage* meta_data = (HalPackage*) (((HalPackage*) pack_ana)->GetObjectByName("HalMetadata"));
    TString analysis_name  = ((HalParameterString*) meta_data->GetObjectByName("Analysis Name"))->GetValue();
    HalHtmlDiv div1("buttontableM_0", "", "");
    HalHtmlTable table1("", "haltable", "");
    HalHtmlRow row1;
    row1.SetClass(HalHtmlTableRowStyles::TaskStyle());
    row1.SetStringContent(Form("<td><button> Task %i "
                               "[%s]</button></td>",
                               task_id,
                               analysis_name.Data()));
    table1.AddContent(row1);
    div1.AddContent(table1);
    fHTML->AddContent(div1);
    HalHtmlDiv div2;
    HalHtmlTable table2(table_name, "maintable", "display:table");
    HalHtmlRow row2;
    row2.SetClass(HalHtmlTableRowStyles::ExpandableStyle());
    HalHtmlCell cell;
    TryExtractAnaResult(cell, package, path);
    row2.AddContent(cell);
    table2.AddContent(row2);
    div2.AddContent(table2);
    fHTML->AddContent(div2);
    pack_counter++;
  } else {
    HalCout::PrintInfo(Form("%s  is not proper Object", pack_ana->ClassName()), Hal::EInfo::kLessWarning);
  }
  //----------------------
  fHTML->Save();
  delete fHTML;
  fHTML = nullptr;
}

void HalPackage2HTML::TryExtractAnaResult(HalHtmlObject& object, HalPackage* pack, TString path) {
  fCutContainerPosition = 0;
  gSystem->mkdir(path);
  fCurrentCutContainer = NULL;
  for (int i = 0; i < pack->GetEntries(); i++) {
    HalPackage* cutpack = (HalPackage*) pack->GetObject(i);
    TString classname    = cutpack->ClassName();
    if (classname == "HalPackage") {  // possible canditate
      TString pack_class_name(cutpack->GetName(), strlen(cutpack->GetName()));
      if (pack_class_name.EqualTo("HalCutContainer")) {
        fCurrentCutContainer  = cutpack;  // found break
        fCutContainerPosition = i + fCutContainerPosition;
        break;
      }
    }
  }
  if (fCurrentCutContainer != NULL) GetCollectionsNumbers();
  CreatePackageList(object, pack, eTableStyle::kStandard, path, 2);
  TString metadata = Form("%s/metadata/", path.Data());
  gSystem->mkdir(metadata);
  CreatePackageList(
    object, (HalPackage*) pack->GetObjectByName("HalMetadata"), eTableStyle::kMetaData, metadata, 3, "drawmerged");
  // //KURWA

  if (fCurrentCutContainer != NULL) {
    CreateCutAndMonitorList(object, path);
    ExportCollections(object, path);
  }
  fDynamicTableCounter++;
}

TString HalPackage2HTML::GetLinkCutMon(TH1* h1, TH1* h2, Int_t no, TString path) const {
  TString classname1 = h1->ClassName();
  TString classname2 = h2->ClassName();
  if (classname1 != classname2) { return "unknown"; };
  Bool_t batch = gROOT->IsBatch();
  gROOT->SetBatch(kTRUE);
  TString url;
  if (classname1.BeginsWith("TH1")) {
    url = GetLink1D(h1, h2, no, path);
  } else if (classname1.BeginsWith("TH2")) {
    url = GetLink2D((TH2*) h1, (TH2*) h2, no, path);
  } else if (classname1.BeginsWith("TH3")) {
    url = GetLink3D((TH3*) h1, (TH3*) h2, no, path);
  } else {
    return "unknown";
  }
  gROOT->SetBatch(batch);
  std::vector<TString> arr = HalStd::ExplodeString(path, '/');
  TString inject           = Form("%s/%s/", arr[arr.size() - 2].Data(), arr[arr.size() - 1].Data());
  url                      = AddToUrl(inject, url);
  return url;
}

void HalPackage2HTML::CreateImagePage(TString filename, TString image_flag, Int_t no, TString /*path*/) const {
  TString id_pic    = Form(image_flag.Data(), 0);
  TObjArray* clones = new TObjArray();
  clones->SetOwner(kTRUE);
  for (int i = 0; i < no; i++) {
    clones->AddLast(new TObjString(Form(image_flag.Data(), i)));
  }

  TString url = HalHtmlCore::ClickablePic(id_pic, clones, 796, 572);
  delete clones;

  HalHtmlFile file(filename, kFALSE);
  file.AddStringContent(url);
  file.AddStringContent("<script type=\'text/javascript\'>");
  file.AddStringContent("</script></br>");
  file.Save();
}

void HalPackage2HTML::CreatePackageList(HalHtmlObject& table,
                                         HalPackage* pack,
                                         eTableStyle style,
                                         TString path,
                                         Int_t depth,
                                         Option_t* opt) {
  TString options = opt;

  TString styleTitle, styleLegend, styleCell, styleExtra;
  styleTitle = HalHtmlTableRowStyles::TitleStyle();
  switch (style) {
    case HalPackage2HTML::eTableStyle::kMetaData: {
      styleLegend = HalHtmlTableRowStyles::DarkBlue();
      styleCell   = HalHtmlTableRowStyles::Grey();
      styleExtra  = HalHtmlTableRowStyles::ExtraStyle();
    } break;
    case HalPackage2HTML::eTableStyle::kStandard: {
      styleLegend = HalHtmlTableRowStyles::LegendStyle();
      styleCell   = HalHtmlTableRowStyles::DefStyle();
      styleExtra  = HalHtmlTableRowStyles::ExtraStyle();
    } break;
  }

  HalHtmlCore::FixAddress(path);
  Bool_t drawShowHideButton = kFALSE;
  Bool_t drawComments       = kFALSE;
  Bool_t drawClassName      = kFALSE;
  Bool_t drawMerged         = kFALSE;
  if (options.Contains("all")) { drawShowHideButton = drawComments = drawClassName = kTRUE; }
  if (options.Contains("drawbutton")) { drawShowHideButton = kTRUE; }
  if (options.Contains("drawcomment")) { drawComments = kTRUE; }
  if (options.Contains("drawclassname")) { drawClassName = kTRUE; }
  if (options.Contains("drawmerged")) { drawMerged = kTRUE; }
  HalHtmlDiv div;
  TString table_name = Form("dynamic_table_%i", fDynamicTableCounter++);
  if (drawShowHideButton) {
    table.AddStringContent("<div id=\"yellow_\" ><table class=\"haltable\" >");
    table.AddStringContent("<tr class=\"yellow_\"><td><button onclick=\"setTable('");
    table.AddStringContent(table_name);
    table.AddStringContent("')\">Hide/Show Main Table</button></td></tr>\n");
    table.AddStringContent("</table></div>");
    div.SetId(table_name);
    div.SetStyle("display:none");
  } else {
    div.SetId(table_name);
    div.SetStyle("display:block");
  }
  HalHtmlTable halTable;
  halTable.SetDefaultClass();
  int RowNo = 0;
  if (drawClassName) {
    HalHtmlRow row;
    row.SetClass(styleTitle);
    HalHtmlCellCol cell("", 5);
    cell.AddStringContent(Form("ClassName : %s", pack->GetName()));
    row.AddContent(cell);
    halTable.AddContent(row);
    RowNo++;
  }
  if (drawComments) {
    HalHtmlRow row;
    row.SetClass(styleLegend);
    HalHtmlCell cell;
    cell.SetColSpan(5);
    cell.AddStringContent(Form("Comment : %s", pack->GetComment().Data()));
    row.AddContent(cell);
    halTable.AddContent(row);
    RowNo++;
  }

  HalHtmlCellCol cell(Form("Object List in (%s)", pack->GetName()), 5);
  HalHtmlRow row1;
  row1.SetClass(styleLegend);
  if (RowNo == 0) row1.SetClass(styleTitle);
  row1.AddContent(cell);
  halTable.AddContent(row1);
  // 486

  HalHtmlCell cell2;
  cell2.SetColSpan(5);

  HalHtmlRow legend1;
  legend1.SetClass(styleLegend);
  HalHtmlCellCol cell3("No", 2);
  legend1.AddContent(cell3);
  legend1.AddContent(HalHtmlCell("ClassName"));
  legend1.AddContent(HalHtmlCell("Name"));
  legend1.AddContent(HalHtmlCell("Value"));
  halTable.AddContent(legend1);

  // path = fDir;
  std::vector<TString> out = HalStd::ExplodeString(path, '/');
  TString inject           = "";
  if (depth == 2) { inject = out[out.size() - 1]; }
  if (depth == 3) { inject = Form("%s/%s", out[out.size() - 2].Data(), out[out.size() - 1].Data()); }
  if (inject.Length() > 0) { inject = inject + "/"; }
  for (int i = 0; i < pack->GetEntries(); i++) {
    TObject* object        = pack->GetObject(i);
    TString nameClass      = object->ClassName();
    TString oryginal_class = object->GetName();
    if (nameClass == "HalPackage") {
      HalPackage* subpack = (HalPackage*) object;
      oryginal_class       = subpack->GetName();
      HalHtmlRow row;
      row.SetClass(styleCell);
      if (oryginal_class == "HalMetadata") { row.SetClass(styleExtra); }
      if (oryginal_class == "HalCutContainer") { row.SetClass(styleExtra); }
      HalHtmlCell first_cell(HalStd::RoundToString(i));
      first_cell.SetColSpan(2);
      row.AddContent(first_cell);
      row.AddContent(HalHtmlCell(object->ClassName()));
      row.AddContent(HalHtmlCell(oryginal_class));

      row.AddContent(HalHtmlCell(AddToUrl(inject, HalHtmlCore::HTMLExtract(object, i, path))));
      halTable.AddContent(row);

    } else if (nameClass == "HalQAPlotReport") {
      HalObject* rep = (HalObject*) object;
      rep->HTMLExtractIntoTable(i, halTable, path, inject);
    } else if (nameClass != "TList" || fListDeep != 0) {
      HalHtmlRow row;
      row.SetClass(styleCell);
      HalHtmlCell first_cell(HalStd::RoundToString(i));
      first_cell.SetColSpan(2);
      row.AddContent(first_cell);
      row.AddContent(HalHtmlCell(object->ClassName()));
      row.AddContent(HalHtmlCell(oryginal_class));
      row.AddContent(HalHtmlCell(AddToUrl(inject, HalHtmlCore::HTMLExtract(object, i, path))));
      halTable.AddContent(row);
    } else {
      CreateListTable(halTable, (TList*) object, i, path, inject, styleCell);
    }
  }
  if (drawMerged) {
    HalHtmlRow merged;
    merged.SetClass(HalHtmlTableRowStyles::SummaryStyle());
    HalHtmlCell cellM("Number of merged packages");
    cellM.SetColSpan(4);
    merged.AddContent(cellM);
    TString no = Form("%i", pack->GetMerged());
    merged.AddContent(HalHtmlCell(no));
    halTable.AddContent(merged);
  }
  div.AddContent(halTable);
  table.AddContent(div);
}

void HalPackage2HTML::CreateCutAndMonitorList(HalHtmlObject& table, TString path) {
  if (fCurrentCutContainer == NULL) return;
  TString table_name = Form("dynamic_table_%i", fDynamicTableCounter++);

  HalHtmlTable buttontable;
  buttontable.SetClass("haltable");
  HalHtmlRow rowButton;
  rowButton.SetClass(HalHtmlTableRowStyles::ExpandableStyle());
  rowButton.AddContent(HalHtmlCell(HalHtmlCore::GetHideButtonTable(table_name, "Show/hide cuts")));

  buttontable.AddContent(rowButton);
  HalHtmlDiv div;
  div.SetId("buttontable2");
  div.AddContent(buttontable);
  table.AddContent(div);

  HalHtmlTable tableCuts;
  tableCuts.SetClass("haltable");
  tableCuts.SetId(table_name);
  tableCuts.SetStyle("display:none");

  path = Form("%s/cut_monitors", path.Data());
  gSystem->mkdir(path);

  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEventUpdate)]) {
    HalHtmlRow row;
    row.SetClass(HalHtmlTableRowStyles::TitleStyle());
    HalHtmlCell cell(" Event Cut Collections ");
    cell.SetColSpan(8);
    row.AddContent(cell);
    tableCuts.AddContent(row);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEventUpdate)]; i++) {
      CreateCutHTML(tableCuts, Hal::ECutUpdate::kEventUpdate, i);
    }
  }
  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrackUpdate)]) {
    HalHtmlRow row;
    row.SetClass(HalHtmlTableRowStyles::TitleStyle());
    HalHtmlCell cell(" TrackCut Cut Collections ");
    cell.SetColSpan(8);
    row.AddContent(cell);
    tableCuts.AddContent(row);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrackUpdate)]; i++) {
      CreateCutHTML(tableCuts, Hal::ECutUpdate::kTrackUpdate, i);
    }
  }
  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackUpdate)]) {
    HalHtmlRow row;
    row.SetClass(HalHtmlTableRowStyles::TitleStyle());
    HalHtmlCell cell(" TwoTrackCut Cut Collections ");
    cell.SetColSpan(8);
    row.AddContent(cell);
    tableCuts.AddContent(row);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackUpdate)]; i++)
      CreateCutHTML(tableCuts, Hal::ECutUpdate::kTwoTrackUpdate, i);
  }
  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackgroundUpdate)]) {
    HalHtmlRow row;
    row.SetClass(HalHtmlTableRowStyles::TitleStyle());
    HalHtmlCell cell(" TwoTrackCut Cut Collections (background) ");
    cell.SetColSpan(8);
    row.AddContent(cell);
    tableCuts.AddContent(row);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackgroundUpdate)]; i++)
      CreateCutHTML(tableCuts, Hal::ECutUpdate::kTwoTrackBackgroundUpdate, i);
  }

  table_name = Form("dynamic_table_%i", fDynamicTableCounter++);

  HalHtmlDiv div2;
  div2.AddContent(tableCuts);
  table.AddContent(div2);
  HalHtmlDiv divCutMonitorButton;
  divCutMonitorButton.SetId("buttontable3");
  HalHtmlTable tableMonitorsButton("", "haltable", "");
  HalHtmlRow clicableRow("", HalHtmlTableRowStyles::ExpandableStyle(), "");
  HalHtmlCell cellButton(HalHtmlCore::GetHideButtonTable(table_name, "Show/hide cut monitors"));
  clicableRow.AddContent(cellButton);
  tableMonitorsButton.AddContent(clicableRow);
  divCutMonitorButton.AddContent(tableMonitorsButton);
  table.AddContent(tableMonitorsButton);
  HalHtmlTable tableMonitors(table_name, "haltable", "display:none");
  Int_t cut_monitor_counter = 0;
  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEventUpdate)]) {
    HalHtmlRow rowGreen("", HalHtmlTableRowStyles::TitleStyle(), "");
    HalHtmlCellCol cell("Event Cut Monitors", 9);
    rowGreen.AddContent(cell);
    tableMonitors.AddContent(rowGreen);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEventUpdate)]; i++) {
      CreateCutMonitorHTML(tableMonitors, Hal::ECutUpdate::kEventUpdate, i, cut_monitor_counter, path);
    }
  }
  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrackUpdate)]) {
    HalHtmlRow rowGreen("", HalHtmlTableRowStyles::TitleStyle(), "");
    HalHtmlCellCol cell("Track Cut Monitors", 9);
    rowGreen.AddContent(cell);
    tableMonitors.AddContent(rowGreen);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrackUpdate)]; i++) {
      CreateCutMonitorHTML(tableMonitors, Hal::ECutUpdate::kTrackUpdate, i, cut_monitor_counter, path);
    }
  }
  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackUpdate)]) {
    HalHtmlRow rowGreen("", HalHtmlTableRowStyles::TitleStyle(), "");
    HalHtmlCellCol cell("TwoTrack Cut Monitors", 9);
    rowGreen.AddContent(cell);
    tableMonitors.AddContent(rowGreen);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackUpdate)]; i++) {
      CreateCutMonitorHTML(tableMonitors, Hal::ECutUpdate::kTwoTrackUpdate, i, cut_monitor_counter, path);
    }
  }
  if (fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackgroundUpdate)]) {
    HalHtmlRow rowGreen("", HalHtmlTableRowStyles::TitleStyle(), "");
    HalHtmlCellCol cell("TwoTrack Cut Monitors (Background)", 9);
    rowGreen.AddContent(cell);
    tableMonitors.AddContent(rowGreen);
    for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackgroundUpdate)]; i++)
      CreateCutMonitorHTML(tableMonitors, Hal::ECutUpdate::kTwoTrackBackgroundUpdate, i, cut_monitor_counter, path);
  }
  HalHtmlDiv divCutMonitor;
  divCutMonitor.AddContent(tableMonitors);
  table.AddContent(divCutMonitor);
}

void HalPackage2HTML::CreateCutHTML(HalHtmlObject& table, Hal::ECutUpdate cut_update, Int_t collection_no) {
  TString group_name, desc, names;
  group_name = GetGroupListName(cut_update);
  switch (cut_update) {
    case Hal::ECutUpdate::kEventUpdate:
      desc  = "EventCollection";
      names = "Events";
      break;
    case Hal::ECutUpdate::kTrackUpdate:
      desc  = "TrackCollection";
      names = "Tracks";
      break;
    case Hal::ECutUpdate::kTwoTrackUpdate:
      desc  = "TwoTrackCollection";
      names = "Track TwoTracks";
      break;
    case Hal::ECutUpdate::kTwoTrackBackgroundUpdate:
      desc  = "TwoTrackCollectionBackground";
      names = "Track TwoTracks (Background)";
      break;
    default: HalCout::PrintInfo("Unknown cut update during HTML export", Hal::EInfo::kLessWarning); break;
  }

  TList* listA = (TList*) fCurrentCutContainer->GetObjectByName(group_name);
  if (listA == NULL) return;
  HalPackage* subcontainer = (HalPackage*) listA->At(collection_no);
  if (subcontainer == NULL) return;
  TList* list = (TList*) subcontainer->GetObjectByName("CutList");
  HalHtmlRow rowGreen("", HalHtmlTableRowStyles::TitleStyle(), "");
  HalHtmlCell cellGreen(Form("Collection No %i [%s]", collection_no, desc.Data()));
  cellGreen.SetColSpan(8);
  rowGreen.AddContent(cellGreen);
  table.AddContent(rowGreen);
  HalHtmlRow rowGreen2;
  rowGreen2.AddContent(HalHtmlCell("No"));
  rowGreen2.AddContent(HalHtmlCell("Cut name"));
  rowGreen2.AddContent(HalHtmlCell("Passed"));
  rowGreen2.AddContent(HalHtmlCell("Failed"));
  rowGreen2.AddContent(HalHtmlCell("Min"));
  rowGreen2.AddContent(HalHtmlCell("Max"));
  rowGreen2.AddContent(HalHtmlCell("Units"));
  rowGreen2.AddContent(HalHtmlCell("Type"));
  rowGreen2.SetClass(HalHtmlTableRowStyles::LegendStyle());
  table.AddContent(rowGreen2);

  TList* listfast = (TList*) subcontainer->GetObjectByName("FastCutList");
  if (listfast != 0x0)
    for (int i = 0; i < listfast->GetEntries(); i++) {
      CreateCutLink(table, cut_update, (HalPackage*) listfast->At(i), i, kTRUE);
    }
  ULong64_t passed = (ULong64_t)((HalParameterULong64*) subcontainer->GetObjectByName("PassedFast"))->GetValue();
  ULong64_t failed = (ULong64_t)((HalParameterULong64*) subcontainer->GetObjectByName("FailedFast"))->GetValue();
  HalHtmlRow rowFast;
  rowFast.SetClass(HalHtmlTableRowStyles::SummaryStyle());
  rowFast.AddContent(HalHtmlCell("-"));
  rowFast.AddContent(HalHtmlCell(Form("TOTAL %s", names.Data())));
  rowFast.AddContent(HalHtmlCell(HalStd::RoundToString(passed, -3, "prefix")));
  rowFast.AddContent(HalHtmlCell(HalStd::RoundToString(failed, -3, "prefix")));
  for (int i = 0; i < 3; i++)
    rowFast.AddContent(HalHtmlCell("-"));
  rowFast.AddContent(HalHtmlCell("Fast"));
  table.AddContent(rowFast);
  for (int i = 0; i < list->GetEntries(); i++) {
    CreateCutLink(table, cut_update, (HalPackage*) list->At(i), i, kFALSE);
  }
  passed = (ULong64_t)((HalParameterULong64*) subcontainer->GetObjectByName("PassedSlow"))->GetValue();
  failed = (ULong64_t)((HalParameterULong64*) subcontainer->GetObjectByName("FailedSlow"))->GetValue();
  HalHtmlRow rowSlow;
  rowSlow.SetClass(HalHtmlTableRowStyles::SummaryStyle());
  rowSlow.AddContent(HalHtmlCell("-"));
  rowSlow.AddContent(HalHtmlCell(Form("TOTAL %s", names.Data())));
  rowSlow.AddContent(HalHtmlCell(HalStd::RoundToString(passed, -3, "prefix")));
  rowSlow.AddContent(HalHtmlCell(HalStd::RoundToString(failed, -3, "prefix")));
  for (int i = 0; i < 3; i++)
    rowSlow.AddContent(HalHtmlCell("-"));
  rowSlow.AddContent(HalHtmlCell("Slow"));
  table.AddContent(rowSlow);
}

void HalPackage2HTML::CreateCutLink(HalHtmlObject& table, Hal::ECutUpdate cut_update, HalPackage* cut, Int_t no, Bool_t fast) {
  TString numer      = HalStd::RoundToString(no);
  TString name       = cut->GetName();
  TClass* class_temp = TClass::GetClass(name);
  Bool_t complex     = kFALSE;
  TString dummy_name = "";
  if (class_temp) {
    if (class_temp->InheritsFrom("HalTrackComplexCut") || class_temp->InheritsFrom("HalEventComplexCut")
        || class_temp->InheritsFrom("HalTwoTrackComplexCut")) {
      complex = kTRUE;
      if (cut->GetObjectByName("CutName_{re}")) {  // FIXME new version
        TString name_real = ((HalParameterString*) cut->GetObjectByName("CutName_{re}"))->GetValue();
        if (cut->GetObjectByName("CutName_{im}")) {
          TString name_img = ((HalParameterString*) cut->GetObjectByName("CutName_{im}"))->GetValue();
          dummy_name       = Form("<br/>(%s %s)", name_real.Data(), name_img.Data());
        }
      }
    }
    if (class_temp->InheritsFrom("HalTrackRealCut") || class_temp->InheritsFrom("HalEventRealCut")
        || class_temp->InheritsFrom("HalTwoTrackRealCut")) {
      if (cut->GetObjectByName("CutName_{re}")) {
        TString name_real = ((HalParameterString*) cut->GetObjectByName("CutName_{re}"))->GetValue();
        dummy_name        = Form("<br/>(%s)", name_real.Data());
      }
    }

    if (class_temp->InheritsFrom("HalTrackImaginaryCut") || class_temp->InheritsFrom("HalEventImaginaryCut")
        || class_temp->InheritsFrom("HalTwoTrackImaginaryCut")) {
      if (cut->GetObjectByName("CutName_{im}")) {
        TString name_real = ((HalParameterString*) cut->GetObjectByName("CutName_{im}"))->GetValue();
        dummy_name        = Form("<br/>(%s)", name_real.Data());
      }
    }
  }

  if (fSoftVer < 201705) { complex = kFALSE; }
  Int_t collection_no = ((HalParameterInt*) cut->GetObjectByName("CollectionID"))->GetValue();
  TString passed =
    HalStd::RoundToString((Float_t)((HalParameterULong64*) cut->GetObjectByName("Passed"))->GetValue(), -3, "prefix");
  TString failed =
    HalStd::RoundToString((Float_t)((HalParameterULong64*) cut->GetObjectByName("Failed"))->GetValue(), -3, "prefix");
  Int_t cut_size  = (((HalParameterInt*) cut->GetObjectByName("CutSize")))->GetValue();
  TString type    = "Slow";
  TString address = HalHtmlCore::GetUrl(GetLinkToCut(cut_update, collection_no, no, fast), name);
  if (dummy_name.Length() > 0) address = address + dummy_name;
  if (fast) type = "Fast";
  if (cut_size == 0) {
    HalHtmlRow row;
    row.SetClass(HalHtmlTableRowStyles::DefStyle());
    row.AddContent(HalHtmlCell(numer));
    row.AddContent(HalHtmlCell(address));
    row.AddContent(HalHtmlCell(passed));
    row.AddContent(HalHtmlCell(failed));
    for (int i = 0; i < 3; i++)
      row.AddContent(HalHtmlCell("-"));
    row.AddContent(HalHtmlCell(type));
    table.AddContent(row);
    return;
  }
  if (complex) {
    TString passed_re =
      HalStd::RoundToString((ULong64_t)((HalParameterULong64*) cut->GetObjectByName("Passed_{re}"))->GetValue(), -3, "prefix");
    TString failed_re =
      HalStd::RoundToString((ULong64_t)((HalParameterULong64*) cut->GetObjectByName("Failed_{re}"))->GetValue(), -3, "prefix");
    TString passed_im =
      HalStd::RoundToString((ULong64_t)((HalParameterULong64*) cut->GetObjectByName("Passed_{im}"))->GetValue(), -3, "prefix");
    TString failed_im =
      HalStd::RoundToString((ULong64_t)((HalParameterULong64*) cut->GetObjectByName("Failed_{im}"))->GetValue(), -3, "prefix");
    passed = Form("%s <br/>(%s %s)", passed.Data(), passed_re.Data(), passed_im.Data());
    failed = Form("%s <br/>(%s %s)", failed.Data(), failed_re.Data(), failed_im.Data());
  }
  HalHtmlRow row;
  row.SetClass(HalHtmlTableRowStyles::LightBlue());
  HalHtmlCell cell1(numer);
  cell1.SetRowSpan(cut_size);
  HalHtmlCell cell2(address);
  cell2.SetRowSpan(cut_size);
  HalHtmlCell cell3(passed);
  cell3.SetRowSpan(cut_size);
  HalHtmlCell cell4(failed);
  cell4.SetRowSpan(cut_size);
  row.AddContent(cell1);
  row.AddContent(cell2);
  row.AddContent(cell3);
  row.AddContent(cell4);

  // i ==1
  TString minima = HalStd::RoundToString(((HalParameterDouble*) cut->GetObjectByName(Form("MinCut_%i", 0)))->GetValue(), 3);
  TString maxima = HalStd::RoundToString(((HalParameterDouble*) cut->GetObjectByName(Form("MaxCut_%i", 0)))->GetValue(), 3);
  TString units  = ((HalParameterString*) cut->GetObjectByName(Form("UnitName_%i", 0)))->GetValue();
  row.AddContent(HalHtmlCell(minima));
  row.AddContent(HalHtmlCell(maxima));
  row.AddContent(HalHtmlCell(units));
  HalHtmlCell type_cell(type);
  type_cell.SetRowSpan(cut_size);
  row.AddContent(type_cell);
  table.AddContent(row);
  TString min, max, unit;

  for (int i = 1; i < cut_size; i++) {
    min    = Form("MinCut_%i", i);
    max    = Form("MaxCut_%i", i);
    unit   = Form("UnitName_%i", i);
    minima = HalStd::RoundToString(((HalParameterDouble*) cut->GetObjectByName(min))->GetValue(), 3);
    maxima = HalStd::RoundToString(((HalParameterDouble*) cut->GetObjectByName(max))->GetValue(), 3);
    units  = ((HalParameterString*) cut->GetObjectByName(unit))->GetValue();
    HalHtmlRow singleCut;
    singleCut.SetClass(HalHtmlTableRowStyles::DefStyle());
    singleCut.AddContent(HalHtmlCell(minima));
    singleCut.AddContent(HalHtmlCell(maxima));
    singleCut.AddContent(HalHtmlCell(units));
    table.AddContent(singleCut);
  }
}

void HalPackage2HTML::CreateCutMonitorLink(HalHtmlObject& table,
                                            Hal::ECutUpdate update,
                                            HalPackage* monitor,
                                            Int_t no,
                                            Int_t counter,
                                            TString path) {
  TString numer   = HalStd::RoundToString(no);
  TString unit    = ((HalParameterString*) monitor->GetObjectByName("AxisX"))->GetValue();
  TString cutname = ((HalParameterString*) monitor->GetObjectByName("CutXName"))->GetValue();

  Int_t collection_no   = ((HalParameterInt*) monitor->GetObjectByName("CollectionID"))->GetValue();
  TString monitor_class = monitor->GetName();
  TString address       = HalHtmlCore::GetUrl(GetLinkToCutMonitor(update, collection_no, no), monitor_class);
  TString axisopt       = HalStd::RoundToString(((HalParameterInt*) monitor->GetObjectByName("CutXAxis"))->GetValue());
  TString cutmin        = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutXMin"))->GetValue(), 3);
  TString cutmax        = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutXMax"))->GetValue(), 3);
  // TString cutimg =
  // CreateTH1Link((TH1*)monitor->GetObjectByName("Passed"),(TH1*)monitor->GetObjectByName("Failed"));
  TString exupdate = "no";
  Int_t ex         = ((HalParameterInt*) monitor->GetObjectByName("ExclusiveUpdate"))->GetValue();
  if (ex == 1) exupdate = "yes";
  Int_t ndim = 0;
  if (monitor->GetObjectByName("CutXAxis") != nullptr) { ndim = 1; }
  if (monitor->GetObjectByName("CutYAxis") != nullptr) { ndim = 2; }
  if (monitor->GetObjectByName("CutZAxis") != nullptr) { ndim = 3; }
  HalHtmlRow rowMonitor;
  rowMonitor.SetClass(HalHtmlTableRowStyles::DefStyle());
  HalHtmlCell cutInfo[9];
  cutInfo[0].SetStringContent(numer);
  cutInfo[1].SetStringContent(address);
  cutInfo[2].SetStringContent(cutname);
  cutInfo[3].SetStringContent(unit);
  cutInfo[4].SetStringContent(axisopt);
  cutInfo[5].SetStringContent(cutmin);
  cutInfo[6].SetStringContent(cutmax);
  TString link =
    GetLinkCutMon((TH1*) monitor->GetObjectByName("Passed"), (TH1*) monitor->GetObjectByName("Failed"), counter, path);
  cutInfo[7].SetStringContent(link);
  cutInfo[8].SetStringContent(exupdate);
  switch (ndim) {
    case 1: {
      for (int i = 0; i < 9; i++) {
        rowMonitor.AddContent(cutInfo[i]);
      }
      table.AddContent(rowMonitor);
    } break;
    case 2: {
      cutInfo[0].SetRowSpan(2);
      cutInfo[1].SetRowSpan(2);
      cutInfo[7].SetRowSpan(2);
      cutInfo[8].SetRowSpan(2);
      for (int i = 0; i < 9; i++) {
        rowMonitor.AddContent(cutInfo[i]);
      }
      table.AddContent(rowMonitor);
      HalHtmlRow rowMonitor2;
      rowMonitor2.SetClass(HalHtmlTableRowStyles::DefStyle());
      unit    = ((HalParameterString*) monitor->GetObjectByName("AxisY"))->GetValue();
      cutname = ((HalParameterString*) monitor->GetObjectByName("CutYName"))->GetValue();
      if (cutname.Contains(")")) cutname = cutname.ReplaceAll("(", "</br>(");
      axisopt = HalStd::RoundToString(((HalParameterInt*) monitor->GetObjectByName("CutYAxis"))->GetValue());
      cutmin  = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutYMin"))->GetValue(), 3);
      cutmax  = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutYMax"))->GetValue(), 3);
      rowMonitor2.AddContent(HalHtmlCell(cutname));
      rowMonitor2.AddContent(HalHtmlCell(unit));
      rowMonitor2.AddContent(HalHtmlCell(axisopt));
      rowMonitor2.AddContent(HalHtmlCell(cutmin));
      rowMonitor2.AddContent(HalHtmlCell(cutmax));
      table.AddContent(rowMonitor2);

    } break;
    case 3: {
      cutInfo[0].SetRowSpan(3);
      cutInfo[1].SetRowSpan(3);
      cutInfo[7].SetRowSpan(3);
      cutInfo[8].SetRowSpan(3);
      for (int i = 0; i < 9; i++) {
        rowMonitor.AddContent(cutInfo[i]);
      }
      table.AddContent(rowMonitor);
      unit    = ((HalParameterString*) monitor->GetObjectByName("AxisY"))->GetValue();
      cutname = ((HalParameterString*) monitor->GetObjectByName("CutYName"))->GetValue();
      if (cutname.Contains(")")) cutname = cutname.ReplaceAll("(", "</br>(");
      axisopt = HalStd::RoundToString(((HalParameterInt*) monitor->GetObjectByName("CutYAxis"))->GetValue());
      cutmin  = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutYMin"))->GetValue(), 3);
      cutmax  = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutYMax"))->GetValue(), 3);
      HalHtmlRow rowMonitor2;
      rowMonitor2.SetClass(HalHtmlTableRowStyles::DefStyle());
      rowMonitor2.AddContent(HalHtmlCell(cutname));
      rowMonitor2.AddContent(HalHtmlCell(unit));
      rowMonitor2.AddContent(HalHtmlCell(axisopt));
      rowMonitor2.AddContent(HalHtmlCell(cutmin));
      rowMonitor2.AddContent(HalHtmlCell(cutmax));
      table.AddContent(rowMonitor2);
      // fHTML<<"</tr>"<<std::endl;
      unit    = ((HalParameterString*) monitor->GetObjectByName("AxisZ"))->GetValue();
      cutname = ((HalParameterString*) monitor->GetObjectByName("CutZName"))->GetValue();
      if (cutname.Contains(")")) cutname = cutname.ReplaceAll("(", "</br>(");
      axisopt = HalStd::RoundToString(((HalParameterInt*) monitor->GetObjectByName("CutZAxis"))->GetValue());
      cutmin  = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutZMin"))->GetValue(), 3);
      cutmax  = HalStd::RoundToString(((HalParameterDouble*) monitor->GetObjectByName("CutZMax"))->GetValue(), 3);
      HalHtmlRow rowMonitor3;
      rowMonitor3.SetClass(HalHtmlTableRowStyles::DefStyle());
      rowMonitor3.AddContent(HalHtmlCell(cutname));
      rowMonitor3.AddContent(HalHtmlCell(unit));
      rowMonitor3.AddContent(HalHtmlCell(axisopt));
      rowMonitor3.AddContent(HalHtmlCell(cutmin));
      rowMonitor3.AddContent(HalHtmlCell(cutmax));
      table.AddContent(rowMonitor3);
    } break;
    default: {
      for (int i = 0; i < 7; i++)
        rowMonitor.AddContent(HalHtmlCell("-"));
      rowMonitor.AddContent(cutInfo[7]);
      rowMonitor.AddContent(HalHtmlCell("-"));
      rowMonitor.AddContent(HalHtmlCell("-"));
      table.AddContent(rowMonitor);
    } break;
  }
}

void HalPackage2HTML::CreateListTable(HalHtmlObject& table,
                                       TList* list,
                                       Int_t no,
                                       TString path_data,
                                       TString path_url,
                                       TString drawClass) {
  if (path_url == "") { path_url = path_data; }
  fListDeep++;  // to prevent draw TList in TList
  HalHtmlRow row;
  row.SetClass(drawClass);
  HalHtmlCell cell1(Form("%i", no));
  cell1.SetColSpan(2);
  row.AddContent(cell1);
  row.AddContent(HalHtmlCell("TList"));
  row.AddContent(HalHtmlCell(list->GetName()));
  TString rowButton = HalHtmlCore::GetHideButtonRow(Form("list_%i", no), "Show/Hide");
  row.AddContent(HalHtmlCell(rowButton));
  table.AddContent(row);
  TString list_path = Form("%s/list_%i", path_data.Data(), no);
  gSystem->mkdir(list_path);
  TString list_dir  = Form("list_%i/", no);
  TString list_dir2 = Form("list_%i/", no);
  if (path_url != path_data) list_dir2 = Form("%s/list_%i/", path_url.Data(), no);
  for (int i = 0; i < list->GetEntries(); i++) {
    TObject* obj         = list->At(i);
    TString name         = obj->GetName();
    TString classname    = obj->ClassName();
    TString temp_classes = Form("%s list_%i", drawClass.Data(), no);
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
    rowElement.AddContent(HalHtmlCell(AddToUrl(list_dir2, HalHtmlCore::HTMLExtract(obj, i, list_path))));
    table.AddContent(rowElement);
  }
  fListDeep--;
}

void HalPackage2HTML::CreateCutMonitorHTML(HalHtmlObject& table,
                                            Hal::ECutUpdate cut_upd,
                                            Int_t collection_no,
                                            Int_t& counter,
                                            TString path) {
  TString group_name, desc, names;
  group_name = GetGroupListName(cut_upd);
  switch (cut_upd) {
    case Hal::ECutUpdate::kEventUpdate:
      desc  = "EventCollection";
      names = "Events";
      break;
    case Hal::ECutUpdate::kTrackUpdate:
      desc  = "TrackCollection";
      names = "Tracks";
      break;
    case Hal::ECutUpdate::kTwoTrackUpdate:
      desc  = "TwoTrackCollection";
      names = "Track TwoTracks";
      break;
    case Hal::ECutUpdate::kTwoTrackBackgroundUpdate:
      desc  = "TwoTrackBackgroundCollection";
      names = "Track TwoTracks Background";
      break;
    default: HalCout::PrintInfo("Unhandled monitor group in HalPackage2HTML", Hal::EInfo::kLessWarning); break;
  }
  HalHtmlRow row;
  row.SetClass(HalHtmlTableRowStyles::TitleStyle());
  HalHtmlCellCol cell(Form("Collection No %i [%s]", collection_no, names.Data()), 9);
  row.AddContent(cell);
  table.AddContent(row);

  TList* listA = (TList*) fCurrentCutContainer->GetObjectByName(group_name);
  if (listA == NULL) return;
  HalPackage* subcontainer = (HalPackage*) listA->At(collection_no);
  if (subcontainer == NULL) return;
  TList* list = (TList*) subcontainer->GetObjectByName("CutMonitorList");
  if (list == NULL) return;
  // fHTML<<HalHTML::GetHTMLRow(9,"No","Classname","CutName","Unit","AxisOption","CutMin","CutMax","Stacked</br>Img","Ex</br>Update")<<std::endl;
  if (list->GetEntries() > 0) {
    HalHtmlRow rowE;
    rowE.SetClass(HalHtmlTableRowStyles::LegendStyle());
    rowE.AddContent(HalHtmlCell("No"));
    rowE.AddContent(HalHtmlCell("ClassName"));
    rowE.AddContent(HalHtmlCell("CutName"));
    rowE.AddContent(HalHtmlCell("Unit"));
    rowE.AddContent(HalHtmlCell("AxisOption"));
    rowE.AddContent(HalHtmlCell("CutMin"));
    rowE.AddContent(HalHtmlCell("CutMax"));
    rowE.AddContent(HalHtmlCell("Stacked</br>Img"));
    rowE.AddContent(HalHtmlCell("Ex</br>Update"));
    table.AddContent(rowE);
    for (int i = 0; i < list->GetEntries(); i++) {
      CreateCutMonitorLink(table, cut_upd, (HalPackage*) list->At(i), i, ++counter, path);
    }
  } else {
    HalHtmlRow rowE;
    rowE.SetClass(HalHtmlTableRowStyles::DefStyle());
    HalHtmlCell cellE("Empty");
    cellE.SetColSpan(9);
    rowE.AddContent(cellE);
    table.AddContent(rowE);
  }
  //-------------two track cut table
}

void HalPackage2HTML::DrawContainer(HalPackage* pack,
                                     Int_t tiers_no,
                                     Int_t tier,
                                     Int_t collections_from_no,
                                     Int_t collections_to_no,
                                     Int_t layer,
                                     Int_t line_style,
                                     Int_t tier_jump) const {
  // UInt_t passedF =
  // ((HalParameterULong64*)cutCont->GetObjectByName("PassedFast"))->GetValue();
  Double_t failedF               = ((HalParameterULong64*) pack->GetObjectByName("FailedFast"))->GetValue();
  Double_t passedS               = ((HalParameterULong64*) pack->GetObjectByName("PassedSlow"))->GetValue();
  Double_t failedS               = ((HalParameterULong64*) pack->GetObjectByName("FailedSlow"))->GetValue();
  Double_t passed                = passedS;
  Double_t failed                = failedF + failedS;
  Double_t passRatio             = ((Double_t) passed) / ((Double_t(failed + passed)));
  HalParameterInt* collectionno = (HalParameterInt*) pack->GetObjectByName("CollectionID");
  Int_t collection_no            = collectionno->GetValue();
  Color_t passed_color           = kGreen + line_style * 2;
  Color_t failed_color           = kRed + line_style * 2;
  TString label                  = Form("%i", collection_no);
  //---------- draw lines from lower level to higher level of collections
  if (tier_jump != 0) {
    TList* linkList;
    if (pack->Exist("NextObj"))
      linkList = (TList*) pack->GetObjectByName("NextObj");  // backward compatibility
    else
      linkList = (TList*) pack->GetObjectByName(Form("NextObj_%i", layer));
    if (!linkList) return;
    for (int i = 0; i < linkList->GetEntries(); i++) {
      Int_t collection_to = ((HalParameterInt*) linkList->At(i))->GetValue();
      Double_t width      = 2.0;
      Int_t style         = 10;
      Color_t color       = line_style + 1;
      if (line_style < 10) {
        style = line_style + 1;
        width = 2.0 * line_style + width;
      }
      DrawLine(tiers_no,
               tier,
               tier + tier_jump,
               collections_from_no,
               collections_to_no,
               collection_no,
               collection_to,
               width,
               style,
               color);
    }
  }
  // draw circles
  DrawCircle(tiers_no,
             tier,
             collections_from_no,
             collection_no,
             passRatio,
             fBasicRadius,
             passed_color,
             failed_color,
             Form("%i", collection_no));
}

void HalPackage2HTML::ExportCollections(HalHtmlObject& object, TString path) {
  if (fCurrentCutContainer == NULL) return;
  Bool_t batch = gROOT->IsBatch();
  gROOT->SetBatch(kTRUE);
  TCanvas* c1 = new TCanvas("canv", "canv", fWindowWidth, fWIndowHeight);
  c1->Range(0, 0, fWindowWidth, fWIndowHeight);
  TList* listE   = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kEventUpdate));
  TList* listT   = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kTrackUpdate));
  TList* listT2  = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kTwoTrackUpdate));
  TList* listT2B = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kTwoTrackBackgroundUpdate));
  Int_t event_collections             = 0;
  Int_t track_collections             = 0;
  Int_t ttrack_collections            = 0;
  Int_t ttrack_collections_background = 0;
  Int_t levels_no                     = 0;
  if (listE) {
    event_collections = listE->GetEntries();
    levels_no++;
  }
  if (listT) {
    track_collections = listT->GetEntries();
    levels_no++;
  }
  if (listT2) {
    ttrack_collections = listT2->GetEntries();
    levels_no++;
  }
  if (listT2B) { ttrack_collections_background = listT2B->GetEntries(); }
  for (int i = 0; i < event_collections; i++) {
    if (listT) {
      DrawContainer((HalPackage*) listE->At(i),
                    levels_no,
                    0,
                    event_collections,
                    track_collections,
                    0,
                    0,
                    1);  // normal drawing with lines
    } else {
      DrawContainer((HalPackage*) listE->At(i),
                    levels_no,
                    0,
                    event_collections,
                    track_collections,
                    0,
                    0,
                    0);  // normal drawing without lines
    }
  }
  for (int i = 0; i < track_collections; i++) {
    if (listT2B) {
      DrawContainer((HalPackage*) listT->At(i),
                    levels_no,
                    1,
                    track_collections,
                    ttrack_collections_background,
                    1,
                    1,
                    1);  // draw lines to backround collections
    }
    if (listT2) {
      DrawContainer((HalPackage*) listT->At(i),
                    levels_no,
                    1,
                    track_collections,
                    ttrack_collections,
                    0,
                    0,
                    1);  // normal drawing with lines
    } else {
      DrawContainer((HalPackage*) listT->At(i),
                    levels_no,
                    1,
                    track_collections,
                    ttrack_collections,
                    0,
                    0,
                    0);  // normal drawing without lines
    }
  }
  for (int i = 0; i < ttrack_collections_background; i++) {
    fBasicRadius += 10;
    DrawContainer((HalPackage*) listT2B->At(i), levels_no, 2, ttrack_collections_background, 0, 0, 1,
                  0);  // draw without lines
    fBasicRadius -= 10;
  }
  for (int i = 0; i < ttrack_collections; i++) {
    DrawContainer((HalPackage*) listT2->At(i), levels_no, 2, ttrack_collections, 0, 0, 0, 0);  // draw only circles
  }
  TString short_path = path(fDir.Length() + 1, path.Length());
  TString text       = Form("<img class=\"pic\" src=\"%s/main_pict.png\"  width=\"%i \" height=\"%i "
                      "\">",
                      short_path.Data(),
                      (int) fWindowWidth,
                      (int) fWIndowHeight);
  object.AddStringContent(text);
  c1->SaveAs(Form("%s/main_pict.png", path.Data()));
  gROOT->SetBatch(batch);
}


TString HalPackage2HTML::GetLink1D(TH1* h1, TH1* h2, Int_t no, TString path) const {
  TString url;
  path = Form("%s/th1_%i", path.Data(), no);
  gSystem->mkdir(path);
  TString filename = Form("%s/histo.html", path.Data());
  TString name     = Form("histo_%i", no);
  TCanvas* c1      = new TCanvas("cutmon", "cutmon", 0, 0, 800, 600);
  gPad->SetLeftMargin(0.15);
  THStack* stack = new THStack();
  stack->Add(h1);
  stack->Add(h2);
  stack->Draw();
  stack->GetXaxis()->SetTitle(h1->GetXaxis()->GetTitle());
  stack->GetYaxis()->SetTitle(h1->GetYaxis()->GetTitle());
  stack->SetTitle(h1->GetTitle());
  c1->SaveAs(Form("%s/cutmon.root", path.Data()));

  HalHtmlFile file(filename, kFALSE);
  file.AddStringContent(HalHtmlCore::GetJsDiv("cutmon.root", "cutmon;1"));
  file.Save();
  return HalHtmlCore::GetUrl(Form("th1_%i/histo.html", no), Form("2x%s", h1->ClassName()));
}

TString HalPackage2HTML::GetLink2D(TH2* h1, TH2* h2, Int_t no, TString path) const {
  TString url;
  path = Form("%s/th1_%i", path.Data(), no);
  gSystem->mkdir(path);
  TString filename = Form("%s/histo.html", path.Data());
  TCanvas* c1      = new TCanvas("cutmon", "cutmon", 0, 0, 800, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  h1->SetTitle(Form("%s (Passed)", h1->GetTitle()));
  h1->Draw("colz");
  c1->cd(2);
  h2->SetTitle(Form("%s (Failed)", h2->GetTitle()));
  h2->Draw("colz");
  c1->SetName("cutmon");
  c1->SaveAs(Form("%s/cutmon.root", path.Data()));
  delete c1;
  HalHtmlFile file(filename, kFALSE);
  file.AddStringContent(HalHtmlCore::GetJsDiv("cutmon.root", "cutmon;1"));
  file.Save();
  return HalHtmlCore::GetUrl(Form("th1_%i/histo.html", no), Form("2x%s", h1->ClassName()));
}

TString HalPackage2HTML::GetLink3D(TH3* h1, TH3* h2, Int_t no, TString path) const {
  TString url;
  gSystem->mkdir(Form("%s/th1_%i", path.Data(), no));
  TString name = "histo";
  TCanvas* c1  = new TCanvas(name, name, 0, 0, 800, 600);
  gPad->SetLeftMargin(0.15);
  THStack* stack = new THStack();
  stack->Add(h1);
  stack->Add(h2);
  stack->Draw(Draw_3D_option);
  c1->SaveAs(Form("%s/th1_%i/histo_0.png", path.Data(), no));
  delete c1;
  delete stack;
  TString filename = Form("%s/th1_%i/histo.html", path.Data(), no);
  CreateImagePage(filename, "histo_%i.png", 1, Form("%s/th1_%i", path.Data(), no));
  return HalHtmlCore::GetUrl(Form("th1_%i/histo.html", no), Form("2x%s", h1->ClassName()));
}

TString HalPackage2HTML::GetLinkToCut(Hal::ECutUpdate update, Int_t collection_no, Int_t cut_no, Bool_t fast) const {
  TString list_name              = GetGroupListName(update);
  TList* list                    = (TList*) fCurrentCutContainer->GetObjectByName(list_name);
  HalPackage* cut_sub_container = (HalPackage*) list->At(collection_no);
  Int_t list_pos                 = 0;
  TString pattern                = "CutList";
  if (fast) { pattern = "FastCutList"; }
  for (list_pos = 0; list_pos < cut_sub_container->GetEntries(); list_pos++) {
    TString name = cut_sub_container->GetObject(list_pos)->GetName();
    if (name.EqualTo(pattern)) break;
  }
  Int_t int_update = static_cast<Int_t>(update);
  return Form("superpack_%i/pack_%i/list_%i/pack_%i/list_%i/pack_%i/index.html",
              fPackageID,
              fCutContainerPosition,
              2 * int_update + 1,
              collection_no,
              list_pos,
              cut_no);
}

TString HalPackage2HTML::GetLinkToCutMonitor(Hal::ECutUpdate update, Int_t collection_no, Int_t no) const {
  TString list_name              = GetGroupListName(update);
  TList* list                    = (TList*) fCurrentCutContainer->GetObjectByName(list_name);
  HalPackage* cut_sub_container = (HalPackage*) list->At(collection_no);
  Int_t list_pos                 = 0;
  TString pattern                = "CutMonitorList";
  for (list_pos = 0; list_pos < cut_sub_container->GetEntries(); list_pos++) {
    TString name = cut_sub_container->GetObject(list_pos)->GetName();
    if (name.EqualTo(pattern)) break;
  }
  Int_t int_update = static_cast<Int_t>(update);
  return Form("superpack_%i/pack_%i/list_%i/pack_%i/list_%i/pack_%i/index.html",
              fPackageID,
              fCutContainerPosition,
              2 * int_update + 1,
              collection_no,
              list_pos,
              no);
}

TString HalPackage2HTML::GetGroupListName(Hal::ECutUpdate update) const {
  switch (update) {
    case Hal::ECutUpdate::kEventUpdate: return "HalEventCutCollectionList"; break;
    case Hal::ECutUpdate::kTrackUpdate: return "HalTrackCutCollectionList"; break;
    case Hal::ECutUpdate::kTwoTrackUpdate: return "HalTwoTrackCutCollectionList"; break;
    case Hal::ECutUpdate::kTwoTrackBackgroundUpdate: return "HalTwoTrackBackgroundCutCollectionList"; break;
    default: return ""; break;
  }
}

HalPackage2HTML::~HalPackage2HTML() {
  if (fHTML) delete fHTML;
}

void HalPackage2HTML::DrawCircle(Double_t tiers_no,
                                  Double_t layer,
                                  Double_t collections_no,
                                  Double_t collection,
                                  Double_t pass_ratio,
                                  Double_t radii,
                                  Color_t passed_col,
                                  Color_t failed_col,
                                  TString label) const {
  Double_t Y_step     = fWIndowHeight / collections_no;
  Double_t X_step     = fWindowWidth / tiers_no;
  Double_t X_center   = X_step * (layer + 0.5);
  Double_t Y_center   = fWIndowHeight - Y_step * (collection + 0.5);
  Double_t angle_pass = 360.0 * pass_ratio;
  Double_t X_coor     = X_center;
  Double_t Y_coor     = Y_center;
  TArc* b2            = new TArc(X_coor, Y_coor, radii, 0, 360);
  b2->SetFillColor(failed_col);
  b2->Draw();
  if (pass_ratio > 1E-3) {
    TArc* b1 = new TArc(X_coor, Y_coor, radii, 90, 90 - angle_pass);
    b1->SetFillColor(passed_col);
    b1->Draw();
  }
  if (label.EqualTo(" ") == kFALSE) {
    TLatex t;
    t.DrawText(X_center - 5, Y_center - 10, label);
  }
}

void HalPackage2HTML::DrawLine(Double_t layers_no,
                                Double_t layer_from,
                                Double_t layer_to,
                                Double_t collections_from_no,
                                Double_t collections_to_no,
                                Double_t collection_from,
                                Double_t collection_to,
                                Double_t width,
                                Int_t style,
                                Color_t color) const {
  Double_t Y1_step = fWIndowHeight / collections_from_no;
  Double_t Y2_step = fWIndowHeight / collections_to_no;
  Double_t X_step  = fWindowWidth / layers_no;
  Double_t X1      = X_step * (layer_from + 0.5);
  Double_t X2      = X_step * (layer_to + 0.5);
  Double_t Y1      = fWIndowHeight - Y1_step * (collection_from + 0.5);
  Double_t Y2      = fWIndowHeight - Y2_step * (collection_to + 0.5);
  TLine* line      = new TLine(X1, Y1, X2, Y2);
  line->SetLineWidth(4.0);
  line->SetLineWidth(width);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->Draw();
}

void HalPackage2HTML::GetCollectionsNumbers() {
  TString collection_name[4] = {
    "Event_collections_No", "Track_collections_No", "TwoTrack_collections_No", "TwoTrack_collections_background_No"};
  HalParameterInt* event_collections_no    = (HalParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[0], 0);
  HalParameterInt* track_collections_no    = (HalParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[1], 0);
  HalParameterInt* twotrack_collections_no = (HalParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[2], 0);
  HalParameterInt* twotrack_collections_background_no =
    (HalParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[3], 0);
  if (event_collections_no)
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEventUpdate)] = event_collections_no->GetValue();
  else
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEventUpdate)] = 0;
  if (track_collections_no)
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrackUpdate)] = track_collections_no->GetValue();
  else
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrackUpdate)] = 0;
  if (twotrack_collections_no)
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackUpdate)] = twotrack_collections_no->GetValue();
  else
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackUpdate)] = 0;
  if (twotrack_collections_background_no) {
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackgroundUpdate)] =
      twotrack_collections_background_no->GetValue();
  } else {
    fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackgroundUpdate)] = 0;
  }
}

TString HalPackage2HTML::AddToUrl(TString add_dir, TString url) const {
  if (!url.Contains("href")) { return url; }
  Int_t start_url = url.First("=") + 2;
  TString first   = url(0, start_url);
  TString end     = url(start_url, url.Length());
  TString output  = Form("%s%s%s", first.Data(), add_dir.Data(), end.Data());
  output.ReplaceAll("//", "/");
  return output;
}
