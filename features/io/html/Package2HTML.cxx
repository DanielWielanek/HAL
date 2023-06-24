/*
 * HalPackage2HTML.cxx
 *
 *  Created on: 20-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Package2HTML.h"
#include "Cout.h"
#include "HtmlCore.h"
#include "HtmlDiv.h"
#include "HtmlFile.h"
#include "HtmlObject.h"
#include "HtmlTable.h"
#include "Object.h"
#include "Package.h"
#include "Parameter.h"

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

#include "StdString.h"
#include <iostream>

namespace Hal {
  Package2HTML::Package2HTML() :
    fHTML(nullptr),
    fFile(nullptr),
    fDir(""),
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

  Package2HTML::Package2HTML(Package* pack, const TString dir, Bool_t main_dir) : Package2HTML() {
    fDir = dir;
    for (int i = 0; i < 4; i++)
      fCollectionsNo[i] = 0;
    gSystem->MakeDirectory(fDir);
    fHTML = new HtmlFile(fDir + "/index.html", main_dir);
    HtmlCell cell;
    CreatePackageList(cell, pack, eTableStyle::kStandard, fDir, 1, "");
    fHTML->AddStringContent(cell.GetContent());
    fHTML->Save();
    delete fHTML;
    fHTML = nullptr;
  }

  Package2HTML::Package2HTML(TString filename, const TString dir) : Package2HTML() {  // uzywany
    fDir = dir;
    for (int i = 0; i < 4; i++)
      fCollectionsNo[i] = 0;
    gSystem->MakeDirectory(fDir);
    fHTML = new HtmlFile(fDir + "/index.html", kTRUE);

    fFile = new TFile(filename);
    ExtractRunInfo();

    TDirectory* tdir = (TDirectory*) fFile->Get("HalPhysics");
    //---------------------

    TList* list = tdir->GetListOfKeys();
    for (fPackageID = 0; fPackageID < list->GetEntries(); fPackageID++) {
      TString path       = Form("%s/superpack_%i", fDir.Data(), fPackageID);
      TString key_name   = ((TKey*) (list->At(fPackageID)))->GetName();
      TObject* object    = tdir->Get(key_name);
      TString table_name = Form("task_table_%i", fTaskTableCounter++);
      if (IsHalPackage(object)) {
        Package* package      = (Package*) object;
        Package* meta_data    = (Package*) (package->GetObjectByName("Metadata"));
        TString analysis_name = ((ParameterString*) meta_data->GetObjectByName("Analysis Name"))->GetValue();
        // object inherits form HalPackage - probably result of physical analysis
        HtmlDiv div;
        div.SetId(Form("buttontableM_%i", fPackageID));
        HtmlTable table;
        table.SetDefaultClass();
        HtmlRow firstRow;
        firstRow.SetClass(Hal::HtmlTableRowClass::TaskStyle());
        TString button = HtmlCore::GetHideButtonTable(table_name,
                                                      Form("Hide/Show Pack %s [Package No. "
                                                           "%i ]",
                                                           analysis_name.Data(),
                                                           fPackageID));
        firstRow.AddContent(HtmlCell(button));
        table.AddContent(firstRow);
        div.AddContent(table);
        fHTML->AddContent(div);
        HtmlDiv div2;
        HtmlTable mainTable(table_name, "maintable", "display:none");
        HtmlRow rowTable("", Hal::HtmlTableRowClass::ExpandableStyle(), "");
        HtmlCell yellowCell;
        TryExtractAnaResult(yellowCell, package, path);
        rowTable.AddContent(yellowCell);
        mainTable.AddContent(rowTable);
        div2.AddContent(mainTable);
        fHTML->AddContent(div2);
      } else {
        Cout::PrintInfo(Form("%s found in Physics but don't inherit from HalPackage", key_name.Data()), Hal::EInfo::kLowWarning);
      }
    }
    // delete list;
    fHTML->AddStringContent("<script>window.onload=checkTask();</script>\n");
    //----------------------
    fHTML->Save();
    delete fHTML;
    fHTML = nullptr;
    HtmlCore::ResetMainDir();
  }
  /*
    Package2HTML::Package2HTML(TString filename, const TString dir, TString packname) : Package2HTML() {
      fDir = dir;
      for (int i = 0; i < 4; i++)
        fCollectionsNo[i] = 0;
      fDynamicTableCounter = 0;
      gSystem->MakeDirectory(fDir);
      fHTML            = new HtmlFile(fDir + "/index.html", kFALSE);
      TFile* file      = new TFile(filename);
      TDirectory* tdir = (TDirectory*) file->Get("HalPhysics");
      //---------------------
      ExtractRunInfo();
      if (fSoftVer <= 201612) {
        Cout::PrintInfo("This file version might be not compatible wtih current version, use "
                        "macro/path/fix_files.C to fix it",
                        Hal::EInfo::kWarning);
      }
      TObject* object    = tdir->Get(packname);
      TString table_name = Form("task_table_%i", fTaskTableCounter++);
      TString path       = Form("%s/superpack_0/", fDir.Data());
      if (object->InheritsFrom("Hal::Package")) {
        Package* package      = (Package*) object;
        Package* meta_data    = (Package*) (((Package*) object)->GetObjectByName("Metadata"));
        TString analysis_name = ((ParameterString*) meta_data->GetObjectByName("Analysis Name"))->GetValue();
        HtmlDiv div;
        div.SetId("buttontableM_0");
        HtmlTable table;
        table.SetDefaultClass();
        HtmlRow firstRow;
        firstRow.SetClass(Hal::HtmlTableRowClass::TaskStyle());
        firstRow.AddStringContent(Form("<td><button> %s[%s]</button></td>", packname.Data(), analysis_name.Data()));
        div.AddContent(table);
        fHTML->AddContent(div);
        HtmlDiv div2;
        HtmlTable table2(table_name, "maintable", "display:table");
        HtmlRow row;
        row.SetClass(Hal::HtmlTableRowClass::ExpandableStyle());
        HtmlCell cell;
        TryExtractAnaResult(cell, package, path);
        row.AddContent(cell);
        table2.AddContent(row);
        div2.AddContent(table2);
        fHTML->AddContent(div2);
      } else {
        Cout::PrintInfo(Form("%s found in HalPhysics but don't inherit from HalPackage", packname.Data()),
    Hal::EInfo::kLowWarning);
      }
      // delete list;
      file->Close();
      fHTML->AddStringContent("<script>window.onload=checkTask();</script>\n");
      //----------------------
      fHTML->Save();
      delete fHTML;
      fHTML = nullptr;
    }
  */
  Package2HTML::Package2HTML(Package* pack_ana, Package* pack_meta, const TString dir, Int_t task_id) : Package2HTML() {
    fDir = dir;
    for (int i = 0; i < 4; i++)
      fCollectionsNo[i] = 0;
    gSystem->MakeDirectory(fDir);
    fHTML = new HtmlFile(fDir + "/index.html", kFALSE);
    if (pack_meta) {
      HtmlCell dummyCell;
      TString temp_dir = Form("%s/global_data/", fDir.Data());
      // gSystem->MakeDirectory(temp_dir);
      CreatePackageList(dummyCell, pack_meta, eTableStyle::kMetaData, temp_dir, 2, "drawmerged");
      fHTML->AddStringContent(dummyCell.GetContent());
    }
    Int_t pack_counter = 0;
    TString table_name = Form("task_table_%i", fTaskTableCounter++);
    TString path       = fDir + "/superpack_0";
    if (IsHalPackage(pack_ana)) {
      Package* package      = (Package*) pack_ana;
      Package* meta_data    = (Package*) (((Package*) pack_ana)->GetObjectByName("Metadata"));
      TString analysis_name = ((ParameterString*) meta_data->GetObjectByName("Analysis Name"))->GetValue();
      HtmlDiv div1("buttontableM_0", "", "");
      HtmlTable table1("", "haltable", "");
      HtmlRow row1;
      row1.SetClass(Hal::HtmlTableRowClass::TaskStyle());
      row1.SetStringContent(Form("<td><button> Task %i "
                                 "[%s]</button></td>",
                                 task_id,
                                 analysis_name.Data()));
      table1.AddContent(row1);
      div1.AddContent(table1);
      fHTML->AddContent(div1);
      HtmlDiv div2;
      HtmlTable table2(table_name, "maintable", "display:table");
      HtmlRow row2;
      row2.SetClass(Hal::HtmlTableRowClass::ExpandableStyle());
      HtmlCell cell;
      TryExtractAnaResult(cell, package, path);
      row2.AddContent(cell);
      table2.AddContent(row2);
      div2.AddContent(table2);
      fHTML->AddContent(div2);
      pack_counter++;
    } else {
      Cout::PrintInfo(Form("%s  is not proper Object", pack_ana->ClassName()), Hal::EInfo::kLowWarning);
    }
    //----------------------
    fHTML->Save();
    delete fHTML;
    fHTML = nullptr;
  }

  void Package2HTML::TryExtractAnaResult(HtmlObject& object, Package* pack, TString path) {
    fCutContainerPosition = 0;
    gSystem->mkdir(path);
    fCurrentCutContainer = NULL;
    for (int i = 0; i < pack->GetEntries(); i++) {
      Package* cutpack = (Package*) pack->GetObject(i);
      if (IsExacltyHalPackage(cutpack)) {  // possible canditate
        TString pack_class_name(cutpack->GetName(), strlen(cutpack->GetName()));
        if (pack_class_name.EqualTo("Hal::CutContainer")) {
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
    CreatePackageList(object, (Package*) pack->GetObjectByName("Metadata"), eTableStyle::kMetaData, metadata, 3, "drawmerged");
    // //KURWA

    if (fCurrentCutContainer != NULL) {
      CreateCutAndMonitorList(object, path);
      ExportCollections(object, path);
    }
    fDynamicTableCounter++;
  }

  TString Package2HTML::GetLinkCutMon(TH1* h1, TH1* h2, Int_t no, TString path) const {
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
    std::vector<TString> arr = Hal::Std::ExplodeString(path, '/');
    TString inject           = Form("%s/%s/", arr[arr.size() - 2].Data(), arr[arr.size() - 1].Data());
    url                      = AddToUrl(inject, url);
    return url;
  }

  void Package2HTML::CreateImagePage(TString filename, TString image_flag, Int_t no, TString /*path*/) const {
    TString id_pic    = Form(image_flag.Data(), 0);
    TObjArray* clones = new TObjArray();
    clones->SetOwner(kTRUE);
    for (int i = 0; i < no; i++) {
      clones->AddLast(new TObjString(Form(image_flag.Data(), i)));
    }

    TString url = HtmlCore::ClickablePic(id_pic, clones, 796, 572);
    delete clones;

    HtmlFile file(filename, kFALSE);
    file.AddStringContent(url);
    file.AddStringContent("<script type=\'text/javascript\'>");
    file.AddStringContent("</script></br>");
    file.Save();
  }

  void
  Package2HTML::CreatePackageList(HtmlObject& table, Package* pack, eTableStyle style, TString path, Int_t depth, Option_t* opt) {
    TString options = opt;

    TString styleTitle, styleLegend, styleCell, styleExtra;
    styleTitle = Hal::HtmlTableRowClass::TitleStyle();
    switch (style) {
      case Package2HTML::eTableStyle::kMetaData: {
        styleLegend = Hal::HtmlTableRowClass::DarkBlue();
        styleCell   = Hal::HtmlTableRowClass::Grey();
        styleExtra  = Hal::HtmlTableRowClass::ExtraStyle();
      } break;
      case Package2HTML::eTableStyle::kStandard: {
        styleLegend = Hal::HtmlTableRowClass::LegendStyle();
        styleCell   = Hal::HtmlTableRowClass::DefStyle();
        styleExtra  = Hal::HtmlTableRowClass::ExtraStyle();
      } break;
    }

    HtmlCore::FixAddress(path);
    Bool_t drawShowHideButton = kFALSE;
    Bool_t drawComments       = kFALSE;
    Bool_t drawClassName      = kFALSE;
    Bool_t drawMerged         = kFALSE;
    if (options.Contains("all")) { drawShowHideButton = drawComments = drawClassName = kTRUE; }
    if (options.Contains("drawbutton")) { drawShowHideButton = kTRUE; }
    if (options.Contains("drawcomment")) { drawComments = kTRUE; }
    if (options.Contains("drawclassname")) { drawClassName = kTRUE; }
    if (options.Contains("drawmerged")) { drawMerged = kTRUE; }
    HtmlDiv div;
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
    HtmlTable halTable;
    halTable.SetDefaultClass();
    int RowNo = 0;
    if (drawClassName) {
      HtmlRow row;
      row.SetClass(styleTitle);
      HtmlCellCol cell("", 5);
      cell.AddStringContent(Form("ClassName : %s", pack->GetName()));
      row.AddContent(cell);
      halTable.AddContent(row);
      RowNo++;
    }
    if (drawComments) {
      HtmlRow row;
      row.SetClass(styleLegend);
      HtmlCell cell;
      cell.SetColSpan(5);
      cell.AddStringContent(Form("Comment : %s", pack->GetComment().Data()));
      row.AddContent(cell);
      halTable.AddContent(row);
      RowNo++;
    }

    HtmlCellCol cell(Form("Object List in (%s)", pack->GetName()), 5);
    HtmlRow row1;
    row1.SetClass(styleLegend);
    if (RowNo == 0) row1.SetClass(styleTitle);
    row1.AddContent(cell);
    halTable.AddContent(row1);
    // 486

    HtmlCell cell2;
    cell2.SetColSpan(5);

    HtmlRow legend1;
    legend1.SetClass(styleLegend);
    HtmlCellCol cell3("No", 2);
    legend1.AddContent(cell3);
    legend1.AddSimpleCells({"ClassName", "Name", "Value"});
    halTable.AddContent(legend1);

    // path = fDir;
    std::vector<TString> out = Hal::Std::ExplodeString(path, '/');
    TString inject           = "";
    if (depth == 2) { inject = out[out.size() - 1]; }
    if (depth == 3) { inject = Form("%s/%s", out[out.size() - 2].Data(), out[out.size() - 1].Data()); }

    if (inject.Length() > 0) { inject = inject + "/"; }
    for (int i = 0; i < pack->GetEntries(); i++) {
      TObject* object        = pack->GetObject(i);
      TString nameClass      = object->ClassName();
      TString oryginal_class = object->GetName();
      if (IsExacltyHalPackage(object)) {
        Package* subpack = (Package*) object;
        oryginal_class   = subpack->GetName();
        HtmlRow row;
        row.SetClass(styleCell);
        if (oryginal_class == "Hal::Metadata") { row.SetClass(styleExtra); }
        if (oryginal_class == "Hal::CutContainer") { row.SetClass(styleExtra); }
        HtmlCell first_cell(Hal::Std::RoundToString(i));
        first_cell.SetColSpan(2);
        row.AddContent(first_cell);
        row.AddContent(HtmlCell(object->ClassName()));
        row.AddContent(HtmlCell(oryginal_class));
        row.AddContent(HtmlCell(AddToUrl(inject, HtmlCore::HTMLExtract(object, i, path))));
        halTable.AddContent(row);

      } else if (nameClass == "Hal::QAPlotReport") {
        Object* rep = (Object*) object;
        rep->HTMLExtractIntoTable(fTObjectCounter["qa"]++, halTable, path, inject);
      } else if (nameClass != "TList" || fListDeep != 0) {
        HtmlRow row;
        row.SetClass(styleCell);
        HtmlCell first_cell(Hal::Std::RoundToString(i));
        first_cell.SetColSpan(2);
        row.AddContent(first_cell);
        row.AddContent(HtmlCell(object->ClassName()));
        row.AddContent(HtmlCell(oryginal_class));
        row.AddContent(HtmlCell(AddToUrl(inject, HtmlCore::HTMLExtract(object, fTObjectCounter["TList"]++, path))));
        halTable.AddContent(row);
      } else {
        CreateListTable(halTable, (TList*) object, fTObjectCounter["TList"]++, path, inject, styleCell);
      }
    }
    if (drawMerged) {
      HtmlRow merged;
      merged.SetClass(Hal::HtmlTableRowClass::SummaryStyle());
      HtmlCell cellM("Number of merged packages");
      cellM.SetColSpan(4);
      merged.AddContent(cellM);
      TString no = Form("%i", pack->GetMerged());
      merged.AddContent(HtmlCell(no));
      halTable.AddContent(merged);
    }
    div.AddContent(halTable);
    table.AddContent(div);
  }

  void Package2HTML::CreateCutAndMonitorList(HtmlObject& table, TString path) {
    if (fCurrentCutContainer == NULL) return;
    TString table_name = Form("dynamic_table_%i", fDynamicTableCounter++);

    HtmlTable buttontable;
    buttontable.SetClass("haltable");
    HtmlRow rowButton;
    rowButton.SetClass(Hal::HtmlTableRowClass::ExpandableStyle());
    rowButton.AddContent(HtmlCell(HtmlCore::GetHideButtonTable(table_name, "Show/hide cuts")));

    buttontable.AddContent(rowButton);
    HtmlDiv div;
    div.SetId("buttontable2");
    div.AddContent(buttontable);
    table.AddContent(div);

    HtmlTable tableCuts;
    tableCuts.SetClass("haltable");
    tableCuts.SetId(table_name);
    tableCuts.SetStyle("display:none");

    path = Form("%s/cut_monitors", path.Data());
    gSystem->mkdir(path);
    Hal::ECutUpdate updList[] = {
      Hal::ECutUpdate::kEvent, Hal::ECutUpdate::kTrack, Hal::ECutUpdate::kTwoTrack, Hal::ECutUpdate::kTwoTrackBackground};
    TString colNames[] = {" Event Cut Collections ",
                          " TrackCut Cut Collections ",
                          " TwoTrackCut Cut Collections ",
                          " TwoTrackCut Cut Collections (background) "};

    auto colLoop = [&](Hal::ECutUpdate upd, TString name) {
      if (fCollectionsNo[static_cast<Int_t>(upd)]) {
        HtmlRow row;
        row.SetClass(Hal::HtmlTableRowClass::TitleStyle());
        HtmlCell cell(name);
        cell.SetColSpan(8);
        row.AddContent(cell);
        tableCuts.AddContent(row);
        for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(upd)]; i++) {
          CreateCutHTML(tableCuts, upd, i);
        }
      }
    };
    for (int i = 0; i < 4; i++)
      colLoop(updList[i], colNames[i]);

    table_name = Form("dynamic_table_%i", fDynamicTableCounter++);

    HtmlDiv div2;
    div2.AddContent(tableCuts);
    table.AddContent(div2);
    HtmlDiv divCutMonitorButton;
    divCutMonitorButton.SetId("buttontable3");
    HtmlTable tableMonitorsButton("", "haltable", "");
    HtmlRow clicableRow("", Hal::HtmlTableRowClass::ExpandableStyle(), "");
    HtmlCell cellButton(HtmlCore::GetHideButtonTable(table_name, "Show/hide cut monitors"));
    clicableRow.AddContent(cellButton);
    tableMonitorsButton.AddContent(clicableRow);
    divCutMonitorButton.AddContent(tableMonitorsButton);
    table.AddContent(tableMonitorsButton);
    HtmlTable tableMonitors(table_name, "haltable", "display:none");
    Int_t cut_monitor_counter = 0;

    TString names[] = {"Event Cut Monitors", "Track Cut Monitors", "TwoTrack Cut Monitors", "TwoTrack Cut Monitors (Background)"};
    auto collectionLoop = [&](Hal::ECutUpdate upd, TString name) {
      if (fCollectionsNo[static_cast<Int_t>(upd)]) {
        HtmlRow rowGreen("", Hal::HtmlTableRowClass::TitleStyle(), "");
        HtmlCellCol cell(name, 9);
        rowGreen.AddContent(cell);
        tableMonitors.AddContent(rowGreen);
        for (int i = 0; i < fCollectionsNo[static_cast<Int_t>(upd)]; i++) {
          CreateCutMonitorHTML(tableMonitors, upd, i, cut_monitor_counter, path);
        }
      }
    };
    for (int i = 0; i < 4; i++)
      collectionLoop(updList[i], names[i]);

    HtmlDiv divCutMonitor;
    divCutMonitor.AddContent(tableMonitors);
    table.AddContent(divCutMonitor);
  }

  void Package2HTML::CreateCutHTML(HtmlObject& table, Hal::ECutUpdate cut_update, Int_t collection_no) {
    TString group_name, desc, names;
    group_name = GetGroupListName(cut_update);
    switch (cut_update) {
      case Hal::ECutUpdate::kEvent:
        desc  = "EventCollection";
        names = "Events";
        break;
      case Hal::ECutUpdate::kTrack:
        desc  = "TrackCollection";
        names = "Tracks";
        break;
      case Hal::ECutUpdate::kTwoTrack:
        desc  = "TwoTrackCollection";
        names = "Track TwoTracks";
        break;
      case Hal::ECutUpdate::kTwoTrackBackground:
        desc  = "TwoTrackCollectionBackground";
        names = "Track TwoTracks (Background)";
        break;
      default: Cout::PrintInfo("Unknown cut update during HTML export", Hal::EInfo::kLowWarning); break;
    }

    TList* listA = (TList*) fCurrentCutContainer->GetObjectByName(group_name);
    if (listA == NULL) return;
    Package* subcontainer = (Package*) listA->At(collection_no);
    if (subcontainer == NULL) return;
    TList* list = (TList*) subcontainer->GetObjectByName("CutList");
    HtmlRow rowGreen("", Hal::HtmlTableRowClass::TitleStyle(), "");
    HtmlCell cellGreen(Form("Collection No %i [%s]", collection_no, desc.Data()));
    cellGreen.SetColSpan(8);
    rowGreen.AddContent(cellGreen);
    table.AddContent(rowGreen);
    HtmlRow rowGreen2;
    rowGreen2.AddSimpleCells({"No", "Cut name", "Passed", "Failed", "Min", "Max", "Units", "Type"});
    rowGreen2.SetClass(Hal::HtmlTableRowClass::LegendStyle());
    table.AddContent(rowGreen2);

    TList* listfast = (TList*) subcontainer->GetObjectByName("FastCutList");
    if (listfast != 0x0)
      for (int i = 0; i < listfast->GetEntries(); i++) {
        CreateCutLink(table, cut_update, (Package*) listfast->At(i), i, kTRUE);
      }
    ULong64_t passed = (ULong64_t)((ParameterULong64*) subcontainer->GetObjectByName("PassedFast"))->GetValue();
    ULong64_t failed = (ULong64_t)((ParameterULong64*) subcontainer->GetObjectByName("FailedFast"))->GetValue();
    HtmlRow rowFast;
    rowFast.SetClass(Hal::HtmlTableRowClass::SummaryStyle());
    rowFast.AddContent(HtmlCell("-"));
    rowFast.AddContent(HtmlCell(Form("TOTAL %s", names.Data())));
    rowFast.AddContent(HtmlCell(Hal::Std::RoundToString(passed, -3, "prefix")));
    rowFast.AddContent(HtmlCell(Hal::Std::RoundToString(failed, -3, "prefix")));
    for (int i = 0; i < 3; i++)
      rowFast.AddContent(HtmlCell("-"));
    rowFast.AddContent(HtmlCell("Fast"));
    table.AddContent(rowFast);
    for (int i = 0; i < list->GetEntries(); i++) {
      CreateCutLink(table, cut_update, (Package*) list->At(i), i, kFALSE);
    }
    passed = GetULong(subcontainer, "PassedSlow");
    failed = GetULong(subcontainer, "FailedSlow");
    HtmlRow rowSlow;
    rowSlow.SetClass(Hal::HtmlTableRowClass::SummaryStyle());
    rowSlow.AddContent(HtmlCell("-"));
    rowSlow.AddContent(HtmlCell(Form("TOTAL %s", names.Data())));
    rowSlow.AddContent(HtmlCell(Hal::Std::RoundToString(passed, -3, "prefix")));
    rowSlow.AddContent(HtmlCell(Hal::Std::RoundToString(failed, -3, "prefix")));
    for (int i = 0; i < 3; i++)
      rowSlow.AddContent(HtmlCell("-"));
    rowSlow.AddContent(HtmlCell("Slow"));
    table.AddContent(rowSlow);
  }

  void Package2HTML::CreateCutLink(HtmlObject& table, Hal::ECutUpdate cut_update, Package* cut, Int_t no, Bool_t fast) {
    TString numer      = Hal::Std::RoundToString(no);
    TString name       = cut->GetName();
    TClass* class_temp = TClass::GetClass(name);
    Bool_t complex     = kFALSE;
    TString dummy_name = "";
    auto IsInhert      = [](TObject* class_temp, std::initializer_list<TString> list) {
      for (auto a : list)
        if (class_temp->InheritsFrom(a)) return kTRUE;
      return kFALSE;
    };
    if (class_temp) {
      if (IsInhert(class_temp, {"Hal::TrackComplexCut", "Hal::EventComplexCut", "Hal::TwoTrackComplexCut"})) {
        complex = kTRUE;
        if (cut->GetObjectByName("CutName_{re}")) {  // FIXME new version
          TString name_real = GetString(cut, "CutName_{re}");
          if (cut->GetObjectByName("CutName_{im}")) {
            TString name_img = GetString(cut, "CutName_{im}");
            dummy_name       = Form("<br/>(%s %s)", name_real.Data(), name_img.Data());
          }
        }
      }
      if (IsInhert(class_temp, {"Hal::TrackRealCut", "Hal::EventRealCut", "Hal::TwoTrackRealCut"})) {
        if (cut->GetObjectByName("CutName_{re}")) {
          TString name_real = GetString(cut, "CutName_{re}");
          dummy_name        = Form("<br/>(%s)", name_real.Data());
        }
      }

      if (IsInhert(class_temp, {"Hal::TrackImaginaryCut", "Hal::EventImaginaryCut", "Hal::TwoTrackImaginaryCut"})) {
        if (cut->GetObjectByName("CutName_{im}")) {
          TString name_real = GetString(cut, "CutName_{im}");
          dummy_name        = Form("<br/>(%s)", name_real.Data());
        }
      }
    }

    if (fSoftVer < 201705) { complex = kFALSE; }
    Int_t collection_no = ((ParameterInt*) cut->GetObjectByName("CollectionID"))->GetValue();
    TString passed      = Hal::Std::RoundToString((Float_t)(GetULong(cut, "Passed")), -3, "prefix");
    TString failed      = Hal::Std::RoundToString((Float_t)(GetULong(cut, "Failed")), -3, "prefix");
    Int_t cut_size      = (((ParameterInt*) cut->GetObjectByName("CutSize")))->GetValue();
    TString type        = "Slow";
    TString address     = HtmlCore::GetUrl(GetLinkToCut(cut_update, collection_no, no, fast), name);
    if (dummy_name.Length() > 0) address = address + dummy_name;
    if (fast) type = "Fast";
    if (cut_size == 0) {
      HtmlRow row;
      row.SetClass(Hal::HtmlTableRowClass::DefStyle());
      row.AddSimpleCells({numer, address, passed, failed});
      for (int i = 0; i < 3; i++)
        row.AddContent(HtmlCell("-"));
      row.AddContent(HtmlCell(type));
      table.AddContent(row);
      return;
    }
    if (complex) {
      TString passed_re = Hal::Std::RoundToString(GetULong(cut, "Passed_{re}"), -3, "prefix");
      TString failed_re = Hal::Std::RoundToString(GetULong(cut, "Failed_{re}"), -3, "prefix");
      TString passed_im = Hal::Std::RoundToString(GetULong(cut, "Passed_{im}"), -3, "prefix");
      TString failed_im = Hal::Std::RoundToString(GetULong(cut, "Failed_{im}"), -3, "prefix");
      passed            = Form("%s <br/>(%s %s)", passed.Data(), passed_re.Data(), passed_im.Data());
      failed            = Form("%s <br/>(%s %s)", failed.Data(), failed_re.Data(), failed_im.Data());
    }
    HtmlRow row;
    row.SetClass(Hal::HtmlTableRowClass::LightBlue());
    HtmlCell cell1(numer);
    cell1.SetRowSpan(cut_size);
    HtmlCell cell2(address);
    cell2.SetRowSpan(cut_size);
    HtmlCell cell3(passed);
    cell3.SetRowSpan(cut_size);
    HtmlCell cell4(failed);
    cell4.SetRowSpan(cut_size);
    row.AddContent(cell1);
    row.AddContent(cell2);
    row.AddContent(cell3);
    row.AddContent(cell4);

    // i ==1
    TString minima = Hal::Std::RoundToString(GetDouble(cut, Form("MinCut_%i", 0)), 3);
    TString maxima = Hal::Std::RoundToString(GetDouble(cut, Form("MaxCut_%i", 0)), 3);
    TString units  = ((ParameterString*) cut->GetObjectByName(Form("UnitName_%i", 0)))->GetValue();
    row.AddSimpleCells({minima, maxima, units});
    HtmlCell type_cell(type);
    type_cell.SetRowSpan(cut_size);
    row.AddContent(type_cell);
    table.AddContent(row);
    TString min, max, unit;

    for (int i = 1; i < cut_size; i++) {
      min    = Form("MinCut_%i", i);
      max    = Form("MaxCut_%i", i);
      unit   = Form("UnitName_%i", i);
      minima = Hal::Std::RoundToString(GetDouble(cut, min), 3);
      maxima = Hal::Std::RoundToString(GetDouble(cut, max), 3);
      units  = ((ParameterString*) cut->GetObjectByName(unit))->GetValue();
      HtmlRow singleCut;
      singleCut.SetClass(Hal::HtmlTableRowClass::DefStyle());
      row.AddSimpleCells({minima, maxima, units});
      table.AddContent(singleCut);
    }
  }

  void Package2HTML::CreateCutMonitorLink(HtmlObject& table,
                                          Hal::ECutUpdate update,
                                          Package* monitor,
                                          Int_t no,
                                          Int_t counter,
                                          TString path) {
    TString numer       = Hal::Std::RoundToString(no);
    Int_t collection_no = ((ParameterInt*) monitor->GetObjectByName("CollectionID"))->GetValue();
    TString exupdate    = "no";
    Int_t ex            = ((ParameterInt*) monitor->GetObjectByName("ExclusiveUpdate"))->GetValue();
    if (ex == 1) exupdate = "yes";
    TString monitor_class = monitor->GetName();
    TString address       = HtmlCore::GetUrl(GetLinkToCutMonitor(update, collection_no, no), monitor_class);
    TString link =
      GetLinkCutMon((TH1*) monitor->GetObjectByName("Passed"), (TH1*) monitor->GetObjectByName("Failed"), counter, path);

    HtmlCell cutInfo[9];
    cutInfo[0].SetStringContent(numer);
    cutInfo[1].SetStringContent(address);
    cutInfo[7].SetStringContent(link);
    cutInfo[8].SetStringContent(exupdate);

    auto fillTable = [&](TString opt) {
      TString unit    = GetString(monitor, Form("Axis%s", opt.Data()));
      TString cutname = GetString(monitor, Form("Cut%sName", opt.Data()));
      if (cutname.Contains(")")) cutname = cutname.ReplaceAll("(", "</br>(");
      TString axisopt =
        Hal::Std::RoundToString(((ParameterInt*) monitor->GetObjectByName(Form("Cut%sAxis", opt.Data())))->GetValue());
      TString cutmin = Hal::Std::RoundToString(GetDouble(monitor, Form("Cut%sMin", opt.Data())), 3);
      TString cutmax = Hal::Std::RoundToString(GetDouble(monitor, Form("Cut%sMax", opt.Data())), 3);
      cutInfo[2].SetStringContent(cutname);
      cutInfo[3].SetStringContent(unit);
      cutInfo[4].SetStringContent(axisopt);
      cutInfo[5].SetStringContent(cutmin);
      cutInfo[6].SetStringContent(cutmax);
    };


    Int_t ndim = 0;
    if (monitor->GetObjectByName("CutXAxis") != nullptr) { ndim = 1; }
    if (monitor->GetObjectByName("CutYAxis") != nullptr) { ndim = 2; }
    if (monitor->GetObjectByName("CutZAxis") != nullptr) { ndim = 3; }
    HtmlRow rowMonitor;
    rowMonitor.SetClass(Hal::HtmlTableRowClass::DefStyle());


    cutInfo[8].SetStringContent(exupdate);
    fillTable("X");
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
        HtmlRow rowMonitor2;
        rowMonitor2.SetClass(Hal::HtmlTableRowClass::DefStyle());
        fillTable("Y");
        for (int i = 2; i < 7; i++) {
          rowMonitor.AddContent(cutInfo[i]);
        }
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
        HtmlRow rowMonitor2;
        rowMonitor2.SetClass(Hal::HtmlTableRowClass::DefStyle());
        fillTable("Y");
        for (int i = 2; i < 7; i++) {
          rowMonitor2.AddContent(cutInfo[i]);
        }
        table.AddContent(rowMonitor2);
        HtmlRow rowMonitor3;
        rowMonitor3.SetClass(Hal::HtmlTableRowClass::DefStyle());
        fillTable("Z");
        for (int i = 2; i < 7; i++) {
          rowMonitor3.AddContent(cutInfo[i]);
        }
        table.AddContent(rowMonitor3);
      } break;
      default: {
        for (int i = 0; i < 7; i++)
          rowMonitor.AddContent(HtmlCell("-"));
        rowMonitor.AddContent(cutInfo[7]);
        rowMonitor.AddContent(HtmlCell("-"));
        rowMonitor.AddContent(HtmlCell("-"));
        table.AddContent(rowMonitor);
      } break;
    }
  }

  void Package2HTML::CreateListTable(HtmlObject& table,
                                     TList* list,
                                     Int_t no,
                                     TString path_data,
                                     TString path_url,
                                     TString drawClass) {
    if (path_url == "") { path_url = path_data; }
    fListDeep++;  // to prevent draw TList in TList
    HtmlRow row;
    row.SetClass(drawClass);
    HtmlCell cell1(Form("%i", no));
    cell1.SetColSpan(2);
    row.AddContent(cell1);
    row.AddContent(HtmlCell("TList"));
    row.AddContent(HtmlCell(list->GetName()));
    TString rowButton = HtmlCore::GetHideButtonRow(Form("list_%i", no), "Show/Hide");
    row.AddContent(HtmlCell(rowButton));
    table.AddContent(row);
    TString list_path = Form("%s/list_%i", path_data.Data(), no);
    gSystem->mkdir(list_path);
    TString list_dir  = Form("list_%i/", no);
    TString list_dir2 = list_dir;
    if (path_url != path_data) list_dir2 = Form("%s/list_%i/", path_url.Data(), no);
    for (int i = 0; i < list->GetEntries(); i++) {
      TObject* obj         = list->At(i);
      TString name         = obj->GetName();
      TString classname    = obj->ClassName();
      TString temp_classes = Form("%s list_%i", drawClass.Data(), no);
      HtmlRow rowElement("", temp_classes, "display:none");
      rowElement.AddSimpleCells({"", Form("%i", i), classname});
      if (IsExacltyHalPackage(obj)) {
        classname = Form("Hal::Package  [%s] ",
                         ((Package*) obj)->GetName());  //</br>
        rowElement.AddContent(HtmlCell(classname));
      } else {
        rowElement.AddContent(HtmlCell(obj->GetName()));
      }
      rowElement.AddContent(HtmlCell(AddToUrl(list_dir2, HtmlCore::HTMLExtract(obj, i, list_path))));
      table.AddContent(rowElement);
    }
    fListDeep--;
  }

  void Package2HTML::CreateCutMonitorHTML(HtmlObject& table,
                                          Hal::ECutUpdate cut_upd,
                                          Int_t collection_no,
                                          Int_t& counter,
                                          TString path) {
    TString group_name, desc, names;
    group_name = GetGroupListName(cut_upd);
    switch (cut_upd) {
      case Hal::ECutUpdate::kEvent:
        desc  = "EventCollection";
        names = "Events";
        break;
      case Hal::ECutUpdate::kTrack:
        desc  = "TrackCollection";
        names = "Tracks";
        break;
      case Hal::ECutUpdate::kTwoTrack:
        desc  = "TwoTrackCollection";
        names = "Track TwoTracks";
        break;
      case Hal::ECutUpdate::kTwoTrackBackground:
        desc  = "TwoTrackBackgroundCollection";
        names = "Track TwoTracks Background";
        break;
      default: Cout::PrintInfo("Unhandled monitor group in HalPackage2HTML", Hal::EInfo::kLowWarning); break;
    }
    HtmlRow row;
    row.SetClass(Hal::HtmlTableRowClass::TitleStyle());
    HtmlCellCol cell(Form("Collection No %i [%s]", collection_no, names.Data()), 9);
    row.AddContent(cell);
    table.AddContent(row);

    TList* listA = (TList*) fCurrentCutContainer->GetObjectByName(group_name);
    if (listA == NULL) return;
    Package* subcontainer = (Package*) listA->At(collection_no);
    if (subcontainer == NULL) return;
    TList* list = (TList*) subcontainer->GetObjectByName("CutMonitorList");
    if (list == NULL) return;
    // fHTML<<HalHTML::GetHTMLRow(9,"No","Classname","CutName","Unit","AxisOption","CutMin","CutMax","Stacked</br>Img","Ex</br>Update")<<std::endl;
    if (list->GetEntries() > 0) {
      HtmlRow rowE;
      rowE.SetClass(Hal::HtmlTableRowClass::LegendStyle());
      rowE.AddSimpleCells(
        {"No", "ClassName", "CutName", "Unit", "AxisOption", "CutMin", "CutMax", "Stacked</br>Img", "Ex</br>Update"});
      table.AddContent(rowE);
      for (int i = 0; i < list->GetEntries(); i++) {
        CreateCutMonitorLink(table, cut_upd, (Package*) list->At(i), i, ++counter, path);
      }
    } else {
      HtmlRow rowE;
      rowE.SetClass(Hal::HtmlTableRowClass::DefStyle());
      HtmlCell cellE("Empty");
      cellE.SetColSpan(9);
      rowE.AddContent(cellE);
      table.AddContent(rowE);
    }
    //-------------two track cut table
  }

  void Package2HTML::DrawContainer(Package* pack,
                                   Int_t tiers_no,
                                   Int_t tier,
                                   Int_t collections_from_no,
                                   Int_t collections_to_no,
                                   Int_t layer,
                                   Int_t line_style,
                                   Int_t tier_jump) const {
    // UInt_t passedF =
    // ((ParameterULong64*)cutCont->GetObjectByName("PassedFast"))->GetValue();
    Double_t failedF           = GetULong(pack, "FailedFast");
    Double_t passedS           = GetULong(pack, "PassedSlow");
    Double_t failedS           = GetULong(pack, "FailedSlow");
    Double_t passed            = passedS;
    Double_t failed            = failedF + failedS;
    Double_t passRatio         = ((Double_t) passed) / ((Double_t(failed + passed)));
    ParameterInt* collectionno = (ParameterInt*) pack->GetObjectByName("CollectionID");
    Int_t collection_no        = collectionno->GetValue();
    Color_t passed_color       = kGreen + line_style * 2;
    Color_t failed_color       = kRed + line_style * 2;
    TString label              = Form("%i", collection_no);
    //---------- draw lines from lower level to higher level of collections
    if (tier_jump != 0) {
      TList* linkList;
      if (pack->Exist("NextObj"))
        linkList = (TList*) pack->GetObjectByName("NextObj");  // backward compatibility
      else
        linkList = (TList*) pack->GetObjectByName(Form("NextObj_%i", layer));
      if (!linkList) return;
      for (int i = 0; i < linkList->GetEntries(); i++) {
        Int_t collection_to = ((ParameterInt*) linkList->At(i))->GetValue();
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

  void Package2HTML::ExportCollections(HtmlObject& object, TString path) {
    if (fCurrentCutContainer == NULL) return;
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    TCanvas* c1 = new TCanvas("canv", "canv", fWindowWidth, fWIndowHeight);
    c1->Range(0, 0, fWindowWidth, fWIndowHeight);
    TList* listE        = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kEvent));
    TList* listT        = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kTrack));
    TList* listT2       = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kTwoTrack));
    TList* listT2B      = (TList*) fCurrentCutContainer->GetObjectByName(GetGroupListName(Hal::ECutUpdate::kTwoTrackBackground));
    Int_t eventCol      = 0;
    Int_t trackCol      = 0;
    Int_t ttrackCol     = 0;
    Int_t ttrackColBckg = 0;
    Int_t levelsNo      = 0;
    if (listE) {
      eventCol = listE->GetEntries();
      levelsNo++;
    }
    if (listT) {
      trackCol = listT->GetEntries();
      levelsNo++;
    }
    if (listT2) {
      ttrackCol = listT2->GetEntries();
      levelsNo++;
    }
    if (listT2B) { ttrackColBckg = listT2B->GetEntries(); }
    for (int i = 0; i < eventCol; i++) {
      auto pack    = (Package*) listE->At(i);
      int tierJump = 0;
      if (listT) tierJump = 1;
      DrawContainer(pack, levelsNo, 0, eventCol, trackCol, 0, 0,
                    tierJump);  // normal drawing without lines
    }
    for (int i = 0; i < trackCol; i++) {
      auto pack = (Package*) listT->At(i);
      if (listT2B) {
        DrawContainer(pack, levelsNo, 1, trackCol, ttrackColBckg, 1, 1,
                      1);  // draw lines to backround collections
      }
      if (listT2) {
        DrawContainer(pack, levelsNo, 1, trackCol, ttrackCol, 0, 0,
                      1);  // normal drawing with lines
      } else {
        DrawContainer(pack, levelsNo, 1, trackCol, ttrackCol, 0, 0,
                      0);  // normal drawing without lines
      }
    }
    for (int i = 0; i < ttrackColBckg; i++) {
      fBasicRadius += 10;
      DrawContainer((Package*) listT2B->At(i), levelsNo, 2, ttrackColBckg, 0, 0, 1,
                    0);  // draw without lines
      fBasicRadius -= 10;
    }
    for (int i = 0; i < ttrackCol; i++) {
      DrawContainer((Package*) listT2->At(i), levelsNo, 2, ttrackCol, 0, 0, 0, 0);  // draw only circles
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

  TString Package2HTML::GetLink1D(TH1* h1, TH1* h2, Int_t no, TString path) const {
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

    HtmlFile file(filename, kFALSE);
    file.AddStringContent(HtmlCore::GetJsDiv("cutmon.root", "cutmon;1"));
    file.Save();
    return HtmlCore::GetUrl(Form("th1_%i/histo.html", no), Form("2x%s", h1->ClassName()));
  }

  TString Package2HTML::GetLink2D(TH2* h1, TH2* h2, Int_t no, TString path) const {
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
    HtmlFile file(filename, kFALSE);
    file.AddStringContent(HtmlCore::GetJsDiv("cutmon.root", "cutmon;1"));
    file.Save();
    return HtmlCore::GetUrl(Form("th1_%i/histo.html", no), Form("2x%s", h1->ClassName()));
  }

  TString Package2HTML::GetLink3D(TH3* h1, TH3* h2, Int_t no, TString path) const {
    TString url;
    path = Form("%s/th1_%i", path.Data(), no);
    gSystem->mkdir(path);
    TString filename = Form("%s/histo.html", path.Data());
    TCanvas* c1      = new TCanvas("cutmon", "cutmon", 0, 0, 800, 600);
    c1->Divide(2, 1);
    c1->cd(1);
    h1->SetTitle(Form("%s (Passed)", h1->GetTitle()));
    h1->Draw(Draw_3D_option);
    c1->cd(2);
    h2->SetTitle(Form("%s (Failed)", h2->GetTitle()));
    h2->Draw(Draw_3D_option);
    c1->SetName("cutmon");
    c1->SaveAs(Form("%s/cutmon.root", path.Data()));
    delete c1;
    HtmlFile file(filename, kFALSE);
    file.AddStringContent(HtmlCore::GetJsDiv("cutmon.root", "cutmon;1"));
    file.Save();
    return HtmlCore::GetUrl(Form("th1_%i/histo.html", no), Form("2x%s", h1->ClassName()));

    /*
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
  return HtmlCore::GetUrl(Form("th1_%i/histo.html", no), Form("2x%s", h1->ClassName()));*/
  }

  TString Package2HTML::GetLinkToCut(Hal::ECutUpdate update, Int_t collection_no, Int_t cut_no, Bool_t fast) const {
    TString list_name          = GetGroupListName(update);
    TList* list                = (TList*) fCurrentCutContainer->GetObjectByName(list_name);
    Package* cut_sub_container = (Package*) list->At(collection_no);
    Int_t list_pos             = 0;
    TString pattern            = "CutList";
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

  TString Package2HTML::GetLinkToCutMonitor(Hal::ECutUpdate update, Int_t collection_no, Int_t no) const {
    TString list_name          = GetGroupListName(update);
    TList* list                = (TList*) fCurrentCutContainer->GetObjectByName(list_name);
    Package* cut_sub_container = (Package*) list->At(collection_no);
    Int_t list_pos             = 0;
    TString pattern            = "CutMonitorList";
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

  TString Package2HTML::GetGroupListName(Hal::ECutUpdate update) const {
    switch (update) {
      case Hal::ECutUpdate::kEvent: return "EventCutCollectionList"; break;
      case Hal::ECutUpdate::kTrack: return "TrackCutCollectionList"; break;
      case Hal::ECutUpdate::kTwoTrack: return "TwoTrackCutCollectionList"; break;
      case Hal::ECutUpdate::kTwoTrackBackground: return "TwoTrackBackgroundCutCollectionList"; break;
      default: return ""; break;
    }
  }

  Package2HTML::~Package2HTML() {
    if (fHTML) delete fHTML;
    if (fFile) {
      fFile->Close();
      delete fFile;
    }
  }

  void Package2HTML::DrawCircle(Double_t tiers_no,
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

  void Package2HTML::DrawLine(Double_t layers_no,
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

  void Package2HTML::GetCollectionsNumbers() {
    TString collection_name[4] = {
      "Event_collections_No", "Track_collections_No", "TwoTrack_collections_No", "TwoTrack_collections_background_No"};
    ParameterInt* event_collections_no    = (ParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[0], 0);
    ParameterInt* track_collections_no    = (ParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[1], 0);
    ParameterInt* twotrack_collections_no = (ParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[2], 0);
    ParameterInt* twotrack_collections_background_no =
      (ParameterInt*) fCurrentCutContainer->GetObjectByName(collection_name[3], 0);
    if (event_collections_no)
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEvent)] = event_collections_no->GetValue();
    else
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kEvent)] = 0;
    if (track_collections_no)
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrack)] = track_collections_no->GetValue();
    else
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTrack)] = 0;
    if (twotrack_collections_no)
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrack)] = twotrack_collections_no->GetValue();
    else
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrack)] = 0;
    if (twotrack_collections_background_no) {
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackground)] = twotrack_collections_background_no->GetValue();
    } else {
      fCollectionsNo[static_cast<Int_t>(Hal::ECutUpdate::kTwoTrackBackground)] = 0;
    }
  }

  TString Package2HTML::AddToUrl(TString add_dir, TString url) const {
    if (!url.Contains("href")) { return url; }
    Int_t start_url = url.First("=") + 2;
    TString first   = url(0, start_url);
    TString end     = url(start_url, url.Length());
    TString output  = Form("%s%s%s", first.Data(), add_dir.Data(), end.Data());
    output.ReplaceAll("//", "/");
    return output;
  }

  Double_t Package2HTML::GetDouble(const Package* pack, TString name) const {
    return ((ParameterDouble*) pack->GetObjectByName(name))->GetValue();
  }

  TString Package2HTML::GetString(const Package* pack, TString name) const {
    return ((ParameterString*) pack->GetObjectByName(name))->GetValue();
  }

  ULong64_t Package2HTML::GetULong(const Package* pack, TString name) const {
    return (ULong64_t)((ParameterULong64*) pack->GetObjectByName(name))->GetValue();
  }

  Bool_t Package2HTML::ExtractRunInfo() {
    HtmlCell runCell;
    TDirectory* global_dir = (TDirectory*) fFile->Get("HalInfo");
    if (!global_dir) return kFALSE;
    TString temp_dir = Form("%s/global_data", fDir.Data());
    gSystem->MakeDirectory(temp_dir);
    Package* pack            = (Package*) fFile->Get("HalInfo/RunInfo");
    ParameterString* version = GetString(pack, "Software ver");
    fSoftVer                 = Hal::Std::VersionId(version->GetValue());
    CreatePackageList(runCell, pack, eTableStyle::kMetaData, temp_dir, 2, "drawmerged");
    fHTML->AddStringContent(runCell.GetContent());
    return kTRUE;
  }

}  // namespace Hal
