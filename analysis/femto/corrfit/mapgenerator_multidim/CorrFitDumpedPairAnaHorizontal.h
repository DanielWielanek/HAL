/*
 * CorrFitDumpedPairAnaHorizontal.h
 *
 *  Created on: 1 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITDUMPEDPAIRANAHORIZONTAL_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITDUMPEDPAIRANAHORIZONTAL_H_

#include "CorrFitDumpedPairAna.h"

namespace Hal {
  class CorrFitDumpedPairAnaHorizontal : public CorrFitDumpedPairAna {
  protected:
    TClonesArray* fPairsSignal     = {nullptr};
    TClonesArray* fPairsBackground = {nullptr};

    void RunSignalPairs(Int_t nEvents);
    void RunSignalBackgroundPairs(Int_t nEvents);
    void RunBackgroundPairs(Int_t nEvents);
    Bool_t Connect();

  public:
    CorrFitDumpedPairAnaHorizontal(Int_t jobid = -1, Int_t mapsPerAna = -1);
    Bool_t Init();
    void Finish();
    virtual ~CorrFitDumpedPairAnaHorizontal();
    ClassDef(CorrFitDumpedPairAnaHorizontal, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITDUMPEDPAIRANAHORIZONTAL_H_ */
