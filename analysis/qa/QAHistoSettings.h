/*
 * QAHistoSettings.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALQAHISTOSETTINGS_H_
#define HALQAHISTOSETTINGS_H_

#include <TNamed.h>
#include <TVector3.h>

#include <vector>

class TH1;
namespace Hal {
  class QAHistoSettings : public TNamed {
    Int_t fSize;
    Int_t fFillFlagIdX, fFillFlagIdY, fFillFlagIdZ;
    Bool_t fCustom;
    TString fFlag;
    TString fAxisNameX, fAxisNameY, fAxisNameZ;
    TVector3 fBinsX, fBinsY, fBinsZ;

  public:
    QAHistoSettings(Int_t dim = 0) : fSize(dim), fFillFlagIdX(0), fFillFlagIdY(0), fFillFlagIdZ(0), fCustom(kFALSE) {};
    QAHistoSettings& operator=(const QAHistoSettings& other) = default;
    QAHistoSettings(const QAHistoSettings& other)            = default;
    virtual void Print(Option_t* option = "") const;
    void SetFlag(TString flag) { fFlag = flag; };
    void SetCustomized(Bool_t custom = kTRUE) { fCustom = custom; };
    void SetXaxis(Int_t nbins, Double_t min, Double_t max) { fBinsX.SetXYZ(nbins, min, max); }
    void SetYaxis(Int_t nbins, Double_t min, Double_t max) { fBinsY.SetXYZ(nbins, min, max); }
    void SetZaxis(Int_t nbins, Double_t min, Double_t max) { fBinsZ.SetXYZ(nbins, min, max); }
    void SetXaxisName(TString name) { fAxisNameX = name; };
    void SetYaxisName(TString name) { fAxisNameY = name; };
    void SetZaxisName(TString name) { fAxisNameZ = name; };
    void SetFillFlagX(Int_t flagID) { fFillFlagIdX = flagID; };
    void SetFillFlagY(Int_t flagID) { fFillFlagIdY = flagID; };
    void SetFillFlagZ(Int_t flagID) { fFillFlagIdZ = flagID; };
    Int_t GetFillFlagX() const { return fFillFlagIdX; };
    Int_t GetFillFlagY() const { return fFillFlagIdY; };
    Int_t GetFillFlagZ() const { return fFillFlagIdZ; };
    /**
     *
     * @return number of dimension
     */
    Int_t GetNDim() const { return fSize; }
    /**
     *
     * @return true if this should be customized plot
     */
    Bool_t IsCustom() const { return fCustom; };
    /**
     *
     * @return title + axis name
     */
    TString GetFullTitle() const;
    /**
     *
     * @return flag that describes property of this histogram
     */
    TString GetFlag() const { return fFlag; };
    TH1* MakeHisto() const;
    virtual ~QAHistoSettings() {};
    ClassDef(QAHistoSettings, 1)
  };
}  // namespace Hal

#endif /* HALQAHISTOSETTINGS_H_ */
