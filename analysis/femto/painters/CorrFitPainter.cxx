/*
 * CorrFitPainter.cxx
 *
 *  Created on: 23 lip 2024
 *      Author: daniel
 */

#include "CorrFitPainter.h"

#include <TLegend.h>
#include <TVirtualPad.h>

#include "CorrFit.h"
#include "CorrFitFunc.h"
#include "Cout.h"
#include "FemtoCFPainter.h"
#include "Std.h"
#include "StdString.h"

namespace Hal {
  const int CorrFitPainter::kAutoNormBit = 8;
  const int CorrFitPainter::kLegendBit   = 9;
  const int CorrFitPainter::kChi2        = 10;
  const int CorrFitPainter::kChi2Short   = 11;
  CorrFitPainter::CorrFitPainter(CorrFitFunc* func) : Painter(), fFittedFunc(func) {
    fNormIndex = fFittedFunc->GetParameterIndex("N");
  }

  void CorrFitPainter::ScaleFunctions() {
    Int_t flag = CheckOpt(kAutoNormBit);
    for (auto x : fFunctions) {
      for (auto y : x) {
        if (y) { y->FixParameter(y->GetNpar() - 1, flag); }
      }
    }
  }

  void CorrFitPainter::DeleteFunctions() {
    for (auto x : fFunctions) {
      for (auto y : x)
        if (y) delete y;
    }
    fFunctions.clear();
  }

  void CorrFitPainter::MakeLegend() {
    if (!CheckOpt(kLegendBit)) return;
    fLegendPad->cd();
    fLegend = new TLegend(fLegendPos[0], fLegendPos[1], fLegendPos[2], fLegendPos[3]);
    fLegend->SetHeader(GetName());
    if (fFittedFunc) { fLegend->SetHeader(fFittedFunc->GetName()); }
    auto label = GetLegendLabels();
    for (auto str : label) {
      fLegendEntries.push_back(fLegend->AddEntry((TObject*) 0x0, str, ""));
    }
  }

  void CorrFitPainter::UpdateLegend() {
    if (!fLegend) return;
    auto labels = GetLegendLabels();
    for (unsigned int i = 0; i < labels.size(); i++) {
      TLegendEntry* ent = fLegendEntries[i];
      ent->SetLabel(labels[i]);
      ent->SetTextColor(kGreen);
    }
    fLegendPad->Modified(1);
    fLegendPad->Update();
  }

  CorrFitPainter::~CorrFitPainter() { DeleteFunctions(); }

  void CorrFitPainter::InnerPaint() {
    DeleteFunctions();
    MakeFunctions();
    UpdateParameters();
    ScaleFunctions();
    ScaleHistograms();
    DrawFunctions();
    MakeLegend();
    if (fLegendPad && CheckOpt(kLegendBit)) {
      fLegendPad->cd();
      fLegend->Draw("SAME");
    }
    Painter::InnerPaint();
    OptionsApplied();
  }

  void CorrFitPainter::InnerRepaint() {
    UpdateParameters();
    ScaleFunctions();
    ScaleHistograms();
    DrawFunctions();
    UpdateLegend();
    Painter::InnerRepaint();
    OptionsApplied();
  }

  void CorrFitPainter::DrawFunctions() {
    LockPad();
    int count = 1;
    for (auto padfunc : fFunctions) {
      GotoPad(count++);
      for (auto func : padfunc) {
        if (func) { func->Draw(fDefFuncDrawOpt); }
      }
    }
    UnlockPad();
  }

  ULong64_t CorrFitPainter::PrepBitTemplate(std::initializer_list<int> temps) const {
    ULong64_t res = 0;
    auto vec      = Hal::Std::GetVector(temps);
    for (auto i : vec)
      SETBIT(res, i);
    return res;
  }

  Bool_t CorrFitPainter::AreSimiliar(ULong64_t current, ULong64_t pattern) const { return pattern == current & pattern; }

