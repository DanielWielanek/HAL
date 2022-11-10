#include "constants.h"
#include "sf.h"
#include "wavefunction.h"

using namespace std;
namespace Hal {
  namespace ScottPratt {
    CWaveFunction_ppiminus_sqwell::CWaveFunction_ppiminus_sqwell(string parsfilename) : CWaveFunction() {
      ParsInit(parsfilename);

      m1        = MPROTON;
      m2        = MPI;
      IDENTICAL = 0;

      q1q2      = -1;
      nchannels = 4;

      ellmax = 1;
      InitArrays();
      printf("Arrays Initialized\n");

      ell[0] = 0;  // S31 I=3/2, J=1/2
      ell[1] = 1;  // P31 I=3/2, J=1/2
      ell[2] = 1;  // P33 (Delta) I=3/2, J=3/2
      ell[3] = 0;  // S11 I=1/2, J=1/2

      InitWaves();
      printf("Partial Waves Initialized\n");

      nwells    = new int[nchannels];
      nwells[0] = 1;
      nwells[1] = 1;
      nwells[2] = 3;
      nwells[3] = 2;
      SquareWell_MakeArrays();

      a[0][0] = 0.238374;
      a[1][0] = 1.05513;
      a[2][0] = 0.248261;
      a[2][1] = 0.385089;
      a[2][2] = 1.80402;
      a[3][0] = 0.226738;
      a[3][1] = 1.00427;

      V0[0][0] = 126509;
      V0[1][0] = 502.198;
      V0[2][0] = -24710.2;
      V0[2][1] = -145.115;
      V0[2][2] = -106.615;
      V0[3][0] = -3815.63;
      V0[3][1] = -8.33945;

      SquareWell_Init();
    }

    CWaveFunction_ppiminus_sqwell::~CWaveFunction_ppiminus_sqwell() { SquareWell_DeleteArrays(); }

    double CWaveFunction_ppiminus_sqwell::CalcPsiSquared(int iq, double r, double ctheta) {
      double psisquared, x, theta = acos(ctheta);
      double q = GetQ(iq);
      complex<double> psia, psi, DelPhi[4];
      complex<double> Xlm00, Xlm10, Xlm11;
      psia = planewave[iq]->planewave(r, ctheta);

      SquareWell_GetDelPhi(iq, r, DelPhi);

      x = q * r / HBARC;

      // First, do I=3/2 piece
      Xlm00 = sqrt(4.0 * PI) * SpherHarmonics::Ylm(0, 0, theta, 0.0) / x;
      Xlm10 = ci * sqrt(12.0 * PI) * SpherHarmonics::Ylm(1, 0, theta, 0.0) / x;
      Xlm11 = ci * sqrt(12.0 * PI) * SpherHarmonics::Ylm(1, 1, theta, 0.0) / x;
      // For no spin flip
      psi = psia;
      psi += Xlm00 * DelPhi[0];
      psi += Xlm10 * ((2.0 / 3.0) * DelPhi[2] + (1.0 / 3.0) * DelPhi[1]);
      psisquared = real(psi * conj(psi));
      // For spin flip
      psi = Xlm11 * sqrt(2.0 / 9.0) * (DelPhi[2] - DelPhi[1]);
      psisquared += real(psi * conj(psi));
      psisquared = psisquared / 3.0;

      // Now, do I=1/2 piece
      psi = psia;
      psi += DelPhi[3] / x;
      psisquared += real(psi * conj(psi)) * 2.0 / 3.0;

      psisquared *= RelativisticCorrection(r, iq);
      return psisquared;
    }
  }  // namespace ScottPratt
}  // namespace Hal
