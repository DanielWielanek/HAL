/*
 * FemtoSerialization.cxx
 *
 *  Created on: 17 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSerializationInterface.h"

#include <TH1D.h>
#include <TH3D.h>

#include <iostream>

namespace Hal {

  void FemtoSerializationInterface::CopyData1D(std::vector<TH1*>& list, int bin, ECopyDir dir, Bool_t error) {
    switch (dir) {
      case ECopyDir::kSerialize: {
        for (auto h : list) {
          if (error) {
            fArray->Set(fCounter, h->GetBinError(bin));
            fCounter++;
          } else {
            fArray->Set(fCounter, h->GetBinContent(bin));
            fCounter++;
          }
        }
      } break;
      case ECopyDir::kDeserialize: {
        for (auto h : list) {
          if (error) {
            h->SetBinError(bin, fArray->Get(fCounter));
            fCounter++;
          } else {
            h->SetBinContent(bin, fArray->Get(fCounter));
            fCounter++;
          }
        }
      } break;
    }
  }

  void
  FemtoSerializationInterface::CopyData3D(std::vector<TH3*>& list, int binx, int biny, int binz, ECopyDir dir, Bool_t error) {
    switch (dir) {
      case ECopyDir::kSerialize: {
        for (auto h : list) {
          if (error) {
            fArray->Set(fCounter++, h->GetBinError(binx, biny, binz));
          } else {
            fArray->Set(fCounter++, h->GetBinContent(binx, biny, binz));
          }
        }
      } break;
      case ECopyDir::kDeserialize: {
        for (auto h : list) {
          if (error) {
            h->SetBinError(binx, biny, binz, fArray->Get(fCounter++));
          } else {
            h->SetBinContent(binx, biny, binz, fArray->Get(fCounter++));
          }
        }
      } break;
    }
  }

  void FemtoSerializationInterface::CopyDataVec(std::vector<std::vector<double>>& list, int pos, ECopyDir dir) {
    switch (dir) {
      case ECopyDir::kSerialize: {
        for (auto& vec : list) {
          fArray->Set(fCounter++, vec[pos]);
        }
      } break;
      case ECopyDir::kDeserialize: {
        for (auto& vec : list) {
          vec[pos] = fArray->Get(fCounter++);
        }
      } break;
    }
  }

  void FemtoSerializationInterface::Serialize(Int_t binLo, Int_t binHi) {
    fBinLow  = binLo;
    fBinHi   = binHi;
    fCounter = 0;
    if (fBinLow == -1) {
      fBinLow = fBinStart;
      fBinHi  = fBinStop;
    }
    if (fBinLow == -1) { std::cout << "Cannot serialize, something is wrong with bin ranges" << std::endl; }
    if (fDataType == EData::kUnkown) {
      if (HaveCF())
        fDataType = EData::kCF;
      else if (HaveSlice())
        fDataType = EData::kSlice;
    }
    ECopyDir dir = ECopyDir::kSerialize;
    switch (fDataType) {
      case EData::kCF: {
        switch (fOption) {
          case EOption::kFull: ConvertCF(dir); break;
          case EOption::kSimple: ConvertCFSimple(dir); break;
        }
      } break;
      case EData::kSlice: {
        switch (fOption) {
          case EOption::kFull: ConvertSlice(dir); break;
          case EOption::kSimple: ConvertSliceSimple(dir); break;
        }
      } break;
      default: break;
    }
  }

  void FemtoSerializationInterface::ExpandArrayIfSerialize(Int_t newSize, ECopyDir dir) {
    if (dir == ECopyDir::kSerialize) fArray->MakeBigger(newSize);
  }

  void FemtoSerializationInterface::CopyData1D2(std::vector<TH1*>& listN, std::vector<TH1*>& listD, int bin, ECopyDir dir) {
    for (int i = 0; i < listN.size(); i++) {
      auto nH = listN[i];
      auto dH = listD[i];
      if (dir == ECopyDir::kSerialize) {
        fArray->Set(fCounter++, nH->GetBinContent(bin) / dH->GetBinContent(bin));
      } else {
        dH->SetBinContent(bin, fArray->Get(fCounter++));
        dH->SetBinContent(bin, 1);
      }
    }
  }

  void FemtoSerializationInterface::CopyData3D2(std::vector<TH3*>& listN,
                                                std::vector<TH3*>& listD,
                                                int binx,
                                                int biny,
                                                int binz,
                                                ECopyDir dir) {
    for (int i = 0; i < listN.size(); i++) {
      auto nH = listN[i];
      auto dH = listD[i];
      if (dir == ECopyDir::kSerialize) {
        fArray->Set(fCounter++, nH->GetBinContent(binx, biny, binz) / dH->GetBinContent(binx, biny, binz));
      } else {
        for (int bin = fBinStart; bin <= fBinStop; bin++) {
          nH->SetBinContent(binx, biny, binz, fArray->Get(fCounter++));
          dH->SetBinContent(binx, biny, binz, 1);
        }
      }
    }
  }

  void FemtoSerializationInterface::Deserialize(Int_t binLo, Int_t binHi) {
    fBinLow  = binLo;
    fBinHi   = binHi;
    fCounter = 0;
    if (fBinLow == -1) {
      fBinLow = fBinStart;
      fBinHi  = fBinStop;
    }
    if (fBinLow == -1) { std::cout << "Cannot deserialize, something is wrong with bin ranges" << std::endl; }
    if (fDataType == EData::kUnkown) {
      if (HaveCF())
        fDataType = EData::kCF;
      else if (HaveSlice())
        fDataType = EData::kSlice;
    }
    ECopyDir dir = ECopyDir::kDeserialize;
    switch (fDataType) {
      case EData::kCF: {
        switch (fOption) {
          case EOption::kFull: ConvertCF(dir); break;
          case EOption::kSimple: ConvertCFSimple(dir); break;
        }
      } break;
      case EData::kSlice: {
        switch (fOption) {
          case EOption::kFull: ConvertSlice(dir); break;
          case EOption::kSimple: ConvertSliceSimple(dir); break;
        }
      } break;
      default: break;
    }
  }


  Bool_t FemtoSerializationInterface::Init() {
    if (!HaveCF() && !HaveSlice()) {
      std::cout << __FILE__ << " " << __LINE__ << " cannot properly without cf's or slice" << std::endl;
      return kFALSE;  // no cf, no slice
    }
    if (HaveCF() && HaveSlice()) {
      std::cout << __FILE__ << " " << __LINE__ << " cannot properly use cf's and slice" << std::endl;
      return kFALSE;  // both wrong!
    }
    if (HaveCF()) GetBinsNo();  // have cf
    if (!fArray) return kFALSE;
    PostInit();
    return kTRUE;
  }

} /* namespace Hal */
