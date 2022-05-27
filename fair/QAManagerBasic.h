/*
 * QAFairCoreManager.h
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FAIR_QAMANAGERBASIC_H_
#define HAL_FAIR_QAMANAGERBASIC_H_

#include "QAManager.h"
#include <TString.h>

class FairRunAna;
namespace Hal {
  namespace Fair {
    class QAManagerBasic : public ::Hal::QAManager {
    public:
      QAManagerBasic();
      virtual FairRunAna* GetRunAna(TString outFile, TString simFile, TString recoFile = "", TString parFile = "");
      virtual ~QAManagerBasic();
      ClassDef(QAManagerBasic, 1)
    };
  }  // namespace Fair
} /* namespace Hal */

#endif /* HAL_FAIR_QAMANAGERBASIC_H_ */
