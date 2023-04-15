/*
 * HalCorrFitParam.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_MINIMIZER_FITPARAM_H_
#define HAL_FEATURES_MINIMIZER_FITPARAM_H_


#include <TObject.h>
#include <TString.h>

namespace Hal {
  class FitParam : public TObject {
    Bool_t fIsFixed {kFALSE};
    Bool_t fIsDiscrete {kFALSE};
    Bool_t fIsMapSet {kFALSE};
    Double_t fMin {0};
    Double_t fMax {0};
    Double_t fMapMin {0};
    Double_t fMapMax {0};
    Double_t fNPoint {0};
    Double_t fDParam {0};
    Double_t fStart {0};
    Double_t fFitted {0};
    Double_t fError {0};
    TString fName;

  public:
    FitParam();
    FitParam(const FitParam& other) = default;
    Bool_t IsDiscrete() const { return fIsDiscrete; };
    Bool_t IsFixed() const { return fIsFixed; };
    Bool_t IsMapSet() const { return fIsMapSet; }
    void Init();
    virtual ~FitParam();
    FitParam& operator=(const FitParam& other) = default;

    void SetMapRange(Double_t min, Double_t max, Int_t points);
    void SetRange(Double_t min, Double_t max);
    void SetStartVal(Double_t val) { fStart = val; };
    void SetIsDiscrete(Bool_t isDiscrete) { fIsDiscrete = isDiscrete; }
    void SetIsFixed(Bool_t isFixed) { fIsFixed = isFixed; }
    void SetMapMax(Double_t mapMax) { fMapMax = mapMax; }
    void SetMapMin(Double_t mapMin) { fMapMin = mapMin; }
    void SetMax(Double_t max) { fMax = max; }
    void SetMin(Double_t min) { fMin = min; }
    void SetParName(const TString& name) { fName = name; }
    void SetFittedValue(Double_t val) { fFitted = val; };
    void SetError(Double_t error) { fError = error; };
    Int_t GetNPoints() const { return fNPoint; };
    Double_t GetDParam() const { return fDParam; };
    Double_t GetMapMax() const { return fMapMax; }
    Double_t GetMapMin() const { return fMapMin; }
    Double_t GetMax() const { return fMax; }
    Double_t GetMin() const { return fMin; }
    Double_t GetStartVal() const { return fStart; };
    Double_t GetFittedValue() const { return fFitted; };
    Double_t GetError() const { return fError; };

    TString GetParName() const { return fName; }

    const std::vector<Double_t> GetValuesArray() const;
    virtual void Print(Option_t* option = "") const;
    ClassDef(FitParam, 1)
  };
}  // namespace Hal

#endif /* HAL_FEATURES_MINIMIZER_FITPARAM_H_ */
