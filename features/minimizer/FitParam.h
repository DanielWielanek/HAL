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
  /**
   * class that represents the fitted parameters
   * additionally support discretization of parameters, in such case user should
   * configure map range and number of points.
   * In discrete parameter only discrete values of parameters are used. The distance between
   * all those values is always the same and equal to GetStepSize(), minimal/maximal values set during
   * fitting should be always inside of map range
   */
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
    Double_t fOverDParam {0};
    Double_t fStart {0};
    Double_t fFitted {0};
    Double_t fError {0};
    TString fName;

  public:
    FitParam();
    FitParam(const FitParam& other) = default;
    /**
     *
     * @return true if parameter is discrete
     */
    Bool_t IsDiscrete() const { return fIsDiscrete; };
    /**
     *
     * @return true if parameter is fixed
     */
    Bool_t IsFixed() const { return fIsFixed; };
    /**
     *
     * @return true if map is set
     */
    Bool_t IsMapSet() const { return fIsMapSet; }
    /**
     * recalculate limits according to map ranges etc.
     */
    void Init();
    /**
     * set range of map, used mostly in discrete parameters
     * @param min
     * @param max
     * @param points
     */
    void SetMapRange(Double_t min, Double_t max, Int_t points);
    /**
     * like SetMapRange but use step instead of number of points
     * @param min
     * @param max
     * @param step
     */
    void SetMapRangeByStep(Double_t min, Double_t max, Double_t step);
    /**
     * set range of parameter (should be the same or smaller than map range
     * @param min
     * @param max
     */
    void SetRange(Double_t min, Double_t max);
    /**
     * set start value
     * @param val
     */
    void SetStartVal(Double_t val) { fStart = val; };
    /**
     * set discrete flag
     * @param isDiscrete
     */
    void SetIsDiscrete(Bool_t isDiscrete) { fIsDiscrete = isDiscrete; }
    /**
     * set fixed parameter flag
     * @param isFixed
     */
    void SetIsFixed(Bool_t isFixed) { fIsFixed = isFixed; }
    /**
     * set map min
     * @param mapMax
     */
    void SetMapMax(Double_t mapMax) { fMapMax = mapMax; }
    /**
     * set map max
     * @param mapMin
     */
    void SetMapMin(Double_t mapMin) { fMapMin = mapMin; }
    /**
     * set maximal value of parameter
     * @param max
     */
    void SetMax(Double_t max) { fMax = max; }
    /**
     * set minimal value of parameter
     * @param min
     */
    void SetMin(Double_t min) { fMin = min; }
    /**
     * set parameter name
     * @param name
     */
    void SetParName(const TString& name) { fName = name; }
    /**
     * set value of fitted parameter
     * @param val
     */
    void SetFittedValue(Double_t val) { fFitted = val; };
    /**
     * set uncertainty of fitted parameter
     * @param error
     */
    void SetError(Double_t error) { fError = error; };
    /**
     * set min and max to be the same as map range
     */
    void ExtendToMapLimts();
    /**
     * shrink min/max range to be inside of the map but not on the edges
     */
    void ShirkBorders();
    /**
     *
     * @return number of points
     */
    Int_t GetNPoints() const { return fNPoint; };
    /**
     *
     * @return step size
     */
    Double_t GetStepSize() const { return fDParam; };
    /**
     *
     * @return upper edge of map
     */
    Double_t GetMapMax() const { return fMapMax; }
    /**
     *
     * @return lower edge of map
     */
    Double_t GetMapMin() const { return fMapMin; }
    /**
     *
     * @return upper edge of parameter during fitting
     */
    Double_t GetMax() const { return fMax; }
    /**
     *
     * @return lower edge of paramter during fitting
     */
    Double_t GetMin() const { return fMin; }
    /**
     *
     * @return initial value
     */
    Double_t GetStartVal() const { return fStart; };
    /**
     *
     * @return fitted value
     */
    Double_t GetFittedValue() const { return fFitted; };
    /**
     *
     * @return uncertainty of parameter
     */
    Double_t GetError() const { return fError; };
    /**
     * return 1.0/GetStepSize()
     * @return
     */
    Double_t GetOverStepSize() const { return fOverDParam; }
    FitParam& operator=(const FitParam& other) = default;
    /**
     *
     * @return parameter name
     */
    TString GetParName() const { return fName; }
    /**
     *
     * @return allowed values of discrete parameters, if parameter is continous
     * returns min
     */
    const std::vector<Double_t> GetValuesArray() const;
    virtual void Print(Option_t* option = "") const;
    virtual ~FitParam();
    ClassDef(FitParam, 1)
  };
}  // namespace Hal

#endif /* HAL_FEATURES_MINIMIZER_FITPARAM_H_ */
