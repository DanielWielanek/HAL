/*
 * MinimizerStepConf.h
 *
 *  Created on: 22 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_MINIMIZER_MINIMIZERSTEPCONF_H_
#define HAL_FEATURES_MINIMIZER_MINIMIZERSTEPCONF_H_

#include "FitParam.h"
#include <vector>

namespace Hal {

  class MinimizerStepConf : public TObject {
    std::vector<FitParam> fParams;

  public:
    MinimizerStepConf(const MinimizerStepConf& othe, std::vector<int> order);
    MinimizerStepConf();
    Int_t GetNParams() const { return fParams.size(); }
    /**
     * configure parameters
     * @param name
     * @param step
     * @param min
     * @param max
     * @param option
     *
     */
    void ConfigureParameter(TString name, Double_t step, Double_t min, Double_t max, TString option = "");
    /**
     * set parameter limits
     */
    void SetParameters(std::vector<FitParam>& input) const;
    virtual ~MinimizerStepConf();
    MinimizerStepConf(const MinimizerStepConf& other) = default;
    /**
     * load configuration from XML file
     * @param xmlFile
     */
    void LoadFromXML(TString xmlFile);
    MinimizerStepConf& operator=(const MinimizerStepConf& other) = default;
    ClassDef(MinimizerStepConf, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MINIMIZERSTEPCONF_H_ */