  ULong64_t CorrFitPainter::SetOptionInternal(TString opt, ULong64_t newFlag) {
    ContitionalPattern(opt, "norm", newFlag, kAutoNormBit, kTRUE);
    if (Hal::Std::FindParam(opt, "chi2s", kTRUE)) {
      CLRBIT(newFlag, kChi2Short);
      SETBIT(newFlag, kChi2);
    }
    if (Hal::Std::FindParam(opt, "chi2", kTRUE)) {
      SETBIT(newFlag, kChi2);
      SETBIT(newFlag, kChi2Short);
    }
    if (Hal::Std::FindParam(opt, "legend", kTRUE)) {
      SETBIT(newFlag, kLegendBit);
      auto bra = Hal::Std::FindBrackets(opt, kTRUE, kTRUE);
      for (auto pat : bra) {
        std::vector<Double_t> vals;
        if (GetPatterns(pat, "leg", vals)) {
          if (vals.size() == 4) {
            for (int i = 0; i < 4; i++)
              fLegendPos[i] = vals[i];
          }
        }
      }
    }
    return newFlag;
  }

  std::vector<TString> CorrFitPainter::GetLegendLabels() const {
    std::vector<TString> label;
    TString chi_label = "";
    if (CheckOpt(kChi2)) {
      Double_t chi2 = fFittedFunc->GetChiTF(fFittedFunc->fTempParamsEval);  // legend present, we have to recalcuate chi2
      TString chi_s, chindf_s, ndf_s;
      Double_t chi    = fFittedFunc->GetChiSquare();
      Double_t chindf = fFittedFunc->GetChiNDF();
      Double_t ndf    = fFittedFunc->GetNDF();
      if (chi <= 1000)
        chi_s = Form("%4.3f", chi);
      else
        chi_s = Hal::Std::RoundToString(chi, 2, "prefix");
      if (chindf <= 1000) {
        chindf_s = Form("%4.3f", chindf);
      } else {
        chindf_s = Hal::Std::RoundToString(chindf, 2, "prefix");
      }
      if (ndf <= 1000) {
        ndf_s = Form("%i", (int) ndf);
      } else {
        ndf_s = Hal::Std::RoundToString(ndf, 2, "prefix");
      }
      if (CheckOpt(kChi2Short)) {
        chi_label = Form("#chi^{2}/NDF %s (%s/%s)", chindf_s.Data(), chi_s.Data(), ndf_s.Data());
      } else {
        chi_label = Form("#chi^{2}/NDF %s", chindf_s.Data());
      }
    }
    for (int i = 0; i < fFittedFunc->GetParametersNo(); i++) {
      if (fFittedFunc->IsParFixed(i)) {
        label.push_back(Form(
          "%s %4.3f (fixed)", fFittedFunc->fParameters[i].GetParName().Data(), fFittedFunc->fParameters[i].GetFittedValue()));
      } else {
        if (TMath::IsNaN(fFittedFunc->fParameters[i].GetError())) {
          label.push_back(Form("%s %4.3f#color[2]{#pm%4.3f}",
                               fFittedFunc->fParameters[i].GetParName().Data(),
                               fFittedFunc->fParameters[i].GetFittedValue(),
                               fFittedFunc->fParameters[i].GetError()));
        } else if (fFittedFunc->fParameters[i].GetError() < 0) {
          label.push_back(Form("%s %4.3f#color[16]{#pm%4.3f}",
                               fFittedFunc->fParameters[i].GetParName().Data(),
                               fFittedFunc->fParameters[i].GetFittedValue(),
                               fFittedFunc->fParameters[i].GetError()));
        } else {
          label.push_back(Form("%s %4.3f#pm%4.3f",
                               fFittedFunc->fParameters[i].GetParName().Data(),
                               fFittedFunc->fParameters[i].GetFittedValue(),
                               fFittedFunc->fParameters[i].GetError()));
        }
      }
    }
    if (chi_label.Length() > 0) label.push_back(chi_label);
    return label;
  }


  void CorrFitPainter::ScaleHistograms() {
    auto func     = (CorrFitFunc*) fFittedFunc;
    Double_t norm = func->GetNorm();
    if (CheckOpt(kAutoNormBit)) {
      fCFPainter->Rescale(1.0 / func->GetNorm());
      fCFPainter->ScaleHistograms();
    }
  }

} /* namespace Hal */