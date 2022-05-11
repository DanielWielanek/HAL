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


namespace Hal {
  class IOManager;
  class RootIOManager;
  class Source : public TObject {
  protected:
    TString fFileName;

  public:
    Source(TString name = "");
    TString GetSourceName() const { return fFileName; };
    virtual IOManager* GetIOManager() const = 0;
    virtual ~Source();
    ClassDef(Source, 1)
  };
  class RootSource : public Source {
  protected:
    RootIOManager* fManager;

  public:
    RootSource(TString name = "");
    IOManager* GetIOManager() const;
    virtual ~RootSource();
    ClassDef(RootSource, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_BASE_STEER_HALSOURCE_H_ */
