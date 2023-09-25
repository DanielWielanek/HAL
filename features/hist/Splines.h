/*
 * HalSplines.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSPLINES_H_
#define HALSPLINES_H_

#include "Array.h"

#include <TString.h>


class TAxis;
class TH1;
class TH1D;
class TH2;
class TH2D;
class TH3;
class TH3D;

/**
 * class that intepolate 1D histogram by parabolic interpolation, interpolation
 * parameters are different fo each bin of this "pseudohistogram". Therefore on
 * bin edges interpolation is not continuous - this can be fixed by call Refit
 * method
 */

namespace Hal {
  class Spline1D : public TObject {
    TAxis* fXaxis;
    Int_t fNbins;
    Double_t* fA;   //[fNbins]
    Double_t* fB;   //[fNbins]
    Double_t* fC;   //[fNbins]
    Double_t* fAe;  //[fNbins]
    Double_t* fBe;  //[fNbins]
    void CalcParams(Double_t x1,
                    Double_t y1,
                    Double_t x2,
                    Double_t y2,
                    Double_t x3,
                    Double_t y3,
                    Double_t& A,
                    Double_t& B,
                    Double_t& C);
    void SetFirstBin(TH1* h, Double_t begval, Bool_t use_begval = kFALSE);
    void SetLastBin(TH1* h, Double_t enval, Bool_t use_endval = kFALSE);

  public:
    Spline1D(TH1* h = NULL, Double_t begval = 0, Double_t endval = 0);
    /**
     * use to overwrite this class without time-consuming redelcaration of
     * object, IMPORATH NOTE: h - will be modified!
     * @param h
     * @param begval
     * @param endval
     */
    void FastOverwrite(TH1D* h, Double_t begval = 0, Double_t endval = 0);
    /**
     * smear interpolation
     */
    void Refit();
    /**
     * return interpolated value at x
     * @param x
     * @return
     */
    Double_t Eval(Double_t x) const;
    /**
     * return interpolated error value at x
     * @param x
     * @return
     */
    Double_t GetError(Double_t x) const;
    /**
     * return value at bin center
     * @param bin
     * @return
     */
    Double_t EvalBin(Int_t bin) const;
    /**
     * return error at given bin
     * @param bin
     * @return
     */
    Double_t ErrorBin(Int_t bin) const;
    /**
     * like Eval(Double_t x) but return value and error simultaneously
     * @param x
     * @param f
     * @param error
     */
    void Eval(Double_t x, Double_t& f, Double_t& error) const;
    /**
     * try to reproduce histogram used to create this class
     * @param name
     * @return
     */
    virtual TH1D* GetTHD(TString name = "spline_thd") const;
    TAxis* GetXaxis() const { return fXaxis; };
    virtual ~Spline1D();
    ClassDef(Spline1D, 1)
  };

