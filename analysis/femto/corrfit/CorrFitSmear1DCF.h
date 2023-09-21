/*
 * CorrFitSmear1DCF.h
 *
 *  Created on: 4 sty 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITSMEAR1DCF_H_
#define HALCORRFITSMEAR1DCF_H_

#include "CorrFitKisiel.h"

#include <TMatrixD.h>
/**
 * class that smear the CF in 1D
 */
class TAxis;
namespace Hal {
  class CorrFitMapKstarRstar;
  class Femto1DCF;
  class Spline1D;
  class CorrFitSmear1DCF : public TObject {
    Bool_t fSeparateSmear      = {kFALSE};
    Bool_t fAutoFill           = {kFALSE};
    Int_t fThresBin            = {0};
    TF1* fFunc                 = {nullptr};
    TF1* fSplot                = {nullptr};
    TH2D* fSmearMatrix         = {nullptr};
    Femto1DCF* fCF             = {nullptr};
    CorrFitMapKstarRstar* fMap = {nullptr};
    Hal::Spline1D* fSpline     = {nullptr};
    Double_t GaussNS(Double_t* x, Double_t* params) const;
    Double_t CorrFunc(Double_t* x, Double_t* params) const;
    Double_t Splot(Double_t* x, Double_t* params) const;
    TMatrixD GetVec(const TH2D& vec, Int_t bin) const;
    TMatrixD GetVec(const TH1D& vec) const;
    TMatrixD Multiply(TMatrixD A, TMatrixD B) const;
    TMatrixD ReverseDenominator(TF1* smearfunc, TH2D* smearMatrix) const;
    std::vector<double> GetAutoFill(const TMatrixD& matrix, Bool_t lower) const;
    std::vector<double> GetGausNorms(TF1* f, TAxis* x) const;
    void NormalizeMatrix(TMatrixD& matrix) const;
    /**
     * check compatiblitity between smear matrix map and
     * @return
     */
    Bool_t CheckCompatiblityMap() const;
    /**
     * check compatibility of smeared map and function, return false if map cannot be used
     * @return
     */
    Bool_t CheckCompatibilityCFMap() const;

  public:
    /**
     * flag that describe smear code algo
     */

    CorrFitSmear1DCF();
    /**
     * set threshold bin this is the bin in which "pair" leaking becomes significant
     * @param no
     */
    void SetThresBin(Int_t no) { fThresBin = no; }
    /**
     * smear matrix on x-axis sim momentum on y-axis reco momentum
     * @param matrix of smearing on x-axis real momentum on y-axis reconstructed
     * @return
     */
    TH2D RecalcSmearMatrix(TH2D& matrix);
    /**
     *
     * @param matrix
     * @return transposed matrix
     */
    static TH2D TransposeHistogram(TH2D& matrix);
    /**
     * option of smearing
     * @param smear if true separately smear numerator and denominator, require to set the CF
     */
    void SmearSeparately(Bool_t smear = kTRUE) { fSeparateSmear = smear; };
    /**
     * enable "automatic filling" the pairs that are escape the smearing matrix
     * @param autofill
     */
    void AutoFillLastBin(Bool_t autofill = kTRUE) { fAutoFill = autofill; };
    /**
     * set correlation function (used when "smear separately" option is used)
     * @param cf
     */
    void SetCF(Hal::Femto1DCF& cf);
    /**
     * sets map that will be smeared
     * @param map
     */
    void SetMap(CorrFitMapKstarRstar* map) { fMap = map; };
    /**
     * basic method for smearing with the method of matrices
     * NOTE: recommended flags for advanced and addLastBin are true, false or false, true
     * @param map map to smear
     * @param smearMatrix smearing matrix - e.g., cut monitor on x-axis real momentum, on y-axis reconstructed momentum
     * @param advanced enables "advanced" mode - in this mode the semaring is performed separately on numerator (denominator*CF)
     * and denominator, ratio of such functions is divided to get the map
     * @param addLastBin this method allow to compensate the leak of pair outside of the CF, it's assumed that this number of
     * pairs is the same as the number of pairs that goes in lower bins
     * @return smeared map
     */
    CorrFitMapKstarRstar* SmearMatrix(const TH2D& smearMatrix);
    /**
     * not implemented yet
     * @param map
     * @param func
     * @param advanced
     * @return
     */
    CorrFitMapKstarRstar* SmearFunction(TF1* func);
    ~CorrFitSmear1DCF();
    TF1* GetFunc() { return fSplot; }
    ClassDef(CorrFitSmear1DCF, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITSMEAR1DCF_H_ */
