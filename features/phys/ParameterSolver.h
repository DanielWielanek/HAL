/*
 * ParameterSolver.h
 *
 *  Created on: 15 lip 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_PHYS_PARAMETERSOLVER_H_
#define HAL_FEATURES_PHYS_PARAMETERSOLVER_H_
#include <cmath>
#include <iostream>
#include <vector>

#include "Object.h"
#ifndef __CLANG__
#include <gsl/gsl_multiroots.h>
#endif
class TF1;

namespace Hal {
  /**
   * class that solves non-linear equations. Thank to this function user can find a values of parameters for a function in a such
   * a way that they goes through points x,y
   */
  class ParameterSolver : public Object {
    std::vector<Double_t> fX;
    std::vector<Double_t> fY;
    TF1* fFunc = {nullptr};
    static TF1* gFunc;
    static std::vector<Double_t>* gX;
    static std::vector<Double_t>* gY;

  public:
    /**
     * default cto-r only for ROOT
     */
    ParameterSolver() {};
    /**
     * static function used by GSL
     * @param params
     * @param
     * @param f
     * @return
     */
    static int Residuals(const gsl_vector* params, void* /*unused*/, gsl_vector* f);
    /**
     * main constructor
     * @param x values of x
     * @param y values of y
     * @param func function to find parameters
     */
    ParameterSolver(std::vector<Double_t> x, std::vector<Double_t> y, TF1* func);
    /**
     *
     * @param initParams - starting parameters (optional)
     * @param opt - if "print" - prints parameters
     * @return vector of found parameters
     */
    std::vector<Double_t> Solve(std::vector<Double_t> initParams = {}, TString opt = "");
    /**
     * release ownershif of TF1, otherwise this class owns TF1 and destroys in destructor
     */
    virtual void ReleaseFunc();
    virtual ~ParameterSolver();
    ClassDef(ParameterSolver, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_PHYS_PARAMETERSOLVER_H_ */
