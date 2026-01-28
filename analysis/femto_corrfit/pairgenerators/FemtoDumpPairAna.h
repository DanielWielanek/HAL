/*
 * FemtoDumpPairAna.h
 *
 *  Created on: 9 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTODUMPPAIRANA_H_
#define HALFEMTODUMPPAIRANA_H_

#include "CorrFitMapGroupConfig.h"
#include "FemtoBasicAna.h"
#include "FemtoMiniPair.h"

class TClonesArray;
namespace Hal {


  class Package;
  class FemtoDumpPairAna : public FemtoBasicAna {
    Int_t fBinLimit;
    Array_1<Double_t> fLimitsN;
    Array_1<Double_t> fLimitsD;
    CorrFitMapGroupConfig fGrouping;
    Bool_t fWriteBackground;
    std::vector<TClonesArray*> fSignalPairs;
    std::vector<TClonesArray*> fBackgroundPairs;

  protected:
    virtual void ProcessFemtoPair();
    virtual void ProcessFemtoPair_Perfect() {};
    virtual void ProcessFemtoPair_Rotated();
    virtual void ProcessFemtoPair_Hemisphere();
    virtual void ProcessFemtoPair_Mixed();
    virtual Task::EInitFlag Init();

  public:
    FemtoDumpPairAna();
    virtual void Exec(Option_t* opt = "");
    void SetPairLimitPerBin(Int_t limit) { fBinLimit = limit; };
    void WriteBackground() { fWriteBackground = kTRUE; };
    virtual void FinishTask();
    virtual ~FemtoDumpPairAna();
    ClassDef(FemtoDumpPairAna, 1)
  };
}  // namespace Hal
#endif /* HALFEMTODUMPPAIRANA_H_ */
