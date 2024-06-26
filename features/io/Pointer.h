/*
 * VoidObject.h
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_IO_OBJECTVOID_H_
#define HAL_FEATURES_IO_OBJECTVOID_H_

#include "Object.h"
/**
 * because ROOT likes TObjects and do nasty things with pointers connected with TTree
 * I created this wrappers
 * NOTE - those classes manage a single pointers not arrays
 */
namespace Hal {
  template<class T>
  class Pointer : public Object {
    T* fPointer   = {nullptr};
    Bool_t fOwner = {kFALSE};

  public:
    Pointer() {};
    Pointer(T* obj, Bool_t owner) : fPointer(obj), fOwner(owner) {}
    T* GetPointer() const { return fPointer; }
    virtual ~Pointer() {
      if (fOwner && fPointer) delete fPointer;
    }
    ClassDef(Pointer, 0)
  };

  /**
   * stored double pointer to any object
   * @tparam T
   */
  template<class T>
  class DoublePointer : public Object {
    T** fPointer  = {nullptr};
    Bool_t fOwner = {kFALSE};

  public:
    DoublePointer() {};
    DoublePointer(T** obj, Bool_t owner) : fPointer(obj), fOwner(owner) {};
    /**
     *
     * @return pointer from double pointer
     */
    T* GetPointer() const { return *fPointer; }
    /**
     *
     * @return double pointer
     */
    T** GetDoublePointer() const { return fPointer; }
    /**
     * deletes double pointer but not the pointer linked to double pointer
     */
    void DeleteDoublePointer() {
      delete fPointer;
      fPointer = nullptr;
    }
    virtual ~DoublePointer() {
      if (fPointer && fOwner) {
        delete *fPointer;
        delete fPointer;
      }
    }
    ClassDef(DoublePointer, 0)
  };
  typedef Pointer<TObject> ObjectPointer;
  typedef DoublePointer<TObject> ObjectDoublePointer;
}  // namespace Hal

#endif /* HAL_FEATURES_IO_OBJECTVOID_H_ */
