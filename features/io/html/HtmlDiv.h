/*
 * HalHTMLDiv.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_HTML_HTMLDIV_H_
#define HAL_FEATURES_IO_HTML_HTMLDIV_H_

#include "HtmlObject.h"
namespace Hal {
  class HtmlDiv : public HtmlObject {
  public:
    HtmlDiv();
    HtmlDiv(TString id, TString className, TString style);
    TString ToString() const;
    virtual ~HtmlDiv();
    HtmlDiv(const HtmlDiv& other)            = default;
    HtmlDiv& operator=(const HtmlDiv& other) = default;
    ClassDef(HtmlDiv, 1)
  };
}  // namespace Hal
#endif /* HAL_FEATURES_IO_HTML_HTMLDIV_H_ */
