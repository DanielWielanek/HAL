/*
 * CorrFitGUI.h
 *
 *  Created on: 11 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITGUI_H_
#define HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITGUI_H_

#include "CorrFit.h"

#include <TF1.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGSlider.h>
#include <TGWindow.h>
#include <TObject.h>
#include <TRandom.h>
#include <TRootEmbeddedCanvas.h>


namespace Hal {
  class CorrFitParButton : public TGHorizontalFrame {
    TGNumberEntry* fNumberEntry;
    TGComboBox* fComboBox;
    std::vector<double> fSteps;
    Bool_t fDiscrete;

  public:
    CorrFitParButton(const TGWindow* p = 0,
                     UInt_t w          = 1,
                     UInt_t h          = 1,
                     UInt_t options    = kChildFrame,
                     Pixel_t back      = GetDefaultFrameBackground());
    void Init(CorrFitGUI* gui, TString parName, FitParam par);
    Double_t GetValue();
    virtual ~CorrFitParButton() {};
    ClassDef(CorrFitParButton, 0)
  };

  class CorrFitChiSelector : public TGHorizontalFrame {
    TGComboBox* fParName;
    TGNumberEntry* fMin;
    TGNumberEntry* fMax;
    TGNumberEntry* fSteps;

  public:
    CorrFitChiSelector(const TGWindow* p = 0,
                       UInt_t w          = 1,
                       UInt_t h          = 1,
                       UInt_t options    = kChildFrame,
                       Pixel_t back      = GetDefaultFrameBackground());
    void Init(CorrFitGUI* gui, CorrFit* f);
    void GetValue(Int_t& par, Int_t& steps, Double_t& min, Double_t& max);
    virtual ~CorrFitChiSelector() {};
    ClassDef(CorrFitChiSelector, 0)
  };


  class CorrFitGUI : public TGMainFrame {
    CorrFit* fFunc;
    CorrFitParButton** fSliders;
    TGCheckButton* fNormStyle;
    CorrFitChiSelector* fPairChi1;
    CorrFitChiSelector* fPairChi2;
    TGCheckButton* fChiMin;
    Double_t fInitalNorm;
    const Int_t fNormIndex;
    void SetParams();

  public:
    CorrFitGUI(CorrFit* f);
    void ApplyParams();
    void DrawChi2();
    virtual ~CorrFitGUI();
    ClassDef(CorrFitGUI, 0)
  };
}  // namespace Hal

#endif /* HALFEMTO_ANALYSIS_FEMTO_CORRFIT_HALCORRFITGUI_H_ */
