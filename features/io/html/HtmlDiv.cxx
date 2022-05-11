/*
 * HalHTMLHalHTMLDiv.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HtmlDiv.h"

#include <TString.h>
namespace Hal {
  HtmlDiv::HtmlDiv() {}

  TString HtmlDiv::ToString() const {
    TString div = "<div";
    div         = div + GetProperties() + ">";
    div         = div + GetContent() + "</div>";
    return div;
  }

  HtmlDiv::~HtmlDiv() {}

  HtmlDiv::HtmlDiv(TString id, TString className, TString style) : HtmlObject(id, className, style) {}
}  // namespace Hal
