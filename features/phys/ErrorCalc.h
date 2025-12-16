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
  class XMLNode;
  /**
   * class for manipulation of errors in principle it calculates the total systematical uncertainty and performs barlow test
   */
  class ErrorCalc : public TNamed {
    Double_t fMeasurement       = {0};
    Double_t fStatisticalUncert = {0};
    Double_t fTotalSysError     = {0};
    std::vector<std::pair<TString, std::vector<std::pair<Double_t, Double_t>>>> fValues;
    std::vector<Double_t> fUncertainties;
    std::vector<std::pair<Double_t, Double_t>> GetAllUncerts(TString name) const;

    Double_t BarlowTestParameterMean(Int_t parId) const;
    Double_t BarlowTestParameterMax(Int_t parId) const;

  public:
    /**
     *
     * @param name
     * @param val
     */
    ErrorCalc(TString name = "", Double_t val = 0, Double_t err = 0);
    /**
     * set measured value of parameter
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
     *
     * @param par
     * @return
     */
    Double_t GetSysError(Int_t par) const;
    /**
     *
     * @param name
     * @return
     */
    Double_t GetSysError(TString name) const;
    /**
     *
     * @return total sys error
     */
    Double_t GetTotalSysError() const;
    /**
     *
     * @return measured value
     */
    Double_t GetMeasuredValue() const { return fMeasurement; }
    /**
     * performs barlow test and print results
     * @param prec precision of printing if negative - do not print test
     * @param opt = "mean" "max" and "max2"
     *  - mean - takes RMS of different variants for given variable
     *  - max - takes into computing maximal difference between measurement and variants
     * @return total uncert
     */
    Double_t BarlowTest(Int_t prec = -4, TString opt = "max");
    /**
     * return
     * @param errs
     * @return error by eq = sqrt(err[0]^2+err[1]^2+...)
     */
    static Double_t SumError(std::vector<Double_t> errs);
    /**
     * export error report to xml
     * @param name name of xml file
     * @param prec precission if negative do not define precission
     */
    void ExportToXML(TString name, Int_t prec = -1);
    /**
     * export error report to xml-node
     * @param name name of xml file
     * @param prec precission if negative do not define precission
     */
    Hal::XMLNode* ExportToXMLNode(Int_t prec = -1);
    virtual ~ErrorCalc() = default;
    ClassDef(ErrorCalc, 0)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_STD_STDERRORCALC_H_ */
