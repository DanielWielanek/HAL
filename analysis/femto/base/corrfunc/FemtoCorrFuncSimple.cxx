/*
 * FemtoCorrFuncSimple.cxx
 *
 *  Created on: 8 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoCorrFuncSimple.h"

#include "FemtoPair.h"

namespace Hal {
  FemtoCorrFuncSimple::FemtoCorrFuncSimple() : FemtoCorrFunc0D() {}

  FemtoCorrFuncSimple::FemtoCorrFuncSimple(const DividedHisto1D& h) : FemtoCorrFunc0D(h, 1) {}

  FemtoCorrFuncSimple::FemtoCorrFuncSimple(const FemtoCorrFuncSimple& other) : FemtoCorrFunc0D(other) {}

  void FemtoCorrFuncSimple::FillDenMixed(Hal::FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillDenObj(pair); }

  void FemtoCorrFuncSimple::FillDenCharged(Hal::FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillDenObj(pair); }

  void FemtoCorrFuncSimple::FillNum(Hal::FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillNumObj(pair); }

  void FemtoCorrFuncSimple::FillDenHemisphere(Hal::FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillDenObj(pair); }

  void FemtoCorrFuncSimple::FillDenPerfect(Hal::FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillDenObj(pair); }

  void FemtoCorrFuncSimple::FillDenRotated(Hal::FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillDenObj(pair); }

}  // namespace Hal
