/*
 * Femto3DCFPainter.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Femto3DCFPainter.h"

namespace Hal {

  Femto3DCFPainter::Femto3DCFPainter(Femto3DCF& other, FemtoDrawOptions opt) : FemtoPainter(opt) {}

  void Femto3DCFPainter::Paint() {}

  void Femto3DCFPainter::Rescale(Double_t newScale) {}

  Femto3DCFPainter::~Femto3DCFPainter() {}

} /* namespace Hal */
