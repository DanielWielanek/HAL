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
  class AbstractPointer : public Object {
  public:
    AbstractPointer() {};
    virtual AbstractPointer* MakeCopy() const = 0;
    virtual Bool_t IsTObject() const { return kFALSE; };
    virtual TString GetStoredClassName() const { return ""; }
    virtual ~AbstractPointer() {};
    ClassDef(AbstractPointer, 1)
  };
  class AbstractDoublePointer : public Object {
    std::type_info& fType = {nullptr};

  protected:
    AbstractDoublePointer(std::type_info info) : fType(info) {};

  public:
    AbstractDoublePointer() {};
    const std::type_info& GetTypeInfo()             = 0;
    virtual AbstractDoublePointer* MakeCopy() const = 0;
    virtual Bool_t IsTObject() const { return kFALSE; };
    virtual TString GetStoredClassName() const { return ""; }
    virtual ~AbstractDoublePointer() {};
    ClassDef(AbstractDoublePointer, 1)
  };
  template<class T>
  class Pointer : public AbstractPointer {
    T* fPointer   = {nullptr};
    Bool_t fOwner = {kFALSE};

  public:
    Pointer() {};
    Pointer(T* obj, Bool_t owner) : fPointer(obj), fOwner(owner) {}
    T* GetPointer() const { return fPointer; }
    Bool_t IsTObject() const {
      if (dynamic_cast<TObject*>(fPointer)) return kTRUE;
      return kFALSE;
    };
    AbstractPointer* MakeCopy() const {
      auto pointer = new Pointer<T>(fPointer, fOwner);
      return pointer;
    }
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
  class DoublePointer : public AbstractDoublePointer {
    T** fPointer  = {nullptr};
    Bool_t fOwner = {kFALSE};

  public:
    DoublePointer() {};
    DoublePointer(T** obj, Bool_t owner) : AbstractDoublePointer(typeid(obj)), fPointer(obj), fOwner(owner) {};
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
    Bool_t IsTObject() const {
      if (dynamic_cast<TObject*>(*fPointer)) return kTRUE;
      return kFALSE;
    };
    Bool_t IsNull() const { return (fPointer == nullptr); }
    void Initialize(T* obj) {
      fPointer  = new T*();
      *fPointer = new T(*obj);
    }
    AbstractDoublePointer* MakeCopy() const {
      auto pointer = new DoublePointer<T>(fPointer, false);
      return pointer;
    }
    template<class T2>
    T2 MakeSpecializedCopy() const {
      if (dynamic_cast<T2*>(*fPointer)) {
        DoublePointer<T2> x;
        x->*fPointer = (T*) fPointer;
        return x;
      }
      DoublePointer<T2> y;
      return y;
    }
    /**
     * deletes pointer if possible, does not delete double pointer
     */
    virtual void DeletePointer() {
      if (fPointer)
        if (*fPointer) {
          delete *fPointer;
          *fPointer = nullptr;
        }
    }
    virtual ~DoublePointer() {
      if (fOwner) DeletePointer();
    }
    ClassDef(DoublePointer, 0)
  };
  typedef Pointer<TObject> ObjectPointer;
  typedef DoublePointer<TObject> ObjectDoublePointer;
}  // namespace Hal

#endif /* HAL_FEATURES_IO_OBJECTVOID_H_ */
