#include "Std.h"
#include "CompressionMap.h"
#include "Const.h"
#include "Cout.h"
#include "XMLNode.h"

#include <TCanvas.h>
#include <TClonesArray.h>
#include <TCollection.h>
#include <TDatime.h>
#include <TDirectory.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TPad.h>
#include <TROOT.h>
#include <TSeqCollection.h>
#include <TString.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TVector3.h>
#include <TVirtualPad.h>
#include <fstream>
#include <iostream>
#include <vector>

NamespaceImp(Hal::Std);

namespace {  // interal functions
  TString GetJsRoot5or6() {
    TString jsrootdir = gSystem->Getenv("JSROOT");
    TString rootdir   = gSystem->Getenv("ROOTSYS");
    TString coreName  = "JSRootCore.js";
    if (Hal::Std::GetJsRootVer() == 6) { coreName = "JSRoot.core.js"; }

    TString rootdirFair = rootdir + "/share/root/js/scripts/" + coreName;
    rootdir             = rootdir + "/js/scripts/" + coreName;
    rootdir.ReplaceAll("//", "/");
    std::cout << "PATH " << rootdir << std::endl;
    std::vector<TString> location;
    location.push_back(rootdirFair);
    location.push_back(rootdir);
    TString mainVal = "";
    if (jsrootdir.Length() > 0) { return jsrootdir; }

    for (auto place : location) {
      if (Hal::Std::FileExists(place)) {
        place.ReplaceAll("scripts/" + coreName, "");
        return place;
      }
    }
    Hal::Cout::PrintInfo("JSROOT not found!", Hal::EInfo::kError);
    return "";
  }

  TString GetJsRoot7() {
    TString jsrootdir = gSystem->Getenv("JSROOT");
    TString rootdir   = gSystem->Getenv("ROOTSYS");
    TString coreName  = "main.mjs";

    TString rootdirFair = rootdir + "/share/root/js/modules/" + coreName;
    rootdir             = rootdir + "/js/modules/" + coreName;
    rootdir.ReplaceAll("//", "/");
    std::cout << "PATH " << rootdir << std::endl;
    std::vector<TString> location;
    location.push_back(rootdirFair);
    location.push_back(rootdir);
    TString mainVal = "";
    if (jsrootdir.Length() > 0) { return jsrootdir; }

    for (auto place : location) {
      if (Hal::Std::FileExists(place)) {
        place.ReplaceAll("modules/" + coreName, "");
        return place;
      }
    }
    Hal::Cout::PrintInfo("JSROOT not found!", Hal::EInfo::kError);
    return "";
  }
}  // namespace

