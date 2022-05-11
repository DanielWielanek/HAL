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
  FemtoCorrFuncSimple::FemtoCorrFuncSimple() : FemtoCorrFunc1D() {}

  FemtoCorrFuncSimple::FemtoCorrFuncSimple(const DividedHisto1D& h) : FemtoCorrFunc1D(h, "", 1) { SetBins({0, 1}); }

  FemtoCorrFuncSimple::FemtoCorrFuncSimple(const FemtoCorrFuncSimple& other) : FemtoCorrFunc1D(other) {}

  void FemtoCorrFuncSimple::FillNum(FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillNumObj(pair); }

  void FemtoCorrFuncSimple::FillDenRotated(FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillDenObj(pair); }

  void FemtoCorrFuncSimple::FillDenMixed(FemtoPair* pair) { ((DividedHisto1D*) fArray->At(0))->FillDenObj(pair); }
}  // namespace Hal
