#include "HalArray.h"
#include <iostream>

template<typename T>
void HalArray_1<T>::MakeBigger(Int_t new_dim) {
  if (fSize < new_dim) {
    T* temp = fArray;
    fArray  = new T[new_dim];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = temp[i];
    }
    for (int i = fSize; i < new_dim; i++) {
      fArray[i] = 0;
    }
    if (temp) delete[] temp;
    fSize = new_dim;
  }
}

template<typename T>
void HalArray_1<T>::Resize(Int_t new_dim) {
  if (fArray) delete[] fArray;
  if (new_dim < 1)
    fSize = 1;
  else
    fSize = new_dim;
  fArray = new T[fSize];
  for (int i = 0; i < fSize; i++)
    fArray[i] = 0;
}
template<>
void HalArray_1<TString>::Resize(Int_t new_dim) {
  if (fArray) delete[] fArray;
  if (new_dim < 1)
    fSize = 1;
  else
    fSize = new_dim;
  fArray = new TString[fSize];
  for (int i = 0; i < fSize; i++)
    fArray[i] = "";
}
template<typename T>
HalArray_1<T>& HalArray_1<T>::operator=(HalArray_1<T> const& rhs) {
  if (this != &rhs) {
    if (rhs.fSize != this->fSize) {
      if (fArray) delete[] fArray;
      fSize = rhs.fSize;
      if (fSize > 0)
        fArray = new T[fSize];
      else
        fArray = nullptr;
    }
    for (int i = 0; i < fSize; i++) {
      this->fArray[i] = rhs.fArray[i];
    }
  }
  return *this;
}

template<typename T>
void HalArray_1<T>::Add(HalArray_1<T> const& rhs) {
  if (fSize == rhs.fSize) {
    for (int i = 0; i < fSize; i++) {
      fArray[i] = fArray[i] + rhs.fArray[i];
    }
  }
}

template<typename T>
HalArray_1<T>::HalArray_1(const HalArray_1<T>& obj) : TObject(obj) {
  fSize  = obj.fSize;
  fArray = new T[fSize];
  for (int i = 0; i < fSize; i++)
    fArray[i] = obj.fArray[i];
}
template<typename T>
HalArray_1<T>::HalArray_1(Int_t x) : HalArray_1() {
  MakeBigger(x);
}

template<typename T>
void HalArray_2<T>::MakeBigger(Int_t sizeA, Int_t sizeB) {
  if (sizeA < fSize) {
    for (int i = 0; i < fSize; i++) {
      fArray[i].MakeBigger(sizeB);
    }
  } else {
    HalArray_1<T>* temp = fArray;
    fArray               = new HalArray_1<T>[sizeA];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = temp[i];
      fArray[i].MakeBigger(sizeB);
    }
    if (temp) delete[] temp;
    for (int i = fSize; i < sizeA; i++) {
      fArray[i].MakeBigger(sizeB);
    }
    fSize = sizeA;
  }
}

template<typename T>
HalArray_2<T>& HalArray_2<T>::operator=(HalArray_2<T> const& rhs) {
  if (this != &rhs) {
    if (this->fSize != rhs.fSize) {
      if (fArray) delete[] fArray;
      fSize = rhs.fSize;
      if (fSize > 0)
        fArray = new HalArray_1<T>[fSize];
      else
        fArray = nullptr;
    }
    for (int i = 0; i < fSize; i++) {
      fArray[i] = rhs.fArray[i];
    }
  }
  return *this;
}

template<typename T>
HalArray_2<T>::HalArray_2(const HalArray_2<T>& obj) : TObject(obj) {
  fSize  = obj.fSize;
  fArray = new HalArray_1<T>[fSize];
  for (int i = 0; i < fSize; i++)
    fArray[i] = obj.fArray[i];
}

template<typename T>
HalArray_2<T>::HalArray_2(Int_t x, Int_t y) : HalArray_2() {
  MakeBigger(x, y);
}

