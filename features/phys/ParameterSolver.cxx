/*
 * ParameterSolver.cxx
 *
 *  Created on: 15 lip 2025
 *      Author: daniel
 */

#include "ParameterSolver.h"


#include "Cout.h"
#include "StdString.h"

#include <TF1.h>


namespace Hal {
  TF1* ParameterSolver::gFunc                = nullptr;
  std::vector<Double_t>* ParameterSolver::gX = nullptr;
  std::vector<Double_t>* ParameterSolver::gY = nullptr;
  int ParameterSolver::Residuals(const gsl_vector* params, void*, gsl_vector* f) {
    for (int i = 0; i < gFunc->GetNpar(); i++) {
      gFunc->SetParameter(i, gsl_vector_get(params, i));
    }
    for (size_t i = 0; i < gX->size(); ++i) {
      double x     = (*gX)[i];
      double model = gFunc->Eval(x);
      gsl_vector_set(f, i, model - (*gY)[i]);
    }

    return GSL_SUCCESS;
  }

  std::vector<Double_t> ParameterSolver::Solve(std::vector<Double_t> initParams, TString opt) {
    const size_t n_params = fX.size();
    const size_t n_eq     = fX.size();
    if (initParams.size() != fX.size()) { initParams.resize(fX.size()); }
    gX                       = &fX;
    gY                       = &fY;
    gFunc                    = fFunc;
    gsl_multiroot_function f = {&ParameterSolver::Residuals, n_eq, this};

    gsl_vector* x = gsl_vector_alloc(n_params);

    int count = 0;
    for (auto par : initParams)
      gsl_vector_set(x, count++, par);  // initial p1

    const gsl_multiroot_fsolver_type* T = gsl_multiroot_fsolver_hybrids;
    gsl_multiroot_fsolver* solver       = gsl_multiroot_fsolver_alloc(T, n_eq);
    gsl_multiroot_fsolver_set(solver, &f, x);

    int status;
    size_t iter = 0;

    do {
      iter++;
      status = gsl_multiroot_fsolver_iterate(solver);

      if (status) break;

      status = gsl_multiroot_test_residual(solver->f, 1e-7);
    } while (status == GSL_CONTINUE && iter < 100);

    if (Hal::Std::FindParam(opt, "print")) {
      std::cout << "Status: " << gsl_strerror(status) << "\n";
      std::cout << "Found parameters:\n";
      for (size_t i = 0; i < n_params; ++i) {
        std::cout << "p" << i + 1 << " = " << gsl_vector_get(solver->x, i) << "\n";
      }
    }
    std::vector<Double_t> solution;
    for (size_t i = 0; i < n_params; ++i) {
      solution.push_back(gsl_vector_get(solver->x, i));
    }
    gsl_multiroot_fsolver_free(solver);
    gsl_vector_free(x);
    gX = gY = nullptr;
    return solution;
  }

  void ParameterSolver::ReleaseFunc() { fFunc = nullptr; }

  ParameterSolver::~ParameterSolver() {
    if (fFunc) delete fFunc;
  }

  ParameterSolver::ParameterSolver(std::vector<Double_t> x, std::vector<Double_t> y, TF1* func) {
    fX    = x;
    fY    = y;
    fFunc = func;
    if (fX.size() != fY.size()) {
      Hal::Cout::PrintInfo(Form("ParameterSolver different x, y size in ct-or"), Hal::EInfo::kWarning);
    }
    if (fX.size() != fFunc->GetNpar()) {
      Hal::Cout::PrintInfo(Form("ParameterSolver size of x is different than number of parameters in function!"),
                           Hal::EInfo::kWarning);
    }
  }

} /* namespace Hal */
