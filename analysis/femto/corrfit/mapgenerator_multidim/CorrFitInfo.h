/*
 * CorrFitInfo.h
 *
 *  Created on: 10 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITINFO_H_
#define HALCORRFITINFO_H_

#include "CorrFitParamsSetup.h"

#include <TNamed.h>
#include <TString.h>

/**
 * class that represent basic info about settings used to calculate CF map
 */
namespace Hal {
  class CorrFitInfo : public TNamed {
    TObject* fCF;
    CorrFitParamsSetup fSetup;
    TString fMacroText;
    TString fPairFile;

  public:
    CorrFitInfo();
    CorrFitInfo(const CorrFitInfo& other);
    void SetCf(TObject* cf) { fCF = cf; }
    void SetPairFile(TString pairFile) { fPairFile = pairFile; }
    void SetSetup(const CorrFitParamsSetup& setup) { fSetup = setup; }
    void SetMacroText(const TString& macroText) { fMacroText = macroText; }
    void Print(Option_t* option = "") const;
    TObject* GetCf() const { return fCF; }
    TString GetMacroText() const { return fMacroText; }
    TString GetPairFile() const { return fPairFile; }
    CorrFitParamsSetup GetSetup() const { return fSetup; }
    virtual ~CorrFitInfo();
    ClassDef(CorrFitInfo, 1)
  };
}  // namespace Hal

#endif /* HALCORRFITINFO_H_ */
