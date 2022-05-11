/*
 * FemtoSHCF.h
 *
 *  Created on: 28 kwi 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOSHCF_H_
#define HALFEMTOSHCF_H_

#include "FemtoConst.h"

#include "Array.h"
#include "DividedHisto.h"

#include <TMath.h>
#include <TString.h>
#include <complex>

namespace Hal {
  class CorrFitSHCF;

  /**
   * class for storing sphercial harmonics correlation functions
   */
  class FemtoSHCF : public DividedHisto1D {
    const Int_t fMaxJM;
    TH1D** fNumReal;  //[fMaxJM] Real parts of Ylm components of the numerator
    TH1D** fNumImag;  // [fMaxJM]Imaginary parts of Ylm components of the numerator
    TH1D** fDenReal;  //[fMaxJM] Real parts of Ylm components of the denominator
    TH1D** fDenImag;  //[fMaxJM] Imaginary parts of Ylm components of the denominator
    Femto::EKinematics fFrame;

    TH1D** fCFReal;  //[fMaxJM] real CF's
    TH1D** fCFImag;  //[fMaxJM] img CF's
    Int_t fFactorialsSize;
    Int_t fMaxL;
    Array_1<Double_t>* covmnum;  // Covariance matrix for the numerator
    Array_1<Double_t>* covmden;  // Covariance matrix for the denominator
    Array_1<Double_t>* covmcfc;  // Covariance matrix for the  CF
    Double_t fNormPurity;        //
    Double_t fNormRadius;        //
    Double_t fNormBohr;          //
    Double_t* fEls;              //[fMaxJM]
    Double_t* fEms;              //[fMaxJM]
    Double_t* fElsi;             //[fMaxJM]
    Double_t* fEmsi;             //[fMaxJM]
    Double_t* fFactorials;       //[fFactorialsSize]
    TH3D* fCfcov;                //

    Double_t Sil(Double_t n) const;
    Double_t Sil2(Double_t n) const;
    Int_t GetIndexForLM(int el, int em) const;
    Int_t GetBin(int qbin, int ilmzero, int zeroimag, int ilmprim, int primimag) const;
    Double_t Sqr(Double_t val1, Double_t val2) const;
    TH1D* Histo(int ilm, int em, Option_t* opt, Double_t scale) const;
    void PackCfcCovariance();
    void UnpackCovariances();
    void GetElEmForIndex(int aIndex, double& aEl, double& aEm) const;
    void GetElEmForIndex(int aIndex, int& aEl, int& aEm) const;
    void GetMtilde(std::complex<double>* aMat, double* aMTilde);
    void InvertYlmIndependentMatrix(double* inmat, double* outmat);
    void UnPackYlmMatrixIndependentOnly(double* inmat, double* outmat, int insize);
    void GetIndependentLM(int ibin, int& el, int& em, int& im) const;
    int PackYlmMatrixIndependentOnly(double* inmat, double* outmat) const;
    int PackYlmVectorIndependentOnly(double* invec, double* outvec) const;
    double WignerSymbol(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const;
    double ClebschGordan(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const;
    double DeltaJ(double aJot1, double aJot2, double aJot) const;
    /**
     * return histogram
     * @param el l
     * @param em m
     * @param norm if norm = true return normalized
     * @param opt can be "im" or "re"
     * @return
     */
    TH1D* GetHisto(int el, int em, Bool_t norm, Option_t* opt = "") const;

  protected:
    /**
     *
     * @return max JM
     */
    Int_t GetMaxJM() const { return fMaxJM; };
    /**
     * fast adding histograms without recalculation of CF's
     * @param obj
     */
    void FastAdd(const FemtoSHCF* obj);

  public:
    /** default constructor for streamer
     */
    FemtoSHCF();
    /**
     *
     * @param name name o f histogram
     * @param maxL maximal L
     * @param bins number of bin in each expression term
     * @param min minimal value of histogram
     * @param max maxima value of histogram
     * @param kinematics kinametics
     */
    FemtoSHCF(TString name, Int_t maxL, Int_t bins, Double_t min, Double_t max, Femto::EKinematics kinematics);
    /**
     * copy constructor
     * @param other
     */
    FemtoSHCF(const FemtoSHCF& other);
    void FillNumObj(TObject* obj);
    void FillDenObj(TObject* obj);
    /**
     * primitive constructor to conver angular Femto3DCF into SH CF
     * @param cf
     * @param maxL
     */
    /* TODO implenet this function in proper way
     FemtoSHCF(Femto3DCF *cf, Int_t maxL=3);
     */
    /**
     * add comment to this object
     * @param comment
     */
    void SetComment(TString comment) { fComment = comment; };
    /**
     * set norm purity
     * @param purity
     */
    void SetNormPurity(Double_t purity) { fNormPurity = purity; }
    /**
     * set norm radius
     * @param radius
     */
    void SetNormRadius(Double_t radius) { fNormRadius = radius; };
    /**
     * set bohr radius for normalization
     * @param bohr
     */
    void SettNormBohr(Double_t bohr) { fNormBohr = bohr; };
    /**
     * @param label
     */
    /**
     * add label to this object
     * @param label
     */
    void AddLabel(TString label);
    /**
     * recalculate correlation function, should be used if numerator/denominator
     * was changed
     */
    void RecalculateCF();
    /**
     * add real numerators
     * @param histograms array of numerators
     * @param clone if true then copy of histograms is made, otherwise only
     * pointer is copied
     */
    void SetNumRe(TH1D** histograms, Bool_t clone = kTRUE);
    /**
     * add imaginary numerators
     * @param histograms array of numerators
     * @param clone if true then copy of histograms is made, otherwise only
     * pointer is copied
     */
    void SetNumIm(TH1D** histograms, Bool_t clone = kTRUE);
    /**
     * add real denominators
     * @param histograms array of numerators
     * @param clone if true then copy of histograms is made, otherwise only
     * pointer is copied
     */
    void SetDenRe(TH1D** histograms, Bool_t clone = kTRUE);
    /**
     * add imaginary denominators
     * @param histograms array of numerators
     * @param clone if true then copy of histograms is made, otherwise only
     * pointer is copied
     */
    void SetDenIm(TH1D** histograms, Bool_t clone = kTRUE);
    /**
     * draw this histograms with default setup
     * @param opt
     */
    void Draw(Option_t* opt = "");
    /**
     * normalize by using normalization algorithm for given histogram
     * @param el L
     * @param em M
     * @param opt specifiy with histogram should be used - real "re" or imaginary
     * "im"
     */
    void NormalizeBy(int el, int em, Option_t* opt = "re");
    /**
     * set scale
     * @param scale
     */
    void SetScale(Double_t scale) { fScale = scale; };
    /**
     * used to browser this class
     * @param b
     */
    void Browse(TBrowser* b);
    /**
     * fit this function
     */
    void Fit(CorrFitSHCF* fit);
    /**
     * make "dummy fit" (allows to draw function)
     * does not perform any fit
     * @param fit
     */
    void FitDummy(CorrFitSHCF* fit);
    /**
     * return computed correlation function
     * @param el
     * @param em
     * @return
     */
    TH1D* GetCFRe(int el, int em) const;
    /**
     *
     * @param el
     * @param em
     * @return computed correlation function
     */
    TH1D* GetCFIm(int el, int em) const;
    /**
     *
     * @param el L
     * @param em M
     * @return real numerator with given index
     */
    TH1D* GetNumRe(int el, int em) const;
    /**
     *
     * @param el L
     * @param em M
     * @return imaginary numerator with given index
     */
    TH1D* GetNumIm(int el, int em) const;
    /**
     *
     * @param el L
     * @param em M
     * @return real denominator with given index
     */
    TH1D* GetDenRe(int el, int em) const;
    /**
     *
     * @param el L
     * @param em M
     * @return imaginary denominator with given index
     */
    TH1D* GetDenIm(int el, int em) const;
    /**
     *
     * @return covariance matrix for correlation function
     */
    TH3D* GetCovCF() const;
    /**
     *
     * @param opt option used to get tesseral harmonics
     * @return R if true return function with "normalization to zero"
     */
    TH1D* GetCubic(Option_t* opt, Bool_t R = kFALSE) const;
    Femto::EKinematics GetFrame() const { return fFrame; }
    /**
     *
     * @return L
     */
    Int_t GetL() const { return TMath::Sqrt(fMaxJM - 1); };
    void Rebin(Int_t ngroup, Option_t* opt = "");
    Array_1<Float_t>* ExportToFlatNum() const;
    virtual void Add(const Object* pack);
    virtual Long64_t Merge(TCollection* collection);
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~FemtoSHCF();
    ClassDef(FemtoSHCF, 4)
  };
}  // namespace Hal
#endif /* HALFEMTOSHCF_H_ */
