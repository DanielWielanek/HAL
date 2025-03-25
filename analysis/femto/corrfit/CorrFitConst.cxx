/*
 * CorrFitConst.cxx
 *
 *  Created on: 4 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitConst.h"

#include "CorrFitVerticalSlices.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"

#include <iostream>

namespace Hal {
  namespace Femto {
    Int_t GetNSlices(TObject* obj) {
      auto cfType = Hal::Femto::GetCFType(obj);
      switch (cfType) {
        case ECFType::kOneDim: {
          auto cf = (Hal::Femto1DCF*) obj;
          return cf->GetNum()->GetNbinsX();
        } break;
        case ECFType::kThreeDim: {
          auto cf = (Hal::Femto3DCF*) obj;
          return cf->GetNum()->GetNbinsX();
        } break;
        case ECFType::kSpherical: {
          auto cf = (Hal::FemtoSHCF*) obj;
          return cf->GetNum()->GetNbinsX();
        } break;
        default: return -1; break;
      }
      return -1;
    }

    CorrFitVerticalSlices* MakeSlice(const Hal::DividedHisto1D& h, Int_t nParams) {
      if (dynamic_cast<const Hal::FemtoSHCF*>(&h)) {
        return new Hal::CorrFitVerticalSlicesSH(dynamic_cast<const Hal::FemtoSHCF&>(h), nParams);
      }
      if (dynamic_cast<const Hal::Femto1DCF*>(&h)) {
        return new Hal::CorrFitVerticalSlices1D(dynamic_cast<const Hal::Femto1DCF&>(h), nParams);
      }
      if (dynamic_cast<const Hal::Femto3DCF*>(&h)) {
        return new Hal::CorrFitVerticalSlices3D(dynamic_cast<const Hal::Femto3DCF&>(h), nParams);
      }
      return nullptr;
    }

    void MergeVertical(Hal::Femto::ECFType cfType, TObject* obj, Array_1<Float_t>* input, Array_1<Float_t>* output, Int_t pos) {
      /*    switch (cfType) { KURWA
            case ECFType::kOneDim: {
              auto cf = (Hal::Femto1DCF*) obj;
              cf->ImportSlice(input, pos);
              if (pos == cf->GetNum()->GetNbinsX()) { cf->ExportIntoToFlatNum(output); }
            } break;
            case ECFType::kThreeDim: {
              auto cf = (Hal::Femto3DCF*) obj;
              cf->ImportSlice(input, pos);
              if (pos == cf->GetNum()->GetNbinsX()) { cf->ExportIntoToFlatNum(output); }
            } break;
            case ECFType::kSpherical: {
              auto cf = (Hal::FemtoSHCF*) obj;
              // this is only temporary for valyria
              cf->ImportSlice(input, pos);
              int maxL  = cf->GetLMax();
              int maxJM = (maxL + 1) * (maxL + 1);
              // cf->ExportIntoToFlatNumValkyria(output);

              // cf->ImportSlice(input, pos);
              if (pos == cf->GetNum()->GetNbinsX()) {
                cf->RecalculateCF();
                cf->ExportIntoToFlatNum(output);
              }

            } break;
          }*/
    }
  }  // namespace Femto
}  // namespace Hal
