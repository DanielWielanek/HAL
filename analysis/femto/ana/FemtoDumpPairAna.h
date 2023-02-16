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
  class CorrFitMapGroupConfig : public Object {
    Int_t fMode = {0};
    Int_t fBins;
    Double_t fMin, fMax, fStep;
    Femto::EKinematics fFrame = {Femto::EKinematics::kLCMS};

  public:
    CorrFitMapGroupConfig();
    void SetGroupByKLong() { fMode = 1; };
    void SetGroupByKStar() { fMode = 0; };
    void SetAxis(Int_t bins, Double_t min, Double_t max);
    Femto::EKinematics GetFrame() const { return fFrame; }
    void SetFrame(Femto::EKinematics frame) { fFrame = frame; }
    Int_t GetBin(const FemtoPair* pair) const;
    Int_t GetNbins() const { return fBins; }
    Bool_t GroupByLong() const;
    Bool_t GroupByKStar() const;
    Double_t GetMin() const { return fMin; };
    Double_t GetMax() const { return fMax; };
    virtual void Add(const Object* pack);
    std::vector<TString> GetBranches(Double_t min, Double_t max, Bool_t signal = kTRUE) const;
    virtual ~CorrFitMapGroupConfig() {};
    ClassDef(CorrFitMapGroupConfig, 1)
  };  // namespace Hal

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