template<typename T>
void HalArray_3<T>::MakeBigger(Int_t sizeA, Int_t sizeB, Int_t sizeC) {
  if (sizeA < fSize) {
    for (int i = 0; i < fSize; i++) {
      fArray[i].MakeBigger(sizeB, sizeC);
    }
  } else {
    HalArray_2<T>* temp = fArray;
    fArray               = new HalArray_2<T>[sizeA];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = temp[i];
      fArray[i].MakeBigger(sizeB, sizeC);
    }
    if (temp) delete[] temp;
    for (int i = fSize; i < sizeA; i++) {
      fArray[i].MakeBigger(sizeB, sizeC);
    }
    fSize = sizeA;
  }
}

template<typename T>
HalArray_3<T>& HalArray_3<T>::operator=(HalArray_3<T> const& rhs) {
  if (this != &rhs) {
    if (this->fSize != rhs.fSize) {
      if (fArray) delete[] fArray;
      fSize = rhs.fSize;
      if (fSize > 0)
        fArray = new HalArray_2<T>[fSize];
      else
        fArray = nullptr;
    }
    for (int i = 0; i < fSize; i++) {
      fArray[i] = rhs.fArray[i];
    }
  }
  return *this;
}

template<typename T>
HalArray_3<T>::HalArray_3(const HalArray_3<T>& obj) : TObject(obj) {
  fSize  = obj.fSize;
  fArray = new HalArray_2<T>[fSize];
  for (int i = 0; i < fSize; i++)
    fArray[i] = obj.fArray[i];
}

template<typename T>
HalArray_3<T>::HalArray_3(Int_t x, Int_t y, Int_t z) : HalArray_3() {
  MakeBigger(x, y, z);
}

template<typename T>
void HalArray_4<T>::MakeBigger(Int_t sizeA, Int_t sizeB, Int_t sizeC, Int_t sizeD) {
  if (sizeA < fSize) {
    for (int i = 0; i < fSize; i++) {
      fArray[i].MakeBigger(sizeB, sizeC, sizeD);
    }
  } else {
    HalArray_3<T>* temp = fArray;
    fArray               = new HalArray_3<T>[sizeA];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = temp[i];
      fArray[i].MakeBigger(sizeB, sizeC, sizeD);
    }
    if (temp) delete[] temp;
    for (int i = fSize; i < sizeA; i++) {
      fArray[i].MakeBigger(sizeB, sizeC, sizeD);
    }
    fSize = sizeA;
  }
}

template<typename T>
HalArray_4<T>& HalArray_4<T>::operator=(HalArray_4<T> const& rhs) {
  if (this != &rhs) {
    if (this->fSize != rhs.fSize) {
      if (fArray) delete[] fArray;
      fSize = rhs.fSize;
      if (fSize > 0)
        fArray = new HalArray_3<T>[fSize];
      else
        fArray = nullptr;
    }
    for (int i = 0; i < fSize; i++) {
      fArray[i] = rhs.fArray[i];
    }
  }
  return *this;
}

template<typename T>
HalArray_4<T>::HalArray_4(const HalArray_4<T>& obj) : TObject(obj), fArray(nullptr) {
  fSize = obj.fSize;
  if (fSize > 0) fArray = new HalArray_3<T>[fSize];
  for (int i = 0; i < fSize; i++)
    fArray[i] = obj.fArray[i];
}

template<typename T>
HalArray_4<T>::HalArray_4(Int_t x, Int_t y, Int_t z, Int_t t) : HalArray_4() {
  MakeBigger(x, y, z, t);
}

template class HalArray_1<Bool_t>;
template class HalArray_1<Int_t>;
template class HalArray_1<Short_t>;
template class HalArray_1<Float_t>;
template class HalArray_1<Double_t>;
template class HalArray_2<Bool_t>;
template class HalArray_2<Int_t>;
template class HalArray_2<Short_t>;
template class HalArray_2<Float_t>;
template class HalArray_2<Double_t>;
template class HalArray_3<Bool_t>;
template class HalArray_3<Int_t>;
template class HalArray_3<Short_t>;
template class HalArray_3<Float_t>;
template class HalArray_3<Double_t>;
template class HalArray_4<Bool_t>;
template class HalArray_4<Int_t>;
template class HalArray_4<Short_t>;
template class HalArray_4<Float_t>;
template class HalArray_4<Double_t>;
