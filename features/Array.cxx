#include "Array.h"
#include <iostream>
namespace Hal {
  template<typename T>
  void Array_1<T>::MakeBigger(Int_t new_dim) {
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
  void Array_1<T>::Resize(Int_t new_dim) {
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
  void Array_1<TString>::Resize(Int_t new_dim) {
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
  Array_1<T>& Array_1<T>::operator=(const Array_1<T>& rhs) {
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
  Array_1<T>& Array_1<T>::operator+=(const Array_1<T>& rhs) {
    if (this != &rhs) {
      if (rhs.fSize == this->fSize) {
        for (int i = 0; i < fSize; i++) {
          this->fArray[i] += rhs.fArray[i];
        }
      } else {
        std::cout << __FILE__ << " " << __LINE__ << "different sizes of arrays!" << std::endl;
      }
    }
    return *this;
  }

  template<typename T>
  Array_1<T>& Array_1<T>::operator-=(const Array_1<T>& rhs) {
    if (rhs.GetSize() != this->GetSize()) return *this;
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] -= rhs.fArray[i];
    }
    return *this;
  }

  template<typename T>
  Array_1<T>& Array_1<T>::operator+(const Array_1<T>& rhs) {
    if (rhs.GetSize() != this->GetSize()) return *this;
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] += rhs.fArray[i];
    }
    return *this;
  }

