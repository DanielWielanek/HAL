/*
 * hal_cmake.cpp
 *
 *  Created on: 13 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Std.h"
#include "StdString.h"
#include <TRegexp.h>
#include <TSystem.h>
#include <fstream>
#include <iostream>

/** \addtogroup hal-cmake
 * helper to building cuts and other class in HAL
 * @param argc not used
 * @param argv array of arguments
 * @return
 *
 *# basic usage
 *
 *this application can do following things:
 * hal-cmake --template=<TYPE> --name=<NAME> prepares a template header and source file for cut, the TYPE can be event-cut,
 * track-cut, twotrack-cut for given type of cut template e.g. hal-cmake --template=event-cut MyVertex create template for event
 * cut class called MyVertex
 * hal-cmake --dir=[LIBRARY_NAME] - create a template CMakeLists.txt file and LinkDef file for given directory, NOTE: the parser
 * trying to find class names but its's at early stage, so please check LinkDef file

 */


void PrepareLib(TString name);

void SetNames(TString& plusPath,
              TString& pattern,
              TString& toCopy,
              TString& headerGuard,
              TString& newGuard,
              TString& name,
              TString inputName);

void PrepareMonitor(TString name, TString type1, TString type2);

void UpdateLib();

void MakeLinkDef(TString name, std::vector<TString> sources);

std::vector<TString> ParseHeader(TString sourceName);

void PrepareCutClass(TString type, TString name);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "not enough params, type --help" << std::endl;
    return 0;
  }
  auto args = Hal::Std::ConvertMainArgs(argc, argv);
  if (args[0].first == "help") {  // print help
    std::cout << "options" << std::endl;
    std::cout << "--dir=[LIBRARYNAME] prepare cmake template for this directory" << std::endl;
    std::cout << "--updatedir update cmake SRC list and linkdef" << std::endl;
    std::cout << "--template=[TYPE] --name=[NAME] prepares template for class" << std::endl;
    std::cout << "\t TYPE = event-cut, track-cut, twotrack-cut for cuts" << std::endl;
    std::cout << "\t TYPE = event-monx, event-mony, event-monz, for x, xy, xyz, property monitors for events" << std::endl;
    std::cout << "\t TYPE = track-monx, track-mony, track-monz, for x, xy, xyz, property monitors for tracks" << std::endl;
    std::cout << "\t TYPE = twotrack-monx, twotrack-mony, twotrack-monz, for x, xy, xyz, property monitors for pairs"
              << std::endl;
    std::cout << "\tName = name of class e.g. hal_cmake --type=event-cut --name=Multiplicity " << std::endl;
  }
  if (args[0].first == "dir") {
    PrepareLib(args[0].second);
    return 1;
  }
  if (args[0].first == "updatedir") {
    UpdateLib();
    return 1;
  }
  if (args[0].first == "template") {
    auto vec = Hal::Std::ExplodeString(args[0].second, '-');
    if (vec.size() <= 1) return 0;
    if (vec[1].EqualTo("cut")) { PrepareCutClass(args[0].second, args[1].second); }
    if (vec[1].EqualTo("monx")) { PrepareMonitor(args[1].second, vec[0], "x"); }
    if (vec[1].EqualTo("mony")) { PrepareMonitor(args[1].second, vec[0], "y"); }
    if (vec[1].EqualTo("monz")) { PrepareMonitor(args[1].second, vec[0], "z"); }
  }


  return 1;
}

void PrepareLib(TString name) {
  TString plusPath    = Hal::Std::GetHalrootPlus();
  TString toCopy      = plusPath + "templates/CMakeLists.txt";
  TString tempFile[2] = {"temp.txt", "temp2.txt"};
  gSystem->CopyFile(toCopy, tempFile[0], kTRUE);

  Hal::Std::ReplaceInFile(tempFile[0], tempFile[1], "__HAL_NAME_OF_LINKDEF__", Form("%sLinkDef.h", name.Data()));
  Hal::Std::ReplaceInFile(tempFile[1], tempFile[0], "__HAL_NAME_OF_LIBRARY__", name);
  auto list  = Hal::Std::GetListOfFiles("", "cxx", kFALSE, 3);
  auto list2 = Hal::Std::GetListOfFiles("", "cpp", kFALSE, 3);

  for (auto i : list2)
    list.push_back(i);

  TString sources = "";
  for (auto i : list) {
    sources = sources + " " + i + "\n";
  }

  Hal::Std::ReplaceInFile(tempFile[0], tempFile[1], "__HAL_NAME_OF_CXX_FILES__", sources);
  gSystem->CopyFile(tempFile[1], "CMakeLists.txt", kTRUE);
  gSystem->Exec("rm temp.txt temp2.txt");
  MakeLinkDef(Form("%sLinkDef.h", name.Data()), list);
}

