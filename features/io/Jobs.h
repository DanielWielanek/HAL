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

namespace Hal {
  class Jobs : public TObject {
    Bool_t fArray;
    Bool_t fDebugCommands;
    TString fFile;
    TString fSubmitCommand;
    Int_t fStartJob, fEndJob;
    std::vector<TString> fCommands;
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