  template<typename T>
  Array_1<T>& Array_1<T>::operator-(const Array_1<T>& rhs) {
    if (rhs.GetSize() != this->GetSize()) return *this;
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] -= rhs.fArray[i];
    }
    return *this;
  }

  template<typename T>
  Array_1<T>& Array_1<T>::operator*(const T& val) {
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] *= val;
    }
    return *this;
  }

  template<>
  Array_1<Bool_t>& Array_1<Bool_t>::operator*(const Bool_t& val) {
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] = fArray[i] && val;
    }
    return *this;
  }

  template<typename T>
  Array_1<T>& Array_1<T>::operator/(const T& val) {
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] /= val;
    }
    return *this;
  }

  template<typename T>
  Array_1<T>& Array_1<T>::operator*=(const T& val) {
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] *= val;
    }
    return *this;
  }

  template<>
  Array_1<Bool_t>& Array_1<Bool_t>::operator*=(const Bool_t& val) {
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] = fArray[i] && val;
    }
    return *this;
  }

  template<typename T>
  Array_1<T>& Array_1<T>::operator/=(const T& val) {
    for (int i = 0; i < GetSize(); i++) {
      fArray[i] /= val;
    }
    return *this;
  }

  template<typename T>
  T Array_1<T>::FindMin(Int_t& index) const {
    T min = std::numeric_limits<T>::max();
    for (int i = 0; i < fSize; i++) {
      if (min > fArray[i]) {
        index = i;
        min   = fArray[i];
      }
    }
    return min;
  }

  template<typename T>
  T Array_1<T>::FindMax(Int_t& index) const {
    T max = std::numeric_limits<T>::min();
    for (int i = 0; i < fSize; i++) {
      if (max > fArray[i]) {
        index = i;
        max   = fArray[i];
      }
    }
    return max;
  }
  template<typename T>

  void Array_1<T>::Add(Array_1<T> const& rhs) {
    if (fSize == rhs.fSize) {
      for (int i = 0; i < fSize; i++) {
        fArray[i] = fArray[i] + rhs.fArray[i];
      }
    }
  }

  template<typename T>
  Array_1<T>::Array_1(const Array_1<T>& obj) : TObject(obj) {
    fSize  = obj.fSize;
    fArray = new T[fSize];
    for (int i = 0; i < fSize; i++)
      fArray[i] = obj.fArray[i];
  }
  template<typename T>
  Array_1<T>::Array_1(Int_t x) : Array_1() {
    MakeBigger(x);
  }

  template<typename T>
  void Array_2<T>::MakeBigger(Int_t sizeA, Int_t sizeB) {
    if (sizeA < fSize) {
      for (int i = 0; i < fSize; i++) {
        fArray[i].MakeBigger(sizeB);
      }
    } else {
      Array_1<T>* temp = fArray;
      fArray           = new Array_1<T>[sizeA];
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
  Array_2<T>& Array_2<T>::operator=(const Array_2<T>& rhs) {
    if (this != &rhs) {
      if (this->fSize != rhs.fSize) {
        if (fArray) delete[] fArray;
        fSize = rhs.fSize;
        if (fSize > 0)
          fArray = new Array_1<T>[fSize];
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
  Array_2<T>& Array_2<T>::operator+=(const Array_2<T>& rhs) {
    if (this != &rhs) {
      if (this->fSize != rhs.fSize) {
        std::cout << __LINE__ << " " << __FILE__ << " incompatible arrays" << std::endl;
        return *this;
      }
      for (int i = 0; i < fSize; i++) {
        fArray[i] += rhs.fArray[i];
      }
    }
    return *this;
  }

  template<typename T>
  Array_2<T>::Array_2(const Array_2<T>& obj) : TObject(obj) {
    fSize  = obj.fSize;
    fArray = new Array_1<T>[fSize];
    for (int i = 0; i < fSize; i++)
      fArray[i] = obj.fArray[i];
  }

  template<typename T>
  Array_2<T>::Array_2(Int_t x, Int_t y) : Array_2() {
    MakeBigger(x, y);
  }

  template<typename T>
  void Array_3<T>::MakeBigger(Int_t sizeA, Int_t sizeB, Int_t sizeC) {
    if (sizeA < fSize) {
      for (int i = 0; i < fSize; i++) {
        fArray[i].MakeBigger(sizeB, sizeC);
      }
    } else {
      Array_2<T>* temp = fArray;
      fArray           = new Array_2<T>[sizeA];
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
  Array_3<T>& Array_3<T>::operator=(const Array_3<T>& rhs) {
    if (this != &rhs) {
      if (this->fSize != rhs.fSize) {
        if (fArray) delete[] fArray;
        fSize = rhs.fSize;
        if (fSize > 0)
          fArray = new Array_2<T>[fSize];
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
  Array_3<T>& Array_3<T>::operator+=(const Array_3<T>& rhs) {
    if (this != &rhs) {
      if (this->fSize != rhs.fSize) { return *this; }
      for (int i = 0; i < fSize; i++) {
        fArray[i] += rhs.fArray[i];
      }
    }
    return *this;
  }

  template<typename T>
  Array_3<T>::Array_3(const Array_3<T>& obj) : TObject(obj) {
    fSize  = obj.fSize;
    fArray = new Array_2<T>[fSize];
    for (int i = 0; i < fSize; i++)
      fArray[i] = obj.fArray[i];
  }

  template<typename T>
  Array_3<T>::Array_3(Int_t x, Int_t y, Int_t z) : Array_3() {
    MakeBigger(x, y, z);
  }

  template<typename T>
  void Array_4<T>::MakeBigger(Int_t sizeA, Int_t sizeB, Int_t sizeC, Int_t sizeD) {
    if (sizeA < fSize) {
      for (int i = 0; i < fSize; i++) {
        fArray[i].MakeBigger(sizeB, sizeC, sizeD);
      }
    } else {
      Array_3<T>* temp = fArray;
      fArray           = new Array_3<T>[sizeA];
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
  Array_4<T>& Array_4<T>::operator=(const Array_4<T>& rhs) {
    if (this != &rhs) {
      if (this->fSize != rhs.fSize) {
        if (fArray) delete[] fArray;
        fSize = rhs.fSize;
        if (fSize > 0)
          fArray = new Array_3<T>[fSize];
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
  Array_4<T>& Array_4<T>::operator+=(const Array_4<T>& rhs) {
    if (this != &rhs) {
      if (this->fSize != rhs.fSize) { return *this; }
      for (int i = 0; i < fSize; i++) {
        fArray[i] += rhs.fArray[i];
      }
    }
    return *this;
  }

  template<typename T>
  Array_4<T>::Array_4(const Array_4<T>& obj) : TObject(obj), fArray(nullptr) {
    fSize = obj.fSize;
    if (fSize > 0) fArray = new Array_3<T>[fSize];
    for (int i = 0; i < fSize; i++)
      fArray[i] = obj.fArray[i];
  }

  template<typename T>
  Array_4<T>::Array_4(Int_t x, Int_t y, Int_t z, Int_t t) : Array_4() {
    MakeBigger(x, y, z, t);
  }

  template class Array_1<Bool_t>;
  template class Array_1<Int_t>;
  template class Array_1<Short_t>;
  template class Array_1<Float_t>;
  template class Array_1<Double_t>;
  template class Array_2<Bool_t>;
  template class Array_2<Int_t>;
  template class Array_2<Short_t>;
  template class Array_2<Float_t>;
  template class Array_2<Double_t>;
  template class Array_3<Bool_t>;
  template class Array_3<Int_t>;
  template class Array_3<Short_t>;
  template class Array_3<Float_t>;
  template class Array_3<Double_t>;
  template class Array_4<Bool_t>;
  template class Array_4<Int_t>;
  template class Array_4<Short_t>;
  template class Array_4<Float_t>;
  template class Array_4<Double_t>;
}  // namespace Hal
