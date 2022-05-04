/*
 * HalHTMLHalHTMLDiv.cxx
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HalHtmlDiv.h"

#include <TString.h>
HalHtmlDiv::HalHtmlDiv() {}

TString HalHtmlDiv::ToString() const {
  TString div = "<div";
  div         = div + GetProperties() + ">";
  div         = div + GetContent() + "</div>";
  return div;
}

HalHtmlDiv::~HalHtmlDiv() {}

HalHtmlDiv::HalHtmlDiv(TString id, TString className, TString style) : HalHtmlObject(id, className, style) {}
