/*
 * Femto1DCFAnaMapMCRoco.h
 *
 *  Created on: 2 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO1DCFANAMAPMCROCO_H_
#define HALFEMTO1DCFANAMAPMCROCO_H_

#include "Array.h"
#include "Femto1DMapGenerator.h"
#include "FemtoConst.h"
#include "FemtoFreezoutGenerator.h"
#include "FemtoWeightGenerator.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>


class TH1D;

/**
 * generate 2-dim CF map for 1D CF,
 */

namespace Hal {
  class DividedHisto2D;
  class Femto1DCF;
  class Femto1DCFAnaMapMCRoco : public Femto1DMapGenerator {
    Double_t fRMinEff, fRStep;
    Double_t fIntegralScale;
    Double_t* fSourceParams;
    TH1D* fSampleRandom;
    enum class EModelType { k1dModel, k3dModel, kOther };
    EModelType fModelType;
    Bool_t fDebugDistribution;


  protected:
    FemtoFreezoutGenerator* fGeneratorIntegrated;

  public:
    Femto1DCFAnaMapMCRoco();
    void DebugDistribution() { fDebugDistribution = kTRUE; };
    virtual Bool_t Init();
    /**
     * create map
     * @param pairs_per_bin number of pairs in single bin
     * @param refRadius reference radius used to calculate the relative density
     */
    virtual void Exec(Int_t pairs_per_bin, Bool_t autoscale = kFALSE);
    /**
     *
     * @param filename name of file with map
     */
    void SaveMap(TString filename);
    virtual ~Femto1DCFAnaMapMCRoco();
    ClassDef(Femto1DCFAnaMapMCRoco, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO1DCFANAMAPMC_H_ */
