/*
 * VoidObject.h
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_IO_OBJECTVOID_H_
#define HAL_FEATURES_IO_OBJECTVOID_H_

#include "Object.h"
namespace Hal {
  template<class T>
  class ObjectVoid : public Object {
    T* fPointer   = {nullptr};
    Bool_t fOwner = {kFALSE};

  public:
    ObjectVoid() {};
    ObjectVoid(T* obj, Bool_t owner) : fPointer(obj), fOwner(owner) {}
    T* GetCore() const { return fPointer; }
    virtual ~ObjectVoid() {
      if (fOwner && fPointer) delete fPointer;
    }
    ClassDef(ObjectVoid, 0)
  };

  template<class T>
  class ObjectVoid2 : public Object {
    T** fPointer  = {nullptr};
    Bool_t fOwner = {kFALSE};

  public:
    ObjectVoid2();
    ObjectVoid2(T** obj, Bool_t owner) : fPointer(obj), fOwner(owner) {};
    T* GetPointer() const { return fPointer[0]; }
    virtual ~ObjectVoid2() {
      if (fPointer && fOwner) {
        delete fPointer[0];
        delete[] fPointer;
      }
    }
    ClassDef(ObjectVoid2, 0)
  };
}  // namespace Hal

#endif /* HAL_FEATURES_IO_OBJECTVOID_H_ */