namespace Hal::Std {

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
      case Hal::ECutUpdate::kNo: return ""; break;
      case Hal::ECutUpdate::kEvent: return "Event"; break;
      case Hal::ECutUpdate::kTrack: return "Track"; break;
      case Hal::ECutUpdate::kTwoTrack: return "TwoTrack"; break;
      case Hal::ECutUpdate::kTwoTrackBackground: return "TwoTrackBackground"; break;
      default: return ""; break;
    }
  }

  TString GetConfigParameter(TString par_name) {
    TString home = gSystem->Getenv("HOME");
    Hal::XMLFile parser(Form("%s/.hal_config.xml", home.Data()));
    Hal::XMLNode* root       = parser.GetRootNode();
    std::vector<TString> arr = Hal::Std::ExplodeString(par_name, '/');
    Hal::XMLNode* node       = root->GetChild(arr[0]);
    for (int i = 1; i < (int) arr.size(); i++) {
      node = node->GetChild(arr[i]);
    }
    if (node == nullptr) {
      Hal::Cout::PrintInfo(Form("Node %s not found in hal configuration file", par_name.Data()), Hal::EInfo::kLowWarning);
      return "";
    }
    TString value = node->GetValue();
    return value;
  }

  std::vector<TString> GetListOfFiles(TString path, TString extension, Bool_t fullPath, Int_t recursive) {
    TString pathLocal, pathFull;
    TString pathPwd = gSystem->pwd();
    if (path == "") {
      pathLocal = ".";
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
          if (path.Length() != 0) {
            listOfFiles.push_back(Form("%s/%s", path.Data(), filename.Data()));
          } else {
            listOfFiles.push_back(filename);
          }
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

  std::vector<std::vector<TVirtualPad*>> GetGridPad(Int_t x_pads, Int_t y_pads, Float_t x_margin, Float_t y_margin) {
    TPad* padsav = (TPad*) gPad;
    if (gPad == NULL) {
      TCanvas* c = new TCanvas();
      c->cd();
      padsav = (TPad*) gPad;
    }
    std::vector<std::vector<TVirtualPad*>> array2d;
    ResizeVector2D(array2d, x_pads, y_pads);
    Double_t colls    = x_pads;
    Double_t rows     = y_pads;
    Double_t x_active = 1.0 - TMath::Abs(x_margin);
    Double_t y_active = 1.0 - TMath::Abs(y_margin);
    Double_t x_shift  = 1.0 / (colls - 1 + 1. / x_active);
    Double_t y_shift  = 1.0 / (rows - 1 + 1. / y_active);
    Double_t x_pad    = x_shift / x_active;
    Double_t y_pad    = y_shift / y_active;
    int glob          = 0;

    double x1, x2, y1, y2;
    auto trimPads = [&](double i, double j, double m1, double m2, double m3, double m4) {
      x1 = i * x_shift;
      x2 = x1 + x_pad;
      y2 = 1. - j * y_shift;
      y1 = y2 - y_pad;
      if (x1 < 0) x1 = 0;
      if (x1 > 1) x1 = 1;
      if (x2 < 0) x2 = 0;
      if (x2 > 1) x2 = 1;
      if (y1 < 0) y1 = 0;
      if (y1 > 1) y1 = 1;
      if (y2 < 0) y2 = 0;
      if (y2 > 1) y2 = 1;
      auto* pad = new TPad(Form("pad_%i_%i", int(i), int(j)), Form("pad_%i_%i", int(i), int(j)), x1, y1, x2, y2);
      pad->SetTopMargin(m1);
      pad->SetRightMargin(m2);
      pad->SetBottomMargin(m3);
      pad->SetLeftMargin(m4);
      pad->Draw();
      array2d[int(i)][int(j)] = pad;
      glob++;
      padsav->cd();
    };

    if (x_margin >= 0 && y_margin >= 0) {  // OK
      for (int i = x_pads - 1; i >= 0; i--) {
        for (int j = 0; j < y_pads; j++) {
          trimPads(i, j, 0, 0.0, y_margin, x_margin);
        }
      }
    }
    if (x_margin >= 0 && y_margin < 0) {  // ok
      y_margin = TMath::Abs(y_margin);
      for (int i = x_pads - 1; i >= 0; i--) {
        for (int j = y_pads - 1; j >= 0; j--) {
          trimPads(i, j, y_margin, 0, 0, x_margin);
        }
      }
    }

    if (x_margin < 0 && y_margin < 0) {  // ok
      y_margin = TMath::Abs(y_margin);
      x_margin = TMath::Abs(x_margin);
      for (int i = 0; i < x_pads; i++) {
        for (int j = y_pads - 1; j >= 0; j--) {
          trimPads(i, j, y_margin, x_margin, 0, 0);
        }
      }
    }

    if (x_margin < 0 && y_margin >= 0) {  // OK
      x_margin = TMath::Abs(x_margin);
      for (int i = 0; i < x_pads; i++) {
        for (int j = 0; j < y_pads; j++) {
          trimPads(i, j, 0, x_margin, y_margin, 0);
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

    Hal::Cout::PrintInfo("Hal plus not found!", Hal::EInfo::kError);
    return "";
  }

  TString GetJsRoot() {
    if (GetJsRootVer() == 7) { return GetJsRoot7(); }
    return GetJsRoot5or6();
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

  TVector3 CalculateBoostVector(Double_t energy_per_nucleon, Int_t n_proj, Int_t p_proj, Int_t n_tar, Int_t p_tar) {
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

  void CompressArray(TClonesArray* array, const CompressionMap& map) {
    for (int i = 0; i < map.GetSize(); i++) {
      if (map.GetNewIndex(i) == -1) { array->RemoveAt(i); }
    }
    array->Compress();
  }

  Int_t GetJsRootVer() {
    Int_t ver = ::ROOT::GetROOT()->GetVersionInt();
    if (ver < 62400) return 5;
    if (ver < 62812) return 6;
    return 7;
  }
}  // namespace Hal::Std
