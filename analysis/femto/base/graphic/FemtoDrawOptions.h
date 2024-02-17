/*
 * FemtoDrawOptions.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_FEMTODRAWOPTIONS_H_
#define HAL_ANALYSIS_FEMTO_BASE_FEMTODRAWOPTIONS_H_
#include <TObject.h>
#include <vector>

#include "UniqueOptions.h"

namespace Hal {

  class FemtoDrawOptions : public UniqueOptions {
  protected:
    std::vector<Double_t> fXRanges;
    std::vector<Double_t> fYRanges;
    Hal::HistoStyle* fHistoStyle  = {nullptr};
    Hal::HistoStyle* fHistoStyle2 = {nullptr};
    Hal::PadStyle* fPadStyle      = {nullptr};

  public:
    /**
     * default ctor
     * @param opt option of drawing:
     * num - draw numerators
     * den - draw denominators
     * cf - draw CF's
     * same - draw "same" (like in TH)
     * re - draw only real data (only for SH)
     * im - draw only img data (only for SH)
     *
     */
    FemtoDrawOptions(TString opt = "cf");
    FemtoDrawOptions(const FemtoDrawOptions& other);
    FemtoDrawOptions& operator=(const FemtoDrawOptions& other);
    /**
     * set draw options separated by +, note - this option does not check conflicts !
     * @param opts
     */
    void SetOptions(TString opts);
    void SetDrawNum() { AddTag("num"); };
    void SetDrawDen() { AddTag("den"); };
    void SetDrawCf() { AddTag("cf"); };
    void SetDrawSame() { AddTag("same"); }
    void SetDrawGrid() { AddTag("grid"); };
    void SetDrawReal() { AddTag("re"); };
    void SetDrawImag() { AddTag("im"); }
    void SetSepRealIm() { AddTag("seperate"); }
    void SetDrawReAndIm() {
      RemoveTag("im");
      RemoveTag("re");
    }
    void SetQRanges(Double_t min, Double_t max);
    void SetCfRanges(std::initializer_list<Double_t> vals);
    void SetDiag1() { AddTag("diag1"); }
    void SetDiag2() { AddTag("diag2"); }
    void SkipNegative() { AddTag("skipneg"); }
    void SetHistoStyle(const Hal::HistoStyle& style, Int_t no = 0);
    void SetPadStyle(const Hal::PadStyle& style);

    Bool_t DrawNum() const { return CheckTag("num"); }
    Bool_t DrawDen() const { return CheckTag("den"); }
    Bool_t DrawIm() const { return CheckTag("im"); }
    Bool_t DrawRe() const { return CheckTag("re"); }
    Bool_t DrawGrid() const { return CheckTag("grid"); }
    Bool_t DrawSame() const { return CheckTag("same"); }
    Bool_t DrawSeparate() const { return CheckTag("separate"); }
    Bool_t SkipNegative() const { return CheckTag("skipneg"); }

    std::vector<Double_t> GetXRanges() const { return fXRanges; };
    std::vector<Double_t> GetYRanges() const { return fYRanges; };

    virtual ~FemtoDrawOptions();
    ClassDef(FemtoDrawOptions, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_FEMTODRAWOPTIONS_H_ */
