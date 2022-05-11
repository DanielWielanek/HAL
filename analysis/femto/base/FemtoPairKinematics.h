/*
 * FemtoPair.h
 *
 *  Created on: 30-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOPAIRKINEMATICS_H_
#define HALFEMTOPAIRKINEMATICS_H_

#include "FemtoConst.h"
#include "FemtoPair.h"

#include <TMath.h>
#include <TString.h>


/**
 * calculate pair kinematics in PRF frame
 */
namespace Hal {
  class FemtoPairPRF : public FemtoPair {
  public:
    FemtoPairPRF(Bool_t use_mc = kFALSE) : FemtoPair(use_mc) {};
    void Compute();
    void Compute_Rotated();
    void Compute_Hemisphere();
    Femto::EKinematics GetFrame() const { return Femto::EKinematics::kPRF; };
    TString GetTags() const { return "prf"; };
    virtual FemtoPair* MakeCopy() const { return new FemtoPairPRF(*this); };
    virtual ~FemtoPairPRF() {};
    ClassDef(FemtoPairPRF, 1)
  };

  /**
   * abstract class for all kinematics that return values in r, phi, eta
   */
  class FemtoPair_SH : public FemtoPair {
  protected:
    /**
     * convert fX, fY, fZ for cartesian to spherical values
     */
    void ConvertToSh();

  public:
    FemtoPair_SH(Bool_t use_mc = kFALSE) : FemtoPair(use_mc) {};
    // virtual FemtoPair* MakeCopy() const { return new FemtoPair_SH(*this); };
    virtual ~FemtoPair_SH() {};
    ClassDef(FemtoPair_SH, 1)
  };

  /**
   * calculate pair kinematics in LCMS frame
   */
  class FemtoPairLCMS : public FemtoPair {
  public:
    FemtoPairLCMS(Bool_t use_mc = kFALSE) : FemtoPair(use_mc) {}
    void Compute();
    void Compute_Rotated();
    void Compute_Hemisphere();
    Femto::EKinematics GetFrame() const { return Femto::EKinematics::kLCMS; };
    TString GetTags() const { return "lcms"; };
    virtual FemtoPair* MakeCopy() const { return new FemtoPairLCMS(*this); };
    virtual ~FemtoPairLCMS() {};
    ClassDef(FemtoPairLCMS, 1)
  };

  /**
   * calculate pair kinematics in PRF frame but return values in SH frame (r, phi,
   * eta)
   */
  class FemtoPairPRF_SH : public FemtoPair_SH {
  public:
    FemtoPairPRF_SH(Bool_t use_mc = kFALSE) : FemtoPair_SH(use_mc) {}
    void Compute();
    void Compute_Rotated();
    void Compute_Hemisphere();
    Femto::EKinematics GetFrame() const { return Femto::EKinematics::kSH_PRF; };
    TString GetTags() const { return "prf+sh"; };
    virtual FemtoPair* MakeCopy() const { return new FemtoPairPRF_SH(*this); };
    virtual ~FemtoPairPRF_SH() {};
    ClassDef(FemtoPairPRF_SH, 1)
  };

  /**
   * calculate pair kinematics in LCMS frame but return value in SH frame (r, phi,
   * eta)
   */
  class FemtoPairLCMS_SH : public FemtoPair_SH {
  public:
    FemtoPairLCMS_SH(Bool_t use_mc = kFALSE) : FemtoPair_SH(use_mc) {}
    void Compute();
    void Compute_Rotated();
    void Compute_Hemisphere();
    Femto::EKinematics GetFrame() const { return Femto::EKinematics::kSH_LCMS; };
    TString GetTags() const { return "lcms+prf"; };
    virtual FemtoPair* MakeCopy() const { return new FemtoPairLCMS_SH(*this); };
    virtual ~FemtoPairLCMS_SH() {};
    ClassDef(FemtoPairLCMS_SH, 1)
  };

  /**
   * calculate pair kinematics in delta-eta delta-phi frame
   */
  class FemtoPairDPhiDEta : public FemtoPair {
    Double_t fPhiMin, fPhiMax;
    Double_t fPI2;

  public:
    /**
     * default constructor
     * @param phi_min mimal value of phi
     */
    FemtoPairDPhiDEta(Bool_t use_mc = kFALSE, Double_t phi_min = -TMath::Pi() * 0.5);
    void Compute();
    void Compute_Rotated();
    void Compute_Hemisphere();
    Femto::EKinematics GetFrame() const { return Femto::EKinematics::kPHIETA; };
    TString GetTags() const { return "phieta"; };
    virtual FemtoPair* MakeCopy() const { return new FemtoPairDPhiDEta(*this); };
    ~FemtoPairDPhiDEta() {};
    ClassDef(FemtoPairDPhiDEta, 1)
  };

  /**
   * class for making kinematics with "lednicky prf" where boost along all
   * direction is made at the same time
   */
  class FemtoPairPRFL : public FemtoPair {
  public:
    FemtoPairPRFL(Bool_t use_mc = kFALSE) : FemtoPair(use_mc) {};
    void Compute();
    void Compute_Rotated();
    void Compute_Hemisphere();
    Femto::EKinematics GetFrame() const { return Femto::EKinematics::kPRFL; };
    TString GetTags() const { return "prfl"; };
    virtual FemtoPair* MakeCopy() const { return new FemtoPairPRFL(*this); };
    virtual ~FemtoPairPRFL() {};
    ClassDef(FemtoPairPRFL, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOKINEMATICS_H_ */
