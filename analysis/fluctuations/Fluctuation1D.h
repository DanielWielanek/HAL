/*
 * FluctuationMath.h
 *
 *  Created on: 31 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFLUCTUATION1D_H_
#define HALFLUCTUATION1D_H_


#include <TBrowser.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObject.h>

#include "Object.h"
/**
 * basic class for calculation variables used in fluctuation analysis
 *
 **/

namespace Hal {
  class Fluctuation1D : public Object {
    Double_t fN;
    Double_t fNError;
    Double_t fM[7];
    Double_t fMErr[7];
    Double_t fOmega;
    Double_t fSSigma;
    Double_t fKappaSigma;
    Double_t fOmegaError;
    Double_t fSigmaError;
    Double_t fKappaError;
    Int_t fNSample;
    TH1D* fHistogram;
    Double_t GetRMS(Double_t* array, Int_t size) const;
    void Calc(TH1D* h);
    void CalcError(TH1D* h);
    void GetFrom2D(TH1D* h, TString opt);
    TH1D* CalcProfile(TH2D* h, Int_t opt = 0);

  public:
    /**
     * constructor that should not be used
     */
    Fluctuation1D();
    /**
     * basic constructor
     * @param h histogram used for calculations
     */
    Fluctuation1D(TH1D* h);
    /**
     * c-torcreate histogram from 2D histogram
     * @param h source of data
     * @param opt if "x" or "pos" then histogram is made from projection along X
     * -axis if opt is "y" or "neg" the histogram is made from projection along
     * Y-axis if opt is equal to "sum" or "+" then histogram is made from sum of x
     * and y otherwise histogram is calculated as netto value of positive and
     * negative particles
     */
    Fluctuation1D(TH2D* h, TString opt);
    virtual TString HTMLExtract(Int_t no, TString dir = "") const;
    /**
     * add object of the same class to this
     * @param pack added object
     */
    virtual void Add(const Object* pack);
    /**
     * used to merge similar objects
     * @param collection
     */
    virtual Long64_t Merge(TCollection* collection);
    /**
     *
     * @return kappa sigma error
     */
    Double_t GetKappaSigmaError() const { return fKappaError; }
    /**
     *
     * @return mean
     */
    Double_t GetMean() const { return fN; }
    /**
     *
     * @return kappa sigma error
     */
    Double_t GetKappaSigma() const { return fKappaSigma; }
    /**
     *
     * @return mean uncertainty
     */
    Double_t GetMeanError() const { return fNError; }
    /**
     *
     * @return omega
     */
    Double_t GetOmega() const { return fOmega; }
    /**
     *
     * @return omega error
     */
    Double_t GetOmegaError() const { return fOmegaError; }
    /**
     *
     * @return sigma error
     */
    Double_t GetSSigmaError() const { return fSigmaError; }
    /**
     *
     * @return ssigma
     */
    Double_t GetSSigma() const { return fSSigma; }
    /**
     *
     * @param i central moment idex <2;6>
     * @return i-th central moment
     */
    Double_t GetCentralMoment(Int_t i) const;
    /**
     *
     * @param i central moment idex <2;6>
     * @return i-th centralm moment error
     */
    Double_t GetCentralMomentError(Int_t i) const;
    /**
     * recalcuate all variables and their uncertainties
     */
    void Recalculate();
    /**
     * set number of samples used in boosttrap method for caluclate sigma, kappa
     * errors
     * @param sample
     */
    void SetSample(Int_t sample) { fNSample = sample; };
    virtual void Draw(Option_t* opt);
    virtual void Browse(TBrowser* b);
    virtual void Print(Option_t* option = "") const;
    /**
     *
     * @return histogram used for calculations
     */
    TH1D* GetHistogram() const { return fHistogram; };
    virtual ~Fluctuation1D();
    ClassDef(Fluctuation1D, 1)
  };
}  // namespace Hal

#endif /* HALFLUCTUATION1D_H_ */
