/*
 * Femto1DMapGenerator.h
 *
 *  Created on: 26 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATORS_HALFEMTO1DMAPGENERATOR_H_
#define HALFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATORS_HALFEMTO1DMAPGENERATOR_H_

#include "Array.h"
#include "FemtoConst.h"

#include <TObject.h>
namespace Hal {
  class FemtoPair;
  class FemtoFreezoutGenerator;
  class FemtoWeightGenerator;
  class FemtoCorrFunc;
  class DividedHisto2D;
  class Femto1DCF;
  /**
   * basic class for "smarter" map generators
   */
  class Femto1DMapGenerator : public TObject {
  protected:
    Double_t fRMin, fRMax;
    Double_t fKStarMin, fKStarMax;
    Double_t fMass12, fMass22;
    Int_t fRBins, fKStarBins;
    Int_t fPid1, fPid2;
    Bool_t fIgnoreSign;
    Bool_t fUseFake;
    Femto::EKinematics fKinematics;
    Array_1<Double_t> fRadiiBins;
    FemtoPair* fPair;
    FemtoFreezoutGenerator* fGenerator;
    FemtoWeightGenerator* fWeight;
    DividedHisto2D* fMap;
    Femto1DMapGenerator(Bool_t fake);

  public:
    Femto1DMapGenerator();
    /**
     * set assumep pid of particles
     * @param pid1
     * @param pid2
     */
    void SetPid(Int_t pid1, Int_t pid2);
    /**
     * if true then histogram are filled without sign (e.g. with abs(kstar))
     * @param ignore
     */
    void IgnoreSign(Bool_t ignore = kTRUE) { fIgnoreSign = ignore; };
    /**
     * set range of R bins
     * @param bins
     * @param rmin
     * @param rmax
     * @param center if true them rmin and rmax correspond to the centers of first and last bin, not the lower/upper edges like
     * in ROOT histograms
     */
    void SetRBins(Int_t bins, Double_t rmin, Double_t rmax, Bool_t center = kFALSE);
    /**
     * set source generator algorithm
     * @param gen
     */
    virtual void SetGenerator(FemtoFreezoutGenerator& gen);
    /**
     * set weight computing algorithm
     * @param calc
     */
    virtual void SetWeightGenerator(FemtoWeightGenerator& calc);
    /**
     * set correlation function used for maps
     * @param cf
     */
    virtual void SetCorrFctn(const Femto1DCF& cf);
    /**
     * save calculated map
     * @param filename
     */
    virtual void SaveMap(TString filename);
    /**
     *
     * @return map
     */
    DividedHisto2D* GetMap() const { return fMap; };
    /** initialize this class, should be called before Run**/
    virtual Bool_t Init();
    virtual ~Femto1DMapGenerator();
    ClassDef(Femto1DMapGenerator, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATORS_HALFEMTO1DMAPGENERATOR_H_ */
