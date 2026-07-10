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
    std::map<TString, TString> fParameters;
    TString fTmpFile;
    std::array<int, 4> GetTime(TString raw) const;

  protected:
    struct ParPair {
      TString name;
      Bool_t enabled = {kFALSE};
    };
    ParPair fTime;
    ParPair fShell, fLogs, fErrors, fQueue;
    ParPair fMem, fMemPerCpu, fTasks, fCpuPerTask, fName;
    ParPair fSource;
    ParPair fExtra;
    ParPair fMerge;
    Bool_t fIsDependent = {kFALSE};
    Int_t fDependencyId = {-1};
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
    TString GetExport(TString val) const;
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
    virtual void Submit(Bool_t submit) = 0;
    /**
     * set this job as depended on job with given id
     * @param id
     */
    void SetDependency(Int_t id) {
      fDependencyId = id;
      fIsDependent  = kTRUE;
    }
    static JobQueue* GetInstance(TString xmlFile);
    virtual ~JobQueue() {};
    ClassDef(JobQueue, 0)
  };


} /* namespace Hal */

#endif /* HAL_FEATURES_IO_JOBQUEUE_H_ */
