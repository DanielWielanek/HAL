/*
 * JobManager.h
 *
 *  Created on: 16 sty 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_IO_JOBQUEUE_H_
#define HAL_FEATURES_IO_JOBQUEUE_H_

#include <RtypesCore.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <array>
#include <map>
#include <vector>

#include "XMLNode.h"

namespace Hal {
  class XMLFile;
  class JobQueue : public TObject {
  protected:
  private:
    Bool_t fDebugCommands = {kFALSE};
    Int_t fStartJob       = {0};
    Int_t fEndJob         = {0};
    Bool_t fArray         = {kFALSE};
    Bool_t fDirectCommand = {kFALSE};
    std::vector<TString> fCommands;
    std::vector<TString> fFiles;
    std::vector<TString> fTrees;
    std::map<TString, TString> fParameters;
    TString fTmpFile;
    std::array<int, 4> GetTime(TString raw) const;

  protected:
    static const TString fgJobIdName;
    static const TString fgJobTotalNoName;
    static const TString fgDataset;
    struct ParPair {
      TString name;
      Bool_t enabled = {kFALSE};
    };
    ParPair fTime;
    ParPair fShell, fLogs, fErrors, fQueue;
    ParPair fMem, fMemPerCpu, fTasks, fCpuPerTask, fName;
    ParPair fSource, fExtra, fMerge, fStartDir;
    Int_t fDependencyId = {-1};
    void LoadParams(Hal::XMLNode& node);
    /**
     * return parameter from sheduler
     * @param name
     * @return
     */
    ParPair GetParameter(TString name) const;
    /**
     *  sends command
     * @param send if true send command
     * @param command command to send
     * @param jobID job id
     */
    void SendCommand(Bool_t send, TString command, Int_t jobID) const;
    /**
     * loads list of input files
     */
    void LoadDataset(const Hal::XMLNode& node);
    /**
     * loads start/end id's
     * @param node
     */
    void LoadStartEnd(const Hal::XMLNode& node);
    /**
     * first job ID
     * @return
     */
    Int_t GetStart() const { return fStartJob; }
    /**
     * last job ID
     * @return
     */
    Int_t GetEnd() const { return fEndJob; }
    /**
     *
     * @return true if send parameters of job by submit command
     */
    Bool_t IsDirect() const { return fDirectCommand; };
    /**
     *
     * @return true if job array requested
     */
    Bool_t IsArray() const { return fArray; }
    /**
     *
     * @return name of temporary file used to script generation
     */
    TString GetTmpFile() const { return fTmpFile; }
    /**
     *
     * @param shell
     * @param val
     * @return export parameter for export val in shell with given shell name (bash, sh, tcsh, zsh)
     */
    TString GetExport(TString val, TString variable = fgJobIdName) const;
    /**
     * return source command for given shell
     * @param shell
     * @param val
     * @return
     */
    TString GetSource(TString val) const;
    /**
     *
     * @return array of commands
     */
    std::vector<TString> GetCommands() const { return fCommands; }
    /**
     *
     * @return common parameters nodes
     */
    static std::vector<XMLNode> GenerateCommonParameters();
    /**
     *
     * @param raw path to log files
     * @return name of log path/flag if need to be modified by job
     */
    virtual TString GetLogPath(TString raw) const = 0;
    /**
     *
     * @param raw path to error files
     * @return name of error path/flag if needed to be modified
     */
    virtual TString GetErrorPath(TString raw) const = 0;
    /**
     *
     * @param raw memory required (in MB)
     * @return memory parameter if need to be modified
     */
    virtual TString GetMemoryFlags(TString raw) const = 0;
    /**
     *
     * @param time time [days,hours,minutes,seconds]
     * @return flag used to setup time flag
     */
    virtual TString GetTimeFlag(std::array<int, 4> time) const = 0;
    /**
     * load main parameters of jobs
     * @param file
     */
    virtual void LoadCore(TString file);
    /**
     * build temporary files (those that will be submitted)
     */
    virtual void MakeJobFiles() const;
    /**
     * create command with parameters used to send job
     * @param jobID if negative its for array
     * @return arguments to send e.g. qsub script.sh
     */
    virtual TString MakeSubmitCommand(Int_t jobID) const = 0;
    /**
     * creates single file taht will be send, if jobid =-1 this is for array
     * @param jobID if negative its for array
     */
    virtual void MakeJobFile(Int_t jobId) const = 0;
    /**
     * make list of files in tmpdir/dataset.txt file
     */
    void MakeDatasetList() const;
    /**
     * make commands to setup the dataset
     */
    void MakeDatasetCommands();


  public:
    JobQueue();
    /**
     * debug mode ( print commands)
     */
    void EnableDebug() { fDebugCommands = kTRUE; }
    /**
     * build example xml file
     * @param name
     */
    static void BuildExample(TString name);
    /**
     * submit command
     * @param submit if false do not submit
     */
    virtual void Submit(Bool_t submit);
    /**
     * set this job as depended on job with given id
     * @param id
     */
    void SetDependency(Int_t depid) { fDependencyId = depid; }
    static JobQueue* GetInstance(TString xmlFile);
    /**
     *
     * @return job id (by looking in HAL_JOB_ID env variable, return -1 if no variable found
     */
    static Int_t GetJobId();
    /**
     *
     * @return total number of jobs (works in arrays only)
     */
    static Int_t GetTotalJobs();
    /**
     *
     * @return path to dataset (works in arrays only)
     */
    static TString GetDatasetPath();
    virtual ~JobQueue() {};
    ClassDef(JobQueue, 0)
  };


} /* namespace Hal */

#endif /* HAL_FEATURES_IO_JOBQUEUE_H_ */
