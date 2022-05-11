/*
 * HalHistogramManager.cxx
 *
 *  Created on: 12-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Package.h"

#include <TList.h>

#include "ObjectMatrix.h"

namespace Hal {
  ObjectMatrix_1::ObjectMatrix_1(const ObjectMatrix_1& manager) :
    TNamed(manager), fSize(manager.fSize), fComments(NULL), fArray(NULL) {
    if (manager.fComments) {
      fComments = new TString(fSize);
      fArray    = new TObject*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (TObject*) manager.fArray[i]->Clone();
      }
    }
  }

  ObjectMatrix_1& ObjectMatrix_1::operator=(const ObjectMatrix_1& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      for (int i = 0; i < fSize; i++) {
        delete fArray[i];
      }
      delete[] fArray;
      delete[] fComments;
      fArray    = NULL;
      fComments = NULL;
    }
    fSize = manager.fSize;
    if (manager.fSize > 0) {
      fComments = new TString(fSize);
      fArray    = new TObject*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (TObject*) manager.fArray[i]->Clone();
      }
    }
    return *this;
  }

  void ObjectMatrix_1::Init(Int_t size, const TObject* temp) {
    fSize         = size;
    fArray        = new TObject*[fSize];
    fComments     = new TString[fSize];
    TString title = Form("%s[%%i]", temp->GetName());
    for (int i = 0; i < fSize; i++) {
      fArray[i]    = (TObject*) temp->Clone(Form(title, i));
      fComments[i] = " ";
    }
  }

  Package* ObjectMatrix_1::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i]);
    }
    return pack;
  }

  TList* ObjectMatrix_1::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      list->Add(fArray[i]);
    }
    return list;
  };

  ObjectMatrix_2::ObjectMatrix_2(const ObjectMatrix_2& manager) : TNamed(manager), fSize(manager.fSize), fArray(NULL) {
    if (manager.fArray) {
      fArray = new ObjectMatrix_1*[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = new ObjectMatrix_1(*manager.fArray[i]);
      }
    }
  }

  ObjectMatrix_2& ObjectMatrix_2::operator=(const ObjectMatrix_2& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize = manager.fSize;
    if (manager.fSize > 0) {
      fArray = new ObjectMatrix_1*[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = new ObjectMatrix_1(*manager.fArray[i]);
      }
    }
    return *this;
  }

  void ObjectMatrix_2::Init(Int_t sizeX, Int_t sizeY, const TObject* temp) {
    fSize         = sizeX;
    fArray        = new ObjectMatrix_1*[fSize];
    TString title = Form("%s[%%i]", temp->GetName());
    for (int i = 0; i < fSize; i++) {
      TString Title     = Form(title, i);
      fArray[i]         = new ObjectMatrix_1();
      TObject* temp_obj = (TObject*) temp->Clone(Form(title, i));
      fArray[i]->Init(sizeY, temp_obj);
      delete temp_obj;
    }
  }

  Package* ObjectMatrix_2::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i]->Report());
    }
    return pack;
  }

  TList* ObjectMatrix_2::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i]->GetSize(); j++) {
        list->Add(fArray[i]->At(j));
      }
    }
    return list;
  };

  ObjectMatrix_2::~ObjectMatrix_2() {
    if (fArray == NULL) return;
    for (int i = 0; i < fSize; i++)
      delete fArray[i];
    delete[] fArray;
  }

  ObjectMatrix_3::ObjectMatrix_3(const ObjectMatrix_3& manager) : TNamed(manager), fSize(manager.fSize), fArray(NULL) {
    if (manager.fArray) {
      fArray = new ObjectMatrix_2*[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = new ObjectMatrix_2(*manager.fArray[i]);
      }
    }
  }

  ObjectMatrix_3& ObjectMatrix_3::operator=(const ObjectMatrix_3& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize = manager.fSize;
    if (manager.fSize > 0) {
      fArray = new ObjectMatrix_2*[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = new ObjectMatrix_2(*manager.fArray[i]);
      }
    }
    return *this;
  }

  void ObjectMatrix_3::Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, const TObject* temp) {
    fSize        = sizeX;
    fArray       = new ObjectMatrix_2*[fSize];
    TString name = "";
    if (temp->InheritsFrom("TNamed")) name = temp->GetName();
    TString title = Form("%s[%%i]", temp->GetName());
    for (int i = 0; i < fSize; i++) {
      TObject* temp_obj = (TObject*) temp->Clone(Form(title, i));
      fArray[i]         = new ObjectMatrix_2();
      fArray[i]->Init(sizeY, sizeZ, temp_obj);
      delete temp_obj;
    }
  }

  Package* ObjectMatrix_3::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i]->Report());
    }
    return pack;
  }

  TList* ObjectMatrix_3::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i]->GetSize(); j++) {
        for (int k = 0; k < fArray[i]->Get(j)->GetSize(); k++) {
          list->Add(At(i, j, k));
        }
      }
    }
    return list;
  };

  ObjectMatrix_3::~ObjectMatrix_3() {
    if (fArray == NULL) return;
    for (int i = 0; i < fSize; i++)
      delete fArray[i];
    delete[] fArray;
  }

  ObjectMatrix_4::ObjectMatrix_4(const ObjectMatrix_4& manager) : TNamed(manager), fSize(manager.fSize), fArray(NULL) {
    if (manager.fArray) {
      fArray = new ObjectMatrix_3*[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = new ObjectMatrix_3(*manager.fArray[i]);
      }
    }
  }

  ObjectMatrix_4& ObjectMatrix_4::operator=(const ObjectMatrix_4& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize = manager.fSize;
    if (manager.fSize > 0) {
      fArray = new ObjectMatrix_3*[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = new ObjectMatrix_3(*manager.fArray[i]);
      }
    }
    return *this;
  }

  void ObjectMatrix_4::Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t sizeD, const TObject* temp) {
    fSize         = sizeX;
    fArray        = new ObjectMatrix_3*[fSize];
    TString title = Form("%s[%%i]", temp->GetName());
    for (int i = 0; i < fSize; i++) {
      TString Title     = Form(title, i);
      TObject* temp_obj = (TObject*) temp->Clone(Form(title, i));
      fArray[i]         = new ObjectMatrix_3();
      fArray[i]->Init(sizeY, sizeZ, sizeD, temp_obj);
      delete temp_obj;
    }
  }

  Package* ObjectMatrix_4::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i]->Report());
    }
    return pack;
  }

  TList* ObjectMatrix_4::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i]->GetSize(); j++) {
        for (int k = 0; k < fArray[i]->Get(j)->GetSize(); k++) {
          for (int l = 0; l < fArray[i]->Get(j)->Get(k)->GetSize(); l++) {
            list->Add(At(i, j, k, l));
          }
        }
      }
    }
    return list;
  };

  ObjectMatrix_4::~ObjectMatrix_4() {
    if (fArray == NULL) return;
    for (int i = 0; i < fSize; i++)
      delete fArray[i];
    delete[] fArray;
  }
}  // namespace Hal
