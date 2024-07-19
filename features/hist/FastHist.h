/*
 * FastHist.h
 *
 *  Created on: 6 cze 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_HIST_FASTHIST_H_
#define HAL_FEATURES_HIST_FASTHIST_H_

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TNamed.h>

namespace Hal {

  class FastHist : public TNamed {
  protected:
    TString fAxisNameX;
    TString fAxisNameY;
    TString fAxisNameZ;
    TString fTitle;
    Int_t fBinsNoX;
    Int_t fBinsNoY;
    Int_t fBinsNoZ;
    Int_t fTotalBinsNo;
    Double_t* fValues;  //[fTotalBinsNo]

  public:
    FastHist(TString name = "", TString title = "");
    void SetXaxisName(TString name) { fAxisNameX = name; }
    void SetYaxisName(TString name) { fAxisNameY = name; }
    void SetZaxisName(TString name) { fAxisNameZ = name; }
    Int_t GetNbinsX() const { return fBinsNoX; };
    Int_t GetNbinsY() const { return fBinsNoY; };
    Int_t GetNbinsZ() const { return fBinsNoZ; };
    virtual Int_t FindBin(Double_t /*x*/) const { return 0; };
    virtual Int_t FindBin(Double_t /*x*/, Double_t /*y*/) const { return 0; };
    virtual Int_t FindBin(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) const { return 0; };
    virtual Double_t GetBinContent(Int_t /*i*/) const { return 0; };
    virtual Double_t GetBinContent(Int_t /*i*/, Int_t /*j*/) const { return 0; };
    virtual Double_t GetBinContent(Int_t /*i*/, Int_t /*j*/, Int_t /*k*/) const { return 0; };
    virtual void Fill(Double_t /*x*/, Double_t /*w*/) {};
    virtual void Fill(Double_t /*x*/, Double_t /*y*/, Double_t /*w*/) {};
    virtual void Fill(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/, Double_t /*w*/) {};
    inline void SetRawBinContent(Int_t bin, Double_t w) { fValues[bin] = w; };
    inline Double_t GetRawBinContent(Int_t bin) const { return fValues[bin]; }
    inline void IncrementRawBinContent(Int_t bin, Double_t w) { fValues[bin] += w; };
    virtual TH1* GetTH1() const { return nullptr; };
    virtual ~FastHist();
    ClassDef(FastHist, 1);
  };
  class FastHist1D : public FastHist {
    Double_t fStepX;
    Double_t fMinX;
    Double_t fMaxX;

  public:
    FastHist1D(TString name = "", TString title = "", Int_t nbins = 0, Double_t min = 0, Double_t max = 0);
    Int_t FindBin(Double_t x) const;
    void Fill(Double_t x, Double_t w);
    Double_t GetBinContent(Int_t i) const { return fValues[i]; }
    virtual TH1* GetTH1() const;
    virtual ~FastHist1D() {};
    ClassDef(FastHist1D, 1);
  };

  class FastHist2D : public FastHist {
    Double_t fStepX;
    Double_t fMinX;
    Double_t fMaxX;
    Double_t fStepY;
    Double_t fMinY;
    Double_t fMaxY;

  public:
    FastHist2D(TString name  = "",
               TString title = "",
               Int_t nbinsX  = 0,
               Double_t xmin = 0,
               Double_t xmax = 0,
               Int_t nbinsY  = 0,
               Double_t ymin = 0,
               Double_t ymax = 0);
    Double_t GetBinContent(Int_t i, Int_t j) const;
    Int_t FindBin(Double_t x, Double_t y) const;
    void Fill(Double_t x, Double_t y, Double_t w);
    virtual TH1* GetTH1() const;
    virtual ~FastHist2D() {};
    ClassDef(FastHist2D, 1)
  };

  class FastHist3D : public FastHist {
    Double_t fStepX;
    Double_t fMinX;
    Double_t fMaxX;
    Double_t fStepY;
    Double_t fMinY;
    Double_t fMaxY;
    Double_t fStepZ;
    Double_t fMinZ;
    Double_t fMaxZ;
    Int_t fBinsYZ;

  public:
    FastHist3D(TString name  = "",
               TString title = "",
               Int_t nbinsX  = 0,
               Double_t xmin = 0,
               Double_t xmax = 0,
               Int_t nbinsY  = 0,
               Double_t ymin = 0,
               Double_t ymax = 0,
               Int_t nbinsZ  = 0,
               Double_t zmin = 0,
               Double_t zmax = 0);
    void Fill(Double_t x, Double_t y, Double_t z, Double_t w);
    Int_t FindBin(Double_t x, Double_t y, Double_t z) const;
    Double_t GetBinContent(Int_t i, Int_t j, Int_t k) const;
    virtual TH1* GetTH1() const;
    virtual ~FastHist3D() {};
    ClassDef(FastHist3D, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_FASTHIST_H_ */
