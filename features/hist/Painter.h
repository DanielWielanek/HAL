/*
 * Painter.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_HIST_PAINTER_H_
#define HAL_FEATURES_HIST_PAINTER_H_

#include <TObject.h>
/**
 * representation of painted objects
 */
class TCanvas;
class TVirtualPad;
class TH1;
namespace Hal {
  class Style;
  class PadStyle;
  class HistoStyle;
  class Painter : public TObject {
  protected:
    TCanvas* fCanvas = {nullptr};
    std::vector<TVirtualPad*> fSubPads;
    TVirtualPad* fMainPad        = {nullptr};
    Hal::HistoStyle* fHistoStyle = {nullptr};
    Hal::PadStyle* fPadStyle     = {nullptr};
    Bool_t fOwnPad               = {kFALSE};
    Bool_t fDrawn                = {kFALSE};
    std::vector<TH1*> GetListHistograms(Int_t subpad) const;
    virtual void SetHistoStyle(const Hal::HistoStyle& style);
    virtual void SetPadStyle(const Hal::PadStyle& style);

  public:
    Painter();
    Painter(const Painter& other);
    virtual void Repaint(TString option);
    virtual void Paint(TString option) = 0;
    virtual void Draw(Option_t* opt = "") {
      if (!fDrawn) {
        fDrawn = kTRUE;
        Paint(opt);
      } else {
        Repaint(opt);
      }
    };
    void SetStyle(const Hal::Style& style);
    Painter& operator=(const Painter& other);
    TCanvas* GetCanvas() const { return fCanvas; }
    virtual ~Painter();
    ClassDef(Painter, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_PAINTER_H_ */
