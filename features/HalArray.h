/*
 * HalMemoryManagerArrays.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALMEMORYMANAGERARRAYS_H_
#define HALMEMORYMANAGERARRAYS_H_
#include <TNamed.h>
#include <TObject.h>

/**
 * Template for 1-dim array of simple types
 */
template<typename T>
class HalArray_1 : public TObject {
  Int_t fSize;
  T* fArray;  //[fSize]
public:
  HalArray_1() : TObject(), fSize(0), fArray(nullptr) {}
  HalArray_1(Int_t size);
  /**
   * cop constructor
   * @param obj
   */
  HalArray_1(const HalArray_1<T>& obj);
  /**
   * make this array bigger, old values are copied new filled by zeroes
   * @param new_dim new size
   */
  void MakeBigger(Int_t new_dim);
  /**
   * resize this array all content of array is set to 0
   * @param new_dim
   */
  void Resize(Int_t new_dim);
  /**
   *
   * @param index index
   * @return value at given index
   */
  T& operator[](const int index) { return fArray[index]; };
  /**
   *
   * @return size of array
   */
  Int_t GetSize() const { return fSize; }
  /**
   *
   * @return array stored in this
   */
  T* GetArray() const { return fArray; };
  /**
   * operator =
   * @param rhs to copy
   * @return
   */
  void Add(HalArray_1<T> const& rhs);
  /**
   * add elements in array
   * @param rhs
   * @return
   */
  HalArray_1<T>& operator=(HalArray_1<T> const& rhs);
  /**
   *
   * @return size of this array
   */
  inline T Get(Int_t i) const { return fArray[i]; };
  /**
   * set value
   * @param index index
   * @param val value
   */
  inline void Set(Int_t index, T val) { fArray[index] = val; };
  /**
   * return value at given index and later increment it
   * @param i index
   * @return value before incrementation
   */
  inline T IncrementAfter(Int_t i);  //{return fArray[i]++;};
  virtual ~HalArray_1() {
    if (fArray) delete[] fArray;
  };
  ClassDef(HalArray_1, 1)
};
template<typename T>
T HalArray_1<T>::IncrementAfter(Int_t i) {
  return fArray[i]++;
};
template<>
inline Bool_t HalArray_1<Bool_t>::IncrementAfter(Int_t i) {
  return fArray[i];
}

/**
 * store 2-dim array of types
 */
template<typename T>
class HalArray_2 : public TObject {
  Int_t fSize;
  HalArray_1<T>* fArray;  //[fSize]
public:
  HalArray_2() : TObject(), fSize(0), fArray(NULL) {};
  /**
   * construtor
   * @param x cols
   * @param y rows
   */
  HalArray_2(Int_t x, Int_t y);
  /**
   * cop constructor
   * @param obj
   */
  HalArray_2(const HalArray_2<T>& obj);
  /**
   * make this array bigger, old values are copied new filled by zeroes
   * @param sizeA
   * @param sizeB
   */
  void MakeBigger(Int_t sizeA, Int_t sizeB);
  /**
   *
   * @return size of array
   */
  Int_t GetSize() const { return fSize; }
  /**
   *
   * @param i index
   * @return sub-array
   */
  HalArray_1<T>* At(const Int_t i) { return &fArray[i]; };
  /**
   * operator =
   * @param rhs to copy
   * @return
   */
  HalArray_2<T>& operator=(HalArray_2<T> const& rhs);
  /**
   *
   * @return size of this array (last dimension)
   */
  inline T Get(Int_t i, Int_t j) const { return fArray[i].Get(j); };
  inline HalArray_1<T>& operator[](const int index) { return fArray[index]; };
  /**
   * set value
   * @param i 1st index
   * @param j 2nd index
   * @param val value
   */
  inline void Set(Int_t i, Int_t j, T val) { fArray[i].Set(j, val); };
  /**
   *  return and later increment value at given place
   * @param i 1st index
   * @param j 2nd index
   * @return value before incrementation
   */
  inline T IncrementAfter(Int_t i, Int_t j) { return fArray[i].IncrementAfter(j); };
  virtual ~HalArray_2() {
    if (fArray) delete[] fArray;
  };
  ClassDef(HalArray_2, 1)
};
/**
 * 3-dim array
 */
