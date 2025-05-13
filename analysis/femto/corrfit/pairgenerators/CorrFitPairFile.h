/*
 * CorrFitPairFile.h
 *
 *  Created on: 22 sty 2025
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRFILE_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRFILE_H_

#include "FemtoConst.h"
#include "Object.h"

class TTree;
class TFile;
class TChain;
class TClonesArray;
namespace Hal {
  class FemtoMicroPair;
  class CorrFitMapGroupConfig;
  class CorrFitPairFile : public Object {
    TFile* fFile = {nullptr};
    TTree* fTree = {nullptr};
    TString fFileName;
    CorrFitMapGroupConfig* fConfig = {nullptr};
    std::vector<TClonesArray*> fSignals;
    std::vector<TClonesArray*> fBackground;
    Int_t fMode = {0};
    virtual Bool_t InitRead(Int_t startBranch, Int_t endBranch, Bool_t enableSignal, Bool_t enableBackground);
    virtual Bool_t InitWrite();

  public:
    /**
     * basic ctor
     * @param fileName name of the file
     * @param mode set create or recreate to make a new file, otherwise it will read data
     */
    CorrFitPairFile(TString fileName, TString mode);
    /**
     * init class, set startBranch and endBranch if want to read only part of data
     * @param startBranch
     * @param endBranch
     * @param return true if suceeded
     */
    Bool_t Init(Int_t startBranch = -1, Int_t endBranch = -1, Bool_t signal = kTRUE, Bool_t background = kTRUE);
    /**
     * return config file
     * @return
     */
    CorrFitMapGroupConfig* GetConfig() const { return fConfig; }
    /**
     *
     * @return number of entries in file
     */
    Int_t GetEntries() const;
    /**
     * like in TTree
     * @param i
     */
    void GetEntry(Int_t i);
    /** fill data
     *
     */
    void Fill();
    /**
     * set configuration, should be called before INIt
     * @param config
     */
    void SetConfig(const CorrFitMapGroupConfig& config);
    /**
     * clear all arrays
     */
    void ClearData();
    /**
     * return signal pairs at given index
     * @param idx
     * @return
     */
    TClonesArray* GetSignal(Int_t idx) const { return fSignals[idx]; }
    /**
     * return background pairs at given index
     * @param idx
     * @return
     */
    TClonesArray* GetBackground(Int_t idx) const { return fBackground[idx]; }
    /**
     *
     * @return file name
     */
    TString GetFileName() const { return fFileName; }
    Bool_t IsVirtual() const;
    Int_t GetSignalLength() const { return fSignals.size(); }
    Int_t GetBackgroundLength() const { return fBackground.size(); };
    virtual ~CorrFitPairFile();
    ClassDef(CorrFitPairFile, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRFILE_H_ */
