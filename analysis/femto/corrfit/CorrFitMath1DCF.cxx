/*
 * CorrFitMath1D.cxx
 *
 *  Created on: 20 lis 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitMath1DCF.h"

namespace Hal {
  CorrFitMath1DCF::CorrFitMath1DCF(Int_t parameters_no, Int_t subfunc) : CorrFit1DCF(parameters_no), fSubFunctionsNo(subfunc) {
    fSubFunctions = new CorrFit1DCF*[subfunc];
  }

  void CorrFitMath1DCF::Draw(Option_t* draw_option) {
    CorrFit1DCF::Draw(draw_option);
    Int_t line_style[5] = {2, 3, 7, 9, 10};
    for (int i = 0; i < fSubFunctionsNo; i++) {
      if (i < 5) {
        GetSubFunction(i)->SetLineStyle(line_style[i]);
      } else {
        GetSubFunction(i)->SetLineColor(i - 2);
      }
      GetSubFunction(i)->Draw("SAME");
    }
  }

  Double_t CorrFitMath1DCF::Call(Int_t subfucnt, const Double_t* x, const Double_t* params) const {
    return fSubFunctions[subfucnt]->CalculateCF(x, params);
  }

  CorrFitMath1DCF::~CorrFitMath1DCF() { delete[] fSubFunctions; }

  Double_t CorrFit1DCF_Sum::CalculateCF(const Double_t* x, const Double_t* params) const {
    return (Call(0, x, params) + Call(1, x, params + GetSubFunction(0)->GetParametersNo()));
  }

  CorrFit1DCF_Sum::CorrFit1DCF_Sum(CorrFit1DCF* f1, CorrFit1DCF* f2) :
    CorrFitMath1DCF(f1->GetParametersNo() + f2->GetParametersNo(), 2) {
    SetSubfunction(f1, 0);
    SetSubfunction(f2, 1);
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
          SetParameterName(i, GetParameterName(i) + "_{1}");
          SetParameterName(par1 + j, GetParameterName(j + par1) + "_{2}");
        }
      }
    }
  }

  CorrFit1DCF_Sum::~CorrFit1DCF_Sum() {}

  Double_t CorrFit1DCF_SumRsame::CalculateCF(const Double_t* x, const Double_t* params) const {
    for (int i = 0; i < GetSubFunction(1)->GetParametersNo(); i++) {
      fNewParams[i] = params[i + GetSubFunction(0)->GetParametersNo()];
    }
    fNewParams[fSecondRadius] = params[fFirstRadius];
    return Call(0, x, params) + Call(1, x, fNewParams);
  }

  CorrFit1DCF_SumRsame::CorrFit1DCF_SumRsame(CorrFit1DCF* f1, CorrFit1DCF* f2) :
    CorrFitMath1DCF(f1->GetParametersNo() + f2->GetParametersNo() - 1, 2) {
    SetSubfunction(f1, 0);
    SetSubfunction(f2, 1);
    fSecondParamtersNo = f2->GetParametersNo();
    fNewParams         = new Double_t[fSecondParamtersNo];
    Int_t par1         = f1->GetParametersNo();
    for (int i = 0; i < f1->GetParametersNo(); i++) {
      SetParameterName(i, f1->GetParameterName(i));
      if (f1->IsParFixed(i)) {
        FixParameter(i, f1->GetParMin(i));
      } else {
        SetParLimits(i, f1->GetParMin(i), f1->GetParMax(i));
      }
      if (i == Radius()) { fFirstRadius = i; }
    }
    for (int i = 0; i < f2->GetParametersNo(); i++) {
      if (i == Radius()) { fSecondRadius = i; }
      SetParameterName(i + par1, f2->GetParameterName(i));
      if (i == Radius()) {
        if (f2->IsParFixed(i)) {
          FixParameter(i + par1, f1->GetParMin(i));
        } else {
          SetParLimits(i + par1, f1->GetParMin(i), f1->GetParMax(i));
        }
      } else {
        if (f2->IsParFixed(i)) {
          FixParameter(i + par1, f2->GetParMin(i));
        } else {
          SetParLimits(i + par1, f2->GetParMin(i), f2->GetParMax(i));
        }
      }
    }
    for (int i = 0; i < f1->GetParametersNo(); i++) {
      if (i == Radius()) continue;
      for (int j = 0; j < f2->GetParametersNo(); j++) {
        if (j == Radius()) continue;
        if ((f1->GetParameterName(i) == f2->GetParameterName(j))) {
          SetParameterName(i, GetParameterName(i) + "_{1}");
          SetParameterName(par1 + j, GetParameterName(j + par1) + "_{2}");
        }
      }
    }
  }

  CorrFit1DCF_SumRsame::~CorrFit1DCF_SumRsame() { delete[] fNewParams; }

  Double_t CorrFit1DCF_Multi::CalculateCF(const Double_t* x, const Double_t* params) const {
    return Call(0, x, params) * Call(1, x, params + fFirstFuncParams);
  }

  CorrFit1DCF_Multi::CorrFit1DCF_Multi(CorrFit1DCF* sign, CorrFit1DCF* back) :
    CorrFitMath1DCF(sign->GetParametersNo() + back->GetParametersNo(), 2), fFirstFuncParams(sign->GetParametersNo()) {
    SetSubfunction(sign, 0);
    SetSubfunction(back, 1);
    TString background_name = back->ClassName();
    if (background_name == "CorrFit1DCF_Poly") { back->FixParameter(0, 1); }
    for (int i = 0; i < GetSubFunction(0)->GetParametersNo(); i++) {
      SetParameterName(i, GetSubFunction(0)->GetParameterName(i));
      if (GetSubFunction(0)->IsParFixed(i)) {
        FixParameter(i, GetSubFunction(0)->GetParMin(i));
      } else {
        SetParLimits(i, GetSubFunction(0)->GetParMin(i), GetSubFunction(0)->GetParMax(i));
      }
    }
    for (int i = 0; i < GetSubFunction(1)->GetParametersNo(); i++) {
      SetParameterName(i + fFirstFuncParams, GetSubFunction(1)->GetParameterName(i));
      if (GetSubFunction(1)->IsParFixed(i)) {
        FixParameter(i + fFirstFuncParams, GetSubFunction(1)->GetParMin(i));
      } else {
        SetParLimits(i + fFirstFuncParams, GetSubFunction(1)->GetParMin(i), GetSubFunction(1)->GetParMax(i));
      }
    }
  }

  CorrFit1DCF_Multi::~CorrFit1DCF_Multi() {}

  void CorrFitMath1DCF::Check() {
    CorrFit1DCF::Check();
    for (int i = 0; i < GetSubfucntionsNo(); i++) {
      GetSubFunction(i)->Check();
    }
  }
}  // namespace Hal
