/*
 * CorrFitFunc3D.cpp
 *
 *  Created on: 12 kwi 2024
 *      Author: daniel
 */

#include "CorrFitFunc3D.h"
#include "Cout.h"
#include "ErrorCalc.h"

namespace Hal {

  CorrFitFunc3D::CorrFitFunc3D(e3DMode mode, Int_t par, Int_t dim) : CorrFitFunc(par, dim), fMode(mode) {
    switch (mode) {
      case e3DMode::kNormal3R: {
        fLambdaParIndex    = 4;
        fNormParIndex      = 3;
        fRoutParIndex      = 0;
        fRsideParIndex     = 1;
        fRlongParIndex     = 2;
        fRoutsideParIndex  = -1;
        fRoutlongParIndex  = -1;
        fRsidelongParIndex = -1;
        SetParameterName(fRsideParIndex, "R_{side}");
        SetParameterName(fRlongParIndex, "R_{long}");
      } break;
      case e3DMode::kPlus3R: {
        fLambdaParIndex    = 4;
        fNormParIndex      = 3;
        fRoutParIndex      = 0;
        fRsideParIndex     = 1;
        fRlongParIndex     = 2;
        fRoutsideParIndex  = -1;
        fRoutlongParIndex  = -1;
        fRsidelongParIndex = -1;
        SetParameterName(fRsideParIndex, "R_{#Delta side}");
        SetParameterName(fRlongParIndex, "R_{#Delta long}");
      } break;
      case e3DMode::kRatio3R: {
        fLambdaParIndex    = 4;
        fNormParIndex      = 3;
        fRoutParIndex      = 0;
        fRsideParIndex     = 1;
        fRlongParIndex     = 2;
        fRoutsideParIndex  = -1;
        fRoutlongParIndex  = -1;
        fRsidelongParIndex = -1;
        SetParameterName(fRsideParIndex, "R_{#times side}");
        SetParameterName(fRlongParIndex, "R_{#times long}");
      } break;
      case e3DMode::kNormal6R: {
        fLambdaParIndex    = 7;
        fNormParIndex      = 6;
        fRoutParIndex      = 0;
        fRsideParIndex     = 1;
        fRlongParIndex     = 2;
        fRoutsideParIndex  = 3;
        fRoutlongParIndex  = 4;
        fRsidelongParIndex = 5;
        SetParameterName(fRsideParIndex, "R_{side}");
        SetParameterName(fRlongParIndex, "R_{long}");
        SetParameterName(fRoutsideParIndex, "R_{out-side}");
        SetParameterName(fRoutlongParIndex, "R_{out-long}");
        SetParameterName(fRsidelongParIndex, "R_{side-long}");
      } break;

      case e3DMode::kRatio6R: {
        fLambdaParIndex    = 7;
        fNormParIndex      = 6;
        fRoutParIndex      = 0;
        fRsideParIndex     = 1;
        fRlongParIndex     = 2;
        fRoutsideParIndex  = 3;
        fRoutlongParIndex  = 4;
        fRsidelongParIndex = 5;

        SetParameterName(fRsideParIndex, "R_{#times side}");
        SetParameterName(fRlongParIndex, "R_{#times long}");
        SetParameterName(fRoutsideParIndex, "R_{out-side}");
        SetParameterName(fRoutlongParIndex, "R_{out-long}");
        SetParameterName(fRsidelongParIndex, "R_{side-long}");
      } break;

      case e3DMode::kPlus6R: {
        fLambdaParIndex    = 7;
        fNormParIndex      = 6;
        fRoutParIndex      = 0;
        fRsideParIndex     = 1;
        fRlongParIndex     = 2;
        fRoutsideParIndex  = 3;
        fRoutlongParIndex  = 4;
        fRsidelongParIndex = 5;
        SetParameterName(fRsideParIndex, "R_{#Delta side}");
        SetParameterName(fRlongParIndex, "R_{#Delta long+}");
        SetParameterName(fRoutsideParIndex, "R_{out-side}");
        SetParameterName(fRoutlongParIndex, "R_{out-long}");
        SetParameterName(fRsidelongParIndex, "R_{side-long}");
      } break;
      default: {
      } break;
    }
    SetParameterName(fLambdaParIndex, "#lambda");
    SetParameterName(fNormParIndex, "N");
    SetParameterName(fRoutParIndex, "R_{out}");
  }

  Double_t CorrFitFunc3D::GetRout() const { return GetParameter(fRoutParIndex); }

  Double_t CorrFitFunc3D::GetRoutError() const { return GetParError(fRoutParIndex); }

