#include "Cout.h"
#include "JobQueue.h"
#include "Std.h"
#include "StdString.h"

#include <TString.h>
#include <TSystem.h>
#include <iostream>
#include <vector>


/** \addtogroup hal-jobs
 * application for job management
 * @param argc not used
 * @param argv array of arguments
 * @return
 *
 *# basic usage
 *
 * This is a application to submit jobs from XML files. In principle this should be used in following way:
 *
 * create xml file
 * call hal-jobs <file>
 *
 *
 * @see HalJobs::CreateDummyTxtFile  @see HalJobs::CreateDummyXMLFile
 * @HalJobs
 */
void printLine(TString line) { std::cout << line << std::endl; }

void printHelp() {
  printLine("Class for managing job arrays/job groups");
  printLine("To submit job just call hal-jobs <file>");
  printLine("hal-jobs --debug=<file>  - debug scheduler");
  printLine("hal-jobs --example=<file>  - create example xml");
  printLine("hal-jobs --help - print help");
}
int main(int argc, char* argv[]) {
  if (argc < 2) {
    Hal::Cout::PrintInfo("No arguments! run: hal-jobs --help to get help", Hal::EInfo::kCriticalError);
    return 0;
  }
  std::vector<std::pair<TString, TString>> args = Hal::Std::ConvertMainArgs(argc, argv);

  Int_t nArgs = args.size();
  if (nArgs == 1) {
    TString flag1 = args[0].first;
    TString val1  = args[0].second;
    if (flag1 == "help") {
      printHelp();
      return 0;
    } else if (flag1 == "" && val1.EndsWith(".xml")) {
      auto job = Hal::JobQueue::GetInstance(val1);
      job->Submit(kTRUE);
      delete job;
      return 0;
    } else if (flag1 == "debug" && val1.EndsWith(".xml")) {
      auto job = Hal::JobQueue::GetInstance(val1);
      job->EnableDebug();
      std::cout << job->ClassName() << std::endl;
      job->Submit(kFALSE);
      delete job;
      return 0;
    } else if (flag1 == "example") {
      Hal::JobQueue::BuildExample(val1);
      return 0;
    }
  }
  std::cout << "Unknown parameter try --help" << std::endl;
  return 1;
}
