/*
 * StdOpenMP.h
 *
 *  Created on: 10 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_STD_STDOPENMP_H_
#define HAL_FEATURES_STD_STDOPENMP_H_


namespace Hal {
  namespace Std {
    namespace OpenMP {
      /**
       *
       * @return maximum number of threads
       */
      int GetMaxThreads();
      /**
       *
       * @return maximum number of used threads
       */
      int GetNumThreads();
      /**
       *
       * @return number of current thread
       */
      int GetThreadNum();
      /**
       *
       * @return numer of logic processes
       */
      int GetNumProcs();
      /**
       * set number of threads
       * @param no
       */
      void SetNumThreads(int no);
      /**
       *
       * @return true if openMP supported
       */
      bool HasMP();
    }  // namespace OpenMP
  }    // namespace Std

} /* namespace Hal */

#endif /* HAL_FEATURES_STD_STDOPENMP_H_ */
