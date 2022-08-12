/*
 * CorrFitKisiel.h
 *
 *  Created on: 27 lis 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITKISIEL_H_
#define HALCORRFITKISIEL_H_

#include "Array.h"
#include "CorrFit1DCF.h"
#include "CorrFitMapKstarRstar.h"
#include "Splines.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <vector>

class TH1D;

class TClonesArray;

/**
 * class for fitting correlation function by using map, similar procedure to
 * those used in A. Kisiel's CorrFit
 */
namespace Hal {
  class AnaFile;
  class CorrFitKisiel : public CorrFit1DCF {

  protected:
    std::vector<CorrFitMapKstarRstar*> fMaps;
    Bool_t fSkipError;
    TH1D* fOldNumErr = {nullptr};
    virtual void Check();
    virtual Double_t GetNumericalError(Int_t binX) const {
      if (fSkipError) return 0;
      return fMaps[0]->EvalNumErrorBin(binX, 1);
    };
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;
    virtual Double_t CalculateCFExtrapolated(const Double_t* x, const Double_t* params) const;
    virtual void Paint(Bool_t repaint = kTRUE, Bool_t refresh = kTRUE);

  public:
    CorrFitKisiel(Int_t params = 3);

    /**
     * skip numerical errrors during calculations
     */
    virtual void SkipNumErrors() { fSkipError = kTRUE; }
    /**
     * set map of CF
     * @param map
     */
    virtual void AddMap(CorrFitMapKstarRstar* map);

    /**
     * set map directly from histogram
     * @param h histogram with map
     * @param mapFrame map frame
     */
    [[deprecated]] virtual void LoadMap(TH2D* h, Femto::EKinematics mapFrame);
    /**
     * load map from "war
     */
    virtual ~CorrFitKisiel();
    ClassDef(CorrFitKisiel, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITKISIEL_H_ */
