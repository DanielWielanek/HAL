/*
 * hal_report.cpp
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Cout.h"
#include "HtmlCore.h"
#include "Package2HTML.h"
#include "PackageSql.h"
#include "Std.h"

#include <RtypesCore.h>
#include <TString.h>
#include <TSystem.h>
#include <iostream>


/** \addtogroup hal-report
 *
 * @param argc
 * @param argv
 * @return
 *
 * Application for making job reports. Types of using:
 * - \b hal-report  data_file directory_path \b created a report in
 * directory_path
 * - \b hal-report data_file \b extract files to local database if there is no
 * database then program will crash There are two additional options :
 * - --online - extract files but uses jsroot from official site instead of
 * creation own copy of jsroot in directory_path, the size of report is much
 * smaller but report will not work offilne (some browsers might also block
 * jsroot files from external servers)
 * --gz - after extraction copy report compress it
 * \warning currently modern browser block reading the root files used in
 * jsroot, therefore after extraction to local directory all jsroot featchures
 * related to reading histograms do not work
 *
 */
int main(int argc, char* argv[]) {
  if (argc < 2) {
    Hal::Cout::PrintInfo("No arguments! run: hal-report --help to get help", Hal::EInfo::kCriticalError);
    return 0;
  }
  TString arg1 = argv[1];
  if (arg1 == "--help") {
    std::cout << " Usage:" << std::endl;
    std::cout << "hal-report inFile outFile [OPTIONS] to extract files to local file" << std::endl;
    std::cout << "hal-report inFile  to extract files to local database" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "--online - extract files but use jsroot from official site " << std::endl;
    std::cout << "--gz - compress archive to zip with using tar " << std::endl;
    return 1;
  }
  if (argc < 3) {
    Hal::Cout::PrintInfo("No output file, export to SQL", Hal::EInfo::kLowWarning);
    Hal::PackageSql* sql = new Hal::PackageSql(argv[1]);
    delete sql;
    return 0;
  }
  gSystem->Load("libTree");
  Bool_t zip = kFALSE;
  if (argc > 3) {
    for (int i = 3; i < argc; i++) {
      TString temp = argv[i];
      if (temp.EqualTo("--online")) Hal::HtmlCore::SetOnline(kTRUE);
      if (temp.EqualTo("--gz")) zip = kTRUE;
    }
  }
  Hal::Package2HTML* package = new Hal::Package2HTML(argv[1], argv[2]);
  delete package;
  if (zip) {
    TString command = Form("tar -czvf %s.tar.gz %s", argv[2], argv[2]);
    gSystem->Exec(command);
    gSystem->Exec(Form("rm -rf %s", argv[2]));
  }
  return 0;
}
