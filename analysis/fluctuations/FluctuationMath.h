/*
 * FluctuationMath.h
 *
 *  Created on: 31 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFLUCTUATIONMATH_H_
#define HALFLUCTUATIONMATH_H_

#include <TH1D.h>
#include <TObject.h>

namespace Hal {
  class FluctuationMath : public TObject {
    Double_t fN;
    Double_t fTollerance;
    Double_t fNError;
    Double_t fM[5];
    Double_t fOmega;
    Double_t fSSigma;
    Double_t fKappaSigma;
    Double_t fOmegaError;
    Double_t fSigmaError;
    Double_t fKappaError;
    Int_t fNSample;
    void GetVals(Double_t& min, Double_t& max, Double_t val, Double_t error);
    void Calc(TH1D* h);
    void CalcError(TH1D* h);
    void CalcErrorMagic(TH1D* h);

  public:
    FluctuationMath();
    void SetHistoTH1D(TH1D* h, TString opt);
    Double_t GetKappaSigmaError() const { return fKappaError; }
    Double_t GetMean() const { return fN; }
    Double_t GetKappaSigma() const { return fKappaSigma; }
    Double_t GetMeanError() const { return fNError; }
    Double_t GetOmega() const { return fOmega; }
    Double_t GetOmegaError() const { return fOmegaError; }
    Double_t GetSSigmaError() const { return fSigmaError; }
    Double_t GetSSigma() const { return fSSigma; }
    Double_t GetCentralMoment(Int_t i) const;
    /**
     * set totlerance during caclucation of variables value between 5-10 are
     * reasonable
     * @param tollerance
     */
    void SetTollerance(Double_t tollerance) { fTollerance = tollerance; };
    virtual ~FluctuationMath();
    void SetSSigma(Double_t sSigma) { fSSigma = sSigma; }
    void SetSample(Int_t sample) { fNSample = sample; };
  };
}  // namespace Hal
#endif /* HALFLUCTUATIONMATH_H_ */
