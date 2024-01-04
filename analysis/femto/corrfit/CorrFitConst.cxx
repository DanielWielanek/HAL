/*
 * CorrFitConst.cxx
 *
 *  Created on: 4 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitConst.h"

#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"

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
      }
      return -1;
    }
    void MergeVertical(Hal::Femto::ECFType cfType, TObject* obj, Array_1<Float_t>* input, Array_1<Float_t>* output, Int_t pos) {
      switch (cfType) {
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
          cf->ImportSlice(input, pos);
          if (pos == cf->GetNum()->GetNbinsX()) {
            cf->RecalculateCF();
            cf->ExportIntoToFlatNum(output);
          }
        } break;
      }
    }
  }  // namespace Femto
}  // namespace Hal
