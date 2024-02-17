/*
 * FemtoPainter.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoPainter.h"

#include "Std.h"

namespace Hal {

  void FemtoPainter::Rescale(Double_t newScale) { fNewScale = newScale / fPrevScale; }

} /* namespace Hal */
