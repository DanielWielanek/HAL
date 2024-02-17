/*
 * FemtoSHCFPainter.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_FUNCS_FEMTOSHCFPAINTER_H_
#define HAL_ANALYSIS_FEMTO_BASE_FUNCS_FEMTOSHCFPAINTER_H_

#include "FemtoPainter.h"
#include <vector>

#include "FemtoDrawOptions.h"
class TH1;
namespace Hal {
  class FemtoSHCF;
  class FemtoSHCFPainter : public FemtoPainter {
    std::vector<TH1*> fReal;
    std::vector<TH1*> fImag;
    Int_t fMaxL               = {0};
    Hal::HistoStyle* fImStyle = {nullptr};

  public:
    FemtoSHCFPainter() {};
    virtual void Repaint();
    virtual void Paint();
    virtual void Rescale(Double_t newScale);
    void SetImStyle(const Hal::HistoStyle& style);
    FemtoSHCFPainter(Hal::FemtoSHCF& cf, FemtoDrawOptions opts);
    virtual ~FemtoSHCFPainter();
    ClassDef(FemtoSHCFPainter, 1);
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_FUNCS_FEMTOSHCFPAINTER_H_ */
