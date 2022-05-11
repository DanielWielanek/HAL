/*
 * FemtoDoubleRatio1DCF.h
 *
 *  Created on: 21 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTODOUBLERATIO1DCF_H_
#define HALFEMTODOUBLERATIO1DCF_H_

#include "FemtoConst.h"

#include "DividedHisto.h"


namespace Hal {
  class FemtoDoubleRatio1DCF : public DividedHisto1D {
  protected:
    TH1D* fNumSide;
    TH1D* fDenSide;
    TH1D* fNumLong;
    TH1D* fDenLong;
    Femto::EKinematics fFrame;
    void SetAxisNames(TH1* h1, TH1* h2, TString label);
    void Normalize(TH1D* h, TH1D* num, TH1D* den, Int_t dir) const;
    TH1D* GetCF(Bool_t normalized, Int_t dir) const;

  public:
    /**
     * constructor for streamer
     */
    FemtoDoubleRatio1DCF();
    /**
     *
     * @param name name of function
     * @param kin frame type
     */
    FemtoDoubleRatio1DCF(TString name, Femto::EKinematics kin = Femto::EKinematics::kPRF);
    /**
     *
     * @param name name of cf
     * @param bins number of bins
     * @param min minimal value on axis
     * @param max max value on axis
     * @param kin frame
     */
    FemtoDoubleRatio1DCF(TString name, Int_t bins, Double_t min, Double_t max, Femto::EKinematics kin = Femto::EKinematics::kPRF);
    /**
     * copy constructor
     * @param other
     */
    FemtoDoubleRatio1DCF(const FemtoDoubleRatio1DCF& other);
    void FillNumObj(TObject* ob);
    void FillDenObj(TObject* obj);
    /**
     * set numerators
     * @param outnum "out" numerator
     * @param sidenum "side" numerator
     * @param longnum "long" numerator
     * @param clone if true then make own copy of histograms
     */
    void SetNum(TH1D* outnum, TH1D* sidenum, TH1D* longnum, Bool_t clone = kFALSE);
    /**
     * set denominators
     * @param outden "out" denominator
     * @param sideden "side" denominator
     * @param longden "long" denominator
     * @param clone if true then make own copy of histograms
     */
    void SetDen(TH1D* outden, TH1D* sideden, TH1D* longden, Bool_t clone = kFALSE);
    /**
     *
     * @return "out" numerator
     */
    TH1D* GetOutNum() const { return fNum; };
    /**
     *
     * @return "out" numerator
     */
    TH1D* GetOutDen() const { return fDen; };
    /**
     *
     * @return "side" numerator
     */
    TH1D* GetSideNum() const { return fNumSide; };
    /**
     *
     * @return "side" denominator
     */
    TH1D* GetSideDen() const { return fDenSide; };
    /**
     *
     * @return "long" numerator
     */
    TH1D* GetLongNum() const { return fNumLong; };
    /**
     *
     * @return "long" denominator
     */
    TH1D* GetLongDen() const { return fDenLong; };
    /**
     *
     * @param normalized normalize according to axis
     * @return cf function "out"
     */
    TH1D* GetCFOut(Bool_t normalized = kTRUE) const;
    /**
     *
     * @param normalized normalize according to axis
     * @return cf function "side"
     */
    TH1D* GetCFSide(Bool_t normalized = kTRUE) const;
    /**
     *
     * @param normalized normalize according to axis
     * @return cf function "long"
     */
    TH1D* GetCFLong(Bool_t normalized = kTRUE) const;
    virtual void Browse(TBrowser* b);
    virtual void Add(const Object* h);
    virtual void Draw(Option_t* opt = "all");
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    /**
     * rebin
     * @param ngroup
     * @param opt can be out side or long to rebin only part of histograms,
     * otherwise all are rebinned
     */
    virtual void Rebin(Int_t ngroup, Option_t* opt);
    virtual ~FemtoDoubleRatio1DCF();
    ClassDef(FemtoDoubleRatio1DCF, 1)
  };
}  // namespace Hal
#endif /* HALFEMTODOUBLERATIO1DCF_H_ */
