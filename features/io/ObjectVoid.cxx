/*
 * VoidObject.cxx
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#include "ObjectVoid.h"

template<class T>
inline Hal::ObjectVoid<T>::ObjectVoid(T* obj, Bool_t owner) : fPointer(obj), fOwner(owner) {}

template<class T>
inline Hal::ObjectVoid<T>::~ObjectVoid() {
  if (fOwner) delete fPointer;
}
