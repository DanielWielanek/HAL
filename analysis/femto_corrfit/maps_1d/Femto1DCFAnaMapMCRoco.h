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
#include "FemtoFreezeoutGenerator.h"
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
  class FastHist2D;
  class Femto1DCFAnaMapMCRoco : public Femto1DMapGenerator {
    class debugData {
    public:
      Bool_t fEnabled = {kFALSE};
      TH1D* monGaus1  = {nullptr};
      TH1D* monGaus2  = {nullptr};
      TH1D* monGaus3  = {nullptr};
      TH1D* monGaus4  = {nullptr};
      TH1D* monGaus5  = {nullptr};
      TH1D* monRaw    = {nullptr};
      void Init();
      void Save();
      ~debugData();
    };
    Double_t fIntegralScale = {1.0};
    std::vector<double> fSourceParams;
    std::vector<double> fKFill;
    std::vector<double> fKStar;
    std::vector<std::vector<double>> fParametrizations;
    enum class EModelType { k1dModel, k3dModel, kOther };
    EModelType fModelType     = {EModelType::kOther};
    Bool_t fDebugDistribution = {kFALSE};


  protected:
    FemtoSourceModel* fSourceModel                = {nullptr};
    FemtoSourceModel* fSourceModelntegrated       = {nullptr};
    FemtoSourceDensity* fDensityModel             = {nullptr};
    FemtoSourceDensity* fDensityIntegratedModel   = {nullptr};
    FemtoFreezeoutGenerator* fGeneratorIntegrated = {nullptr};
    void ComputePair(FastHist2D* num1, FastHist2D* num2, int& counter, int ikst);
    debugData fDebugData;  //!
    void InitParametrizations();
    void CalculateIntegral();

  public:
    Femto1DCFAnaMapMCRoco();
    void DebugDistribution() { fDebugDistribution = kTRUE; };
    virtual Bool_t Init();
    /**
     * create map
     * @param pairs_per_bin number of pairs in single bin
     * @param refRadius reference radius used to calculate the relative density
     */
    virtual void Run(Int_t pairs_per_bin, Bool_t autoscale = kFALSE);
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
