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
  class CorrFitMask1D;
  class CorrFit1DCFPainter;
  /**
   * abstract class for fitting 1-dim correlation functions
   */
  class CorrFit1DCF : public CorrFitFunc {
    friend class Femto1DCF;
    friend class CorrFitMath1DCF;
    friend class CorrFit1DCFPainter;

  private:
    Double_t EvalDenominator(Double_t x) const;

  protected:
    Int_t fRinvParIndex   = {1};
    Int_t fLambdaParIndex = {2};
    /**
     * pointer of acutally calculated bin during chi-square computation
     */
    mutable Int_t fBinX;

    virtual void MakePainter(TString opt);

    CorrFitMask1D* GetMask() const { return (CorrFitMask1D*) fMask; };
    /**
     * calculate numerical errors
     * @param h numerator/denominator of CF - used to get bin center
     */
    virtual void CalculateNumErrors(TH1* /*h*/) {};
    virtual Double_t GetNumericalError(Int_t /*binX*/) const { return 0; };
    void PrepareRaw();
    void EstimateActiveBins();
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
     * functions used for drawing
     * @param x value of momentum difference
     * @param params parameters array
     * @return
     */
    Double_t GetFunDrawable(Double_t* x, Double_t* params) const;
    /**
     * function that return value of correlated function at given bin. If
     * calculation is performed with option "taking into account bin widht" the
     * this return  CalculateCF()
     * @param x array of values (only x[0] is used)
     * @param params array of parameters
     * @return value of CF at x[0]
     */
    Double_t EvalCF(const Double_t* x, const Double_t* params) const;
    /**
     * returns EvalCF but normalized
     * @param x
     * @param params
     * @return
     */
    Double_t EvalCFNormalized(const Double_t* x, const Double_t* params) const;
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
    void SetFittingMask(const CorrFitMask& map);
    /**
     * set limits of fitted radii
     * @param min minimal value
     * @param max maximal value
     */
    void SetRLimits(Double_t min, Double_t max) { SetParLimits(RadiusID(), min, max); };
    /**
     * set limits of fitted lambda
     * @param min minimal value
     * @param max maximal value
     */
    void SetLambdaLimits(Double_t min, Double_t max) { SetParLimits(LambdaID(), min, max); }
    /**
     *
     * @return return radius value
     */
    Double_t GetRadius() const { return GetParameter(RadiusID()); };
    /**
     *
     * @return radius error
     */
    Double_t GetRadiusError() const { return GetParError(RadiusID()); };
    /**
     *
     * @return lambda value (for fermions can be negative)
     */
    Double_t GetLambda() const { return GetParameter(LambdaID()); };
    /**
     *
     * @return lambda error
     */
    Double_t GetLambdaError() const { return GetParError(LambdaID()); };
    /**
     *
     * @return param number that correspond to radii
     */
    inline Int_t RadiusID() const { return fRinvParIndex; };
    /**
     *
     * @return param number that correspond to lambda
     */
    inline Int_t LambdaID() const { return fLambdaParIndex; };
    virtual ~CorrFit1DCF();
    ClassDef(CorrFit1DCF, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT1DCF_H_ */
