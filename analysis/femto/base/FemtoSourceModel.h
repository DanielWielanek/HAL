/*
 * FemtoSourceModel.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOSOURCEMODEL_H_
#define HALFEMTOSOURCEMODEL_H_

#include <TRandom.h>
#include <TString.h>
#include <TVector3.h>

namespace Hal {
  class FemtoPair;
  class Package;
  class FemtoSourceDensity;

  /**
   * basic class for all source models
   */
  class FemtoSourceModel : public TObject {
  private:
    /**
     * numer of parameters
     */
    const Int_t fParametersNo;
    Double_t* fParams;  //[fParametersNo]
    /**
     * array of parameter names
     */
    TString* fParameterNames;  //[fParametersNo]
  protected:
    /**
     * used for generating random coordinates
     */
    TRandom* fRandom;
    /**
     * stores information about source model density
     */
    FemtoSourceDensity* fDensity;
    /**
     * current out-radius
     */
    Double_t fRout;
    /**
     * current side radius
     */
    Double_t fRside;
    /**
     * current long radius
     */
    Double_t fRlong;
    /**
     * name of this model
     */
    TString fModelName;
    void SetParName(TString name, Int_t no) { fParameterNames[no] = name; };
    // FemtoSourceModel();
  public:
    enum class ENumProperty { kNonAnalytical, kAnalytical1d, kAnalytical3d, kFullyAnalytical };
    /**
     * default ctor
     */
    FemtoSourceModel(Int_t nparams);
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModel(const FemtoSourceModel& model);
    /**
     *
     * @return  generated out radius
     */
    inline Double_t GetROut() const { return fRout; };
    /**
     *
     * @return generated  out radius
     */
    inline Double_t GetRSide() const { return fRside; };
    /**
     *
     * @return generated long out radius
     */
    inline Double_t GetRLong() const { return fRlong; };
    /**
     * set generator seed
     * @param seed
     */
    void SetSeed(UInt_t seed) { fRandom->SetSeed(seed); };
    /**
     * se
     * @param par_val value of parameter
     * @param par_no parameter number
     */
    inline void SetParameter(Double_t par_val, Int_t par_no) { fParams[par_no] = par_val; };
    /**
     * set value of parameter
     * @param par
     * @param name
     */
    void SetParameterByName(Double_t par, TString name);
    /**
     * calculate coordinates
     */
    virtual void GenerateCoordinates(FemtoPair* Pair) = 0;
    /**
     * set 1-dim radius, if source is 3D that each radii componet will be scaled
     * by 1/sqrt(3) (e.g. r_out = radii/sqrt(3)
     * @param radii
     */
    virtual void SetRadius(Double_t /*radii*/) {};
    /**
     *
     * @return number of parameters
     */
    Int_t GetNpar() const { return fParametersNo; };
    /**
     *
     * @param n
     * @return name of n-th parameter
     */
    TString GetParamName(Int_t n) const;
    /**
     * return value of parameter with numer N
     * @param n
     * @return
     */
    Double_t GetParameter(Int_t n) const { return fParams[n]; };
    virtual void Print(Option_t* option = "") const;
    /**
     *
     * @return copy of this
     */
    virtual FemtoSourceModel* MakeCopy() const = 0;
    /**
     *
     * @return class that calculates the source emission function density
     */
    FemtoSourceDensity* GetDensityProb() const { return fDensity; }
    /**
     *
     * @return type of model, this define if probability density might be calculated fast
     */
    ENumProperty GetModelNumProp() const;
    /**
     * create package with report
     * @return package with report
     */
    virtual Package* Report() const;
    virtual ~FemtoSourceModel();
    ClassDef(FemtoSourceModel, 1)
  };
  /**
   * class for generating of coordinates for 1Dim sources
   */
  class FemtoSourceModel1D : public FemtoSourceModel {
  protected:
    FemtoSourceModel1D(Int_t par);

  public:
    FemtoSourceModel1D();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModel1D(const FemtoSourceModel1D& model);
    /**
     * set generated radiius
     * @param radii
     */
    void SetRadius(Double_t radii) { SetParameter(radii, 0); };
    virtual FemtoSourceModel* MakeCopy() const = 0;
    virtual ~FemtoSourceModel1D();
    ClassDef(FemtoSourceModel1D, 1)
  };
  /**
   * abstract class for all 3-dim sources
   */
  class FemtoSourceModel3D : public FemtoSourceModel {
  protected:
    FemtoSourceModel3D(Int_t no);

  public:
    FemtoSourceModel3D();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModel3D(const FemtoSourceModel3D& model);
    /**
     * set out radius
     * @param out radius
     */
    void SetOutRadius(Double_t out) { SetParameter(out, 0); };
    /**
     * set side radius
     * @param side radius
     */
    void SetSideRadius(Double_t side) { SetParameter(side, 1); };
    /**
     * set long radius
     * @param longr radius
     */
    void SetLongRadius(Double_t longr) { SetParameter(longr, 2); };
    /**
     * set out long and side radii where R-out/side/long = radii/sqrt(3)
     * @param radii
     */
    virtual void SetRadius(Double_t radii);
    virtual FemtoSourceModel* MakeCopy() const = 0;
    virtual ~FemtoSourceModel3D();
    ClassDef(FemtoSourceModel3D, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOSOURCEMODEL_H_ */
