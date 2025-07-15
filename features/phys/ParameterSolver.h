/*
 * ParameterSolver.h
 *
 *  Created on: 15 lip 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_PHYS_PARAMETERSOLVER_H_
#define HAL_FEATURES_PHYS_PARAMETERSOLVER_H_
#include <cmath>
#include <gsl/gsl_multiroots.h>
#include <iostream>
#include <vector>

#include "Object.h"
class TF1;

namespace Hal {
  class ParameterSolver : public Object {
    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    TF1* fFunc = {nullptr};
    static TF1* gFunc;
    static std::vector<Double_t>* gX;
    static std::vector<Double_t>* gY;

  public:
    ParameterSolver() {};
    static int Residuals(const gsl_vector* params, void* /*unused*/, gsl_vector* f);
    ParameterSolver(std::vector<Double_t> x, std::vector<Double_t> y, TF1* func) {
      fX    = x;
      fY    = y;
      fFunc = func;
    };
    /**
     *
     * @param initParams - starting parameters (optional)
     * @param opt - if "print" - prints parameters
     * @return
     */
    std::vector<Double_t> Solve(std::vector<Double_t> initParams = {}, TString opt = "");
    virtual void ReleaseFunc();
    virtual ~ParameterSolver();
    ClassDef(ParameterSolver, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_PHYS_PARAMETERSOLVER_H_ */
