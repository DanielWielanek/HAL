/*
 * Source.h
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_STEER_HALSOURCE_H_
#define HAL_ANALYSIS_BASE_STEER_HALSOURCE_H_

#include <TObject.h>
#include <TString.h>

/**
 * class that represents the sources of data for AnalysisManager
 * source is responsible for reaction of IOManager
 * IOManager contains InputDataInfo - an array with input files (used if no external data manager is present)
 * IOManager is a member of DataManager that is responsible for I/O operations
 */

namespace Hal {
  class IOManager;
  class RootIOManager;
  class Source : public TObject {
  protected:
    IOManager* fManager = {nullptr};
    Source(IOManager* manager) : fManager(manager) {};

  public:
    /**
     * constructor
     * @param name name of file
     */
    Source(TString name = "") {};
    /**
     * add  file to analysis
     * @param friendName
     */
    virtual void AddFile(TString file = "");
    /**
     * add  file to analysis (friend file will be processed  together with other friends and this file
     * @param friendName
     */
    virtual void AddFriend(TString friendName, Int_t level);
    /**
     *
     * @return name of the source data (usually root file)
     */
    TString GetSourceName() const;
    /**
     *
     * @return IO Manager that is used to acces the data
     */
    virtual IOManager* GetIOManager() const;
    virtual ~Source();
    ClassDef(Source, 1)
  };
  /**
   * represents the source for root files with trees
   */
  class RootSource : public Source {
  protected:
  public:
    RootSource(TString name = "");
    virtual ~RootSource() {};
    ClassDef(RootSource, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_BASE_STEER_HALSOURCE_H_ */
