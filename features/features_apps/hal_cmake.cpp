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
#include <TSystem.h>
#include <fstream>
#include <iostream>

void PrepareLib(TString name);

void PrepareClass(TString type, TString name);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "not enough params, type --help" << std::endl;
    return 0;
  }
  auto args = Hal::Std::ConvertMainArgs(argc, argv);
  if (args[0].first == "help") {  // print help
    std::cout << "options" << std::endl;
    std::cout << "--dir=[LIBRARYNAME] prepare cmake template for this directory" << std::endl;
    std::cout << "--template=[TYPE] --name=[NAME] prepares template for class" << std::endl;
    std::cout << "\t TYPE = event-cut, track-cut, twotrack-cut " << std::endl;
    std::cout << "\tName = name of class e.g. hal_cmake --type=event-cut --name=Multiplicity " << std::endl;
  }
  if (args[0].first == "dir") {
    PrepareLib(args[0].second);
    return 1;
  }
  if (args[0].first == "template") { PrepareClass(args[0].second, args[1].second); }


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
  std::ofstream linkdef;
  linkdef.open(Form("%sLinkDef.h", name.Data()));
  linkdef << "#ifdef __CINT__" << std::endl;
  linkdef << "" << std::endl;
  linkdef << "#pragma link off all globals;" << std::endl;
  linkdef << "#pragma link off all classes;" << std::endl;
  linkdef << "#pragma link off all functions;" << std::endl;
  linkdef << "" << std::endl;
  for (auto i : list) {
    linkdef << "#pragma link C++ class ";
    linkdef << Hal::Std::RemoveNChars(i, 4, 'e') << "+;" << std::endl;  // remove 4 last characters
  }
  linkdef << "" << std::endl;
  linkdef << "#endif" << std::endl;
  linkdef.close();
}

void PrepareClass(TString type, TString name) {
  TString plusPath = Hal::Std::GetHalrootPlus();
  TString toCopy;
  TString pattern;
  TString headerGuard;
  TString newGuard;
  auto setNames = [&plusPath, &pattern, &toCopy, &headerGuard, &newGuard, &name](TString inputName) {
    pattern     = inputName;
    toCopy      = plusPath + "templates/" + inputName;
    headerGuard = "TEMPLATES_" + inputName;
    headerGuard.ToUpper();
    newGuard = name;
    newGuard.ToUpper();
  };

  if (type.EqualTo("event-cut", TString::ECaseCompare::kIgnoreCase)) { setNames("EventCutTemplate"); }
  if (type.EqualTo("track-cut", TString::ECaseCompare::kIgnoreCase)) { setNames("TrackCutTemplate"); }
  if (type.EqualTo("twotrack-cut", TString::ECaseCompare::kIgnoreCase)) { setNames("TwoTrackCutTemplate"); }
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
