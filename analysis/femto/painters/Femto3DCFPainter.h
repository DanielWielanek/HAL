/*
 * Femto3DCFPainter.h
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_PAINTERS_FEMTO3DCFPAINTER_H_
#define HAL_ANALYSIS_PAINTERS_FEMTO3DCFPAINTER_H_

#include "FemtoCFPainter.h"

class TH1D;
namespace Hal {
  class Femto3DCF;

  class Femto3DCFPainter : public FemtoCFPainter {
    static const int kRgbBit, kDiag1Bit, kDiag2Bit, kRawBit;
    static const int kTwoDimBit, kTwoDimPlusBit, kAngles;

  protected:
    Femto3DCF* fCF;
    Double_t fThetaPad = {30.};
    Double_t fPhiPad   = {30.};
    virtual void MakePadsAndCanvases();
    Int_t GetPadsRequired() const;
    /**
     *  see @see #Hal::FemtoCFPainter#SetOptionInternal
     * @param opt
     * - diag1 - diagonal one projections
     * - diag2 - diagonal two projections (not working correctly) TODO -fix this
     * - hidetitles - hide titles of projections
     * - {ang=theta,phi} set theta phi angles of pad
     * @param prev
     * @return
     */
    virtual ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0);
    virtual void MakeHistograms();
    virtual void ScaleHistograms();
    virtual void PrepareHtml(TH1* h);
    virtual void Prepare3DFull(TH1* h);
    virtual void Prepare3D(TH1* h);
    virtual void PrepareDiagonal1(TH1* h);
    virtual void PrepareDiagonal2(TH1* h);
    virtual void PrepareTwoDim(TH1* h);
    virtual void DrawHistograms();
    virtual Bool_t CheckPads() const;
    TH1D* GetProjection1D(TH1* h, Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) const;

  public:
    Femto3DCFPainter(Femto3DCF* cf = nullptr);
    std::pair<Double_t, Double_t> GetXaxisRange() const;
    std::pair<Double_t, Double_t> GetYaxisRange() const;
    std::pair<Double_t, Double_t> GetZaxisRange() const;
    virtual TString GetOptionForCorrFit() const;
    virtual ~Femto3DCFPainter();
    ClassDef(Femto3DCFPainter, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_PAINTERS_FEMTO3DCFPAINTER_H_ */
