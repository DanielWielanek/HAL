/*
 * CorrFitFunctor.h
 *
 *  Created on: 18 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITFUNCTOR_H_
#define HALCORRFITFUNCTOR_H_

#include "CorrFitParamsSetup.h"

#include "Array.h"

#include <TString.h>


class TFile;
class TTree;
namespace Hal {
  class CorrFitInfo;
  /**
   * class that represents entire map from corrfit tree
   */

  class CorrFitFunctor : public TObject {
    TString fMapFile;
    CorrFitInfo* fInfo;
    CorrFitParamsSetup fSetup;
    TTree* fChain;
    TFile* fFile;
    Int_t fParams;
    Array_1<Float_t>* fData;
    std::vector<int> fDimsArray;
    std::vector<int> fPosArray;

  public:
    CorrFitFunctor(TString map_file = "");
    CorrFitInfo* GetInfo() const { return fInfo; };
    void PrintInfo();
    /**
     * get parameter config
     * @param name
     * @param min
     * @param max
     * @param npoints
     * @return false if parameter not found
     */
    Bool_t GetParameterConfig(TString name, Double_t& min, Double_t& max, Int_t& npoints);
    Int_t GetNParams() const;
    TString GetParameterName(Int_t id) const { return fSetup.GetParName(id); };
    Array_1<Float_t>* GetData(Double_t* params);
    virtual ~CorrFitFunctor();
    ClassDef(CorrFitFunctor, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_HALCORRFITFUNCTOR_H_                                             \
        */
