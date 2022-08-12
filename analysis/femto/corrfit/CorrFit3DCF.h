/*
 * CorrFit3DCF.h
 *
 *  Created on: 08-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFIT3DCF_H_
#define HALCORRFIT3DCF_H_

#include "CorrFitFunc.h"


class TH3;
class TH1D;
/**
 * abstract class for fitting 3-dim correlation function
 */
namespace Hal {
  class CorrFitMath3DCF;
  class CorrFitSHCF;
  class CorrFit3DCF : public CorrFitFunc {

  public:
    enum class EFitExtraMask {
      kStandard,
      kSlice,
      kDiagonalSlice,
      kUltraDiagonalSlice,
      kDiagonalSliceIgnored,
      kUltraDiagonalSliceIgnored
    };

  private:
    enum class EDrawMode { kNormal, kDiagonal1, kDiagonal2 };
    friend class CorrFitMath3DCF;
    friend class CorrFitSHCF;
    static const Int_t fgRout;
    static const Int_t fgRside;
    static const Int_t fgRlong;
    static const Int_t fgLambda;
    static const Int_t fgNorm;
    EFitExtraMask fFitMaskFlag;
    Array_1<Double_t>* fXbins;
    Array_1<Double_t>* fYbins;
    Array_1<Double_t>* fZbins;
    Double_t fXBinf;
    Double_t fYBinf;
    Double_t fZBinf;
    Double_t fXAxisf;
    Double_t fYAxisf;
    Double_t fZAxisf;
    Double_t GetFunX(Double_t* x, Double_t* params) const;
    Double_t GetFunY(Double_t* x, Double_t* params) const;
    Double_t GetFunZ(Double_t* x, Double_t* params) const;
    Double_t GetFunXYpp(Double_t* x, Double_t* params) const;
    Double_t GetFunXYpm(Double_t* x, Double_t* params) const;
    Double_t GetFunXZpp(Double_t* x, Double_t* params) const;
    Double_t GetFunXZpm(Double_t* x, Double_t* params) const;
    Double_t GetFunYZpp(Double_t* x, Double_t* params) const;
    Double_t GetFunYZpm(Double_t* x, Double_t* params) const;
    Double_t GetFunXYZppp(Double_t* x, Double_t* params) const;
    Double_t GetFunXYZpmp(Double_t* x, Double_t* params) const;
    Double_t GetFunXYZppm(Double_t* x, Double_t* params) const;
    Double_t GetFunXYZpmm(Double_t* x, Double_t* params) const;
    void SetParametersToTF1(TF1* f) const;
    void SwapMap(TH3* h);
    void Calculatef(Double_t width);

  protected:
    TVirtualPad* fTempPad = {nullptr};
    Double_t fOldNorm     = {1};
    /**
     * processed currednly binX, binY and binZ;
     */
    mutable Int_t fBinX, fBinY, fBinZ;
    virtual void GetTF1s(Bool_t makeNew, EDrawMode drawMode);
    virtual void GetTH1s(EDrawMode drawMode);
    /**
     * called for each calculation of chi2 or loglikehood minimalization - used
     * for recalculation CF is parameters are changed
     */
    virtual void RecalculateFunction() const {};
    virtual void RecalculateSmoothFunction() const;
    /**
     * recompute errors in num by using information from den histogram
     * @param num
     * @param den
     */
    void SetErrors(TH1* num, const TH1* den) const;
    virtual void EstimateActiveBins();
    void DrawPureFunction(TString option);
    void DrawFunctionWithCF(TString option);
    void DrawOther(TString option);
    void DrawDiagonalOther(TString option);
    void DrawDiagonalWithCF(TString option);
    void CalculateRangeBins(TH3* h);
    void CalculateDiagonalBins(TH3* h);
    void CalculateUltradiagonalBins(TH3* h);
    void CalculateSliceBins(TH3* h);
    void CalculateThresholdBins();
    double GetChiTFD(const double* par) const;
    double GetChiTF(const double* par) const;
    double GetLogTFD(const double* par) const;
    void ReadParametersName();
    /**
     * return evaluated value of denominator at point x,y,z
     * @param x out momentum difference
     * @param y side momentum difference
     * @param z long momentum difference
     * @return value of denomitor at (out,side,long)
     */
    Double_t EvalDenominator(Double_t x, Double_t y, Double_t z) const;
    /**
     * calulate value of cf
     * @param x bin center
     * @param params parameters
     * @return return value at bin center (CalculateCF) if option "bin" was not
     * used otherwise return value of CF calculated with taken into account
     * averaged value in the middle of CF bin
     */
    virtual Double_t EvalCF(const Double_t* x, const Double_t* params) const;
    /**
     * return value of CF at given point
     * @param x values x[0] - out, x[1] -side, x[2] -long
     * @param params
     * @return value of CF at (x[0],x[1],x[2])
     */
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const = 0;
    /**
     *
     * @param x q-out
     * @param y q-side
     * @param z q-long
     * @return numerical error for given bin
     */
    virtual Double_t GetNumericalError(Int_t /*x*/, Int_t /*y*/, Int_t /*z*/) const { return 0; };
    virtual void Paint(Bool_t repaint, Bool_t refresh);