template<typename T>
class HalArray_3 : public TObject {
  Int_t fSize;
  HalArray_2<T>* fArray;  //[fSize]
public:
  HalArray_3() : TObject(), fSize(0), fArray(NULL) {};
  HalArray_3(Int_t x, Int_t y, Int_t z);
  /**
   * cop constructor
   * @param obj
   */
  HalArray_3(const HalArray_3<T>& obj);
  /**
   * make this array bigger, old values are copied new filled by zeroes
   * @param sizeA
   * @param sizeB
   * @param sizeC
   */
  void MakeBigger(Int_t sizeA, Int_t sizeB, Int_t sizeC);
  /**
   *
   * @return size of this array (last dimension)
   */
  Int_t GetSize() const { return fSize; }
  inline HalArray_2<T>& operator[](const int index) { return fArray[index]; };
  /**
   * return sub-array
   * @param i index
   * @return subarray
   */
  HalArray_2<T>* At(Int_t i) const { return &fArray[i]; };
  /**
   * operator =
   * @param rhs to copy
   * @return
   */
  HalArray_3<T>& operator=(HalArray_3<T> const& rhs);
  /**
   *
   * @param A 1st index
   * @param B 2nd index
   * @param C 3rd index
   * @return value
   */
  inline T Get(Int_t A, Int_t B, Int_t C) const { return fArray[A].Get(B, C); };
  /**
   *  return and later increment value at given place
   * @param A 1st index
   * @param B 2nd index
   * @param C 3rd index
   * @return value before incrementation
   */
  inline T IncrementAfter(Int_t A, Int_t B, Int_t C) { return fArray[A].IncrementAfter(B, C); };  // do i++ and return i !
  /**
   *
   * set value
   * @param A 1st index
   * @param B 2nd index
   * @param C 3rd index
   * @param val value
   */
  inline void Set(Int_t A, Int_t B, Int_t C, T val) { fArray[A].Set(B, C, val); };
  virtual ~HalArray_3() {
    if (fArray) delete[] fArray;
  }
  ClassDef(HalArray_3, 1)
};

template<typename T>
/**
 * 4-dim array
 */
class HalArray_4 : public TObject {
  Int_t fSize;
  HalArray_3<T>* fArray;  //[fSize]
public:
  HalArray_4() : TObject(), fSize(0), fArray(NULL) {};
  HalArray_4(Int_t x, Int_t y, Int_t z, Int_t t);
  /**
   * cop constructor
   * @param obj
   */
  HalArray_4(const HalArray_4<T>& obj);
  /**
   * make this array bigger, old values are copied new filled by zeroes
   * @param sizeA
   * @param sizeB
   * @param sizeC
   * @param sizeD
   */
  void MakeBigger(Int_t sizeA, Int_t sizeB, Int_t sizeC, Int_t sizeD);
  inline HalArray_3<T>& operator[](const int index) { return fArray[index]; };
  /**
   * return sub-array
   * @param i index
   * @return sub-array
   */
  HalArray_3<T>* At(Int_t i) const { return &fArray[i]; };
  /**
   * operator =
   * @param rhs to copy
   * @return
   */
  HalArray_4<T>& operator=(HalArray_4<T> const& rhs);
  /**
   *
   * @return size of this array (last dimension)
   */
  Int_t GetSize() const { return fSize; }
  /**
   * get value
   * @param A 1st index
   * @param B 2nd index
   * @param C 3rd index
   * @param D 4th index
   * @return value
   */
  inline T Get(Int_t A, Int_t B, Int_t C, Int_t D) const { return fArray[A].Get(B, C, D); };
  /**
   *  return and later increment value at given place
   * @param A 1st index
   * @param B 2nd index
   * @param C 3rd index
   * @param D 4th idnex
   * @return value before incrementation
   */
  inline T IncrementAfter(Int_t A, Int_t B, Int_t C, Int_t D) { return fArray[A].IncrementAfter(B, C, D); };
  /**
   * set value
   * @param A 1st index
   * @param B 2nd index
   * @param C 3rd index
   * @param D 4th index
   * @param val value
   */
  inline void Set(Int_t A, Int_t B, Int_t C, Int_t D, Int_t val) { fArray[A].Set(B, C, D, val); };
  virtual ~HalArray_4() {
    if (fArray) delete[] fArray;
  }
  ClassDef(HalArray_4, 1)
};

#endif /* HALMEMORYMANAGERARRAYS_H_ */
