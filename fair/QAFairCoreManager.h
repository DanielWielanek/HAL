/*
 * QAFairCoreManager.h
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FAIR_QAFAIRCOREMANAGER_H_
#define HAL_FAIR_QAFAIRCOREMANAGER_H_

#include <TString.h>

class FairRunAna;
namespace Hal {

  class QAFairCoreManager {
  public:
    QAFairCoreManager();
    virtual FairRunAna* GetRunAna(TString outFile, TString simFile, TString recoFile = "", TString parFile = "");
    virtual ~QAFairCoreManager();
    ClassDef(QAFairCoreManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FAIR_QAFAIRCOREMANAGER_H_ */
