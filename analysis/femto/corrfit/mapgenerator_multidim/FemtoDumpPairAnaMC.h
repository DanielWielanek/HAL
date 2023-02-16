/*
 * FemtoDumpPairAnaMC.h
 *
 *  Created on: 15 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_FEMTODUMPPAIRANAMC_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_FEMTODUMPPAIRANAMC_H_

#include "Array.h"
#include "FemtoDumpPairAna.h"
#include "FemtoWeightGenerator.h"

#include <vector>

#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TString.h>

class TTree;

class TFile;

/**
 * class for generation of micro pairs for corrfit-multidim
 */
namespace Hal {
  class FemtoDumpPairAnaMC : public TObject {
    TString fFileName;
    TFile* fOutFile    = {nullptr};
    FemtoCorrFunc* fCF = {nullptr};
    TTree* fOutTree    = {nullptr};
    Bool_t fInited     = {kFALSE};
    Int_t fPid1, fPid2;
    Double_t fM1, fM2;
    Int_t fBinLimit = {10000};
    Femto::EKinematics fFrame;
    Array_1<Double_t> fLimitsN;
    Array_1<Double_t> fCentersX;
    Array_1<Double_t> fCentersY, fCentersZ;
    CorrFitMapGroupConfig fGrouping;

  protected:
    std::vector<TClonesArray*> fSignalPairs;
    virtual void GenerateEvent();

  public:
    FemtoDumpPairAnaMC();
    Bool_t Init();
    void SetPdg(Int_t pid) { fPid1 = fPid2 = pid; };
    void SetPdg(Int_t pid1, Int_t pid2) {
      fPid1 = pid1;
      fPid2 = pid2;
    };
    void SetCorrFctn(const FemtoCorrFunc& cf);
    void SetOutput(TString name) { fFileName = name; };
    void SetPairLimitPerBin(Int_t limit) { fBinLimit = limit; };
    void Run(Int_t entries);
    virtual ~FemtoDumpPairAnaMC();
    ClassDef(FemtoDumpPairAnaMC, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_FEMTODUMPPAIRANAMC_H_ */
