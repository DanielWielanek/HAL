/*
 * ConvolutionPairGenerator.h
 *
 *  Created on: 1 kwi 2025
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CONVOLUTIONPAIRGENERATOR_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CONVOLUTIONPAIRGENERATOR_H_

#include <TH2.h>
#include <TH3.h>
#include <TLorentzVector.h>

#include "FemtoConst.h"

namespace Hal {
  class FemtoMicroPair;
} /* namespace Hal */

namespace Hal {
  /**
   * stand alone generator that uses convolution method
   */
  class ConvolutionPairGenerator : public Object {
    TH2D fHist1, fHist2;
    TH3D fConvolution;
    Double_t fX = {0}, fY = {0}, fZ = {0};
    Int_t fPid1 = {0}, fPid2 = {0};
    Double_t fM1 = {0}, fM2 = {0}, fSumM = {0};
    Double_t fKt[2] = {0, 1};
    TLorentzVector fP1, fP2;
    Hal::Femto::EKinematics fFrame;
    void GeneratePairLCMS(const Double_t ptTot, const Double_t pzTot, const Double_t phi);
    void GeneratePairPRF(const Double_t ptTot, const Double_t pzTot, const Double_t phi);

  public:
    ConvolutionPairGenerator(Hal::Femto::EKinematics frame = Hal::Femto::EKinematics::kPRF);
    void SetFrame(Hal::Femto::EKinematics frame) { fFrame = frame; }
    void Recalculate(Double_t kout, Double_t kside, Double_t klong);
    void FillPair(Hal::FemtoMicroPair& pair);
    void FillPair(TLorentzVector& p1, TLorentzVector& p2);
    void SetData(const TH2D& hist1, Int_t pdg1, const TH2D& hist2, Int_t pdg2);
    void SetKt(Double_t min, Double_t max) {
      fKt[0] = min;
      fKt[1] = max;
    };
    Bool_t Init();
    virtual ~ConvolutionPairGenerator();
    ClassDef(ConvolutionPairGenerator, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CONVOLUTIONPAIRGENERATOR_H_ */
