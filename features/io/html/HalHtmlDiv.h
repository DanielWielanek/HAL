/*
 * HalHTMLDiv.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_HTML_HALHTMLDIV_H_
#define HAL_FEATURES_IO_HTML_HALHTMLDIV_H_

#include "HalHtmlObject.h"
class HalHtmlDiv : public HalHtmlObject {
public:
  HalHtmlDiv();
  HalHtmlDiv(TString id, TString className, TString style);
  TString ToString() const;
  virtual ~HalHtmlDiv();
  HalHtmlDiv(const HalHtmlDiv& other) = default;
  HalHtmlDiv& operator=(const HalHtmlDiv& other) = default;
  ClassDef(HalHtmlDiv, 1)
};
#endif /* HAL_FEATURES_IO_HTML_HALHTMLDIV_H_ */
