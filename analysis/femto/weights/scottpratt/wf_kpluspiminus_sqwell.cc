#include "constants.h"
#include "wavefunction.h"

using namespace std;
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_kpluspiminus_sqwell::CWaveFunction_kpluspiminus_sqwell(string parsfilename) : CWaveFunction() {
      ParsInit(parsfilename);

      m1        = MPI;
      m2        = MKAON;
      IDENTICAL = 0;

      q1q2 = -1;

      nchannels = 3;

      ellmax = 1;
      InitArrays();
      printf("Arrays Initialized\n");

      ell[0] = 0;  // S I=1/2
      ell[1] = 0;  // S I=3/2
      ell[2] = 1;  // P I=1/2 (kstar)
      InitWaves();


      nwells    = new int[nchannels];
      nwells[0] = 2;
      nwells[1] = 1;
      nwells[2] = 3;

      SquareWell_MakeArrays();

      a[0][0] = 0.204619;
      a[0][1] = 0.965437;  //
      a[1][0] = 0.54948;   //
      a[2][0] = 0.23947;
      a[2][1] = 0.765751;
      a[2][2] = 1.23877;

      V0[0][0] = -9266.34;
      V0[0][1] = 87.0632;  //
      V0[1][0] = 1057.62;  //
      V0[2][0] = -31505.8;
      V0[2][1] = 1391.0;
      V0[2][2] = -190.325;

      SquareWell_Init();
    }

    CWaveFunction_kpluspiminus_sqwell::~CWaveFunction_kpluspiminus_sqwell() { SquareWell_DeleteArrays(); }


    double CWaveFunction_kpluspiminus_sqwell::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared;
      double q = GetQ(iq);
      complex<double> psi, psia;
      psia = planewave[iq]->planewave(r, ctheta);
      double x;
      complex<double> DelPhi[3];

      SquareWell_GetDelPhi(iq, r, DelPhi);

      x = q * r / HBARC;

      // For I=1/2 (s & p wave)
      psi = psia;
      psi += DelPhi[0] / x;
      psi += 3.0 * ci * DelPhi[2] / x;
      psisquared = (2.0 / 3.0) * real(psi * conj(psi));

      // For I=3/2 (s wave only)
      psi = psia;
      psi += DelPhi[1] / x;
      psisquared += (1.0 / 3.0) * real(psi * conj(psi));

      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }
  }  // namespace ScottPratt
}  // namespace Hal
