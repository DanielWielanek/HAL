/*
 * FemtoFreezoutsAna.h
 *
 *  Created on: 15-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOFREEZOUTSANA_H_
#define HALFEMTOFREEZOUTSANA_H_

#define SIMPLIFIED_PRF_COMPUTING_IN_FREEZOUT
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH3.h>

#include "Cut.h"
#include "FemtoBasicAna.h"
#include "FemtoConst.h"
#include "FemtoFastCut.h"
#include "FemtoFreezoutGenerator.h"
#include "HistogramManager.h"
#include "TwoTrackAna.h"

/**
 * basic class for obtaint source emission function
 */

namespace Hal {
  class FemtoFreezoutsAna : public TwoTrackAna {
  public:
    enum class EMode { kPRF, kLCMS, kGammaLCMS, kRaw };

  protected:
    /**
     * assumed masses of particles (if <0 then value from data format is used)
     */
    Int_t fPdg1, fPdg2;
    Double_t fX, fY, fZ, fT, fCut;
    FemtoFastCut* fFastCut;
    EMode fKinematicsMode;
    Bool_t fIgnoreSign;
    Bool_t fUseFakeMomenta;
    FemtoPair* fFemtoPair;
    Int_t fBins[3];
    Double_t fHistoMin[3];
    Double_t fHistoMax[3];
    FemtoFreezoutGenerator* fFreezoutGenerator;
    HistogramManager_3_1D<TH1D>* fHistograms1d;
    HistogramManager_3_3D<TH3D>* fHistograms3d;
    void ComputePRF();
    void ComputeLCMS();
    void ComputePRFL();
    void ComputeRaw();
    void ComputeLCMSGamma();
    virtual void ProcessFemtoPair();
    virtual void PreprocessFemtoPair();
    virtual void ProcessPair();
    virtual void ProcessPair2() { ProcessPair(); };
    virtual Package* Report() const;
    virtual Task::EInitFlag Init();

  public:
    FemtoFreezoutsAna();
    /**
     * cop constructor
     * @param ana
     */
    FemtoFreezoutsAna(const FemtoFreezoutsAna& ana);
    /**
     * set PID (if no MC mass/energy is avaiable
     * @param pdg
     */
    void SetPdg(Int_t pdg) { fPdg1 = fPdg2 = pdg; };
    /**
     * set PID (if no MC mass/energy is avaiable for  nonidentical pairs
     * @param pdg
     */
    void SetPdg(Int_t pdg1, Int_t pdg2) {
      fPdg1 = pdg1;
      fPdg2 = pdg2;
    };
    void SetOption(Option_t* opt);
    /**
     * set cut on momentum difference particles (k* in prf or qinv in lcms frame)
     * @param cut momentu cut
     */
    void SetMomentumCut(Double_t cut);
    void IgnoreSign() { fIgnoreSign = kTRUE; };
    void SetFrame(EMode mode) { fKinematicsMode = mode; }
    void SetAxes(Int_t bins, Double_t min, Double_t max);
    void SetOutAxis(Int_t bins, Double_t min, Double_t max);
    void SetSideAxis(Int_t bins, Double_t min, Double_t max);
    void SetLongAxis(Int_t bins, Double_t min, Double_t max);
    virtual void AddCut(const Cut& cut, Option_t* opt = " ");
    /**
     * set freezout generator (this is optional)
     * @param freez
     */
    void SetFreezoutGenerator(const FemtoFreezoutGenerator& freez) { fFreezoutGenerator = freez.MakeCopy(); };
    void SetFastPairCut(const FemtoFastCut& cut) { fFastCut = cut.MakeCopy(); };
    virtual ~FemtoFreezoutsAna();
    ClassDef(FemtoFreezoutsAna, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOFREEZOUTSANA_H_ */
