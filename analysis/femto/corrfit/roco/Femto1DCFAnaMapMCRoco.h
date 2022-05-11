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
  class Femto1DCFAnaMapMCRoco : public TObject {
    Double_t fKStarMin, fKStarMax;
    Double_t fRMinEff, fRStep;
    Double_t fRmin;
    Double_t fRmax;
    Double_t* fSourceParams;
    Double_t fM1, fM2;
    Int_t fKStarBins;
    Femto::EKinematics fKinematics;
    TH1D* fSampleRandom;
    enum class EModelType { k1dModel, k3dModel, kOther };
    EModelType fModelType;

  protected:
    Int_t fRbins;
    FemtoPair* fPair;
    FemtoFreezoutGenerator* fGeneratorRaw;
    FemtoFreezoutGenerator* fGeneratorIntegrated;
    FemtoWeightGenerator* fWeight;
    DividedHisto2D* fMap;
    Array_1<Double_t> fRadiiBins;

  public:
    Femto1DCFAnaMapMCRoco();
    /**
     * set correlation function used for maps
     * @param cf
     */
    virtual void SetCorrFctn(const Femto1DCF& cf);
    /**
     * set radii range in generated map
     * @param r_bins number of r bins
     * @param r_min minimal radius
     * @param r_max maximal radius
     */
    void SetRadiusRange(Int_t r_bins, Double_t r_min, Double_t r_max);
    void SetFreezoutGenerator(const FemtoFreezoutGenerator& freez) { fGeneratorRaw = freez.MakeCopy(); }
    void SetWeight(const FemtoWeightGenerator& gen) { fWeight = gen.MakeCopy(); }
    virtual Bool_t Init();
    /**
     * create map
     * @param pairs_per_bin number of pairs in single bin
     * @param refRadius reference radius used to calculate the relative density
     */
    virtual void Exec(Int_t pairs_per_bin, Double_t refRadius = 0);
    /**
     *
     * @param filename name of file with map
     */
    void SaveMap(TString filename);
    DividedHisto2D* GetMap() const { return fMap; };
    virtual ~Femto1DCFAnaMapMCRoco();
    ClassDef(Femto1DCFAnaMapMCRoco, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO1DCFANAMAPMC_H_ */
