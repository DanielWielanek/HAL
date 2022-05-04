/*
 * HalCorrFitParam.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_MINIMIZER_HALFITPARAM_H_
#define HAL_FEATURES_MINIMIZER_HALFITPARAM_H_


#include <TObject.h>
#include <TString.h>

class HalFitParam : public TObject {
  Bool_t fIsFixed;
  Bool_t fIsDiscrete;
  Double_t fMin;
  Double_t fMax;
  Double_t fMapMin;
  Double_t fMapMax;
  Double_t fNPoint;
  Double_t fDParam;
  Double_t fStart;
  Double_t fFitted;
  Double_t fError;
  std::vector<Double_t> fValues;
  TString fName;

public:
  HalFitParam();
  HalFitParam(const HalFitParam& other) = default;
  Bool_t IsDiscrete() const { return fIsDiscrete; };
  Bool_t IsFixed() const { return fIsFixed; };
  void Init();
  virtual ~HalFitParam();
  HalFitParam& operator=(const HalFitParam& other) = default;

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
  Double_t GetValue(Int_t i) const { return fValues[i]; };
  Double_t GetStartVal() const { return fStart; };
  Double_t GetFittedValue() const { return fFitted; };
  Double_t GetError() const { return fError; };

  TString GetParName() const { return fName; }

  const std::vector<Double_t>& GetValues() const { return fValues; }
  virtual void Print(Option_t* option = "") const;
  void SetValues(const std::vector<Double_t>& values) { fValues = values; }
  ClassDef(HalFitParam, 1)
};


#endif /* HAL_FEATURES_MINIMIZER_HALFITPARAM_H_ */
