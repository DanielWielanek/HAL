#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

#include <Eigen/Dense>
#include <array>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_poly.h>
#include <gsl/gsl_sf.h>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

using namespace std;
namespace Hal {
  namespace ScottPratt {

    class CSampler;
    class CWaveFunction;
    class CSourceCalc;
    class CKernelWF;
    class CMSUPart;
    class CdecayInfo;

    typedef unordered_map<int, CdecayInfo*> CdecayInfoMap;
    typedef pair<long int, CdecayInfo*> CdecayInfoPair;
    // typedef multimap<int,CPart* > mapip;
    typedef array<double, 4> FourVector;
    // typedef double FourVector[4];
    typedef double FourTensor[4][4];
  }  // namespace ScottPratt
}  // namespace Hal
#endif
