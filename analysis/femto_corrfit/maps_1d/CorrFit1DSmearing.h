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
   * smeared map in PRF frame,recommended usage:
   * - set your experimental CF - this allow realistically estimate the shape of denominator
   * - recommended inversion is kNone - matrix inversion, Thikonov are sometimes unstable
   * - set acceptance map - using M^T allow estimate more realistic shape of real denominator
   * - acceptance map is not mandatory
   */
  class CorrFit1DSmearing : public TObject {
  public:
    enum class ESmearInput { kXsimYreco, kXrecoYsim };

  private:
    SmearAlgoMatrix::EMethod fInversion    = {SmearAlgoMatrix::EMethod::kNone};
    Hal::Femto1DCF* fCF                    = {nullptr};
    Hal::CorrFitMapKstarRstar* fInputMap   = {nullptr};
    Hal::CorrFitMapKstarRstar* fSmearedMap = {nullptr};
    Hal::SmearAlgoMatrix* fAlgo            = {nullptr};
    TH2D* fRatio                           = {nullptr};
    TH2D* fSmearingMap                     = {nullptr};
    TH2D* DoInversion(TH2D* h) const;
    TH1D* fAcceptanceMap = {nullptr};
    TMatrixD fSimDenominator;
    TMatrixD fAcceptance;
    Double_t fLambda = {0.01};
    Bool_t InitSmearing();
    void DoMapSmearing();
    void FinishSmearing();
    TMatrixD VecMultiply(const TMatrixD& a, const TMatrixD& b) const;

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
     * set acceptance map, used to correct unsmeared sim distribution
     * @param map
     */
    void SetAcceptanceMap(const TH1D& map);
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
    /**
     *
     * @return expected simulated denominator
     */
    TMatrixD GetSimDenominator() const { return fSimDenominator; }
    void SetTikhonovLambda(Double_t lambda) { fLambda = lambda; };
    virtual ~CorrFit1DSmearing();
    ClassDef(CorrFit1DSmearing, 0)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFIT1DSMEARING_H_ */
