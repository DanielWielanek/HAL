/*
 * CorrFit1DMomemtumSmearingCorrection.h
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFIT1DSMEARING_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFIT1DSMEARING_H_

#include <TObject.h>
class TH2D;

namespace Hal {
  class CorrFitMapKstarRstar;
  class Femto1DCF;
  /**
   * smeared map in PRF frame
   */
  class CorrFit1DSmearing : public TObject {
    Hal::Femto1DCF* fCF                    = {nullptr};
    Hal::CorrFitMapKstarRstar* fInputMap   = {nullptr};
    Hal::CorrFitMapKstarRstar* fSmearedMap = {nullptr};
    TH2D* fRatio                           = {nullptr};
    TH2D* fSmearingMap                     = {nullptr};
    TH2D* DoInversion(TH2D* h) const;

  public:
    enum class ESmearInput { kXsimYreco, kXrecoYsim };
    CorrFit1DSmearing() {};
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
    virtual ~CorrFit1DSmearing();
    ClassDef(CorrFit1DSmearing, 0)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFIT1DSMEARING_H_ */