  /**
   * class that intepolate 2D histogram by parabolic interpolation, interpolation
   * parameters are different fo each bin of this "pseudohistogram". Therefore on
   * bin edges interpolation is not continuous - this can be fixed by call Refit
   * method
   */
  class Spline2D : public TObject {
    TAxis* fXaxis;
    TAxis* fYaxis;
    Int_t fNbinsX;
    Int_t fNbinsY;
    Array_3<Double_t> fA;
    Array_2<Double_t>* fAe;
    void CalcParams(Double_t x[3], Double_t y[3], Double_t z[3][3], Double_t params[9]);
    void Extrapolate(TH2* h, Option_t* extraopolation_opt) const;
    Double_t Extrapolate(TH2* h, Int_t ix1, Int_t ix2, Int_t x, Int_t iy1, Int_t iy2, Int_t y, Int_t opt) const;
    // TODO default constructor & copy constructor
  public:
    /**
     *
     * @param h
     * @param interpolation edge option of "edge" interpolation, by default no
     * interpolation is used and "edge values" corresponds to underflow/overflow
     * bins other options are "linear" to linear interpolation,  "const" -
     * assunming that upper bin is equal to first bin and underflow bin is the
     * same as last bin
     */
    Spline2D(TH2* h = NULL, Option_t* interpolation = "");
    /**
     * smear this histogram
     */
    void Refit();
    /**
     * return A(param)
     * @param x
     * @param y
     * @param param
     * @return
     */
    Double_t GetExtrapolationParam(Double_t x, Double_t y, Int_t param) const;
    /**
     * return interpolated value at x,y
     * @param x
     * @param y
     * @return
     */
    Double_t Eval(Double_t x, Double_t y) const;
    /**
     * return error of interpolated value at x, y
     * @param x
     * @param y
     * @return
     */
    Double_t GetError(Double_t x, Double_t y) const;
    /**
     * return value at given bin
     * @param bin_x
     * @param bin_y
     * @return
     */
    Double_t EvalBin(Int_t bin_x, Int_t bin_y) const;
    /**
     * return value error at given bin
     * @param bin_x
     * @param bin_y
     * @return
     */
    Double_t ErrorBin(Int_t bin_x, Int_t bin_y) const;
    /**
     * set const value at given bin
     * @param xbin
     * @param ybin
     * @param val
     * @param err
     */
    void SetPoint(Int_t xbin, Int_t ybin, Double_t val, Double_t err = 0);
    /**
     * print interpolation parameters at given x, y
     * @param x
     * @param y
     */
    void PrintParams(Double_t x, Double_t y);
    /**
     * like Double_t Eval(Double_t x,Double_t y) but return value and error
     * simultaneously
     * @param x
     * @param y
     * @param f
     * @param error
     */
    void Eval(Double_t x, Double_t y, Double_t& f, Double_t& error) const;
    /**
     *
     * @return
     */
    TAxis* GetXaxis() const { return fXaxis; };
    /**
     *
     * @return
     */
    TAxis* GetYaxis() const { return fYaxis; };
    /**
     * try to reproduce histogram used to create of this object
     * @param name
     * @return
     */
    virtual TH2D* GetTHD(TString name = "spline_thd") const;
    virtual ~Spline2D();
    ClassDef(Spline2D, 1)
  };
  /**
   * class that intepolate 3D histogram by trilinear intepolation, interpolation
   * base on neighbour bins, therefore value at the center of given bin might not
   * correspond to the value of intepolated histogram. If created with "mid" then
   * value at the bin center of intepolation will be the same as at the bin center
   * of interpolated histogram - this is done by "shifting" interpolation to the
   * value of histogram.
   */
  class Spline3D : public TObject {
    TAxis* fXaxis;
    TAxis* fYaxis;
    TAxis* fZaxis;
    Int_t fNbinsX;
    Int_t fNbinsY;
    Int_t fNbinsZ;
    Array_4<Double_t> fA;
    Array_3<Double_t> fAe;
    Double_t EvalBin(Int_t ix, Int_t iy, Int_t iz, Double_t x, Double_t y, Double_t z) const;
    void CalcParams(Double_t x[2], Double_t y[2], Double_t z[2], Double_t v[2][2][2], Double_t params[8]);

  public:
    /**
     *
     * @param h histogram to interpolate
     * @param interpolation if "nointerpolation" - don't do interpolation
     * "fix1". "yes"  - try to fix edges by "mean" method, "mid" - value at bin
     * center value must agree with value of intepolated histogram
     *
     */
    Spline3D(TH3* h = NULL, Option_t* interpolation = "");
    /**
     * recalculate this histogram without reallocation of resources
     * @param h
     * @param interpolation
     */
    void FastOverwrite(TH3* h, Option_t* interpolation = "");
    /**
     * calculate value at given point
     * @param x
     * @param y
     * @param z
     * @return
     */
    Double_t Eval(Double_t x, Double_t y, Double_t z) const;
    /**
     * get error at given point
     * @param x
     * @param y
     * @param z
     * @return
     */
    Double_t GetError(Double_t x, Double_t y, Double_t z) const;
    /**
     * calculate function value at the middle of histogram bin that was used to
     * create this class
     * @param x
     * @param y
     * @param z
     * @return
     */
    Double_t EvalBin(Int_t x, Int_t y, Int_t z) const;
    /**
     * get error ov value at bin (of histogram used to create this object)
     * @param x
     * @param y
     * @param z
     * @return
     */
    Double_t ErrorBin(Int_t x, Int_t y, Int_t z) const;
    /**
     * smear this function, still not works perfectly
     */
    void Refit();
    /**
     * print interpolation parametsers at give value
     * @param x
     * @param y
     * @param z
     */
    void PrintParams(Double_t x, Double_t y, Double_t z) const;
    /**
     * works like HalSpline3D: Eval(Double_t x,Double_t y, Double_t z)
     * but return both value and error at once
     * @param x
     * @param y
     * @param z
     * @param f
     * @param error
     */
    void Eval(Double_t x, Double_t y, Double_t z, Double_t& f, Double_t& error) const;
    /**
     *
     * @return axis of this
     */
    TAxis* GetXaxis() const { return fXaxis; };
    /**
     *
     * @return axis of this
     */
    TAxis* GetYaxis() const { return fYaxis; };
    /**
     *
     * @return axis of this
     */
    TAxis* GetZaxis() const { return fZaxis; };
    /**
     * try to get histogram used to create this object
     * @param name
     * @return
     */
    virtual TH3D* GetTHD(TString name = "spline_thd") const;
    virtual ~Spline3D();
    ClassDef(Spline3D, 1)
  };
}  // namespace Hal
#endif /* HALSPLINES_H_ */
