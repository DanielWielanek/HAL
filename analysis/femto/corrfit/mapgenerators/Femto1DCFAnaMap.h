/*
 * Femto1DCFAnaMap.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO1DCFANAMAP_H_
#define HALFEMTO1DCFANAMAP_H_

#include "FemtoBasicAna.h"
#include "FemtoConst.h"
#include "FemtoCorrFunc0D.h"


namespace Hal {
  class Femto1DCF;
  class CorrFitMapKstarRstar;
  /**
   * class for generating maps of CF's based on 1Dim CF's
   */

  class FemtoCorrFuncMap : public FemtoCorrFunc0D {
    Double_t fR;

  public:
    FemtoCorrFuncMap() : fR(0) {};
    FemtoCorrFuncMap(const Femto1DCF& h, Int_t Rbins, Double_t Rmin, Double_t Rmax);
    virtual Bool_t Check() { return FemtoCorrFunc0D::Check(); };
    inline void SetR(Double_t R) { fR = R; };
    virtual void FillNum(FemtoPair* pair);
    virtual void FillDenPerfect(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenRotated(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenMixed(FemtoPair* pair);
    virtual void FillDenHemisphere(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenCharged(FemtoPair* pair) { FillDenMixed(pair); };
    CorrFitMapKstarRstar* GetKstarMap(Int_t kt_bin);
    virtual ~FemtoCorrFuncMap();
    ClassDef(FemtoCorrFuncMap, 1)
  };

  class Femto1DCFAnaMap : public FemtoBasicAna {
    Int_t fRbins;
    Double_t fRMinEff, fRStep;
    Double_t fRmin;  //[fRbins]
    Double_t fRmax;  //[fRbins]
  protected:
    virtual void ProcessFemtoPair();
    virtual void ProcessFemtoPair_Perfect();
    virtual void ProcessFemtoPair_Rotated();
    virtual void ProcessFemtoPair_Hemisphere();
    virtual void ProcessFemtoPair_Mixed();
    virtual void ProcessFemtoPair_Charged();
    virtual void CheckCutContainerCollections();
    virtual Bool_t InitArray();

  public:
    Femto1DCFAnaMap();
    /**
     * set radii range in generated map
     * @param r_bins number of r bins
     * @param r_min minimal radius
     * @param r_max maximal radius
     */
    void SetRadiusRange(Int_t r_bins, Double_t r_min, Double_t r_max);
    virtual Task::EInitFlag Init();
    virtual Package* Report() const;
    virtual ~Femto1DCFAnaMap();
    ClassDef(Femto1DCFAnaMap, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO1DCFANAMAP_H_ */
