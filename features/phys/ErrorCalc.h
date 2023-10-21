/*
 * StdError.h
 *
 *  Created on: 21 paź 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_STD_STDERRORCALC_H_
#define HAL_FEATURES_STD_STDERRORCALC_H_

#include "Std.h"
#include <TNamed.h>
namespace Hal {
  class ErrorCalc : public TNamed {
    Double_t fMeasurement       = {0};
    Double_t fStatisticalUncert = {0};
    std::vector<std::pair<TString, std::vector<std::pair<Double_t, Double_t>>>> fValues;
    std::vector<std::pair<Double_t, Double_t>> GetAllUncerts(TString name) const;

  public:
    /**
     *
     * @param name
     * @param val
     */
    ErrorCalc(TString name = "", Double_t val = 0, Double_t err = 0);
    /**
     * set measured value of paramter
     * @param val
     */
    void SetMeasuredVal(Double_t val) { fMeasurement = val; }
    /**
     *
     * @param val value of statistical error
     */
    void SetStatError(Double_t val) { fStatisticalUncert = val; };
    /**
     * add systematic error source, if name is duplicated then error treated as a next measurement
     * @param name
     * @param value
     * @param sysUncert  - statistical uncert of variation measurement,
     * if -2 then assumed as abs(measurement - value)
     * if any other negative value then assumed the same as stat error of measurement
     */
    void AddSysError(TString name, Double_t value, Double_t uncert = -1);
    /**
     * return statistical uncertainty for measurement
     * @param name
     * @return
     */
    Double_t GetStatError() const;
    /**
     * performs barlow test and print results
     * @param prec precision of printing
     * @return
     */
    Double_t BarlowTest(Int_t prec = 4) const;
    /**
     * pefrom other test and print results in principle:
     * - the uncertainty of measurement with single method is assued as max(X-xi)
     * - then total uncertainty is sqrt(sum uncert_j)
     * @param prec
     * @return
     */
    Double_t TotalSys(Int_t prec = 4) const;
    virtual ~ErrorCalc() = default;
    ClassDef(ErrorCalc, 0)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_STD_STDERRORCALC_H_ */
