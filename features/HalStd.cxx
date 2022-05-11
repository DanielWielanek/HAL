#include "HalStd.h"
#include "Const.h"
#include "Cout.h"
#include "XMLNode.h"

#include <TCanvas.h>
#include <TCollection.h>
#include <TDatime.h>
#include <TDirectory.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TPad.h>
#include <TSeqCollection.h>
#include <TString.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TVector3.h>
#include <TVirtualPad.h>
#include <fstream>
#include <iostream>
#include <vector>


NamespaceImp(HalStd);
namespace HalStd {

  void CopyFiles(TString from, TString to, Bool_t hidden) {
    TSystemFile file(from, from);
    TString full_name = file.GetName();
    full_name.ReplaceAll("//", "");
    if (full_name.EndsWith("/")) { full_name = TString(full_name(0, full_name.Length() - 1)); }
    TString name(full_name(full_name.Last('/'), full_name.Length()));
    if (name.BeginsWith(".") && hidden == kFALSE) return;
    if (name.EqualTo(".") || name.EqualTo("..")) return;
    if (file.IsDirectory()) {
      gSystem->mkdir(to);
      TSystemDirectory dir(from, from);
      TList* list = dir.GetListOfFiles();
      for (int i = 0; i < list->GetEntries(); i++) {
        TSystemFile* subfile = (TSystemFile*) list->At(i);
        TString subname      = subfile->GetName();
        if (subname.EqualTo(".") || subname.EqualTo("..")) continue;
        TString from_path = Form("%s/%s", from.Data(), subname.Data());
        TString to_path   = Form("%s//%s", to.Data(), subname.Data());
        CopyFiles(from_path, to_path);
      }
    } else {
      if (name.Contains("..")) return;
      if ((name.BeginsWith(".")) && hidden == kFALSE) return;
      gSystem->CopyFile(from, to);
    }
  }

  TString GetDate() {
    TDatime* time = new TDatime();
    Int_t day     = time->GetDay();
    Int_t month   = time->GetMonth();
    Int_t year    = time->GetYear();
    delete time;
    return Form("%04d-%02d-%02d", year, month, day);
  }

  TString GetTime() {
    TDatime* time = new TDatime();
    Int_t h       = time->GetHour();
    Int_t m       = time->GetMinute();
    delete time;
    return Form("%02d:%02d", h, m);
  }

  TString GetUniqueName(TString name) {
    TObject* obj = gDirectory->Get(name);
    if (obj == NULL) return name;
    Int_t id = 0;
    while (gDirectory->Get(Form("%s_%i", name.Data(), id))) {
      id++;
    }
    return Form("%s_%i", name.Data(), id);
  }

  TString UpdateEnumToString(Hal::ECutUpdate upd) {
    switch (upd) {
      case Hal::ECutUpdate::kNoUpdate: return ""; break;
      case Hal::ECutUpdate::kEventUpdate: return "Event"; break;
      case Hal::ECutUpdate::kTrackUpdate: return "Track"; break;
      case Hal::ECutUpdate::kTwoTrackUpdate: return "TwoTrack"; break;
      case Hal::ECutUpdate::kTwoTrackBackgroundUpdate: return "TwoTrackBackground"; break;
      default: return ""; break;
    }
  }
  TString GetConfigParameter(TString par_name) {
    TString home = gSystem->Getenv("HOME");
    Hal::XMLFile parser(Form("%s/.hal_config.xml", home.Data()));
    Hal::XMLNode* root       = parser.GetRootNode();
    std::vector<TString> arr = HalStd::ExplodeString(par_name, '/');
    Hal::XMLNode* node       = root->GetChild(arr[0]);
    for (int i = 1; i < (int) arr.size(); i++) {
      node = node->GetChild(arr[i]);
    }
    if (node == nullptr) {
      Hal::Cout::PrintInfo(Form("Node %s not found in hal configuration file", par_name.Data()), Hal::EInfo::kLessWarning);
      return "";
    }
    TString value = node->GetValue();
    return value;
  }

