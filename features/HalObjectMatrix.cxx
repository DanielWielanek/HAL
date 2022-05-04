/*
 * HalHistogramManager.cxx
 *
 *  Created on: 12-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalObjectMatrix.h"

#include <TList.h>

#include "HalPackage.h"

HalObjectMatrix_1::HalObjectMatrix_1(const HalObjectMatrix_1& manager) :
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

HalObjectMatrix_1& HalObjectMatrix_1::operator=(const HalObjectMatrix_1& manager) {
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

void HalObjectMatrix_1::Init(Int_t size, const TObject* temp) {
  fSize         = size;
  fArray        = new TObject*[fSize];
  fComments     = new TString[fSize];
  TString title = Form("%s[%%i]", temp->GetName());
  for (int i = 0; i < fSize; i++) {
    fArray[i]    = (TObject*) temp->Clone(Form(title, i));
    fComments[i] = " ";
  }
}

HalPackage* HalObjectMatrix_1::Report() const {
  HalPackage* pack = new HalPackage(this);
  for (int i = 0; i < fSize; i++) {
    pack->AddObject(fArray[i]);
  }
  return pack;
}

TList* HalObjectMatrix_1::GetFlatList() const {
  TList* list = new TList();
  list->SetOwner(kFALSE);
  for (int i = 0; i < fSize; i++) {
    list->Add(fArray[i]);
  }
  return list;
};

HalObjectMatrix_2::HalObjectMatrix_2(const HalObjectMatrix_2& manager) : TNamed(manager), fSize(manager.fSize), fArray(NULL) {
  if (manager.fArray) {
    fArray = new HalObjectMatrix_1*[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new HalObjectMatrix_1(*manager.fArray[i]);
    }
  }
}

HalObjectMatrix_2& HalObjectMatrix_2::operator=(const HalObjectMatrix_2& manager) {
  if (this == &manager) { return *this; }
  if (fSize > 0) {
    delete[] fArray;
    fArray = NULL;
  }
  fSize = manager.fSize;
  if (manager.fSize > 0) {
    fArray = new HalObjectMatrix_1*[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new HalObjectMatrix_1(*manager.fArray[i]);
    }
  }
  return *this;
}

void HalObjectMatrix_2::Init(Int_t sizeX, Int_t sizeY, const TObject* temp) {
  fSize         = sizeX;
  fArray        = new HalObjectMatrix_1*[fSize];
  TString title = Form("%s[%%i]", temp->GetName());
  for (int i = 0; i < fSize; i++) {
    TString Title     = Form(title, i);
    fArray[i]         = new HalObjectMatrix_1();
    TObject* temp_obj = (TObject*) temp->Clone(Form(title, i));
    fArray[i]->Init(sizeY, temp_obj);
    delete temp_obj;
  }
}

HalPackage* HalObjectMatrix_2::Report() const {
  HalPackage* pack = new HalPackage(this);
  for (int i = 0; i < fSize; i++) {
    pack->AddObject(fArray[i]->Report());
  }
  return pack;
}

TList* HalObjectMatrix_2::GetFlatList() const {
  TList* list = new TList();
  list->SetOwner(kFALSE);
  for (int i = 0; i < fSize; i++) {
    for (int j = 0; j < fArray[i]->GetSize(); j++) {
      list->Add(fArray[i]->At(j));
    }
  }
  return list;
};

HalObjectMatrix_2::~HalObjectMatrix_2() {
  if (fArray == NULL) return;
  for (int i = 0; i < fSize; i++)
    delete fArray[i];
  delete[] fArray;
}

HalObjectMatrix_3::HalObjectMatrix_3(const HalObjectMatrix_3& manager) : TNamed(manager), fSize(manager.fSize), fArray(NULL) {
  if (manager.fArray) {
    fArray = new HalObjectMatrix_2*[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new HalObjectMatrix_2(*manager.fArray[i]);
    }
  }
}

HalObjectMatrix_3& HalObjectMatrix_3::operator=(const HalObjectMatrix_3& manager) {
  if (this == &manager) { return *this; }
  if (fSize > 0) {
    delete[] fArray;
    fArray = NULL;
  }
  fSize = manager.fSize;
  if (manager.fSize > 0) {
    fArray = new HalObjectMatrix_2*[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new HalObjectMatrix_2(*manager.fArray[i]);
    }
  }
  return *this;
}

void HalObjectMatrix_3::Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, const TObject* temp) {
  fSize        = sizeX;
  fArray       = new HalObjectMatrix_2*[fSize];
  TString name = "";
  if (temp->InheritsFrom("TNamed")) name = temp->GetName();
  TString title = Form("%s[%%i]", temp->GetName());
  for (int i = 0; i < fSize; i++) {
    TObject* temp_obj = (TObject*) temp->Clone(Form(title, i));
    fArray[i]         = new HalObjectMatrix_2();
    fArray[i]->Init(sizeY, sizeZ, temp_obj);
    delete temp_obj;
  }
}

HalPackage* HalObjectMatrix_3::Report() const {
  HalPackage* pack = new HalPackage(this);
  for (int i = 0; i < fSize; i++) {
    pack->AddObject(fArray[i]->Report());
  }
  return pack;
}

TList* HalObjectMatrix_3::GetFlatList() const {
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

HalObjectMatrix_3::~HalObjectMatrix_3() {
  if (fArray == NULL) return;
  for (int i = 0; i < fSize; i++)
    delete fArray[i];
  delete[] fArray;
}

HalObjectMatrix_4::HalObjectMatrix_4(const HalObjectMatrix_4& manager) : TNamed(manager), fSize(manager.fSize), fArray(NULL) {
  if (manager.fArray) {
    fArray = new HalObjectMatrix_3*[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new HalObjectMatrix_3(*manager.fArray[i]);
    }
  }
}

HalObjectMatrix_4& HalObjectMatrix_4::operator=(const HalObjectMatrix_4& manager) {
  if (this == &manager) { return *this; }
  if (fSize > 0) {
    delete[] fArray;
    fArray = NULL;
  }
  fSize = manager.fSize;
  if (manager.fSize > 0) {
    fArray = new HalObjectMatrix_3*[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new HalObjectMatrix_3(*manager.fArray[i]);
    }
  }
  return *this;
}

void HalObjectMatrix_4::Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t sizeD, const TObject* temp) {
  fSize         = sizeX;
  fArray        = new HalObjectMatrix_3*[fSize];
  TString title = Form("%s[%%i]", temp->GetName());
  for (int i = 0; i < fSize; i++) {
    TString Title     = Form(title, i);
    TObject* temp_obj = (TObject*) temp->Clone(Form(title, i));
    fArray[i]         = new HalObjectMatrix_3();
    fArray[i]->Init(sizeY, sizeZ, sizeD, temp_obj);
    delete temp_obj;
  }
}

HalPackage* HalObjectMatrix_4::Report() const {
  HalPackage* pack = new HalPackage(this);
  for (int i = 0; i < fSize; i++) {
    pack->AddObject(fArray[i]->Report());
  }
  return pack;
}

TList* HalObjectMatrix_4::GetFlatList() const {
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

HalObjectMatrix_4::~HalObjectMatrix_4() {
  if (fArray == NULL) return;
  for (int i = 0; i < fSize; i++)
    delete fArray[i];
  delete[] fArray;
}
