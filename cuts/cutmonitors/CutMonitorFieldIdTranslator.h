/*
 * CutMonitorTranslator.h
 *
 *  Created on: 4 sie 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUTMONITORFIELDIDTRANSLATOR_H_
#define HAL_CUTS_CUTMONITORS_CUTMONITORFIELDIDTRANSLATOR_H_

#include <TObject.h>

namespace Hal {
  /**
   * a little translation class to handle complex field monitors
   */
  class CutMonitorFieldIdTranslator : public TObject {
    Int_t fFieldIds[3] = {-1, -1, -1};

  public:
    CutMonitorFieldIdTranslator() {};
    void SetFieldId(Int_t val, Int_t idx) { fFieldIds[idx] = val; }
    void SwitchToRe();
    void SwitchToIm();
    Int_t GetFieldFull(Int_t id) const { return fFieldIds[id]; };
    virtual ~CutMonitorFieldIdTranslator() {};
    ClassDef(CutMonitorFieldIdTranslator, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUTMONITORFIELDIDTRANSLATOR_H_ */
