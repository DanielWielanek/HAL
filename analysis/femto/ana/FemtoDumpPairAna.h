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

#include "FemtoBasicAna.h"

#include "FemtoMiniPair.h"

class TClonesArray;
namespace Hal {
  class Package;
  class FemtoDumpPairAna : public FemtoBasicAna {
    Int_t fBinLimit;
    Int_t fNBins;
    Array_1<Double_t> fLimitsN;
    Array_1<Double_t> fLimitsD;

    Double_t fStep, fMax;
    Bool_t fWriteBackground;
    Bool_t fGroup;
    Bool_t fGroupKstar;
    std::vector<TClonesArray*> fSignalPairs;
    std::vector<TClonesArray*> fBackgroundPairs;
    std::vector<Double_t> fQinvLimits;
    Double_t fXmin, fXmax, fXstep;
    Int_t fXbins;
    Int_t FindBinZ() const;
    Int_t FindBinX() const;

  protected:
    virtual void ProcessFemtoPair();
    virtual void ProcessFemtoPair_Perfect() {};
    virtual void ProcessFemtoPair_Rotated();
    virtual void ProcessFemtoPair_Hemisphere();
    virtual void ProcessFemtoPair_Mixed();
    virtual Task::EInitFlag Init();

  public:
    FemtoDumpPairAna();
    void Group() { fGroup = kTRUE; }
    virtual void Exec(Option_t* opt = "");
    void SetPairLimitPerBin(Int_t limit) { fBinLimit = limit; };
    void WriteBackground() { fWriteBackground = kTRUE; }
    virtual Package* Report() const;
    virtual ~FemtoDumpPairAna();
    ClassDef(FemtoDumpPairAna, 1)
  };
}  // namespace Hal
#endif /* HALFEMTODUMPPAIRANA_H_ */
