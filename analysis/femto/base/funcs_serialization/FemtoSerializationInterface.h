/*
 * FemtoSerialization.h
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACETEMPLATE_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACETEMPLATE_H_

#include "Array.h"

#include <TObject.h>
#include <iostream>
#include <vector>

/**
 * class for conversion of cf's into flat array
 */
class TH1;
class TH3;
namespace Hal {
  class FemtoSerializationInterface : public TObject {
  public:
    enum class EOption { kFull, kSimple };
    enum class ECopyDir { kSerialize, kDeserialize };
    enum class EData { kSlice, kCF, kUnkown };

  protected:
    Int_t fBinLow            = {-1};
    Int_t fBinHi             = {-1};
    Int_t fBinStart          = {-1};       // real bin in CF
    Int_t fBinStop           = {-1};       // real bin in CF
    Array_1<Float_t>* fArray = {nullptr};  //!
    EOption fOption          = {EOption::kFull};
    EData fDataType          = {EData::kUnkown};
    Int_t fCounter           = {0};

    virtual void ConvertCF(ECopyDir dir)          = 0;
    virtual void ConvertSlice(ECopyDir dir)       = 0;
    virtual void ConvertCFSimple(ECopyDir dir)    = 0;
    virtual void ConvertSliceSimple(ECopyDir dir) = 0;
    virtual void PostInit() {};
    virtual Bool_t HaveCF() const    = 0;
    virtual Bool_t HaveSlice() const = 0;
    virtual Bool_t GetBinsNo()       = 0;  // calculate start/stop bin
    void CopyData1D(std::vector<TH1*>& list, int bin, ECopyDir dir, Bool_t error = false);
    void CopyData3D(std::vector<TH3*>& list, int binx, int biny, int binz, ECopyDir dir, Bool_t error = false);
    void CopyData1D2(std::vector<TH1*>& listN, std::vector<TH1*>& listD, int bin, ECopyDir dir);
    void CopyData3D2(std::vector<TH3*>& listN, std::vector<TH3*>& listD, int binx, int biny, int binz, ECopyDir dir);
    void CopyDataVec(std::vector<std::vector<double>>& list, int pos, ECopyDir dir);
    void ExpandArrayIfSerialize(Int_t newSize, ECopyDir dir);
    template<typename T>
    void CopyDataSingle(T& x, ECopyDir dir) {
      switch (dir) {
        case ECopyDir::kSerialize:
          fArray->Set(fCounter, x);
          fCounter++;
          break;
        case ECopyDir::kDeserialize:
          x = fArray->Get(fCounter);
          fCounter++;
          break;
      }
    }
    template<typename T>
    void CopyDataPair(T& n, T& d, ECopyDir dir) {
      switch (dir) {
        case ECopyDir::kSerialize:
          fArray->Set(fCounter, n / d);
          fCounter++;
          break;
        case ECopyDir::kDeserialize:
          n = fArray->Get(fCounter);
          fCounter++;
          d = 1;
          break;
      }
    }

  public:
    FemtoSerializationInterface() {}
    void Data(EData opt) { fDataType = opt; }
    void SetOption(EOption opt) { fOption = opt; }
    virtual Bool_t BindCFs(TObject* cf)       = 0;
    virtual Bool_t BindSlices(TObject* slice) = 0;
    virtual Bool_t BindArray(Array_1<Float_t>* array) {
      fArray = array;
      if (fArray) return kTRUE;
      return kFALSE;
    }
    virtual Bool_t Init();
    /**
     * serialize cf or slice
     * @param binLo low bin (for CF) or parameter iD (for slice)
     */
    virtual void Serialize(Int_t binLo = -1, Int_t binHi = -1);
    virtual void Deserialize(Int_t binLo = -1, Int_t binHi = -1);
    virtual ~FemtoSerializationInterface() {};
    ClassDef(FemtoSerializationInterface, 0);
  };
  template<typename T1, typename T2>
  class FemtoSerializationInterfaceTemplate : public FemtoSerializationInterface {

  protected:
    T1* fCF    = {nullptr};  //!
    T2* fSlice = {nullptr};  //!
    Bool_t HaveCF() const final;
    Bool_t HaveSlice() const final;

  public:
    FemtoSerializationInterfaceTemplate() {};
    Bool_t BindCFs(TObject* cf);
    Bool_t BindSlices(TObject* slice);
    virtual ~FemtoSerializationInterfaceTemplate() {};
    ClassDef(FemtoSerializationInterfaceTemplate, 0)
  };

} /* namespace Hal */

template<typename T1, typename T2>
inline Bool_t Hal::FemtoSerializationInterfaceTemplate<T1, T2>::BindCFs(TObject* cf) {
  fCF = dynamic_cast<T1*>(cf);
  if (fCF) return kTRUE;
  std::cout << "Can't bind " << cf->ClassName() << " to " << this->ClassName() << std::endl;
  return kFALSE;
}

template<typename T1, typename T2>
inline Bool_t Hal::FemtoSerializationInterfaceTemplate<T1, T2>::BindSlices(TObject* slice) {
  fSlice = dynamic_cast<T2*>(slice);
  if (fSlice) return kTRUE;
  return kFALSE;
}

template<typename T1, typename T2>
inline Bool_t Hal::FemtoSerializationInterfaceTemplate<T1, T2>::HaveCF() const {
  if (fCF) return kTRUE;
  return kFALSE;
}

template<typename T1, typename T2>
inline Bool_t Hal::FemtoSerializationInterfaceTemplate<T1, T2>::HaveSlice() const {
  if (fSlice) return kTRUE;
  return kFALSE;
}


#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_SERIALIZATION_FEMTOSERIALIZATIONINTERFACETEMPLATE_H_ */