  std::vector<TString> GetListOfFiles(TString path, TString extension, Bool_t fullPath, Int_t recursive) {
    TString pathLocal, pathFull;
    TString pathPwd = gSystem->pwd();
    if (path == "") {
      pathLocal = "";
      pathFull  = Form("%s", pathPwd.Data());
    } else if (path.BeginsWith("/")) {
      pathLocal = path;
      pathFull  = path;
    } else {
      pathLocal = path;
      pathFull  = Form("%s/%s", pathPwd.Data(), pathLocal.Data());
    }
    if (path.EndsWith("/")) { path = path(0, path.Length() - 1); }
    if (pathLocal.EndsWith("/")) { pathLocal = pathLocal(0, pathLocal.Length() - 1); }
    if (pathFull.EndsWith("/")) { pathFull = pathFull(0, pathFull.Length() - 1); }

    TSystemDirectory* dir = new TSystemDirectory(pathLocal, pathLocal);
    TList* list           = dir->GetListOfFiles();
    std::vector<TString> listOfFiles;
    for (int i = 0; i < list->GetEntries(); i++) {
      TSystemFile* subfile = (TSystemFile*) list->At(i);
      TString filename     = subfile->GetName();
      if (filename == "." || filename == "..") continue;
      if (filename.EndsWith(Form(".%s", extension.Data())) && extension.Length()) {
        if (fullPath) {
          listOfFiles.push_back(Form("%s/%s", pathFull.Data(), filename.Data()));
        } else {
          listOfFiles.push_back(Form("%s/%s", path.Data(), filename.Data()));
        }
      }
      if (subfile->IsDirectory() && recursive != 0) {
        std::vector<TString> sublist =
          GetListOfFiles(Form("%s/%s", path.Data(), filename.Data()), extension, fullPath, recursive - 1);
        listOfFiles.insert(std::end(listOfFiles), std::begin(sublist), std::end(sublist));
      }
    }
    delete dir;
    delete list;
    return listOfFiles;
  }

