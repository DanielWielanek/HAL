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
 * structure of file:
 * - scheduler - type of sheduler (sbatch for slurm or pbs for torque)
 * - parameters list of parameters in "parameter" node, each parameter has given name and value, list of names:
 *   - queue - partition type
 *   - error - path for errors if emtpy, discard error logs
 *   - log - path to log files (discard if empty)
 *   - shell - shell type in first line of job script (e.g. #!/bin/bash)
 *   - start - id of first job
 *   - end - id of last job
 *   - name - name of job
 *   - array - if "yes" send job as array, otherwise generates n-jobs
 *   - time - wall time, notation D:H:M:S
 *   - direct - if yes send job parameters directly to command e.g. sbatch --time, otherwise pass parameter inside of script e.g.
 *#SBATCH welltime
 *   - extra - pass extra command to submit command
 *   - tmpfile - path to directory with jobs (by default hal_jobs directory is used)
 *   - source - path to script that setup the env variables
 *   - ram_per_cpu - memory per cpu
 *   - cpu_per_task
 *   - ram - max memory per job
 *   - merge - path to another sheduler xml file, this file will be executed when previous jobs are done (works only for array)
 * - command - list of commands in "command" nodes
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
  printLine("hal-jobs --dependency=<id>  - send dependency job (not to use by user)");
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
    } else if (flag1 == "dep") {
      auto job = Hal::JobQueue::GetInstance(val1);
      job->EnableDebug();
      std::cout << job->ClassName() << std::endl;
      job->SetDependency(val1.Atoi());
      job->Submit(kFALSE);
      delete job;
    } else if (flag1 == "example") {
      Hal::JobQueue::BuildExample(val1);
      return 0;
    }
  }
  std::cout << "Unknown parameter try --help" << std::endl;
  return 1;
}
