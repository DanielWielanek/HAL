/*
 * HalHistogramManager.cxx
 *
 *  Created on: 12-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HistogramManager.h"

#include "Package.h"

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TList.h>
#include <TRandom.h>

#include <iostream>
namespace Hal {
  HistogramAxisConf::HistogramAxisConf() : fBins(100), fMin(0), fMax(0) { fTitle = ""; }
  HistogramAxisConf::HistogramAxisConf(Int_t bins, Double_t min, Double_t max) : fBins(bins), fMin(min), fMax(max) {
    fTitle = "";
  }
  HistogramAxisConf::HistogramAxisConf(TString title, Int_t bins, Double_t min, Double_t max) :
    fBins(bins), fMin(min), fMax(max) {
    fTitle = title;
  }

  template<class T>
  HistogramManager_1_1D<T>::HistogramManager_1_1D(const HistogramManager_1_1D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fComments(NULL), fArray(NULL) {
    if (manager.fComments) {
      fComments = new TString[fSize];
      fArray    = new T*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (T*) manager.fArray[i]->Clone();
      }
    }
  }

  template<class T>
  HistogramManager_1_1D<T>& HistogramManager_1_1D<T>::operator=(const HistogramManager_1_1D& manager) {
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
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fComments = new TString[fSize];
      fArray    = new T*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (T*) manager.fArray[i]->Clone();
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_1_1D<T>::Init(Int_t size, HistogramAxisConf** axisconf, TString title, Bool_t Sumw) {
    fSize     = size;
    fAxisNo   = 1;
    fArray    = new T*[size];
    fComments = new TString[size];
    title     = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new T(Form(title, i), Form(title, i), axisconf[0]->GetNBins(), axisconf[0]->GetMin(), axisconf[0]->GetMax());
      fArray[i]->GetXaxis()->SetTitle(axisconf[0]->GetTitle());
      fArray[i]->GetYaxis()->SetTitle(axisconf[1]->GetTitle());
      fComments[i] = " ";
      if (Sumw) { fArray[i]->Sumw2(); }
    }
  }

  template<class T>
  void HistogramManager_1_1D<T>::Init(Int_t size, Int_t binsX, Double_t minX, Double_t maxX, Bool_t sumw) {
    fSize         = size;
    fAxisNo       = 1;
    fArray        = new T*[size];
    fComments     = new TString[size];
    TString title = "[%i]";
    for (int i = 0; i < fSize; i++) {
      Int_t ran    = gRandom->Rndm() * 1E+9;
      fArray[i]    = new T(Form(title, ran), Form(title, ran), binsX, minX, maxX);
      fComments[i] = " ";
      if (sumw) { fArray[i]->Sumw2(); }
    }
  }

  template<class T>
  void HistogramManager_1_1D<T>::OverwriteAt(T* t, Int_t pos) {
    if (pos < 0) return;
    if (pos > fSize) return;
    if (fArray[pos]) delete fArray[pos];
    fArray[pos] = t;
    fArray[pos]->SetDirectory(0x0);
    fComments[pos] = "";
  }

  template<class T>
  Package* HistogramManager_1_1D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i]);
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_1_1D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      list->Add(fArray[i]);
    }
    return list;
  };

  template<class T>
  HistogramManager_1_2D<T>::HistogramManager_1_2D(const HistogramManager_1_2D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fComments(NULL), fArray(NULL) {
    if (manager.fComments) {
      fComments = new TString[fSize];
      fArray    = new T*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (T*) manager.fArray[i]->Clone();
      }
    }
  }
  template<class T>
  HistogramManager_1_2D<T>& HistogramManager_1_2D<T>::operator=(const HistogramManager_1_2D& manager) {
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
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fComments = new TString[fSize];
      fArray    = new T*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (T*) manager.fArray[i]->Clone();
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_1_2D<T>::Init(Int_t size, HistogramAxisConf** axisconf, TString title, Bool_t Sumw) {
    fSize     = size;
    fAxisNo   = 2;
    fArray    = new T*[size];
    fComments = new TString[size];
    title     = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new T(Form(title, i),
                        Form(title, i),
                        axisconf[0]->GetNBins(),
                        axisconf[0]->GetMin(),
                        axisconf[0]->GetMax(),
                        axisconf[1]->GetNBins(),
                        axisconf[1]->GetMin(),
                        axisconf[1]->GetMax());
      fArray[i]->GetXaxis()->SetTitle(axisconf[0]->GetTitle());
      fArray[i]->GetYaxis()->SetTitle(axisconf[1]->GetTitle());
      fArray[i]->GetZaxis()->SetTitle(axisconf[2]->GetTitle());
      if (Sumw) { fArray[i]->Sumw2(); }
      fComments[i] = " ";
    }
  }
  template<class T>
  void HistogramManager_1_2D<T>::Init(Int_t size,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binY,
                                      Double_t minY,
                                      Double_t maxY,
                                      Bool_t Sumw) {
    fSize         = size;
    fAxisNo       = 2;
    fArray        = new T*[size];
    fComments     = new TString[size];
    TString title = "[%i]";
    for (int i = 0; i < fSize; i++) {
      Int_t ran = gRandom->Rndm() * 1E+9;
      fArray[i] = new T(Form(title, ran), Form(title, ran), binsX, minX, maxX, binY, minY, maxY);
      if (Sumw) { fArray[i]->Sumw2(); }
      fComments[i] = " ";
    }
  }

  template<class T>
  void HistogramManager_1_2D<T>::OverwriteAt(T* t, Int_t pos) {
    if (pos < 0) return;
    if (pos > fSize) return;
    if (fArray[pos]) delete fArray[pos];
    fArray[pos] = t;
    fArray[pos]->SetDirectory(0x0);
    fComments[pos] = "";
  }

  template<class T>
  Package* HistogramManager_1_2D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i]);
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_1_2D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      list->Add(fArray[i]);
    }
    return list;
  };

  template<class T>
  HistogramManager_1_3D<T>::HistogramManager_1_3D(const HistogramManager_1_3D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fComments(NULL), fArray(NULL) {
    if (manager.fComments) {
      fComments = new TString[fSize];
      fArray    = new T*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (T*) manager.fArray[i]->Clone();
      }
    }
  }
  template<class T>
  HistogramManager_1_3D<T>& HistogramManager_1_3D<T>::operator=(const HistogramManager_1_3D& manager) {
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
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fComments = new TString[fSize];
      fArray    = new T*[fSize];
      for (int i = 0; i < fSize; i++) {
        fComments[i] = manager.fComments[i];
        fArray[i]    = (T*) manager.fArray[i]->Clone();
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_1_3D<T>::Init(Int_t size, HistogramAxisConf** axisconf, TString title, Bool_t Sumw) {
    fSize     = size;
    fAxisNo   = 3;
    fArray    = new T*[size];
    fComments = new TString[size];
    title     = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      fArray[i] = new T(Form(title, i),
                        Form(title, i),
                        axisconf[0]->GetNBins(),
                        axisconf[0]->GetMin(),
                        axisconf[0]->GetMax(),
                        axisconf[1]->GetNBins(),
                        axisconf[1]->GetMin(),
                        axisconf[1]->GetMax(),
                        axisconf[2]->GetNBins(),
                        axisconf[2]->GetMin(),
                        axisconf[2]->GetMax());
      fArray[i]->GetXaxis()->SetTitle(axisconf[0]->GetTitle());
      fArray[i]->GetYaxis()->SetTitle(axisconf[1]->GetTitle());
      fArray[i]->GetZaxis()->SetTitle(axisconf[2]->GetTitle());
      fComments[i] = " ";
      if (Sumw) { fArray[i]->Sumw2(); }
    }
  }

  template<class T>
  void HistogramManager_1_3D<T>::Init(Int_t size,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binsY,
                                      Double_t minY,
                                      Double_t maxY,
                                      Int_t binsZ,
                                      Double_t minZ,
                                      Double_t maxZ,
                                      Bool_t Sumw) {
    fSize         = size;
    fAxisNo       = 3;
    fArray        = new T*[size];
    fComments     = new TString[size];
    TString title = "[%i]";
    for (int i = 0; i < fSize; i++) {
      Int_t ran    = gRandom->Rndm() * 1E+9;
      fArray[i]    = new T(Form(title, ran), Form(title, ran), binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      fComments[i] = " ";
      if (Sumw) { fArray[i]->Sumw2(); }
    }
  }

  template<class T>
  void HistogramManager_1_3D<T>::OverwriteAt(T* t, Int_t pos) {
    if (pos < 0) return;
    if (pos > fSize) return;
    if (fArray[pos]) delete fArray[pos];
    fArray[pos] = t;
    fArray[pos]->SetDirectory(0x0);
    fComments[pos] = "";
  }

  template<class T>
  Package* HistogramManager_1_3D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i]);
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_1_3D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      list->Add(fArray[i]);
    }
    return list;
  };

  template<class T>
  HistogramManager_2_1D<T>::HistogramManager_2_1D(const HistogramManager_2_1D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_1_1D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_2_1D<T>& HistogramManager_2_1D<T>::operator=(const HistogramManager_2_1D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_1_1D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_2_1D<T>::Init(Int_t sizeX, Int_t sizeY, HistogramAxisConf** axisconf, TString title, Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_1_1D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_2_1D<T>::Init(Int_t sizeX, Int_t sizeY, Int_t binsX, Double_t minX, Double_t maxX, Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_1_1D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, binsX, minX, maxX, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_2_1D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_2_1D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        list->Add(fArray[i].fArray[j]);
      }
    }
    return list;
  };

  template<class T>
  HistogramManager_2_2D<T>::HistogramManager_2_2D(const HistogramManager_2_2D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_1_2D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_2_2D<T>& HistogramManager_2_2D<T>::operator=(const HistogramManager_2_2D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_1_2D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_2_2D<T>::Init(Int_t sizeX, Int_t sizeY, HistogramAxisConf** axisconf, TString title, Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_1_2D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_2_2D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binsY,
                                      Int_t minY,
                                      Int_t maxY,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_1_2D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, binsX, minX, maxX, binsY, minY, maxY, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_2_2D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_2_2D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        list->Add(fArray[i].fArray[j]);
      }
    }
    return list;
  };

  template<class T>
  HistogramManager_2_3D<T>::HistogramManager_2_3D(const HistogramManager_2_3D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_1_3D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_2_3D<T>& HistogramManager_2_3D<T>::operator=(const HistogramManager_2_3D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_1_3D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_2_3D<T>::Init(Int_t sizeX, Int_t sizeY, HistogramAxisConf** axisconf, TString title, Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 3;
    fArray  = new HistogramManager_1_3D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_2_3D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binsY,
                                      Double_t minY,
                                      Double_t maxY,
                                      Int_t binsZ,
                                      Double_t minZ,
                                      Double_t maxZ,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 3;
    fArray  = new HistogramManager_1_3D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_2_3D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_2_3D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        list->Add(fArray[i].fArray[j]);
      }
    }
    return list;
  };

  template<class T>
  HistogramManager_3_1D<T>::HistogramManager_3_1D(const HistogramManager_3_1D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_2_1D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_3_1D<T>& HistogramManager_3_1D<T>::operator=(const HistogramManager_3_1D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_2_1D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_3_1D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      HistogramAxisConf** axisconf,
                                      TString title,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 1;
    fArray  = new HistogramManager_2_1D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, sizeZ, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void
  HistogramManager_3_1D<T>::Init(Int_t sizeX, Int_t sizeY, Int_t sizeZ, Int_t binsX, Double_t minX, Double_t maxX, Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 1;
    fArray  = new HistogramManager_2_1D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, sizeZ, binsX, minX, maxX, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_3_1D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_3_1D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        for (int k = 0; k < fArray[i].fArray[j].GetSize(); k++) {
          list->Add(fArray[i].fArray[j].fArray[k]);
        }
      }
    }
    return list;
  };

  template<class T>
  HistogramManager_3_2D<T>::HistogramManager_3_2D(const HistogramManager_3_2D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_2_2D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_3_2D<T>& HistogramManager_3_2D<T>::operator=(const HistogramManager_3_2D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_2_2D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_3_2D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      HistogramAxisConf** axisconf,
                                      TString title,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_2_2D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, sizeZ, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_3_2D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binsY,
                                      Int_t minY,
                                      Int_t maxY,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_2_2D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, sizeZ, binsX, minX, maxX, binsY, minY, maxY, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_3_2D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_3_2D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        for (int k = 0; k < fArray[i].fArray[j].GetSize(); k++) {
          list->Add(fArray[i].fArray[j].fArray[k]);
        }
      }
    }
    return list;
  };

  template<class T>
  HistogramManager_3_3D<T>::HistogramManager_3_3D(const HistogramManager_3_3D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_2_3D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_3_3D<T>& HistogramManager_3_3D<T>::operator=(const HistogramManager_3_3D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_2_3D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_3_3D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      HistogramAxisConf** axisconf,
                                      TString title,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 3;
    fArray  = new HistogramManager_2_3D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, sizeZ, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_3_3D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binsY,
                                      Double_t minY,
                                      Double_t maxY,
                                      Int_t binsZ,
                                      Double_t minZ,
                                      Double_t maxZ,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 3;
    fArray  = new HistogramManager_2_3D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, sizeZ, binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_3_3D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_3_3D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        for (int k = 0; k < fArray[i].fArray[j].GetSize(); k++) {
          list->Add(fArray[i].fArray[j].fArray[k]);
        }
      }
    }
    return list;
  };

  template<class T>
  HistogramManager_4_1D<T>::HistogramManager_4_1D(const HistogramManager_4_1D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_3_1D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_4_1D<T>& HistogramManager_4_1D<T>::operator=(const HistogramManager_4_1D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_3_1D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_4_1D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t sizeD,
                                      HistogramAxisConf** axisconf,
                                      TString title,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 1;
    fArray  = new HistogramManager_3_1D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, sizeZ, sizeD, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_4_1D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t sizeT,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 1;
    fArray  = new HistogramManager_3_1D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, sizeZ, sizeT, binsX, minX, maxX, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_4_1D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_4_1D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        for (int k = 0; k < fArray[i].fArray[j].GetSize(); k++) {
          for (int l = 0; l < fArray[i].fArray[j].fArray[j].GetSize(); l++) {
            list->Add(fArray[i].fArray[j].fArray[k].fArray[l]);
          }
        }
      }
    }
    return list;
  };

  template<class T>
  HistogramManager_4_2D<T>::HistogramManager_4_2D(const HistogramManager_4_2D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_3_2D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_4_2D<T>& HistogramManager_4_2D<T>::operator=(const HistogramManager_4_2D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_3_2D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_4_2D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t sizeD,
                                      HistogramAxisConf** axisconf,
                                      TString title,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_3_2D<T>[fSize];
    title   = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, sizeZ, sizeD, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_4_2D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t sizeT,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binsY,
                                      Int_t minY,
                                      Int_t maxY,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 2;
    fArray  = new HistogramManager_3_2D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, sizeZ, sizeT, binsX, minX, maxX, binsY, minY, maxY, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_4_2D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_4_2D<T>::GetFlatList() const {
    TList* list = new TList();
    list->SetOwner(kFALSE);
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        for (int k = 0; k < fArray[i].fArray[j].GetSize(); k++) {
          for (int l = 0; l < fArray[i].fArray[j].fArray[j].GetSize(); l++) {
            list->Add(fArray[i].fArray[j].fArray[k].fArray[l]);
          }
        }
      }
    }
    return list;
  };
  template<class T>
  HistogramManager_4_3D<T>::HistogramManager_4_3D(const HistogramManager_4_3D& manager) :
    TObject(manager), fSize(manager.fSize), fAxisNo(manager.fAxisNo), fArray(NULL) {
    if (manager.fArray) {
      fArray = new HistogramManager_3_3D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
  }
  template<class T>
  HistogramManager_4_3D<T>& HistogramManager_4_3D<T>::operator=(const HistogramManager_4_3D& manager) {
    if (this == &manager) { return *this; }
    if (fSize > 0) {
      delete[] fArray;
      fArray = NULL;
    }
    fSize   = manager.fSize;
    fAxisNo = manager.fAxisNo;
    if (manager.fSize > 0) {
      fArray = new HistogramManager_3_3D<T>[fSize];
      for (int i = 0; i < fSize; i++) {
        fArray[i] = manager.fArray[i];
      }
    }
    return *this;
  }
  template<class T>
  void HistogramManager_4_3D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t sizeD,
                                      HistogramAxisConf** axisconf,
                                      TString title,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 3;
    ;
    fArray = new HistogramManager_3_3D<T>[fSize];
    title  = title + "[%i]";
    for (int i = 0; i < fSize; i++) {
      TString Title = Form(title, i);
      fArray[i].Init(sizeY, sizeZ, sizeD, axisconf, Title, Sumw);
    }
  }
  template<class T>
  void HistogramManager_4_3D<T>::Init(Int_t sizeX,
                                      Int_t sizeY,
                                      Int_t sizeZ,
                                      Int_t sizeT,
                                      Int_t binsX,
                                      Double_t minX,
                                      Double_t maxX,
                                      Int_t binsY,
                                      Double_t minY,
                                      Double_t maxY,
                                      Int_t binsZ,
                                      Double_t minZ,
                                      Double_t maxZ,
                                      Bool_t Sumw) {
    fSize   = sizeX;
    fAxisNo = 3;
    ;
    fArray = new HistogramManager_3_3D<T>[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i].Init(sizeY, sizeZ, sizeT, binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ, Sumw);
    }
  }
  template<class T>
  Package* HistogramManager_4_3D<T>::Report() const {
    Package* pack = new Package(this);
    for (int i = 0; i < fSize; i++) {
      pack->AddObject(fArray[i].Report());
    }
    return pack;
  }
  template<class T>
  TList* HistogramManager_4_3D<T>::GetFlatList() const {
    TList* list = new TList();

    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fArray[i].GetSize(); j++) {
        for (int k = 0; k < fArray[i].fArray[j].GetSize(); k++) {
          for (int l = 0; l < fArray[i].fArray[j].fArray[j].GetSize(); l++) {
            list->Add(fArray[i].fArray[j].fArray[k].fArray[l]);
          }
        }
      }
    }
    list->SetOwner(kFALSE);
    return list;
  };

  template class HistogramManager_1_1D<TH1D>;
  template class HistogramManager_1_1D<TH1F>;
  template class HistogramManager_1_1D<TH1I>;
  template class HistogramManager_1_2D<TH2D>;
  template class HistogramManager_1_2D<TH2F>;
  template class HistogramManager_1_2D<TH2I>;
  template class HistogramManager_1_3D<TH3D>;
  template class HistogramManager_1_3D<TH3F>;
  template class HistogramManager_1_3D<TH3I>;

  template class HistogramManager_2_1D<TH1D>;
  template class HistogramManager_2_1D<TH1F>;
  template class HistogramManager_2_1D<TH1I>;
  template class HistogramManager_2_2D<TH2D>;
  template class HistogramManager_2_2D<TH2F>;
  template class HistogramManager_2_2D<TH2I>;
  template class HistogramManager_2_3D<TH3D>;
  template class HistogramManager_2_3D<TH3F>;
  template class HistogramManager_2_3D<TH3I>;

  template class HistogramManager_3_1D<TH1D>;
  template class HistogramManager_3_1D<TH1F>;
  template class HistogramManager_3_1D<TH1I>;
  template class HistogramManager_3_2D<TH2D>;
  template class HistogramManager_3_2D<TH2F>;
  template class HistogramManager_3_2D<TH2I>;
  template class HistogramManager_3_3D<TH3D>;
  template class HistogramManager_3_3D<TH3F>;
  template class HistogramManager_3_3D<TH3I>;

  template class HistogramManager_4_1D<TH1D>;
  template class HistogramManager_4_1D<TH1F>;
  template class HistogramManager_4_1D<TH1I>;
  template class HistogramManager_4_2D<TH2D>;
  template class HistogramManager_4_2D<TH2F>;
  template class HistogramManager_4_2D<TH2I>;
  template class HistogramManager_4_3D<TH3D>;
  template class HistogramManager_4_3D<TH3F>;
  template class HistogramManager_4_3D<TH3I>;
}  // namespace Hal