void UpdateLib() {
  std::ifstream cmake;
  cmake.open("CMakeLists.txt");
  if (!cmake.good()) {  // file cannot be parsed
    cmake.close();
    std::cout << "Cannto find CMakeLists.txt here" << std::endl;
    return;
  }
  TString temp;
  std::ofstream other;
  other.open("CMakeLists.temp");
  Bool_t listOpen = kFALSE;
  auto cppFiles   = Hal::Std::GetListOfFiles("", "cxx", kFALSE, 3);
  auto list2      = Hal::Std::GetListOfFiles("", "cpp", kFALSE, 3);

  for (auto i : list2)
    cppFiles.push_back(i);
  while (!temp.ReadLine(cmake, kFALSE).eof()) {
    if (temp.Contains("SRCS")) {
      other << temp << std::endl;
      listOpen = kTRUE;
      for (auto file : cppFiles) {
        other << file << std::endl;
      }
    }
    if (listOpen && temp.Contains(")")) listOpen = kFALSE;
    if (!listOpen) other << temp << std::endl;
  }
  gSystem->Exec("mv CMakeLists.temp CMakeLists.txt");
  auto headers = Hal::Std::GetListOfFiles("", "h", kFALSE, 1);
  TString linkdef;
  for (auto header : headers) {
    if (header.EndsWith("LinkDef.h")) {
      linkdef = header;
      break;
    }
  }
  if (linkdef.Length() == 0) {
    std::cout << "Cannot find linkdef !" << std::endl;
    return;
  }
  MakeLinkDef(linkdef, cppFiles);
};

void SetNames(TString& plusPath,
              TString& pattern,
              TString& toCopy,
              TString& headerGuard,
              TString& newGuard,
              TString& name,
              TString inputName) {
  pattern     = inputName;
  toCopy      = plusPath + "templates/" + inputName;
  headerGuard = "TEMPLATES_" + inputName;
  headerGuard.ToUpper();
  newGuard = name;
  newGuard.ToUpper();
}

void PrepareCutClass(TString type, TString name) {
  TString plusPath = Hal::Std::GetHalrootPlus();
  TString toCopy;
  TString pattern;
  TString headerGuard;
  TString newGuard;


  if (type.EqualTo("event-cut", TString::ECaseCompare::kIgnoreCase)) {
    SetNames(plusPath, pattern, toCopy, headerGuard, newGuard, name, "EventCutTemplate");
  }
  if (type.EqualTo("track-cut", TString::ECaseCompare::kIgnoreCase)) {
    SetNames(plusPath, pattern, toCopy, headerGuard, newGuard, name, "TrackCutTemplate");
  }
  if (type.EqualTo("twotrack-cut", TString::ECaseCompare::kIgnoreCase)) {
    SetNames(plusPath, pattern, toCopy, headerGuard, newGuard, name, "TwoTrackCutTemplate");
  }
  if (toCopy.Length() == 0) {
    std::cout << "Wrong flag in template type" << std::endl;
    return;
  }
  TString tempFile[2] = {"temp.txt", "temp2.txt"};
  gSystem->CopyFile(Form("%s.cxx", toCopy.Data()), tempFile[0]);
  Hal::Std::ReplaceInFile(tempFile[0], Form("%s.cxx", name.Data()), pattern, name);
  gSystem->CopyFile(Form("%s.h", toCopy.Data()), tempFile[0], kTRUE);
  Hal::Std::ReplaceInFile(tempFile[0], tempFile[1], pattern, name);  // replace classname
  Hal::Std::ReplaceInFile(tempFile[1], Form("%s.h", name.Data()), headerGuard, newGuard);
  gSystem->Exec("rm temp.txt temp2.txt");
}

