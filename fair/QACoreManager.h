/*
 * QAFairCoreManager.h
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FAIR_QACOREMANAGER_H_
#define HAL_FAIR_QACOREMANAGER_H_

#include <TString.h>

class FairRunAna;
namespace Hal {
  namespace Fair {
    class QACoreManager {
    public:
      QACoreManager();
      virtual FairRunAna* GetRunAna(TString outFile, TString simFile, TString recoFile = "", TString parFile = "");
      virtual ~QACoreManager();
      ClassDef(QACoreManager, 1)
    };
  }  // namespace Fair
} /* namespace Hal */

#endif /* HAL_FAIR_QACOREMANAGER_H_ */
