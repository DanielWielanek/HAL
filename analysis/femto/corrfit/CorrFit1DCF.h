/*
 * CorrFit1DCF.h
 *
 *  Created on: 02-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFIT1DCF_H_
#define HALCORRFIT1DCF_H_

/*abstract class that represent theoretical correlation function */


#include "CorrFitFunc.h"

class TLegend;
namespace Hal {
  class CorrFitMath1DCF;
  /**
   * abstract class for fitting 1-dim correlation functions
   */
  class CorrFit1DCF : public CorrFitFunc {
    friend class Femto1DCF;
    friend class CorrFitMath1DCF;

  private:
    Double_t EvalDenominator(Double_t x) const;
    static const Int_t fgRinv;
    static const Int_t fgLambda;
    static const Int_t fgNorm;

  protected:
    /**
     * pointer of acutally calculated bin during chi-square computation
     */
    mutable Int_t fBinX;
    /**
     * calculate numerical errors
     * @param h numerator/denominator of CF - used to get bin center
     */
    virtual void CalculateNumErrors(TH1* /*h*/) {};
    virtual Double_t GetNumericalError(Int_t /*binX*/) const { return 0; };
    void EstimateActiveBins();
    void ReadParametersName();
    void RecalculateSmoothFunction() const;
    void SetErrors(TH1* num, const TH1* den) const;
    double GetChiTF(const double* par) const;
    double GetChiTFD(const double* par) const;
    double GetLogTFD(const double* par) const;
    /**
     *
     * @return TF1 function isued for drawing
     */
    virtual TF1* GetFunctionForDrawing() const;
    /**
     *@param normalize normalize this func
     * @return TH1D used for drawing function
     *
     */
    virtual TH1* GetTHForDrawing(Bool_t normalize) const;
    /**
     *
     * @return TLegend with fit parameters for drawing
     */
    virtual TLegend* GetLegendForDrawing() const;
    /**
     * functions used for drawing
     * @param x value of momentum difference
     * @param params parameters array
     * @return
     */
    Double_t GetFunDrawable(Double_t* x, Double_t* params) const;
    /**
     * fuction that return value of correlated fuction at given bin. If
     * calculation is performed with option "taking into account bin widht" the
     * this return  CalculateCF()
     * @param x array of values (only x[0] is used)
     * @param params array of parameters
     * @return value of CF at x[0]
     */
    Double_t EvalCF(const Double_t* x, const Double_t* params) const;
    /**
     * fuction that return value of correlated fuction at given point
     * @param x array of values (only x[0] is used)
     * @param params array of parameters
     * @return value of CF at x[0]
     */
    virtual Double_t CalculateCF(const Double_t* /*x*/, const Double_t* /*params*/) const { return 1; };
    virtual void Fit(TObject* histo);

  public:
    /** default constructor
     *
     * @param parameters number of parameters
     */
    CorrFit1DCF(Int_t parameters = 3);
    /**
     *
     * @param q momentum difference (k* or qinv)
     * @return vallue of CF
     */
    virtual Double_t Eval(Double_t q) const;
    virtual void Draw(Option_t* draw_option = "full+norm");
    /**
     * set limits of fitted radii
     * @param min minimal value
     * @param max maximal value
     */
    void SetRLimits(Double_t min, Double_t max) { SetParLimits(Radius(), min, max); };
    /**
     * set limits of fitted lambda
     * @param min minimal value
     * @param max maximal value
     */
    void SetLambdaLimits(Double_t min, Double_t max) { SetParLimits(Lambda(), min, max); }
    /**
     * set limits of fitted norm
     * @param min minimal value
     * @param max maximal value
     */
    void SetNormLimits(Double_t min, Double_t max) { SetParLimits(Norm(), min, max); };
    /**
     *
     * @return return radius value
     */
    Double_t GetRadius() const { return GetParameter(Radius()); };
    /**
     *
     * @return radius error
     */
    Double_t GetRadiusError() const { return GetParError(Radius()); };
    /**
     *
     * @return lambda value (for fermions can be negative)
     */
    Double_t GetLambda() const { return GetParameter(Lambda()); };
    /**
     *
     * @return lambda error
     */
    Double_t GetLambdaError() const { return GetParError(Lambda()); };
    /**
     *
     * @return normalization parameter
     */
    Double_t GetNorm() const { return GetParameter(Norm()); };
    /**
     *
     * @return normalization error
     */
    Double_t GetNormError() const { return GetParError(Norm()); };
    /**
     *
     * @return param number that correspond to radii
     */
    inline static Int_t Radius() { return fgRinv; };
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
    virtual ~CorrFit1DCF();
    ClassDef(CorrFit1DCF, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT1DCF_H_ */