  TVirtualPad*** GetGridPad(Int_t x_pads, Int_t y_pads, Float_t x_margin, Float_t y_margin) {
    TPad* padsav = (TPad*) gPad;
    if (gPad == NULL) {
      TCanvas* c = new TCanvas();
      c->cd();
      padsav = (TPad*) gPad;
    }
    TVirtualPad*** array2d = new TVirtualPad**[x_pads];
    for (int i = 0; i < x_pads; i++) {
      array2d[i] = new TVirtualPad*[y_pads];
    }
    Double_t colls   = x_pads;
    Double_t rows    = y_pads;
    Double_t x_shift = 1.0 / (colls - 1 + 1 / (1 - TMath::Abs(x_margin)));
    Double_t y_shift = 1.0 / (rows - 1 + 1 / (1 - TMath::Abs(y_margin)));
    Double_t x_pad   = x_shift / (1.0 - TMath::Abs(x_margin));
    Double_t y_pad   = y_shift / (1.0 - TMath::Abs(y_margin));
    int glob         = 0;
    if (x_margin >= 0 && y_margin >= 0) {  // OK
      for (int i = x_pads - 1; i >= 0; i--) {
        for (int j = 0; j < y_pads; j++) {
          Double_t x1 = i * x_shift;
          Double_t x2 = x1 + x_pad;
          Double_t y1 = 1 - j * y_shift;
          Double_t y2 = y1 - y_pad;
          if (x1 < 0) x1 = 0;
          if (x1 > 1) x1 = 1;
          if (x2 < 0) x2 = 0;
          if (x2 > 1) x2 = 1;
          if (y1 < 0) y1 = 0;
          if (y1 > 1) y1 = 1;
          if (y2 < 0) y2 = 0;
          if (y2 > 1) y2 = 1;

          TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
          pad->SetTopMargin(0);
          pad->SetRightMargin(0);
          pad->SetBottomMargin(y_margin);
          pad->SetLeftMargin(x_margin);
          pad->SetNumber(glob);
          pad->Draw();
          array2d[i][j] = pad;
          glob++;
          padsav->cd();
        }
      }
    }
    if (x_margin >= 0 && y_margin < 0) {  // ok
      y_margin = TMath::Abs(y_margin);
      for (int i = x_pads - 1; i >= 0; i--) {
        for (int j = y_pads - 1; j >= 0; j--) {
          Double_t x1 = i * x_shift;
          Double_t x2 = x1 + x_pad;
          Double_t y1 = 1 - j * y_shift;
          Double_t y2 = y1 - y_pad;
          if (x1 < 0) x1 = 0;
          if (x1 > 1) x1 = 1;
          if (x2 < 0) x2 = 0;
          if (x2 > 1) x2 = 1;
          if (y1 < 0) y1 = 0;
          if (y1 > 1) y1 = 1;
          if (y2 < 0) y2 = 0;
          if (y2 > 1) y2 = 1;
          TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
          pad->SetTopMargin(y_margin);
          pad->SetRightMargin(0);
          pad->SetBottomMargin(0);
          pad->SetLeftMargin(x_margin);
          pad->Draw();
          array2d[i][j] = pad;
          glob++;
          padsav->cd();
        }
      }
    }

    if (x_margin < 0 && y_margin < 0) {  // ok
      y_margin = TMath::Abs(y_margin);
      x_margin = TMath::Abs(x_margin);
      for (int i = 0; i < x_pads; i++) {
        for (int j = y_pads - 1; j >= 0; j--) {
          Double_t x1 = i * x_shift;
          Double_t x2 = x1 + x_pad;
          Double_t y1 = 1 - j * y_shift;
          Double_t y2 = y1 - y_pad;
          if (x1 < 0) x1 = 0;
          if (x1 > 1) x1 = 1;
          if (x2 < 0) x2 = 0;
          if (x2 > 1) x2 = 1;
          if (y1 < 0) y1 = 0;
          if (y1 > 1) y1 = 1;
          if (y2 < 0) y2 = 0;
          if (y2 > 1) y2 = 1;
          TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
          pad->SetTopMargin(y_margin);
          pad->SetRightMargin(x_margin);
          pad->SetBottomMargin(0);
          pad->SetLeftMargin(0);
          pad->Draw();
          array2d[i][j] = pad;
          glob++;
          padsav->cd();
        }
      }
    }

    if (x_margin < 0 && y_margin >= 0) {  // OK
      x_margin = TMath::Abs(x_margin);
      for (int i = 0; i < x_pads; i++) {
        for (int j = 0; j < y_pads; j++) {
          Double_t x1 = i * x_shift;
          Double_t x2 = x1 + x_pad;
          Double_t y1 = 1 - j * y_shift;
          Double_t y2 = y1 - y_pad;
          if (x1 < 0) x1 = 0;
          if (x1 > 1) x1 = 1;
          if (x2 < 0) x2 = 0;
          if (x2 > 1) x2 = 1;
          if (y1 < 0) y1 = 0;
          if (y1 > 1) y1 = 1;
          if (y2 < 0) y2 = 0;
          if (y2 > 1) y2 = 1;
          TPad* pad = new TPad(Form("pad_%i_%i", i, j), Form("pad_%i_%i", i, j), x1, y1, x2, y2);
          pad->SetTopMargin(0);
          pad->SetRightMargin(x_margin);
          pad->SetBottomMargin(y_margin);
          pad->SetLeftMargin(0);
          pad->Draw();
          array2d[i][j] = pad;
          glob++;
          padsav->cd();
        }
      }
    }
    return array2d;
  }

