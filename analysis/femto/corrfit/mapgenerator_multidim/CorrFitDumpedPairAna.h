/*
 * FemtoNDimMap.h
 *
 *  Created on: 24 paź 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTONDIMMAP_H_
#define HALFEMTONDIMMAP_H_

#include "FemtoFreezoutGenerator.h"
#include "XMLNode.h"

#include <TObject.h>
#include <TString.h>


/**
 * basic class for generation of multidim maps
 */
class TFile;
class TTree;
class TClonesArray;
namespace Hal {
  class Femto1DCF;
  class Femto3DCF;
  class FemtoCorrFunc;
  class FemtoMicroPair;
  class FemtoPair;
  class FemtoSHCF;
  class FemtoWeightGenerator;

  /**
   * analysis of pairs created by FemtoDumpPairAna
   */

  class CorrFitDumpedPairAna : public TObject {
  protected:
    TString fPairFile;
    TFile* fFile;
    TTree* fTree;
    Int_t fJobId;
    Int_t fMultiplyWeight;
    Int_t fMultiplyPreprocess;
    Int_t fMultiplyJobs;
    Bool_t fIgnoreSing;
    Bool_t fImgMom;
    FemtoCorrFunc* fTempCF;
    FemtoCorrFunc** fCF;
    FemtoPair* fPair;
    FemtoMicroPair* fMiniPair;
    FemtoFreezoutGenerator* fTempGenerator;
    FemtoFreezoutGenerator** fGenerator;
    FemtoWeightGenerator* fWeight;
    enum class eDumpCalcMode { kSignalPairs = 0, kSignalBackgroundPairs = 1, kBackgroundPairsOnly = 2 };
    eDumpCalcMode fMode;
    /** export CF to root tree, this is used for compression of data, you can set
     * value of bin to -1, then such bin will not be used **/
    void RootExport1D(Femto1DCF* cf, Int_t step);
    void RootExport3D(Femto3DCF* cf, Int_t step);
    void RootExportSH(FemtoSHCF* cf, Int_t step);
    Bool_t ConfigureInput();
    Bool_t ConfigureFromXML();
    virtual void RunSignalPairs(Int_t nEvents)           = 0;
    virtual void RunSignalBackgroundPairs(Int_t nEvents) = 0;
    virtual void RunBackgroundPairs(Int_t nEvents)       = 0;
    virtual Bool_t IsVertical() const { return kFALSE; }
    virtual Bool_t Connect() = 0;

  protected:
    /**
     * set freezout generator
     * @param gen
     */
    void SetGenerator(const FemtoFreezoutGenerator& gen) { fTempGenerator = gen.MakeCopy(); };

  public:
    CorrFitDumpedPairAna(Int_t jobid = -1, Int_t mapsPerAna = -1);
    /**
     * set correlation function, in current version is should be FemtoCorrFuncSimple
     * @param func
     */
    void SetCorrFunc(const FemtoCorrFunc& func);
    /**
     * set weight  generator of weights
     * @param weight
     */
    void SetWeightGenerator(const FemtoWeightGenerator& weight);
    /**
     * use background pairs for background and signal pairs for signal
     */
    void UseBackground() { fMode = eDumpCalcMode::kSignalBackgroundPairs; };
    /**
     * use background pairs for numerator and denominator
     */
    void UseBackgroundOnly() { fMode = eDumpCalcMode::kBackgroundPairsOnly; }
    /**
     * use MC momentum not reconstructed
     */
    void UseImgMomenta() { fImgMom = kTRUE; };
    virtual Bool_t Init();
    virtual void Run(Int_t maxEvents = -1);
    virtual void Finish() = 0;
    /**
     * ignore sign of k* q
     */
    void IgnoreSign() { fIgnoreSing = kTRUE; };
    /**
     * for each pair generatore freezout coords and calculate weight m-times
     * @param m
     */
    void SetMultiplyFactorWeight(Int_t m) { fMultiplyWeight = m; };
    /**
     * for each pair preprocess pair m-times, if multiply factor weight is not 1 then total number of calculations per pair
     * is fMultiplyWeight*fMultiplyPreprocess
     * @param m
     */
    void SetMultiplyFactorPreprocess(Int_t m) { fMultiplyPreprocess = m; };
    /**
     * set job id
     * @param jobId
     */
    void SetJobID(Int_t jobId) { fJobId = jobId; };
    /**
     * set path to pair file
     * @param pairFile
     */
    void SetPairFile(TString pairFile) { fPairFile = pairFile; };
    virtual void Print(Option_t* option = "") const;
    /**
     * preprocess pair
     */
    virtual void PreprocessPair() {};
    virtual void PreprocessMixedPair() {};
    virtual ~CorrFitDumpedPairAna();
    ClassDef(CorrFitDumpedPairAna, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATORS_HALFEMTONDIMMAP_H_                                                       \
        */
