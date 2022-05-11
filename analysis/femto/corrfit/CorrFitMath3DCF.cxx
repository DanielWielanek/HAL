/*
 * CorrFit1DCFSum.cxx
 *
 *  Created on: 21 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitMath3DCF.h"

namespace Hal {
  CorrFit3DCF_Sum::CorrFit3DCF_Sum(CorrFit3DCF* f1, CorrFit3DCF* f2) :
    CorrFitMath3DCF(f1->GetParametersNo() + f2->GetParametersNo()) {
    SetSubfunction(f1, 0);
    SetSubfunction(f2, 1);
    // SetParameterName(GetParametersNo()-1,"N");
    Int_t par1 = f1->GetParametersNo();
    for (int i = 0; i < f1->GetParametersNo(); i++) {
      SetParameterName(i, f1->GetParameterName(i));
      if (f1->IsParFixed(i)) {
        FixParameter(i, f1->GetParMin(i));
      } else {
        SetParLimits(i, f1->GetParMin(i), f1->GetParMax(i));
      }
    }
    for (int i = 0; i < f2->GetParametersNo(); i++) {
      SetParameterName(i + par1, f2->GetParameterName(i));
      if (f2->IsParFixed(i)) {
        FixParameter(i + par1, f2->GetParMin(i));
      } else {
        SetParLimits(i + par1, f2->GetParMin(i), f2->GetParMax(i));
      }
    }
    for (int i = 0; i < f1->GetParametersNo(); i++) {
      for (int j = 0; j < f2->GetParametersNo(); j++) {
        if (f1->GetParameterName(i) == f2->GetParameterName(j)) {
          SetParameterName(i, f1->GetParameterName(i) + "_{1}");
          SetParameterName(par1 + j, f2->GetParameterName(j) + "_{2}");
        }
      }
    }
  }

  Double_t CorrFit3DCF_Sum::CalculateCF(const Double_t* x, const Double_t* params) const {
    return Call(0, x, params) + Call(1, x, params + GetSubFunction(0)->GetParametersNo());
  }

  CorrFit3DCF_Sum::~CorrFit3DCF_Sum() {}

  CorrFitMath3DCF::CorrFitMath3DCF(Int_t parameters_no, Int_t subfunct) : CorrFit3DCF(parameters_no), fSubFunctionsNo(subfunct) {
    fSubFunction = new CorrFit3DCF*[fSubFunctionsNo];
  }

  Double_t CorrFitMath3DCF::Call(Int_t subfucnt, const Double_t* x, const Double_t* params) const {
    return fSubFunction[subfucnt]->CalculateCF(x, params);
  }

  CorrFitMath3DCF::~CorrFitMath3DCF() { delete[] fSubFunction; }

  void CorrFitMath3DCF::ParametersChanged() const {
    for (int i = 0; i < GetSubunctionsNo(); i++) {
      for (int j = 0; j < GetSubFunction(i)->GetParametersNo(); j++) {
        GetSubFunction(i)->fTempParamsEval[j] = fTempParamsEval[j];
      }
      GetSubFunction(i)->ParametersChanged();
    }
  }
}  // namespace Hal
