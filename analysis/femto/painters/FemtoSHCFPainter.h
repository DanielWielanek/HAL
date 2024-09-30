/*
 * FemtoSHCFPainter.h
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_PAINTERS_FEMTOSHCFPAINTER_H_
#define HAL_ANALYSIS_PAINTERS_FEMTOSHCFPAINTER_H_

#include <Rtypes.h>
#include <TString.h>

#include "FemtoCFPainter.h"

class TH1;
namespace Hal {
  class FemtoSHCF;
} /* namespace Hal */

/**
 * class that paints SHCF function, created when Draw or DrawRanges is called
 */
namespace Hal {
  class FemtoSHCFPainter : public FemtoCFPainter {
    static const int kReBit, kImBit, kShortBit, kSepBit;
    Hal::FemtoSHCF* fSHCF = {nullptr};
    std::vector<Double_t> fRangesYY;
    Color_t fColorIm = {kRed}, fColorRe = {kBlue};

  protected:
    TH1* GetRe(Int_t no) const { return fHistograms[no][0]; };
    TH1* GetIm(Int_t no) const { return fHistograms[no][1]; };
    void AddHistograms(TH1* re, TH1* im);
    std::pair<TH1*, TH1*> GetNewHistPair(Int_t l, Int_t m);
    void MakeHistograms();
    Bool_t CheckPads() const;
    void SetHistRanges(TH1* hist, Int_t l);
    /**
     * set draw options
     * @param opts
     * - im - draw imaginary part
     * - re - draw real part if none of this (or both) are set - draw imaginary and real part
     * - short - draw only chosen components of cf
     * - neg - skip drawing the negative components of CF
     */
    virtual ULong64_t SetOptionInternal(TString opts, ULong64_t prev = 0);
    virtual void ScaleHistograms();
    virtual void MakePadsAndCanvases();

  public:
    FemtoSHCFPainter(Hal::FemtoSHCF* cf = nullptr);
    virtual TString GetOptionForCorrFit() const;
    void SetColors(Color_t real, Color_t imag) {
      fColorIm = imag;
      fColorRe = real;
    }
    virtual ~FemtoSHCFPainter() {};
    ClassDef(FemtoSHCFPainter, 0)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_PAINTERS_FEMTOSHCFPAINTER_H_ */