  TString GetHalrootPlus() {
    TString hal_var         = gSystem->Getenv("HAL");
    TString vmcworkdir      = gSystem->Getenv("VMCWORKDIR");
    TString main_dir        = hal_var;
    TString internalPathXml = "hal_plus/conversion_table/generators_patterns.xml";
    TString mainVal;
    std::vector<TString> location;
    if (hal_var.Length() == 0) {
      mainVal = vmcworkdir;
      location.push_back("%s/../share/");
      location.push_back("%s/../../share/");
      location.push_back("%s/external/Hal/features/");
      location.push_back("%s/hal/features/");
      location.push_back("%s/../external/hal/features/");
      location.push_back("%s/external/hal/features/");
    } else {
      location.push_back("%s/share/");
      location.push_back("%s/../hal/features/");
      mainVal = hal_var;
    }
    for (unsigned int i = 0; i < location.size(); i++) {
      TString make_install = Form(location[i].Data(), mainVal.Data());
      make_install         = make_install + internalPathXml;
      if (FileExists(make_install)) {
        make_install.ReplaceAll("conversion_table/generators_patterns.xml", "");
        return make_install;
      }
    }

    Hal::Cout::PrintInfo("Hal plus not found!", Hal::EInfo::kLessError);
    return "";
  }

  TString GetJsRoot() {
    TString hal_var    = gSystem->Getenv("HAL");
    TString vmcworkdir = gSystem->Getenv("VMCWORKDIR");
    TString jsrootdir  = gSystem->Getenv("JSROOT");
    std::vector<TString> location;
    TString mainVal = "";
    if (jsrootdir.Length() > 0)
      return jsrootdir;
    else if (hal_var.Length() > 0) {
      location.push_back("%s/share/jsroot/readme.md");
      location.push_back("%s/jsroot/readme.md");
      mainVal = hal_var;
    } else {
      location.push_back("%s/../jsroot/readme.md");
      location.push_back("%s/external/Hal/jsroot/readme.md");
      location.push_back("%s/external/jsroot/readme.md");
      location.push_back("%s/build/jsroot/readme.md");
      mainVal = vmcworkdir;
    }

    for (unsigned int i = 0; i < location.size(); i++) {
      TString make_install = Form(location[i].Data(), mainVal.Data());
      if (FileExists(make_install)) {
        make_install.ReplaceAll("readme.md", "");
        return make_install;
      }
    }
    Hal::Cout::PrintInfo("JSROOT not found!", Hal::EInfo::kLessError);
    return "";
  }

  Int_t VersionId(TString name) {
    TString year(name(3, 4));
    Int_t year_ver = year.Atoi();
    TString month(name(0, 3));
    Int_t month_ver       = 0;
    TString month_arr[12] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "sep", "oct", "nov", "dec"};
    for (int i = 0; i < 12; i++) {
      if (month.EqualTo(month_arr[i])) month_ver = i + 1;
    }
    return year_ver * 100 + month_ver;
  }

  TVector3 CalcualteBoostVector(Double_t energy_per_nucleon, Int_t n_proj, Int_t p_proj, Int_t n_tar, Int_t p_tar) {
    Double_t Np      = n_proj;
    Double_t Pp      = p_proj;
    Double_t Nt      = n_tar;
    Double_t Pt      = p_tar;
    Double_t mass1   = Hal::Const::NeutronMass() * Np + Hal::Const::ProtonMass() * Pp;
    Double_t mass2   = Hal::Const::NeutronMass() * Nt + Hal::Const::ProtonMass() * Pt;
    Double_t energy1 = energy_per_nucleon * (Np + Pp);
    Double_t energy2 = mass2;
    Double_t mom1    = TMath::Sqrt(energy1 * energy1 - mass1 * mass1);
    TLorentzVector vec1(0, 0, mom1, energy1), vec2(0, 0, 0, energy2);
    return -(vec1 + vec2).BoostVector();
  }
  Bool_t FileExists(TString path) {
    std::ifstream test_file;
    test_file.open(path);
    if (test_file.good()) {
      test_file.close();
      return kTRUE;
    } else {
      test_file.close();
      return kFALSE;
    }
  }
}  // namespace HalStd
