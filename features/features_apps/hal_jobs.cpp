#include "Cout.h"
#include "Jobs.h"
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
 * 1. use \b hal-jobs \b --prepare \b <batch_system> where batch_system specify the batch system used on
 * cluster, curennlty following batch system are supported torque (\b pbs_single  or \b pbs_array) and slurm
 * (\b sbatch_singe   or \b sbatch_array ). This script also creates directories for erros, logs etc.
 * 2. edit your xml configuration file - usually by modifying "job commands" section
 * 3a. create your job scripts by \b hal-jobs \b --submit \b <xml_configuration_file>  \b--stage=prepare, this command create job
 * files that will be sent to batch system 3b. send your job scripts by \b hal-jobs \b --sumbit \b <xml_configuration_file> \b
 * --stage=deploy
 * 4. alternatively instead of calling 3a and 3b you can prepare & submit you jobs by \b hal-jobs \b <xml_configuration_file>
 *
 *# structure of input file
 * ## settings - main settings of jobs
 * 1. submit - the submit command - the command used to submit job file
 * 2. submit_start the id of first job
 * 3. submit_end the id of last job
 * 4. shell - the shell, first line in script sent to the computing cluster
 * ## job_options
 * Options of the jobs. In principle they are added to the scripts just like commands.
 * Here use can also specify \b array \b attribute, if it set to "yes" only one job file is prepared and sent to the cluster -
 this is so called
 * job array, overwrite many separate jobs after send. The size of array/number of jobs is defined by \b submit_start and \b
 submit_end .
 * ## job_commands
 * The lines that will be written to the job file,in principle they are commands executed by a job script..
 * \note Some of job_commands should not be changed e.g.
 * \ export JOB_ID_HAL=`expr $SLURM_ARRAY_TASK_ID + $ONE` in sbatch_array is the only way to get unique_id for each job in array

 * # export option
 * hal jobs has the option \b --export=<data_file> --id=<id> --par=<par_id> returns the parameter number \b par_id \b for job
 number \b par_id from \b data_file.
 * The data file might look like this.
 * \verbatim
=======_HAL_JOBS_INPUT_FILE_=======
NVAR 2 NJOBS 3
job_1 data_1
job_2 data_2
job_3 data_3
 \endverbatim
 In this case we have two variables defined for 3 jobs, \b hal-jobs \b --export file \b--id=1 \b--par=2 returns data_2
 *# special flags
 * There are some specjal flags, if they are present in xml file they will be replaced before creation of job file, following
special flags
 * are present:
 * 1. ${HAL_PWD} path to current directory
 * 2. ${HAL_START_ARRAY} value in \b submit_start
 * 3. ${HAL_END_ARRAY} value in \b submit_end
 * 4. ${HAL_JOB_FILE} relative path to job file(files) usually jobs/job_array (for arrays) or jobs/job_i for i-th file
 * 5. ${HAL_JOB_ID} the id for given file (only for sending many small jobs - doesn't work for an array
 *
 * @see HalJobs::CreateDummyTxtFile  @see HalJobs::CreateDummyXMLFile
 * @HalJobs
 */

void printHelp() {
  std::cout << "Class for managing job arrays/job groups" << std::endl;
  std::cout << " Usage:" << std::endl;
  std::cout << "\thal-jobs [file] where file is name of xml file with settings" << std::endl;
  std::cout << "\thal-jobs --debug=[file] where file is name of xml file with settings, submit commands are displayed"
            << std::endl;
  std::cout << "hal-jobs --submit=[Y] --stage=[X]    prepares jobs" << std::endl;
  std::cout << "\t Y - name of  the files with configured jobs " << std::endl;
  std::cout << "\t X stage of deploying:" << std::endl;
  std::cout << "\t create - only creates job file(s)" << std::endl;
  std::cout << "\t deploy - only submit job(s)" << std::endl;
  std::cout << "--export=[X] --id=[Y] --par=[Z]  - prints the name of the value from "
               "configuration file"
            << std::endl;
  std::cout << "X - configuration parm, Y job id, par parameter number" << std::endl;
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
      Hal::Jobs jobs(val1);
      jobs.CreateJobs();
      jobs.SubmitJobs();
      return 0;
    } else if (flag1 == "debug" && val1.EndsWith(".xml")) {
      Hal::Jobs jobs(val1);
      jobs.DebugCommands();
      jobs.CreateJobs();
      jobs.SubmitJobs();
      return 0;
    }
  } else if (nArgs == 2) {
    TString flag1 = args[0].first;
    TString val1  = args[0].second;
    TString flag2 = args[1].first;
    TString val2  = args[1].second;
    if (flag1 == "submit" && val2 == "create") {
      Hal::Jobs jobs(val1);
      jobs.CreateJobs();
      return 0;
    } else if (flag1 == "submit" && val2 == "deploy") {
      Hal::Jobs jobs(val1);
      jobs.SubmitJobs();
      return 0;
    }
  } else if (nArgs == 3) {
    TString flag1 = args[0].first;
    TString flag2 = args[1].first;
    TString flag3 = args[2].first;
    TString val1  = args[0].second;
    TString val2  = args[1].second;
    TString val3  = args[2].second;
    if (flag1 == "export" && flag2 == "id" && flag3 == "par") {
      return 0;
      TString job_id       = val1;
      TString parameter_id = val2;
      TString textfile     = val3;
      std::cout << Hal::Jobs::GetParameter(textfile, job_id.Atoi(), parameter_id.Atoi()) << std::endl;
      return 0;
    }
  }
  std::cout << "Uknown parameter try --help" << std::endl;
  return 1;
}
