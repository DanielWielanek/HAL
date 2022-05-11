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
#include "FemtoCorrFunc.h"
/**
 * collection of cf's grouped by pair-phi and kT bins
 */

namespace Hal {
  class FemtoCorrFuncAzimuthalKt : public FemtoCorrFunc2D {
    Double_t fPhiCorrection[2];

  protected:
    /**
     * return bin number in fArray
     * @param px total px of pair
     * @param py total py of pair
     * @param event_phi event reaction plane angle
     * @return
     */
    Int_t CalculateBin(Double_t px, Double_t py, Double_t event_phi);
    FemtoCorrFuncAzimuthalKt(const DividedHisto1D* h = NULL, const std::initializer_list<double>& init = {0, 10}, Int_t phi = 1);

  public:
    FemtoCorrFuncAzimuthalKt() : FemtoCorrFunc2D() {};
    FemtoCorrFuncAzimuthalKt(const DividedHisto1D& h, const std::initializer_list<double>& init = {0, 10}, Int_t phi = 1);
    virtual Bool_t Check();
    virtual TString HTMLExtract(Int_t no, TString dir = "") const;
    virtual void FillNum(FemtoPair* pair);
    virtual void FillDenMixed(FemtoPair* pair);
    virtual void FillDenRotated(FemtoPair* pair);
    virtual void FillDenHemisphere(FemtoPair* pair) { FillDenRotated(pair); };
    virtual ~FemtoCorrFuncAzimuthalKt();
    ClassDef(FemtoCorrFuncAzimuthalKt, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOCORRFUNCAZIMUTHALKT_H_ */
