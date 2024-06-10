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

#include "CorrFitFunc3D.h"

#include "CorrFitMask.h"
class TH3;
class TH1D;
/**
 * abstract class for fitting 3-dim correlation function
 */
namespace Hal {
  class CorrFitMask3D;
  class CorrFitMath3DCF;
  class CorrFitSHCF;
  class CorrFit3DCF : public CorrFitFunc3D {

  public:
  private:
    enum class EDrawMode { kNormal, kDiagonal1, kDiagonal2 };
    friend class CorrFitMath3DCF;
    friend class CorrFitSHCF;
    Array_1<Double_t>* fXbins = {nullptr};
    Array_1<Double_t>* fYbins = {nullptr};
    Array_1<Double_t>* fZbins = {nullptr};
    Double_t fXBinf           = {0};
    Double_t fYBinf           = {0};
    Double_t fZBinf           = {0};
    Double_t fXAxisf          = {0};
    Double_t fYAxisf          = {0};
    Double_t fZAxisf          = {0};
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
    void Calculatef(Double_t width);

  protected:
    Double_t fOldNorm = {1};
    /**
     * processed currednly binX, binY and binZ;
     */
    mutable Int_t fBinX = {0}, fBinY = {0}, fBinZ = {0};
    CorrFitMask3D* GetMask() const { return (CorrFitMask3D*) fMask; };
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
    void PrepareRaw();
    double GetChiTFD(const double* par) const;
    double GetChiTF(const double* par) const;
    double GetLogTFD(const double* par) const;
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
    CorrFit3DCF(e3DMode mode, Int_t parameters = 3);

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

    void SetFittingMask(const CorrFitMask& map);
    /**
     * return function at given point
     * @param x
     * @param y
     * @param z
     * @return
     */
    Double_t Eval(Double_t x, Double_t y, Double_t z);
    virtual ~CorrFit3DCF();
    ClassDef(CorrFit3DCF, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT3DCF_H_ */
