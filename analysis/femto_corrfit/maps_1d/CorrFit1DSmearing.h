/*
 * CorrFit1DMomemtumSmearingCorrection.h
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFIT1DSMEARING_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFIT1DSMEARING_H_

#include <TObject.h>

#include "SmearAlgo.h"
class TH2D;

namespace Hal {
  class CorrFitMapKstarRstar;
  class Femto1DCF;
  /**
   * smeared map in PRF frame
   */
  class CorrFit1DSmearing : public TObject {
  public:
    enum class ESmearInput { kXsimYreco, kXrecoYsim };

  private:
    SmearAlgoMatrix::EMethod fInversion    = {SmearAlgoMatrix::EMethod::kNone};
    Hal::Femto1DCF* fCF                    = {nullptr};
    Hal::CorrFitMapKstarRstar* fInputMap   = {nullptr};
    Hal::CorrFitMapKstarRstar* fSmearedMap = {nullptr};
    TH2D* fRatio                           = {nullptr};
    TH2D* fSmearingMap                     = {nullptr};
    TH2D* DoInversion(TH2D* h) const;
    Double_t fLambda = {0.01};

  public:
    CorrFit1DSmearing() {};
    /**
     * sets inverion algo used to calculate unsmeared numerator/denominator
     * @param algo
     */
    void SetInvertionAlgo(SmearAlgoMatrix::EMethod algo) { fInversion = algo; };
    /**
     * set smearing map - the correlation between reconstruced and simulated k*
     * @param smearing
     * @param opt
     */
    void SetSmearingMap(TH2D& smearing, ESmearInput opt = ESmearInput::kXsimYreco);
    /**
     * set the correlation function, important to correctly apply the smearing procedure
     * @param cf
     */
    void SetCorrelationFunction(const Hal::Femto1DCF& cf);
    /**
     * set corrfit map that will be smeared
     * @param map
     */
    void SetCorrFitMap(const Hal::CorrFitMapKstarRstar& map);
    /**
     *
     * @return smeared map, call after @see Calculate
     */
    Hal::CorrFitMapKstarRstar* GetMap() { return fSmearedMap; };
    /**
     * do mathematical calculations to get smearing map
     */
    void Calculate();
    /**
     *
     * @return correction C_unsmeared/C_smeared
     */
    TH2D* GetCorrection() const { return fRatio; };
    void SetTikhonovLambda(Double_t lambda) { fLambda = lambda; };
    virtual ~CorrFit1DSmearing();
    ClassDef(CorrFit1DSmearing, 0)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFIT1DSMEARING_H_ */
