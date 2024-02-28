/*
 * FemtoConst.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOCONST_H_
#define HALFEMTOCONST_H_

#include "Array.h"
#include "TString.h"
/**
 * femtoscopic kinematics mode
 */
class TVector3;
namespace Hal {
  namespace Femto {

    inline Double_t FmToGeV() { return 5.06842372; }
    inline Double_t FmToGeV(Double_t fm) { return fm * FmToGeV(); }

    enum class EKinematics {
      kPRF,   //!< kPRF
      kLCMS,  //!< kLCMS
      kSH_LCMS,
      kSH_PRF,
      kPHIETA,
      kPRFL
    };
    enum class ECFType { kOneDim, kThreeDim, kSpherical, kPhiEta, kUnkown };
    enum class EPairType {
      kUnknown           = 100,
      kPionPlusPionPlus  = 101,
      kPionPlusPionMinus = 102,
      kKaonPlusKaonPlus  = 103,
      kKaonPlusKaonMinus = 4,

      kProtonProton      = 105,
      kProtonAntiproton  = 6,
      kPionPlusKaonPlus  = 107,
      kPionPlusKaonMinus = 108,

      kPionPlusProton     = 109,
      kPionPlusAntiproton = 110,
      kKaonPlusProton     = 111,
      kKaonPlusAntiproton = 112,

      kProtonLambda        = 113,
      kLambdaLambda        = 114,
      kKaonZeroKaonZero    = 15,
      kKaonZeroKaonZeroBar = 16,

      kNeutronNeutron   = 117,
      kNeutronProton    = 118,
      kPionZeroPionZero = 19,
      kNeutronLambda    = 20,

      // TPI weights only
      kProtonSigmaPlus     = 201,
      kProtonAntiSigmaPlus = 202,
      kProtonAntiLambda    = 203,
      kSigmaPlusSigmaPlus  = 204,

      kSigmaPlusAntiSigmaPlus = 205,
      kProtonXiZero           = 206,
      kNeutronXiMinus         = 207,
      kProtonXIMInus          = 208,

      kNeutronXiZero      = 209,
      kProtonSigmaZero    = 210,
      kSigmaZeroSigmaZero = 211,
      kLambdaSigmaZero    = 212,

      kLambdaAntiLambda = 213

    };
  }  // namespace Femto

  class FemtoPair;
  class DividedHisto1D;
  class XMLNode;
  class FemtoWeightGenerator;
  namespace Femto {
    /**
     * return true if pair contains identical particles
     * @param pt
     * @return
     */
    Bool_t IsPairIdentical(EPairType pt);
    /**
     * convert pair type to PDG
     * @param pt
     * @return
     */
    std::pair<Int_t, Int_t> PairTypeToPid(EPairType pt);
    /**
     * get pair type from PDG codes
     * @param pid1
     * @param pid2
     * @return
     */
    EPairType PidToPairType(Int_t pid1, Int_t pid2);
    /**
     * change pair type to string
     * @param type
     * @return
     */
    TString PairTypeToString(EPairType type);
    /**
     * convert label to kinematics enum
     * @param label
     * @return
     */
    EKinematics LabelToKinematics(TString label);
    /**
     * convert C++ name of Femto::EKinematics enum to kinematics
     * @param label
     * @return
     */
    EKinematics CodeLabelToKinematics(TString label);
    /**
     * return hbt pair that calculates proper kinematics variables, replaces
     * FemtoPair::Factory
     * @param kin
     * @param use_fake - use "hidde" momenta for calculations instead of "real"
     * @return
     */
    FemtoPair* MakePair(EKinematics kin, Bool_t use_fake);
    /**
     * create CF from xml node
     * @param nod
     * @return
     */
    DividedHisto1D* GetHistoFromXML(XMLNode* nod);
    /**
     * create weight algo from xml node
     * @param nod
     * @return
     */
    FemtoWeightGenerator* GetWeightGeneratorFromXLM(XMLNode* nod);
    /**
     * convert kinematic type to label used to description of DividedHisto
     * @param kin
     * @return
     */
    TString KinematicsToLabel(EKinematics kin);
    /**
     *
     * @param kin kinematic type
     * @param ax axis x-0, y-1, z -2 , t -3
     * @param ndim number of dimensions in CF
     * @return return name of axis
     */
    TString KinematicsToAxisLabel(EKinematics kin, Int_t ax, Int_t ndim);
    /**
     * fill femto pair with random variables
     * @param p
     * @param pid1 pid of first particle
     * @param pid2 pid of second particle
     * @param sigmaq -std of momentum coordinate
     * @param sigmar - std of freezout coordinate
     */
    void FillRandomPair(FemtoPair& p, Int_t pid1, Int_t pid2, Double_t sigmaq = 0.1, Double_t sigmar = 1);
    /**
     * fill pair with random kinematics
     * @param p pair to fill
     * @param sum total momentum of pair
     * @param diff difference in pair (k* out/side/long or q out/side/long)
     * @param kin rame - supported are LCMS and PRF
     */
    void FillRandomKinematics(FemtoPair& p, const TVector3& sum, const TVector3& diff, EKinematics kin);
    /**
     * return CF type if object can be convereted in CF
     * @param obj
     * @return cf type kUnkown if failed to export
     */
    ECFType GetCFType(TObject* obj);
  }  // namespace Femto
}  // namespace Hal

#endif /* HALFEMTOCONST_H_ */
