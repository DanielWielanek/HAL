/*
 * CorrFuncAzimuthalKt.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOCORRFUNCAZIMUTHALKT_H_
#define HALFEMTOCORRFUNCAZIMUTHALKT_H_
#include "FemtoCorrFunc2D.h"
/**
 * collection of cf's grouped by pair-phi and kT bins
 */

namespace Hal {
  class FemtoCorrFuncAzimuthalKt : public FemtoCorrFunc2D {
    Double_t fPhiCorrection[2];

  protected:
    // FemtoCorrFuncAzimuthalKt(const DividedHisto1D* h = NULL, const std::initializer_list<double>& init = {0, 10}, Int_t phi =
    // 1);
    virtual std::pair<Double_t, Double_t> GetPairValNum(FemtoPair* pair) const;
    virtual std::pair<Double_t, Double_t> GetPairValDenRot(FemtoPair* pair) const;
    virtual std::pair<Double_t, Double_t> GetPairValDenHemi(FemtoPair* pair) const;
    virtual Bool_t Angle(Char_t opt) const;

  public:
    FemtoCorrFuncAzimuthalKt() : FemtoCorrFunc2D() {};
    FemtoCorrFuncAzimuthalKt(const DividedHisto1D& h,
                             const std::initializer_list<double>& init = {0, 10},
                             Int_t phi                                 = 1,
                             Int_t nbins                               = 100);
    virtual Bool_t Check();
    virtual ~FemtoCorrFuncAzimuthalKt();
    ClassDef(FemtoCorrFuncAzimuthalKt, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOCORRFUNCAZIMUTHALKT_H_ */
