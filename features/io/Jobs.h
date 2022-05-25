/*
 * HalJobs.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_HALJOBS_H_
#define HAL_FEATURES_HALJOBS_H_

#include <TObject.h>
#include <TString.h>
#include <vector>

/**
 * base class for job creation base on the input file that contains:
 * PARAMETERS
 * Here user define parameter name and parameter value, these values are later replaces
 * e.g. if submit command is "qsub ${MY_PARAMETER} then string ${MY_PARAMETER} is replated by value of parameter called \
 * MY_PARAMETER. Note: there are forbidden names of parameters used by the framework:
 * HAL::CONST::PWD - points to current directory
 * HAL::CONST::START - id of first job
 * HAL::CONST::END - id of last job
 * HAL::CONST::JOB_ID - id of current job (used when not in array mode)
 * HAL::CONST::JOB_FILE - path to job file
 * SETTINGS
 * the setting node contain instruction how to call the command that execut job file/ files, following attributes are supported:
 * * submit - command to submit script
 * * start  - id of first job
 * * end - id of last job
 * * array - "yes" if job is in array mode, "no" if job is in normal mode (many scripts are created)
 * * dir - path to the directory with job scripts
 * * shell - shell command - defile value of first line in job script
 * COMMANDS
 * This section define the commands in script file that is send to the cluster.
 */

namespace Hal {
  class Jobs : public TObject {
    Bool_t fArray;
    Bool_t fDebugCommands;
    TString fDir;
    TString fFile;
    TString fSubmitCommand;
    Int_t fStartJob, fEndJob;
    std::vector<TString> fCommands;
    std::vector<std::pair<TString, TString>> fParameters;
    static Int_t GetNVariablesTxt(TString textfile);
    static Int_t GetNVariablesXML(TString xmlfile);
    static TString GetParameterTxt(TString textfile, Int_t job, Int_t var);
    static TString GetParameterXml(TString xmlfile, Int_t job, Int_t var);

  public:
    Jobs(TString name);
    void DebugCommands() { fDebugCommands = kTRUE; }
    void CreateJobs();
    void SubmitJobs();
    /**
     * create dummy text file that can be used as pattern with parameters passed
     * to job
     * @param textfile
     * @param jobs
     * @param vars
     */
    static void CreateDummyTxtFile(TString textfile, Int_t jobs, Int_t vars);
    /**
     * create dummy text file that can be used as pattern with parameters passed
     * to job
     * @param textfile
     * @param jobs
     * @param vars
     */
    static void CreateDummyXMLFile(TString xmlfile, Int_t jobs, Int_t vars);
    /**
     *
     * @param textfile file with parameters
     * @return number of parameters in text with parameters
     */
    static Int_t GetNVariables(TString textfile);
    /**
     *
     * @param textfile file with job parameters
     * @param job_no number of job
     * @param var_no number of variable
     * @return return given variable from given job
     */
    static TString GetParameter(TString textfile, Int_t job_no, Int_t var_no);
    virtual ~Jobs();
    ClassDef(Jobs, 1)
  };
}  // namespace Hal

#endif /* HAL_FEATURES_HALJOBS_H_ */
