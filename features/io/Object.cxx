/*
 * HalPack.cxx
 *
 *  Created on: 21-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Object.h"
#include "Painter.h"

#include <iostream>

#include <TCollection.h>

namespace Hal {

  Object::Object() {}

  TString Object::GetExtractType() const { return "String"; }

  TString Object::HTMLExtract(Int_t /*no*/, TString /*dir*/) const { return "NULL"; }

  Object::~Object() {}

  void Object::Add(const Object* /*pack*/) {}

  Long64_t Object::Merge(TCollection* collection) {
    if (collection) {
      Object* pack = nullptr;
      TIter iterator(collection);
      while ((pack = (Object*) iterator())) {
        Add(pack);
      }
    }
    return 1;
  }

  void DrawableObject::Draw(Option_t* option) {
    if (!fPainter) fPainter = MakePainter();  // try to make a painter
    if (fPainter) {
      TString options = option;
      fPainter->SetOption(options);
      fPainter->Paint();
    }
  }

  void DrawableObject::cd() {
    if (!fPainter) {
      std::cout << ClassName() << " has no painter " << std::endl;
    } else
      fPainter->cd();
  }

  DrawableObject::DrawableObject(const DrawableObject& other) : Object(other) { fPainter = nullptr; }

  DrawableObject& DrawableObject::operator=(const DrawableObject& b) {
    if (this == &b) return *this;
    TNamed::operator=(b);
    fPainter = nullptr;
    return *this;
  }

  DrawableObject::~DrawableObject() {
    if (fPainter) delete fPainter;
  }

}  // namespace Hal