  public:
    /**
     * default constructor
     * @param parameters number of parameters
     */
    CorrFit3DCF(Int_t parameters = 3);
    /**
     * set range fo fitting
     * @param x_min min. value in out direction
     * @param x_max max. value in out direction
     * @param y_min min. value in side direction
     * @param y_max max. value in side direction
     * @param z_min min. value in long direction
     * @param z_max max. value in long direction
     */
    void SetFuncRange(Double_t x_min, Double_t x_max, Double_t y_min, Double_t y_max, Double_t z_min, Double_t z_max);
    /**
     * set radii limits (out,side, and long)
     * @param min
     * @param max
     */
    void SetRadiusLimits(Double_t min, Double_t max);
    /**
     * set norm limits
     * @param min
     * @param max
     */
    void SetNormLimits(Double_t min, Double_t max) { SetParLimits(Norm(), min, max); }
    /**
     * set lambda limits
     * @param min
     * @param max
     */
    void SetLambdaLimits(Double_t min, Double_t max) { SetParLimits(Lambda(), min, max); }
    /**
     * set limits of r-out
     * @param min
     * @param max
     */
    void SetRoutLimits(Double_t min, Double_t max) { SetParLimits(Rout(), min, max); }
    /**
     * set limits of r-side
     * @param min
     * @param max
     */
    void SetRsideLimits(Double_t min, Double_t max) { SetParLimits(Rside(), min, max); }
    /**
     * set limits of r-long
     * @param min
     * @param max
     */
    void SetRlongLimits(Double_t min, Double_t max) { SetParLimits(Rlong(), min, max); }
    /**
     *
     * @return Rout
     */
    Double_t GetRout() const { return GetParameter(Rout()); };
    /**
     *
     * @return Rout error
     */
    Double_t GetRoutError() const { return GetParError(Rout()); };
    /**
     *
     * @return Rside
     */
    Double_t GetRside() const { return GetParameter(Rside()); };
    /**
     *
     * @return Rside error
     */
    Double_t GetRsideError() const { return GetParError(Rside()); };
    /**
     *
     * @return Rlong
     */
    Double_t GetRlong() const { return GetParameter(Rlong()); };
    /**
     *
     * @return Rlong error
     */
    Double_t GetRlongError() const { return GetParError(Rlong()); };
    /**
     *
     * @return labmda value (can be negative for fermions)
     */
    Double_t GetLambda() const { return GetParameter(Lambda()); };
    /**
     *
     * @return lambda error
     */
    Double_t GetLambdaError() const { return GetParError(Lambda()); };
    /**
     *
     * @return normalization
     */
    Double_t GetNorm() const { return GetParameter(Norm()); };
    /**
     *
     * @return normalization error
     */
    Double_t GetNormError() const { return GetParError(Norm()); };
    /**
     * return function at given point
     * @param x
     * @param y
     * @param z
     * @return
     */
    Double_t Eval(Double_t x, Double_t y, Double_t z);
    /**
     *
     * @return param number that correspond to Rout
     */
    inline static Int_t Rout() { return fgRout; };
    /**
     *
     * @return param number that correspond to Rside
     */
    inline static Int_t Rside() { return fgRside; };
    /**
     *
     * @return param number that correspond to Rlong
     */
    inline static Int_t Rlong() { return fgRlong; };
    /**
     *
     * @return param number that correspond to lambda
     */
    inline static Int_t Lambda() { return fgLambda; };
    /**
     *
     * @return param number that correspond to norm
     */
    inline static Int_t Norm() { return fgNorm; };
    /**
     * make diagonal map
     * @param r
     */
    void MakeDiagonal(EFitExtraMask flag, TH3* mask);
    /**
     * used to apply additional fit mas
     * @param r fit mask  - kStandard - nothing to chage, kSlice - use only "slice bins" , kCrossSlice - use only diagonals +
     * slice
     */
    void SetFitExtraMask(EFitExtraMask r) { fFitMaskFlag = r; };
    virtual ~CorrFit3DCF();
    ClassDef(CorrFit3DCF, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT3DCF_H_ */
