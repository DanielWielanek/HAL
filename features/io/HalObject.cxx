/*
 * HalPack.cxx
 *
 *  Created on: 21-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalObject.h"

#include <TCollection.h>

HalObject::HalObject() {}

TString HalObject::GetExtractType() const { return "String"; }

TString HalObject::HTMLExtract(Int_t /*no*/, TString /*dir*/) const { return "NULL"; }

HalObject::~HalObject() {
  // TODO Auto-generated destructor stub
}

void HalObject::Add(const HalObject* /*pack*/) {}

Long64_t HalObject::Merge(TCollection* collection) {
  if (collection) {
    HalObject* pack = NULL;
    TIter iterator(collection);
    while ((pack = (HalObject*) iterator())) {
      Add(pack);
    }
  }
  return 1;
}
