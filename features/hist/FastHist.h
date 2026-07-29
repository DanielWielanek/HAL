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
#include <algorithm>
#include <vector>

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
    std::vector<Double_t> fValues;
    Color_t fFillColor = {kWhite};
    Double_t GetLabelValue(const std::vector<TString>& input, TString& val) const;

  public:
    FastHist(TString name = "", TString title = "");
    void SetXaxisName(TString name) { fAxisNameX = name; }
    void SetYaxisName(TString name) { fAxisNameY = name; }
    void SetZaxisName(TString name) { fAxisNameZ = name; }
    Int_t GetNbinsX() const { return fBinsNoX; };
    Int_t GetNbinsY() const { return fBinsNoY; };
    Int_t GetNbinsZ() const { return fBinsNoZ; };
    void SetFillColor(Color_t col) { fFillColor = col; }
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
    void Reset() { std::fill(fValues.begin(), fValues.end(), 0.0); }
    virtual TH1* GetTH1() const { return nullptr; };
    virtual ~FastHist();
    ClassDef(FastHist, 1);
  };
  class FastHist1D : public FastHist {
    Double_t fStepX;
    Double_t fMinX;
    Double_t fMaxX;
    Double_t fOffsetX;
    std::vector<TString> fLabelsX;
    void Initialize();

  public:
    FastHist1D(TString name = "", TString title = "", Int_t nbins = 0, Double_t min = 0, Double_t max = 0);
    void LabelizeAxis(const std::vector<TString>& labels);
    Double_t GetLabelVal(TString label) const { return GetLabelValue(fLabelsX, label); };
    inline Int_t FindBin(Double_t x) const final {
      Int_t bin = x * fStepX + fOffsetX;
      if (std::isnan(x)) bin = fBinsNoX - 1;
      return std::clamp(bin, 0, fBinsNoX - 1);
    }
    Double_t GetBinCenterX(Int_t bin) const { return fMinX + (-0.5 + bin) * fStepX; }
    Double_t GetBinWidthX() const { return 1.0 / fStepX; }
    inline void Fill(Double_t x, Double_t w) final { fValues[FindBin(x)] += w; }
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
    Double_t fOffsetX;
    Double_t fOffsetY;
    std::vector<TString> fLabelsX;
    std::vector<TString> fLabelsY;
    void Initialize();
    void LabelizeAxis(const std::vector<TString>& labels, Char_t axis);

  public:
    FastHist2D(TString name  = "",
               TString title = "",
               Int_t nbinsX  = 0,
               Double_t xmin = 0,
               Double_t xmax = 0,
               Int_t nbinsY  = 0,
               Double_t ymin = 0,
               Double_t ymax = 0);
    void LabelizeAxisX(const std::vector<TString>& labels) { LabelizeAxis(labels, 'x'); }
    void LabelizeAxisY(const std::vector<TString>& labels) { LabelizeAxis(labels, 'y'); }
    Double_t GetBinContent(Int_t i, Int_t j) const final;
    inline Int_t FindBin(Double_t x, Double_t y) const final {
      Int_t binX = x * fStepX + fOffsetX;
      Int_t binY = y * fStepY + fOffsetY;
      binX       = std::clamp(binX, 0, fBinsNoX - 1);
      binY       = std::clamp(binY, 0, fBinsNoY - 1);
      if (std::isnan(x)) binX = fBinsNoX - 1;
      if (std::isnan(y)) binY = fBinsNoY - 1;
      return binX * fBinsNoY + binY;
    }
    Double_t GetLabelValX(TString label) const { return GetLabelValue(fLabelsX, label); };
    Double_t GetLabelValY(TString label) const { return GetLabelValue(fLabelsY, label); };
    Double_t GetBinCenterX(Int_t bin) const { return fMinX + (-0.5 + bin) * fStepX; }
    Double_t GetBinWidthX() const { return 1.0 / fStepX; }
    Double_t GetBinCenterY(Int_t bin) const { return fMinY + (-0.5 + bin) * fStepY; }
    Double_t GetBinWidthY() const { return 1.0 / fStepY; }
    inline void Fill(Double_t x, Double_t y, Double_t w) final { fValues[FindBin(x, y)] += w; }
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
    Double_t fOffsetX;
    Double_t fOffsetY;
    Double_t fOffsetZ;
    std::vector<TString> fLabelsX;
    std::vector<TString> fLabelsY;
    std::vector<TString> fLabelsZ;
    void Initialize();
    void LabelizeAxis(const std::vector<TString>& labels, Char_t axis);

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
    inline void Fill(Double_t x, Double_t y, Double_t z, Double_t w) final { fValues[FindBin(x, y, z)] += w; }
    void LabelizeAxisX(const std::vector<TString>& labels) { LabelizeAxis(labels, 'x'); }
    void LabelizeAxisY(const std::vector<TString>& labels) { LabelizeAxis(labels, 'y'); }
    void LabelizeAxisZ(const std::vector<TString>& labels) { LabelizeAxis(labels, 'z'); }
    Double_t GetLabelValX(TString label) const { return GetLabelValue(fLabelsX, label); };
    Double_t GetLabelValY(TString label) const { return GetLabelValue(fLabelsY, label); };
    Double_t GetLabelValZ(TString label) const { return GetLabelValue(fLabelsZ, label); };
    inline Int_t FindBin(Double_t x, Double_t y, Double_t z) const final {
      Int_t binX = x * fStepX + fOffsetX;
      Int_t binY = y * fStepY + fOffsetY;
      Int_t binZ = z * fStepZ + fOffsetZ;
      binX       = std::clamp(binX, 0, fBinsNoX - 1);
      binY       = std::clamp(binY, 0, fBinsNoY - 1);
      binZ       = std::clamp(binZ, 0, fBinsNoZ - 1);
      if (std::isnan(x)) binX = fBinsNoX - 1;
      if (std::isnan(y)) binY = fBinsNoY - 1;
      if (std::isnan(z)) binZ = fBinsNoZ - 1;
      return binX * fBinsYZ + binY * fBinsNoZ + binZ;
    }
    Double_t GetBinContent(Int_t i, Int_t j, Int_t k) const final;
    Double_t GetBinCenterX(Int_t bin) const { return fMinX + (-0.5 + bin) * fStepX; }
    Double_t GetBinWidthX() const { return 1.0 / fStepX; }
    Double_t GetBinCenterY(Int_t bin) const { return fMinY + (-0.5 + bin) * fStepY; }
    Double_t GetBinWidthY() const { return 1.0 / fStepY; }
    Double_t GetBinCenterZ(Int_t bin) const { return fMinZ + (-0.5 + bin) * fStepZ; }
    Double_t GetBinWidthZ() const { return 1.0 / fStepZ; }
    virtual TH1* GetTH1() const;
    virtual ~FastHist3D() {};
    ClassDef(FastHist3D, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_FASTHIST_H_ */
