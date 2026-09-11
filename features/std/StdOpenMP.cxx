/*
 * StdOpenMP.cxx
 *
 *  Created on: 10 wrz 2026
 *      Author: daniel
 */

#include "StdOpenMP.h"
#include <iostream>

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

namespace Hal {

  namespace Std {
    namespace OpenMP {
#ifdef HAVE_OPENMP
      int GetMaxThreads() { return omp_get_max_threads(); }
      int GetNumThreads() { return omp_get_num_threads(); }
      int GetThreadNum() { return omp_get_thread_num(); }
      int GetNumProcs() { return omp_get_num_procs(); }
      void SetNumThreads(int n) { omp_set_num_threads(n); }
      bool HasMP() { return true; }
#else
      int GetMaxThreads() { return 1; }
      int GetNumThreads() { return 1; }
      int GetThreadNum() { return 0; }
      int GetNumProcs() { return 1; }
      void SetNumThreads(int n) { std::cout << "Cannot set num threads no OpenMP !" << std::endl; }
      bool HasMP() { return false; };

#endif
    }  // namespace OpenMP
  }    // namespace Std

} /* namespace Hal */
