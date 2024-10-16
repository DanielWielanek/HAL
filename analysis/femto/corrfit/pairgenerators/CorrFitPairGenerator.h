/*
 * CorrFitPairGenerator.h
 *
 *  Created on: 27 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATOR_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATOR_H_


#include "Array.h"
#include "CorrFitMapGroupConfig.h"
#include "FastAxisCalc.h"
#include "FemtoConst.h"

#include <TObject.h>

class TFile;
class TTree;
class TH1;
class TClonesArray;
/**
 * basic class for generate hbt pairs without analysis of data from generator (FemtoDumpPairAna)
 */


namespace Hal {
  class FemtoCorrFunc;
  class CorrFitPairGenerator : public TObject {
  protected:
    Bool_t fInited = {kFALSE};
    Bool_t fDebug  = {kFALSE};
    Bool_t fAbs    = {kFALSE};
    Int_t fPid1 = {0}, fPid2 = {0};
    Int_t fNBins = {0};
    Double_t fM1 = {0}, fM2 = {0};
    Double_t fLow = {0}, fHi = {1};
    Double_t fOutCut[2]       = {0, 0};
    Double_t fSideCut[2]      = {0, 0};
    Double_t fOverStep        = {0};
    Int_t fBinLimit           = {10000};
    TString fFileName         = {"pair.root"};
    TFile* fOutFile           = {nullptr};
    FemtoCorrFunc* fCF        = {nullptr};
    TTree* fOutTree           = {nullptr};
    TH1* fDebugHisto          = {nullptr};
    Hal::FemtoPair* fHbtPair  = {nullptr};
    Femto::EKinematics fFrame = {Femto::EKinematics::kLCMS};
    Array_1<Int_t> fLimitsN;
    Array_3<Int_t> fLimits3D;
    FastAxisCalc fXaxis, fYaxis, fZaxis;
    Array_1<Double_t> fCentersX;
    Array_1<Double_t> fCentersY, fCentersZ;
    enum class EGrouping { kOneDim, kThreeDim };
    EGrouping fGroupingFlag = {EGrouping::kOneDim};
    CorrFitMapGroupConfig fGrouping;
    std::vector<TClonesArray*> fSignalPairs;
    virtual void GenerateEvent() = 0;
    Int_t GetBin(Double_t val) const;

  public:
    CorrFitPairGenerator();
    /**
     * enables debug histogram
     */
    void EnableDebug() { fDebug = kTRUE; }
    virtual Bool_t Init();
    /**
     * set PID code for identical particles
     * @param pid
     */
    void SetPdg(Int_t pid) { fPid1 = fPid2 = pid; };
    /**
     * set pid for nonidentical codes
     * @param pid1
     * @param pid2
     */
    void SetPdg(Int_t pid1, Int_t pid2) {
      fPid1 = pid1;
      fPid2 = pid2;
    };
    /**
     * set correlation functions
     * @param cf
     */
    void SetCorrFctn(const FemtoCorrFunc& cf);
    /**
     * set name of the pair file
     * @param name
     */
    void SetOutput(TString name) { fFileName = name; };
    /**
     * set maximum number of pairs per bin
     * @param limit
     */
    void SetPairLimitPerBin(Int_t limit) { fBinLimit = limit; };
    virtual void Run(Int_t entries);
    virtual ~CorrFitPairGenerator();
    ClassDef(CorrFitPairGenerator, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATOR_H_ */
