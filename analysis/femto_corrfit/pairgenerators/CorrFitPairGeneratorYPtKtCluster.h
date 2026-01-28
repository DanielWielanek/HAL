/*
 * CorrFitPairGeneratorYPtKtCluster.h
 *
 *  Created on: 14 paź 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTKTCLUSTER_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTKTCLUSTER_H_

#include <Rtypes.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include "CorrFitPairGenerator.h"

/**
 * use different method of calculation - generate rho(pt1,y1,phi1) that describe density of
 * pairs for given k* qinv
 */

namespace Hal {

  class CorrFitPairGeneratorYPtKtCluster : public CorrFitPairGenerator {
    TLorentzVector GeneratePairPRF(TLorentzVector& p1, TVector3& vec);
    TLorentzVector GeneratePairLCMS(TLorentzVector& p1, TVector3& vec);

  public:
    CorrFitPairGeneratorYPtKtCluster() {};
    virtual ~CorrFitPairGeneratorYPtKtCluster() {};
    ClassDef(CorrFitPairGeneratorYPtKtCluster, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTKTCLUSTER_H_ */