std::vector<TString> ParseHeader(TString sourceName) {
  TString header = sourceName.ReplaceAll(".cpp", ".h");
  header         = sourceName.ReplaceAll(".cxx", ".h");
  std::ifstream file;
  file.open(header);
  std::vector<TString> res;
  if (!file.good()) {  // file cannot be parsed
    file.close();
    return res;
  }
  TString temp;
  auto replaceEverything = [](TString& str, std::initializer_list<TString> list) {
    for (auto i : list) {
      str = str.ReplaceAll(i, "");
    }
  };

  std::vector<std::pair<TString, int>> namespaceArray;
  Int_t bracket = 0;
  while (!temp.ReadLine(file).eof()) {
    TString line   = temp;
    Int_t bra      = line.CountChar('{');
    Int_t ket      = line.CountChar('}');
    Int_t dBracket = bra - ket;
    TString curNamespace;
    if (line.Contains("namespace") && !line.Contains("using")) {
      /**
       * not perfect methods to skip comments
       */
      Int_t npos  = line.First('n');
      Int_t nLast = -1;
      if (line.Contains("/*") || line.Contains("*/") || line.Contains("//")) { nLast = line.First('/'); }
      if (nLast != -1) {
        if (npos > nLast) {
          bracket += dBracket;
          continue;  // namespace after comment
        }
      }
      curNamespace = line;
      replaceEverything(curNamespace, {"namespace", "}", " ", "\t", "{"});
      if (curNamespace.Length()) {
        std::pair<TString, int> name(curNamespace, bracket + dBracket);
        Bool_t replaced = kFALSE;
        for (auto& i : namespaceArray) {
          if (i.second == name.second) {
            i        = name;  // replace existing namespace
            replaced = kTRUE;
            break;
          }
        }
        if (!replaced) { namespaceArray.push_back(name); }
      }
    }
    bracket += dBracket;

    if (line.Contains("ClassDef")) {  // we have a classdef
      replaceEverything(line, {"ClassDef(", ")", ";", " "});
      line                = line(0, line.Last(','));
      TString myNamespace = "";
      Int_t realBracket   = bracket - 1;  // usually we are at class brackets
      for (auto i : namespaceArray) {
        if (realBracket >= i.second) { myNamespace = myNamespace + "::" + i.first; }
      }
      if (myNamespace.BeginsWith("::")) { myNamespace = Hal::Std::RemoveNChars(myNamespace, 2, 'b'); }
      if (myNamespace.Length() > 0 && !myNamespace.EndsWith("::")) { myNamespace = myNamespace + "::"; }
      res.push_back(myNamespace + line);
    };
  }
  return res;
};

void MakeLinkDef(TString name, std::vector<TString> sources) {
  std::ofstream linkdef;
  linkdef.open(name);
  linkdef << "#ifdef __CINT__" << std::endl;
  linkdef << "" << std::endl;
  linkdef << "#pragma link off all globals;" << std::endl;
  linkdef << "#pragma link off all classes;" << std::endl;
  linkdef << "#pragma link off all functions;" << std::endl;
  linkdef << "" << std::endl;
  for (auto cppFIle : sources) {
    auto cppClasses = ParseHeader(cppFIle);
    for (auto singleClass : cppClasses) {
      linkdef << "#pragma link C++ class ";
      linkdef << singleClass << "+;" << std::endl;  // remove 4 last characters
    }
  }
  linkdef << "" << std::endl;
  linkdef << "#endif" << std::endl;
  linkdef.close();
};

void PrepareMonitor(TString name, TString type1, TString type2) {
  TString baseClassName;
  if (type2 == "x") {
    baseClassName = "PropertyMonitorTemplateX";
  } else if (type2 == "y") {
    baseClassName = "PropertyMonitorTemplateXY";
  } else if (type2 == "z") {
    baseClassName = "PropertyMonitorTemplateXYZ";
  } else {
    return;
  }
  TString cutUpdate;
  TString updateMethod;
  if (type1 == "event") {
    cutUpdate    = "Hal::ECutUpdate::kEvent";
    updateMethod = "  Hal::Event *event = (Hal::Event*)obj;";
  } else if (type1 == "track") {
    cutUpdate    = "Hal::ECutUpdate::kTrack";
    updateMethod = "  Hal::Track *track = (Hal::Track*)obj; ";
  } else if (type1 == "twotrack") {
    cutUpdate    = "Hal::ECutUpdate::kTwoTrack";
    updateMethod = "  Hal::TwoTrack *pair = (Hal::TwoTrack*)obj; ";
  } else {
    return;
  }
  TString plusPath = Hal::Std::GetHalrootPlus();
  TString toCopy;
  TString pattern;
  TString headerGuard;
  TString newGuard;
  SetNames(plusPath, pattern, toCopy, headerGuard, newGuard, name, baseClassName);
  gSystem->CopyFile(Form("%s.cxx", toCopy.Data()), Form("%s.cxx", name.Data()));
  gSystem->CopyFile(Form("%s.h", toCopy.Data()), Form("%s.h", name.Data()));
  Hal::Std::ReplaceInFile(Form("%s.h", name.Data()), Form("%s.h", name.Data()), {headerGuard, baseClassName}, {newGuard, name});
  // header
  Hal::Std::ReplaceInFile(Form("%s.cxx", name.Data()),
                          Form("%s.cxx", name.Data()),
                          {TString("__UPDATE_METHOD__"), TString("__UPDATE__"), baseClassName},
                          {updateMethod, cutUpdate, name});
  // header
}
