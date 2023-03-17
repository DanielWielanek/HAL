/*
 * FemtoWeightGeneratorLednicky.h
 *
 *   derived from ALICE AliFemtoWeightGeneratorLednicky
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOWEIGHTGENERATORLEDNICKY_H_
#define HALFEMTOWEIGHTGENERATORLEDNICKY_H_

#include "FemtoWeightGenerator.h"

/**
 * wrapper in C++ for R. Lednicky's code, derived form AliROOT
 */
namespace Hal {
  class FemtoWeightGeneratorLednicky : public FemtoWeightGenerator {
    friend class FemtoWeightGeneratorResidual;

  protected:
    /**
     * normal weight
     */
    double fWei;
    /**
     * weight with nuclear influence
     */
    double fWein;
    /**
     * weight
     */
    double fWeif;
    /**
     * weight for the denominator
     */
    double fWeightDen;
    /**
     * if set to 1 default parameters will be used
     */
    int fItest;  //

    // int mNs;
    /**
     * switch coulomb interaction on/off
     */
    int fIch;
    /**
     * switch quantum statistics on/off
     */
    int fIqs;
    /**
     * switch strong interaction on/off
     */
    int fIsi;
    /**
     * switch 3rd body influence on/off
     */
    int fI3c;
    /**
     * mass of the third body
     */
    double fNuclMass;
    /**
     * charge of the third body
     */
    double fNuclCharge;
    /**
     * use spherical approximation
     */
    bool fSphereApp;
    /**
     * use square well approximation
     */
    bool fT0App;

    // Pair identification
    /**
     * internal pair type code
     */
    int fLL;
    /**
     * sign of the 3rd body charge
     */
    short fNuclChargeSign;
    /**
     * are particle in right order ?
     */
    bool fSwap;
    /**
     * number of supported pairs
     */
    int const fLLMax;
    /**
     * precision
     */
    int fNS;
    /**
     * number of process pairs of each type
     */
    int* fNumProcessPair;  //! [30]
    // WTF? Streamer?
    /**
     *number of unidentified pairs
     */
    int fNumbNonId;

    int fKpKmModel;  // ij (i=1..4, j=1..4; see AliFemtoFsiWeightLednicky.F)
    int fPhi_OffOn;  // 0->Phi Off,1->Phi On
    int fNS_4;       // set NS is equal to 4

    // Interface to the fortran functions
    /**
     * interface to fortran fucntion
     */
    void FsiInit();
    /**
     * interface to fortran fucntion
     */
    void FsiSetLL();
    /**
     * interface to fortran fucntion
     */
    void FsiNucl();
    /**
     * interface to fortran fucntion
     */
    bool SetPid(const int aPid1, const int aPid2);
    /**
     * interface to fortran function
     */
    void FsiSetKpKmModelType();
    /**
     * set pair type and configure proper FSI/QS options
     * @param pair
     */
    virtual void SetPairTypeFromPairAndConfigureFSI(FemtoPair* pair);
    /**
     * convert pair ID to lednicky pair ID
     * @param pair_pid
     * @return
     */
    void PairIDToLL(Femto::EPairType pair_pid, Int_t& LL, Int_t& tCharge) const;
    /**
     * convert lednicky's weight ID to pair ID
     * @param ll
     * @return
     */
    Femto::EPairType LLToPairID(Int_t ll) const;
    /**
     * fast function to enable flags
     * @param qs quantum statistics
     * @param s strong interaction
     * @param c coulomb interaction
     */
    inline void SetFlags(Int_t qs, Int_t s, Int_t c) {
      fIqs = qs;
      fIch = c;
      fIsi = s;
    };
    /**
     * only for debugging
     * @param pair
     */
    void RawCalc(FemtoPair* pair);

  public:
    /**
     * default c-tor
     */
    FemtoWeightGeneratorLednicky();  // call SetDefaultCalcPar
    /**
     * copy c-tor
     * @param aWeight
     */
    FemtoWeightGeneratorLednicky(const FemtoWeightGeneratorLednicky& aWeight);  // call SetDefaultCalcPar
    // --- Destructor : nothing to explicitly delete
    /**
     * = operator
     * @param aWeight
     * @return
     */
    FemtoWeightGeneratorLednicky& operator=(const FemtoWeightGeneratorLednicky& aWeight);
    /**
     * default d-tor
     */
    virtual ~FemtoWeightGeneratorLednicky();

    virtual Double_t GenerateWeight(FemtoPair* pair);

    virtual void SetPairType(Femto::EPairType aPairType);
    virtual void SetPairTypeFromPair(FemtoPair* pair);
    virtual Femto::EPairType GetPairType() const;

    virtual Double_t GetKStar() const;
    virtual Double_t GetKStarOut() const;
    virtual Double_t GetKStarSide() const;
    virtual Double_t GetKStarLong() const;
    virtual Double_t GetRStar() const;
    virtual Double_t GetRStarOut() const;
    virtual Double_t GetRStarSide() const;
    virtual Double_t GetRStarLong() const;
    virtual FemtoWeightGeneratorLednicky* MakeCopy() const { return new FemtoWeightGeneratorLednicky(*this); };

    virtual FemtoWeightGenerator* Clone(const char* newname = "") const;

    // --- Setting
    Bool_t Init(Int_t taks_id, FemtoPair* pair);
    virtual Bool_t IsPairSupported(Femto::EPairType type) const;
    // >>> Calculation mode
    /**
     *  Default is CoulOn, QuantumOn, StrongOn, 3BodyOff, Square, T0ApproxOff
     */
    void SetDefaultCalcPar();  // Default is CoulOn, QuantumOn, StrongOn,
                               // 3BodyOff, Square, T0ApproxOff
    /**
     * enable coulomb effect
     */
    void SetCoulOn();
    /**
     * disble coulom effect
     */
    void SetCoulOff();
    /**
     * enable quantum stattisic effect
     */
    void SetQuantumOn();
    /**
     * disable quantum statistic effect
     */
    void SetQuantumOff();
    /**
     * enable strong interaction effect
     */
    void SetStrongOn();
    /**
     * disable strong interaction effect
     */
    void SetStrongOff();
    /**
     * enable 3-body effect
     */
    void Set3BodyOn();
    /**
     * disable 3-body efffect
     */
    void Set3BodyOff();
    /**
     * use spherical wave approximations
     */
    void SetSphere();  // use Spherical wave approximation
    /**
     * use use Square potential (only for p-p and pi+Pi-) otherwise, use spherical
     * wave approx
     */
    void SetSquare();
    /**
     * only with  Spherical wave Approximation - this is default mode
     */
    void SetT0ApproxOff();
    /**
     * disable T0 approximateion
     */
    void SetT0ApproxOn();
    /**
     * test Lambda parameters
     */
    void PrintLambdas() { ; }
    /**
     * set nucleus charge for 3-body calculation
     * @param aNuclCharge nucleus charge
     */
    void SetNuclCharge(const double aNuclCharge);  // for 3-body calculation
    /**
     * set nuclesu mass for 3-body calculation
     * @param aNuclMass nucleus mass
     */
    void SetNuclMass(const double aNuclMass);
    /**
     *
     * @param nNs
     */
    void SetNS(int nNs) { fNS = nNs; };

    void FsiSetKpKmModelType(const int aModelType, const int aPhi_OffOn);
    virtual void Print(Option_t* option = "") const;
    virtual Package* Report() const;
    ClassDef(FemtoWeightGeneratorLednicky, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOWEIGHTGENERATORLEDNICKY_H_ */
