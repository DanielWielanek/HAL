/*
 * Painter.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Painter.h"

#include <iostream>

#include "Cout.h"  //KURWA
#include "PadStyle.h"
#include "StdString.h"
#include "Style.h"

#include <TCanvas.h>
namespace Hal {
  const int Painter::kHtmlBit = 0;
  const int Painter::kGridBit = 1;

  Painter::Painter() {
    fCommonData.fCanvases = new std::vector<TCanvas*>();
    fCommonData.fPads     = new std::vector<std::vector<TVirtualPad*>>();
  }

  Painter::~Painter() {
    CleanCommonData();
    for (auto x : fSubPainters)
      delete x;
  }

  void Painter::AddPainter(Painter* painter) {
    painter->CleanCommonData();
    painter->fOwnPad     = kFALSE;
    painter->fParent     = this;
    painter->fCommonData = fCommonData;
    fSubPainters.push_back(painter);
  }

  void Painter::Paint() {
    if (!HasParent()) {
      if (!fPainted) {
        MakePadsAndCanvases();
        fOwnPad = kTRUE;
      }
      LockPad();
      TryPaint();
      UpdateAllPads();
      UnlockPad();
    } else {
      auto grand = GetAncestor();
      if (grand->fPainted) {
        grand->TryPaint();
        grand->UpdateAllPads();
      } else {
        grand->Paint();
      }
    }
  }

  void Painter::SetFlag(Int_t bit, Bool_t state) {
    if (state)
      SETBIT(fDrawFlags, bit);
    else
      CLRBIT(fDrawFlags, bit);
  }

  void Painter::MakeCanvasPads(Int_t x, Int_t y, Int_t canvasNo) {
    auto dividePads = [&]() {
      auto canva = GetCanvas(canvasNo);
      canva->Divide(x, y);
      int count = 0;
      for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
          (*fCommonData.fPads)[canvasNo].push_back(canva->cd(++count));
        }
      }
    };
    LockPad();
    if (fCommonData.fCanvases->size() > canvasNo) {
      dividePads();
    } else {
      for (int i = fCommonData.fCanvases->size(); i <= canvasNo; i++) {
        auto newCanv = new TCanvas();
        fCommonData.fCanvases->push_back(newCanv);
        std::vector<TVirtualPad*> pads;
        pads.push_back(newCanv);
        fCommonData.fPads->push_back(pads);
      }
      dividePads();
    }
    UnlockPad();
  }

  void Painter::UpdateAllPads() {
    if (!fOwnPad) return;
    for (auto pads : *fCommonData.fPads) {
      for (unsigned int i = 1; i < pads.size(); i++) {
        auto pad = pads[i];
        if (fPadStyle) { fPadStyle->Apply(pad); }
        pad->Modified(kTRUE);
        pad->Update();
      }
    }
  }

  void Painter::TryPaint() {
    if (fPainted) {
      InnerRepaint();
    } else {
      if (!CheckPads() && HasParent()) {
        HalCoutDebug("Cannot paint this object due to failing pads dependencies");
        return;
      }
      fPainted = kTRUE;
      InnerPaint();
    }
    for (auto x : fSubPainters)
      x->TryPaint();
  }

  void Painter::SetOption(TString option) {
    if (Hal::Std::FindParam(option, "skip")) return;
    if (Hal::Std::FindParam(option, "default!")) {
      SetDefaultFlag();
      return;
    }
    if (Hal::Std::FindParam(option, "grid")) {
      Hal::PadStyle style;
      style.SetGridx(1);
      style.SetGridy(1);
      SetGlobalPadStyle(style);
    }
    if (Hal::Std::FindParam(option, "default")) { SetDefaultFlag(); }
    ULong64_t defFlags = 0;
    if (Hal::Std::FindParam(option, "keep", kTRUE)) { defFlags = fDrawFlags; }
    if (Hal::Std::FindParam(option, "html", kTRUE)) {
      SETBIT(defFlags, kHtmlBit);
      if (defFlags != fDrawFlags) {
        fOptionsChanged = kTRUE;
        fDrawFlags      = defFlags;
        return;
      }
    } else {
      CLRBIT(defFlags, kHtmlBit);
    }
    auto newOpt = SetOptionInternal(option, defFlags);
    if (newOpt != fDrawFlags) {
      fOptionsChanged = kTRUE;
      fDrawFlags      = newOpt;
    }
  }

  Bool_t Painter::HasParent() const {
    if (fParent) return kTRUE;
    return kFALSE;
  }

  Bool_t Painter::GetPatterns(TString opt, TString flag, std::vector<double>& vals) const {
    auto vec = Hal::Std::ExplodeString(opt, ',');
    if (vec.size() == 0) return kFALSE;
    TString match = Form("%s=", flag.Data());
    if (!vec[0].Contains(match)) return kFALSE;
    vec[0].ReplaceAll(match, "");
    for (auto i : vec) {
      double val = i.Atof();
      vals.push_back(val);
    }
    return kTRUE;
  }

  void Painter::ContitionalPattern(TString& option, TString pattern, ULong64_t& drawOpt, Int_t bit, Bool_t remove) const {
    Int_t flag = Hal::Std::FindParam2(option, pattern, remove);
    if (flag == 1) SETBIT(drawOpt, bit);
    if (flag == -1) CLRBIT(drawOpt, bit);
  }

  void Painter::LockPad() { fTempPad = gPad; }

  void Painter::UnlockPad() {
    gPad = fTempPad;
    if (gPad) gPad->cd();
  }

  void Painter::GotoPad(Int_t no, Int_t canvasNo) { (*fCommonData.fPads)[canvasNo][no]->cd(); }

  void Painter::ClearCanvas(Int_t canvasNo) {
    for (auto canv : (*fCommonData.fCanvases)) {
      canv->Clear();
      (*fCommonData.fPads)[canvasNo].clear();
    }
  }

  TVirtualPad* Painter::GetPad(Int_t index, Int_t canvasNo) const {
    if (index < 0) index = 0;
    return (*fCommonData.fPads)[canvasNo][index];
  }

  Painter* Painter::GetAncestor() const {
    if (HasParent()) { return GetParent(); }
    return GetAncestor();
  }

  void Painter::SetGlobalPadStyle(Hal::PadStyle& pad) { fPadStyle = new Hal::PadStyle(pad); }

  Bool_t Painter::CanvasExist(Int_t canvasNo) const {
    if (fCommonData.fCanvases->size() > canvasNo) return kTRUE;
    return kFALSE;
  }

  void Painter::CleanCommonData() {
    if (!fOwnPad) return;
    if (fCommonData.fCanvases) {
      for (auto canv : *fCommonData.fCanvases)
        delete canv;
      delete fCommonData.fCanvases;
      fCommonData.fCanvases = nullptr;
    }
    if (fCommonData.fPads) {
      delete fCommonData.fPads;
      fCommonData.fPads = nullptr;
    }
  }

} /* namespace Hal */
