/*
 * CorrFitDumpedAnaVertical.h
 *
 *  Created on: 1 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITDUMPEDPAIRANAVERTICAL_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITDUMPEDPAIRANAVERTICAL_H_

#include "CorrFitDumpedPairAna.h"
namespace Hal {
  class CorrFitVerticalSlices;
  class CorrFitDumpedPairAnaVertical : public CorrFitDumpedPairAna {
  protected:
    TClonesArray** fPairsSignal     = {nullptr};
    TClonesArray** fPairsBackground = {nullptr};
    CorrFitVerticalSlices** fSlices = {nullptr};  //[fMultiplyJobs]
    Double_t fMomMin, fMomMax;
    std::vector<std::vector<std::pair<Double_t, Int_t>>> fParSet;
    void RunSignalPairs(Int_t nEvents);
    void RunSignalBackgroundPairs(Int_t nEvents);
    void RunBackgroundPairs(Int_t nEvents);
    Bool_t Connect();

  public:
    CorrFitDumpedPairAnaVertical(Int_t jobid = -1, Int_t mapsPerAna = -1);
    Bool_t Init();
    void Finish();
    virtual ~CorrFitDumpedPairAnaVertical();
    ClassDef(CorrFitDumpedPairAnaVertical, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_CORRFITDUMPEDPAIRANAVERTICAL_H_ */
