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
class TChain;
class TClonesArray;
namespace Hal {
  class CorrFitParamsSetup;
  class CorrFitMapGroupConfig;
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
    std::vector<TString> fUsedBranches;
    TFile* fFile  = {nullptr};
    TChain* fTree = {nullptr};

  protected:
    TString fPairFile;
    Int_t fJobId               = {-1};
    Int_t fMultiplyWeight      = {1};
    Int_t fMultiplyPreprocess  = {1};
    Int_t fMultiplyJobs        = {1};
    Int_t fTotalNumberOfPoints = {0};
    Int_t fPairThreshold       = {0};
    Int_t fPairsProcessed      = {0};
    Bool_t fIgnoreSing         = {kFALSE};
    Bool_t fImgMom             = {kFALSE};
    FemtoCorrFunc* fTempCF     = {nullptr};
    FemtoPair* fPair           = {nullptr};
    std::vector<FemtoCorrFunc*> fCF;
    FemtoFreezoutGenerator* fTempGenerator = {nullptr};
    std::vector<FemtoFreezoutGenerator*> fGenerator;
    FemtoWeightGenerator* fWeight    = {nullptr};
    CorrFitMapGroupConfig* fGrouping = {nullptr};
    std::vector<TClonesArray*> fSignalClones;      //!
    std::vector<TClonesArray*> fBackgroundClones;  //!
    enum class eDumpCalcMode { kSignalPairs = 0, kSignalBackgroundPairs = 1, kBackgroundPairsOnly = 2 };
    eDumpCalcMode fMode;
    /** export CF to root tree, this is used for compression of data, you can set
     * value of bin to -1, then such bin will not be used **/
    Bool_t SaveAsRawArray(TObject* cf, Int_t step);
    Bool_t ConfigureInput();
    Bool_t ConfigureRootInput();
    Bool_t ConfigureListInput();
    TString FindTreeName(TString name) const;
    Bool_t ConfigureFromXML();
    Int_t GetSimStepNo() const { return fMultiplyJobs * fJobId; }
    virtual void RunSignalPair()           = 0;
    virtual void RunSignalBackgroundPair() = 0;
    virtual void RunBackgroundPair()       = 0;
    virtual Bool_t IsVertical() const { return kFALSE; }
    /**
     * connects to needed branches
     * @return
     */
    virtual Bool_t ConnectToData() = 0;
    void LockUnusedBranches();
    void ConnectToSignal(const std::vector<TString>& branches);
    void ConnectToBackground(const std::vector<TString>& branches);
    /**
     * init place for data
     * @return
     */
    virtual Bool_t InitCFs() = 0;
    /**
     * init  generators - for vertical init all generators for horizontal init generators x multiplicity
     */
    virtual Bool_t InitGenerators(const std::vector<int>& dims, XMLNode* parameters, const CorrFitParamsSetup& setup) = 0;
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
