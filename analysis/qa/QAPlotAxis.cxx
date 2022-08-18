/*
 * QAPlotAxis.cxx
 *
 *  Created on: 17 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "QAPlotAxis.h"

namespace Hal {

  QAPlotAxis::QAPlotAxis() : QAPlotAxis(0, 0, 0, 0) {}

  QAPlotAxis::QAPlotAxis(Int_t flag, Int_t bins, Double_t min, Double_t max) : fFlag(flag), fNBins(bins), fMin(min), fMax(max) {}

  QAPlotAxis::~QAPlotAxis() {}
} /* namespace Hal */
