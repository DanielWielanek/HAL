/*
 * Femto1DCFAnaMapPairs.h
 *
 *  Created on: 27 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO1DCFANAMAPPAIRSDUMPED_H_
#define HALFEMTO1DCFANAMAPPAIRSDUMPED_H_

#include "Femto1DCFAnaMapMC.h"

#include <TString.h>


class TFile;
class TTree;
class TClonesArray;

/**
 * class for generating 1-dim HBT map, use pairs from dumped file
 */
namespace Hal {
  class CorrFitPairFile;
  class Femto1DCFAnaMapPairsDumped : public Femto1DCFAnaMapMC {
  protected:
    TString fInFile;
    CorrFitPairFile* fPairFile = {nullptr};
    Bool_t fUseBackground;
    Bool_t fWeightedBackround;
    Int_t fMaxEvents;
    void MakePairsWeighted(TClonesArray* arr, Bool_t num);
    void MakePairsUnWeighted(TClonesArray* arr, Bool_t num);
    virtual void Exec(Int_t /* npairs*/) { Run(0, 0); }

  public:
    Femto1DCFAnaMapPairsDumped();
    void SetInFile(TString filename);
    void UseBackground() { fUseBackground = kTRUE; };
    void UseWeightedBackground() { fWeightedBackround = kTRUE; };
    virtual Bool_t Init();

    /**
     *
     * @param start_event - first event
     * @param end_event - last event
     * if start == end process all events
     */
    virtual void Run(Int_t start_event = 0, Int_t end_event = 0);
    virtual ~Femto1DCFAnaMapPairsDumped();
    ClassDef(Femto1DCFAnaMapPairsDumped, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO1DCFANAMAPPAIRSDUMPED_H_ */
