#include "constants.h"
#include "wavefunction.h"

using namespace std;
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_kpluspiplus_sqwell::CWaveFunction_kpluspiplus_sqwell(string parsfilename) : CWaveFunction() {
      ParsInit(parsfilename);

      m1        = MPI;
      m2        = MKAON;
      IDENTICAL = 0;

      q1q2 = 1;

      nchannels = 1;

      ellmax = 1;
      InitArrays();
      printf("Arrays Initialized\n");
      ell[0] = 0;  // S I=3/2
      InitWaves();

      nwells    = new int[nchannels];
      nwells[0] = 1;

      SquareWell_MakeArrays();

      a[0][0]  = 1.26401;
      V0[0][0] = 49.8849;

      SquareWell_Init();
    }

    CWaveFunction_kpluspiplus_sqwell::~CWaveFunction_kpluspiplus_sqwell() { SquareWell_DeleteArrays(); }


    double CWaveFunction_kpluspiplus_sqwell::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared;
      double q = GetQ(iq);
      complex<double> psi;
      psi = planewave[iq]->planewave(r, ctheta);
      double x;
      complex<double> DelPhi[1];

      SquareWell_GetDelPhi(iq, r, DelPhi);

      x = q * r / HBARC;
      // For I=3/2
      psi += DelPhi[0] / x;
      psisquared = real(psi * conj(psi));
      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }
  }  // namespace ScottPratt
}  // namespace Hal
