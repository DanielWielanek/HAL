/*
 * FemtoSourceModel.cxx
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoSourceModel.h"

#include "Cout.h"
#include "FemtoSourceDensity.h"
#include "Package.h"
#include "Parameter.h"

#include <TRandom2.h>


// FemtoSourceModel
/*
 FemtoSourceModel::FemtoSourceModel() :fRandom( new TRandom2()) ,
 fRout(0),fRside(0),fRlong(0),fModelName("unknown"),
 fParametersNo(0),
 fParameterNames(NULL)

 {
 }

 */
namespace Hal {
  FemtoSourceModel::FemtoSourceModel(Int_t params_no) :
    fParametersNo(params_no),
    fParams(nullptr),
    fParameterNames(nullptr),
    fRandom(new TRandom2()),
    fDensity(nullptr),
    fRout(0),
    fRside(0),
    fRlong(0),
    fModelName("unknown") {
    fParameterNames = new TString[fParametersNo]();
    fParams         = new Double_t[fParametersNo];
  }

  FemtoSourceModel::FemtoSourceModel(const FemtoSourceModel& model) :
    TObject(model),
    fParametersNo(model.fParametersNo),
    fRandom(new TRandom2()),
    fRout(model.fRout),
    fRside(model.fRside),
    fRlong(model.fRlong),
    fModelName(model.fModelName) {
    fParameterNames = new TString[fParametersNo];
    fParams         = new Double_t[fParametersNo];
    for (int i = 0; i < fParametersNo; i++) {
      fParameterNames[i] = model.fParameterNames[i];
      fParams[i]         = model.fParams[i];
    }
    fDensity = nullptr;
    if (model.fDensity) { fDensity = (FemtoSourceDensity*) model.fDensity->Clone(); }
  }

  Package* FemtoSourceModel::Report() const {
    Package* pack = new Package(this);
    pack->AddObject(new ParameterString("Model_name", fModelName));
    for (int i = 0; i < GetNpar(); i++) {
      pack->AddObject(new ParameterDouble(GetParamName(i), GetParameter(i)));
    }
    return pack;
  }

  TString FemtoSourceModel::GetParamName(Int_t n) const {
    if (n < 0) return "";
    if (n >= fParametersNo) return "";
    return fParameterNames[n];
  }

  void FemtoSourceModel::SetParameterByName(Double_t par, TString name) {
    Int_t n = -1;
    for (int i = 0; i < fParametersNo; i++) {
      if (name == fParameterNames[i]) {
        n = i;
        break;
      }
    }
    if (n >= 0) SetParameter(par, n);
  }

  FemtoSourceModel::ENumProperty FemtoSourceModel::GetModelNumProp() const {
    if (!fDensity) return ENumProperty::kNonAnalytical;
    if (fDensity->IsAna1d() && fDensity->IsAna3d()) return ENumProperty::kFullyAnalytical;
    if (fDensity->IsAna1d()) return ENumProperty::kAnalytical1d;
    if (fDensity->IsAna3d()) return ENumProperty::kAnalytical3d;
    return ENumProperty::kNonAnalytical;
  }

  FemtoSourceModel::~FemtoSourceModel() {
    delete fRandom;
    if (fDensity) delete fDensity;
    if (fParameterNames) delete[] fParameterNames;
    if (fParams) delete[] fParams;
  }
  // FemtoSourceModel1D

  FemtoSourceModel1D::FemtoSourceModel1D(Int_t par) : FemtoSourceModel(par) {
    SetParName("R", 0);
    SetParameter(1.0, 1);
  }

  FemtoSourceModel1D::FemtoSourceModel1D() : FemtoSourceModel1D(1) {}

  FemtoSourceModel1D::FemtoSourceModel1D(const FemtoSourceModel1D& model) : FemtoSourceModel(model) {}

  FemtoSourceModel1D::~FemtoSourceModel1D() {}

  // FemtoSourceModel3D

  FemtoSourceModel3D::FemtoSourceModel3D() : FemtoSourceModel3D(3) {}

  FemtoSourceModel3D::FemtoSourceModel3D(Int_t no) : FemtoSourceModel(no) {
    if (no >= 3) {
      SetParameter(1, 0);
      SetParameter(1, 1);
      SetParameter(1, 2);
      SetParName("R_{out}", 0);
      SetParName("R_{side}", 1);
      SetParName("R_{long}", 2);
    }
  }

  FemtoSourceModel3D::FemtoSourceModel3D(const FemtoSourceModel3D& model) : FemtoSourceModel(model) {}

  void FemtoSourceModel3D::SetRadius(Double_t radii) {
    SetOutRadius(radii);
    SetSideRadius(radii);
    SetLongRadius(radii);
  }

  FemtoSourceModel3D::~FemtoSourceModel3D() {}

  void FemtoSourceModel::Print(Option_t* /*option*/) const {
    Cout::Text(ClassName(), "L");
    Cout::Database({"ParName", "Value"});
    for (int i = 0; i < GetNpar(); i++) {
      TString val = Form("%4.4f", GetParameter(i));
      Cout::Database({GetParamName(i), val});
    }
  }
}  // namespace Hal
