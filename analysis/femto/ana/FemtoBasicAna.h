/*
 Be * FemtoBasicAna2.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOBASICANA_H_
#define HALFEMTOBASICANA_H_

#include "Array.h"
#include "FemtoCorrFunc.h"
#include "FemtoFreezoutGenerator.h"
#include "FemtoWeightGenerator.h"
#include "TwoTrackAna.h"

#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>


/**
 * basic class for all femtoscopic analysis user can add here weight algorithm
 * and freezoug generator
 */

namespace Hal {
  class FemtoBasicAna : public TwoTrackAna {
  protected:
    /**
     * assumed masses of particles (if <0 then value from data format is used)
     */
    Int_t fPdg1, fPdg2;
    /**
     * if false then true momenta are always used
     */
    Bool_t fUseImgMomenta;
    /**
     * true if absolute values of some variables should be used
     */
    Bool_t fIgnoreSign;
    /**
     * weight caused by quantum statistic or FSI
     */
    Double_t fFsiWeight;
    /**
     * collection of CF assigned to given pair
     */
    Int_t fEventCollectionCF;
    /**
     * pointer to processed femtoscopic pair
     */
    FemtoPair* fFemtoPair;
    /**
     * pointer to code that calculate weight of pair
     */
    FemtoWeightGenerator* fCalc;
    /**
     * pointer to class that generate freezout positions
     */
    FemtoFreezoutGenerator* fFreezoutGenerator;
    /**
     * array of correlation function
     */
    ObjectMatrix_2* fCFs;
    /**
     * temporary correlation function removed after init
     */
    FemtoCorrFunc* fCFTemp;
    virtual void CheckCutContainerCollections();
    /**
     * preprocess femto pair
     */
    virtual void PreprocessFemtoPair();
    /**
     * preprocess femto pair from "perfect background"
     */
    virtual void PreprocessFemtoPair_Perfect() {};
    /**
     * preprocess femto pair "rotated  background"
     */
    virtual void PreprocessFemtoPair_Rotated() { PreprocessFemtoPair(); };
    /**
     * preprocess femto pair "hemisphere background"
     */
    virtual void PreprocessFemtoPair_Hemisphere() { PreprocessFemtoPair(); };
    /**
     * preprocess femto pair "mixed background"
     */
    virtual void PreprocessFemtoPair_Mixed() {};
    /**
     * preprocess femto pair "charged background"
     */
    virtual void PreprocessFemtoPair_Charged() { PreprocessFemtoPair(); };
    /**
     * process  femto pair from "charged background"
     */
    virtual void ProcessFemtoPair();
    /**
     * process  femto pair from "perfect background"
     */
    virtual void ProcessFemtoPair_Perfect();
    /**
     * process  femto pair from "roated background"
     */
    virtual void ProcessFemtoPair_Rotated();
    /**
     * process  femto pair from "hemisphere background"
     */
    virtual void ProcessFemtoPair_Hemisphere();
    /**
     * process  femto pair from "mixed background"
     */
    virtual void ProcessFemtoPair_Mixed();
    /**
     * process  femto pair from "charged background"
     */
    virtual void ProcessFemtoPair_Charged();
    // converting to Pair, using preprocess method to compute stuff like
    // freezout coordinates
    virtual void ProcessPair();
    virtual void ProcessPair2() { ProcessPair(); };
    virtual void ProcessPair_Perfect();
    virtual void ProcessPair2_Perfect() { ProcessPair_Perfect(); };
    virtual void ProcessPair_Mixed();
    virtual void ProcessPair2_Mixed() { ProcessPair_Mixed(); };
    virtual void ProcessPair_Rotated();
    virtual void ProcessPair2_Rotated() { ProcessPair_Rotated(); };
    virtual void ProcessPair_Hemisphere();
    virtual void ProcessPair2_Hemisphere() { ProcessPair_Hemisphere(); };
    virtual void ProcessPair_ChargedId();
    virtual void ProcessPair_ChargedNId() { ProcessPair_ChargedId(); };
    virtual Package* Report() const;
    virtual void InitMemoryMap();
    virtual void ProcessEvent();
    /**
     * init map with CF-s
     * @return
     */
    virtual Bool_t InitArray();
    virtual Task::EInitFlag Init();

  public:
    /**
     * default c-tor
     * @param histo_axis_no number of axis in histograms
     */
    FemtoBasicAna();
    /**
     * copy c-tor
     * @param ana
     */
    FemtoBasicAna(const FemtoBasicAna& ana);
    /**
     * assignement operator
     * @param other
     * @return
     */
    FemtoBasicAna& operator=(const FemtoBasicAna& other);
    virtual void FinishTask();
    /**
     * set correlaction function
     * @param h correlaction function class
     */
#if __cplusplus >= 201402L
    [[deprecated]]
#endif
    virtual void
    SetCorrFctn(FemtoCorrFunc* h);
    /**
     * set correlaction function
     * @param h correlaction function class
     */
    virtual void SetCorrFctn(const FemtoCorrFunc& h);
    /**
     * set weight algorithm used for calculation CF in MC data
     * @param gen
     */
    void SetWeight(const FemtoWeightGenerator& gen) { fCalc = gen.MakeCopy(); }
    /**
     * set freezout generator that creates points of particles creation
     * @param gen freezout generator
     */
    void SetFreezoutGenerator(const FemtoFreezoutGenerator& gen) { fFreezoutGenerator = gen.MakeCopy(); }
    /**
     * set assumed pid of particle (replaces SetMass)
     * @param PID
     */
    void SetPdg(Int_t PID) { fPdg1 = fPdg2 = PID; }
    /**
     * set assumed pids of particles (replaces SetMass)
     * @param PID1
     * @param PID2
     */
    void SetPdg(Int_t PID1, Int_t PID2) {
      fPdg1 = PID1;
      fPdg2 = PID2;
    };
    /**
     *  ignore sign during fillng CF, e.g. if function is filled with k*_out=-0.5
     * it will be filled as with k*_out = 0.5
     */
    void IgnoreSign() { SetOption("ignore_sign"); };
    /**
     * use imaginary momenta for calculations (works only with complex data
     * format!)
     */
    void UseImgMomenta() { SetOption("use_im_momenta"); };
    /**
     * set option
     * @param option - option from TwoTrackAna or  "ignore_sign" if user want
     * to fill abs values calculated by kinematics class. . It's also possibe to
     * set "use_im_momenta" for using complex format with imaginary data
     * kinematics
     */
    virtual void SetOption(Option_t* option);
    virtual ~FemtoBasicAna();
    ClassDef(FemtoBasicAna, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOBASICANA_H_ */
