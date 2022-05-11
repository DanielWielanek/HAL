/*
 * HalPack.cxx
 *
 *  Created on: 21-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Object.h"

#include <TCollection.h>

namespace Hal {

  Object::Object() {}

  TString Object::GetExtractType() const { return "String"; }

  TString Object::HTMLExtract(Int_t /*no*/, TString /*dir*/) const { return "NULL"; }

  Object::~Object() {
    // TODO Auto-generated destructor stub
  }

  void Object::Add(const Object* /*pack*/) {}

  Long64_t Object::Merge(TCollection* collection) {
    if (collection) {
      Object* pack = NULL;
      TIter iterator(collection);
      while ((pack = (Object*) iterator())) {
        Add(pack);
      }
    }
    return 1;
  }
}  // namespace Hal