  Double_t CorrFitFunc3D::GetRside() const {
    int idx = fRsideParIndex;
    switch (fMode) {
      case e3DMode::kNormal3R: {
        return GetParameter(idx);
      } break;
      case e3DMode::kNormal6R: {
        return GetParameter(idx);
      } break;
      case e3DMode::kRatio3R: {
        return GetParameter(fRoutParIndex) * GetParameter(idx);
      } break;
      case e3DMode::kRatio6R: {
        return GetParameter(fRoutParIndex) * GetParameter(idx);
      } break;
      case e3DMode::kPlus3R: {
        return GetParameter(fRoutParIndex) + GetParameter(idx);
      } break;
      case e3DMode::kPlus6R: {
        return GetParameter(fRoutParIndex) + GetParameter(idx);
      } break;
      default: {
      } break;
    }
    return -1;
  }

  Double_t CorrFitFunc3D::GetRsideError() const {
    int idx = fRsideParIndex;
    switch (fMode) {
      case e3DMode::kNormal3R: {
        return GetParError(idx);
      } break;
      case e3DMode::kNormal6R: {
        return GetParError(idx);
      } break;
      case e3DMode::kRatio3R: {
        return ErrorCalc::SumError(
          {GetParError(fRoutParIndex) * GetParameter(idx), GetParameter(fRoutParIndex) * GetParError(idx)});

      } break;
      case e3DMode::kRatio6R: {
        return ErrorCalc::SumError(
          {GetParError(fRoutParIndex) * GetParameter(idx), GetParameter(fRoutParIndex) * GetParError(idx)});
      } break;
      case e3DMode::kPlus3R: {
        return ErrorCalc::SumError({GetParameter(fRoutParIndex), GetParError(idx)});
      } break;
      case e3DMode::kPlus6R: {
        return ErrorCalc::SumError({GetParameter(fRoutParIndex), GetParError(idx)});
      } break;
      default: {
      } break;
    }
    return -1;
  }

  Double_t CorrFitFunc3D::GetRlong() const {
    int idx = fRlongParIndex;
    switch (fMode) {
      case e3DMode::kNormal3R: {
        return GetParameter(idx);
      } break;
      case e3DMode::kNormal6R: {
        return GetParameter(idx);
      } break;
      case e3DMode::kRatio3R: {
        return GetParameter(fRoutParIndex) * GetParameter(idx);
      } break;
      case e3DMode::kRatio6R: {
        return GetParameter(fRoutParIndex) * GetParameter(idx);
      } break;
      case e3DMode::kPlus3R: {
        return GetParameter(fRoutParIndex) + GetParameter(idx);
      } break;
      case e3DMode::kPlus6R: {
        return GetParameter(fRoutParIndex) + GetParameter(idx);
      } break;
      default: {
      } break;
    }
    return -1;
  }

  Double_t CorrFitFunc3D::GetRlongError() const {
    int idx = fRlongParIndex;
    switch (fMode) {
      case e3DMode::kNormal3R: {
        return GetParError(idx);
      } break;
      case e3DMode::kNormal6R: {
        return GetParError(idx);
      } break;
      case e3DMode::kRatio3R: {
        return ErrorCalc::SumError(
          {GetParError(fRoutParIndex) * GetParameter(idx), GetParameter(fRoutParIndex) * GetParError(idx)});

      } break;
      case e3DMode::kRatio6R: {
        return ErrorCalc::SumError(
          {GetParError(fRoutParIndex) * GetParameter(idx), GetParameter(fRoutParIndex) * GetParError(idx)});
      } break;
      case e3DMode::kPlus3R: {
        return ErrorCalc::SumError({GetParameter(fRoutParIndex), GetParError(idx)});
      } break;
      case e3DMode::kPlus6R: {
        return ErrorCalc::SumError({GetParameter(fRoutParIndex), GetParError(idx)});
      } break;
      default: {
      } break;
    }
    return -1;
  }

  void CorrFitFunc3D::SetRsideLimits(Double_t min, Double_t max) {
    if (fMode == e3DMode::kNormal3R || fMode == e3DMode::kNormal6R) {
      SetParLimits(RsideID(), min, max);
    } else {
      Hal::Cout::PrintInfo("Cannot set rside limits wrong 3dmode", EInfo::kWarning);
    }
  }

  void CorrFitFunc3D::SetRlongLimits(Double_t min, Double_t max) {
    if (fMode == e3DMode::kNormal3R || fMode == e3DMode::kNormal6R) {
      SetParLimits(RlongID(), min, max);
    } else {
      Hal::Cout::PrintInfo("Cannot set rlong limits wrong 3dmode", EInfo::kWarning);
    }
  }

} /* namespace Hal */
