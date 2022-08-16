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
 */

namespace Hal {
  class IOManager;
  class RootIOManager;
  class Source : public TObject {
  protected:
    std::vector<TString> fFileName;

  public:
    /**
     * constructor
     * @param name name of file
     */
    Source(TString name = "");
    /**
     * add friend file to analysis (friend file will be processed  together with other friends and this file
     * @param friendName
     */
    virtual void AddFriend(TString /*friendName*/ = "") {};
    /**
     *
     * @return name of the source data (usually root file)
     */
    TString GetSourceName() const { return fFileName[0]; };
    /**
     *
     * @return IO Manager that is used to acces the data
     */
    virtual IOManager* GetIOManager() const = 0;
    virtual ~Source();
    ClassDef(Source, 1)
  };
  /**
   * represents the source for root files with trees
   */
  class RootSource : public Source {
  protected:
    RootIOManager* fManager;

  public:
    RootSource(TString name = "");
    virtual void AddFriend(TString friendName = "");
    IOManager* GetIOManager() const;
    virtual ~RootSource();
    ClassDef(RootSource, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_BASE_STEER_HALSOURCE_H_ */
