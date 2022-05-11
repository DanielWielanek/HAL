/*
 * FemtoWeightGenerator.h
 * 	derived from ALICE AliFemtoWeightGenerator
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOWEIGHTGENERATOR_H_
#define HALFEMTOWEIGHTGENERATOR_H_

#include "FemtoConst.h"
#include "Package.h"

#include <Rtypes.h>
#include <RtypesCore.h>


namespace Hal {
  class FemtoPair;
  /**
   * abstract class for generating weights, code derived from AliROOT
   */
  class FemtoWeightGenerator : public TObject {
  protected:
    /**
     * Type of the pair for which the calculation is done
     */
    Femto::EPairType fPairType;
    /**
     * id of task that own this weight
     */
    Int_t fTaskID;
    /**
     * relative momentum out component in PRF
     */
    Double_t fKStarOut;
    /**
     * relative momentum side component in PRF
     */
    Double_t fKStarSide;
    /**
     * relative momentum long component in PRF
     */
    Double_t fKStarLong;
    /**
     * relative momentum magnitude
     */
    Double_t fKStar;
    /**
     * elative separation out component in PRF
     */
    Double_t fRStarOut;
    /**
     * relative separation side component in PRF
     */
    Double_t fRStarSide;
    /**
     * relative separation long component in PRF
     */
    Double_t fRStarLong;
    /**
     *  relative separation magnitude
     */
    Double_t fRStar;

  public:
    /**
     * default constructor
     */
    FemtoWeightGenerator();
    /**
     * copy constructor
     * @param aModel object to copy
     */
    FemtoWeightGenerator(const FemtoWeightGenerator& aModel);
    /**
     * make copy
     * @return
     */
    virtual FemtoWeightGenerator* MakeCopy() const { return new FemtoWeightGenerator(*this); };
    /**
     * = operator
     * @param aModel object to copy
     * @return new copy of class
     */
    virtual FemtoWeightGenerator& operator=(const FemtoWeightGenerator& aModel);
    /**
     * return weight of pair
     * @param pair pair used for calculating weights
     * @return calculated weight
     */
    virtual Double_t GenerateWeight(FemtoPair* pair);
    /**
     * set pair type
     * @param aPairType pair type
     */
    virtual void SetPairType(Femto::EPairType aPairType);
    /**
     * set pair type from pair
     * @param pair pair of particles
     */
    virtual void SetPairTypeFromPair(FemtoPair* pair);
    /**
     *
     * @return pair type
     */
    virtual Femto::EPairType GetPairType() const;
    /**
     *
     * @param pair pair of tracks
     * @return pair type form pai
     */
    virtual Femto::EPairType GetPairTypeFromPair(FemtoPair* pair);
    /**
     *
     * @return k* momentum difference of last pair where weight was calculated
     */
    virtual Double_t GetKStar() const;
    /**
     *
     * @return k*out of last pair where weight was calculated
     */
    virtual Double_t GetKStarOut() const;
    /**
     *
     * @return k*side of last pair where weight was calculated
     */
    virtual Double_t GetKStarSide() const;
    /**
     *
     * @return k*long of last pair where weight was calculated
     */
    virtual Double_t GetKStarLong() const;
    /**
     *
     * @return r* of last pair where weight was calculated
     */
    virtual Double_t GetRStar() const;
    /**
     *
     * @return r*out of last pair where weight was calculated
     */
    virtual Double_t GetRStarOut() const;
    /**
     *
     * @return r*side of last pair where weight was calculated
     */
    virtual Double_t GetRStarSide() const;
    /**
     *
     * @return r*long of last pair where weight was calculated
     */
    virtual Double_t GetRStarLong() const;
    /**
     * clone this class
     * @return clone of this class
     */
    virtual FemtoWeightGenerator* Clone(const char* /*newname*/ = "") const { return 0; };
    /**
     *
     * @return pdg code for first particle
     */
    virtual Int_t GetPdg1();
    /**
     *
     * @return pdg code for second particle
     */
    virtual Int_t GetPdg2();
    /**
     * initlalize this class
     */
    virtual Bool_t Init(Int_t task_id, FemtoPair* pair);
    /**
     * return true if pair type is correctly supported
     * @param type
     * @return
     */
    virtual Bool_t IsPairSupported(Femto::EPairType type) const { return kFALSE; };
    virtual void Print(Option_t* option = "") const;
    virtual Package* Report() const;
    virtual ~FemtoWeightGenerator();

    ClassDef(FemtoWeightGenerator, 1)
  };

  inline Double_t FemtoWeightGenerator::GetKStar() const { return fKStar; }

  inline Double_t FemtoWeightGenerator::GetKStarOut() const { return fKStarOut; }

  inline Double_t FemtoWeightGenerator::GetKStarSide() const { return fKStarSide; }

  inline Double_t FemtoWeightGenerator::GetKStarLong() const { return fKStarLong; }

  inline Double_t FemtoWeightGenerator::GetRStar() const { return fRStar; }

  inline Double_t FemtoWeightGenerator::GetRStarOut() const { return fRStarOut; }

  inline Double_t FemtoWeightGenerator::GetRStarSide() const { return fRStarSide; }

  inline Double_t FemtoWeightGenerator::GetRStarLong() const { return fRStarLong; }

  inline Bool_t FemtoWeightGenerator::Init(Int_t task_id, FemtoPair* /*pair*/) {
    fTaskID = task_id;
    return kTRUE;
  }

  inline Package* FemtoWeightGenerator::Report() const { return new Package(this); }
}  // namespace Hal
#endif /* HALFEMTOWEIGHTGENERATOR_H_ */
