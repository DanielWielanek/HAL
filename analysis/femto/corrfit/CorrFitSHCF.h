/*
 * CorrFitSHCF.h
 *
 *  Created on: 23 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITSHCF_H_
#define HALCORRFITSHCF_H_

#include "CorrFit3DCF.h"

#include "Array.h"
#include "FemtoYlmIndexes.h"

#include <complex>
#include <vector>

class TH1D;
class TH3D;
class TClonesArray;
namespace Hal {
  class CorrFitSHCF : public CorrFitFunc {
    friend class Femto1DCF;
    friend class Femto3DCF;
    friend class FemtoSHCF;
    static const Int_t fgRout;
    static const Int_t fgRside;
    static const Int_t fgRlong;
    static const Int_t fgLambda;
    static const Int_t fgNorm;
    Bool_t fPhysical;

    Int_t fBins;
    Int_t fBinRange[2];

    Double_t fAxisMin;
    Double_t fAxisStepOver;
    TH3D* fCovCF;
    std::vector<TH1D*> fCFHistogramsRe;
    std::vector<TH1D*> fCFHistogramsIm;
    std::vector<std::vector<Double_t>> fSqrErrorsIm;
    std::vector<std::vector<Double_t>> fSqrErrorsRe;
    mutable std::vector<std::vector<std::vector<Double_t>>> fCalculatedRe;
    mutable std::vector<std::vector<std::vector<Double_t>>> fCalculatedIm;
    std::vector<std::complex<double>> fYlmValBuffer;
    TObject* fPrevCF;


  protected:
    Int_t fMaxJM;
    mutable std::complex<double>* fYlmBuffer;  //[fMaxJM]
    FemtoYlmIndexes fLmVals;
    virtual void Prepare(TObject* obj);
    virtual void Check();
    /**
     * called for each calculation of chi2 or loglikehood minimalization - used
     * for recalculation CF is parameters are changed
     */
    virtual void RecalculateFunction() const;
    /**
     * not implemented !
     */
    virtual void RecalculateSmoothFunction() const {};
    /**
     * recompute errors in num by using information from den histogram
     * @param num
     * @param den
     */
    void SetErrors(TH1* /*num*/, const TH1* /*den*/) const {};
    void EstimateActiveBins();
    virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;
    double GetChiTFD(const double* /*par*/) const { return 0; };
    double GetChiTF(const double* par) const;
    double GetLogTFD(const double* /*par*/) const { return 0; };
    void ReadParametersName();
    TF1* GetFittingFunction(Option_t* opt) const;
    TF1* GetFittingFunction(Int_t el, Int_t em, Bool_t re = kTRUE) const;
    Double_t GetFun(Double_t* x, Double_t* params) const;
    Double_t GetFunD(Double_t* /*x*/, Double_t* /*params*/) const { return 0; };
    /**
     *
     * @param x q-out
     * @param y q-side
     * @param z q-long
     * @return numerical error for given bin
     */
    virtual Double_t GetNumericalError(Int_t /*x*/, Int_t /*y*/, Int_t /*z*/) const { return 0; };
    /**
     * make fit
     * @param histo
     * @param opt
     */
    virtual void Fit(TObject* histo);
    /**
     * return value of theoretical cf from interpolation
     * @param q
     * @param elm
     * @return
     */
    Double_t GetCFValRe(Double_t q, Int_t elm) const;
    /**
     * return value of theoretical cf from interpolation
     * @param q
     * @param elm
     * @return
     */
    Double_t GetCFValIm(Double_t q, Int_t elm) const;

  public:
    CorrFitSHCF(Int_t parNo = 3);
    /**
     * set range fo fitting
     * @param x_min min. value in out direction
     * @param x_max max. value in out direction
     * @param y_min min. value in side direction
     * @param y_max max. value in side direction
     * @param z_min min. value in long direction
     * @param z_max max. value in long direction
     */
    void SetFuncRange(Double_t x_min, Double_t x_max, Int_t el, Int_t em);
    /**
     *
     * @param draw_option if full(then draw fitted and fitting function, if
     * "cotains" range then only fitted range is drawn (event if correlation
     * function is beyond fitting range)
     */
    void Draw(Option_t* draw_option = "full") { DrawAdv(draw_option, 0); };
    /**
     * additional option for bin drawing
     * @param draw_option
     * @param width number of bins in projection  if negative then function is
     * calculated as average at middle bin and edges
     */
    void DrawAdv(Option_t* draw_option, Int_t width);
    /**
     *
     * @return Rout
     */
    Double_t GetRout() const { return GetParameter(Rout()); };
    /**
     *
     * @return Rout error
     */
    Double_t GetRoutErr() const { return GetParError(Rout()); };
    /**
     *
     * @return Rside
     */
    Double_t GetRside() const { return GetParameter(Rside()); };
    /**
     *
     * @return Rside error
     */
    Double_t GetRsideErr() const { return GetParError(Rside()); };
    /**
     *
     * @return Rlong
     */
    Double_t GetRlong() const { return GetParameter(Rlong()); };
    /**
     *
     * @return Rlong error
     */
    Double_t GetRlongErr() const { return GetParError(Rlong()); };
    /**
     *
     * @return labmda value (can be negative for fermions)
     */
    Double_t GetLambda() const { return GetParameter(Lambda()); };
    /**
     *
     * @return lambda error
     */
    Double_t GetLambdaErr() const { return GetParError(Lambda()); };
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
    virtual ~CorrFitSHCF();
    ClassDef(CorrFitSHCF, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITSHCF_H_ */
