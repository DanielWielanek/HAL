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
#include "StdString.h"
#include "Style.h"

#include <TCanvas.h>
namespace Hal {
  const int Painter::kHtmlBit = 0;
  const int Painter::kGridBit = 1;
  Painter::Painter() : fSubPads(new std::vector<TVirtualPad*>()) {}
  Painter::~Painter() {
    if (fOwnPad) {
      if (fCanvas) delete fCanvas;
      if (fSubPads) delete fSubPads;
    }
    for (auto x : fSubPainters)
      delete x;
  }

  void Painter::AddPainter(Painter* painter) {
    if (painter->fSubPads && painter->fOwnPad) {
      delete painter->fSubPads;
      painter->fSubPads = nullptr;
    }
    painter->fOwnPad  = kFALSE;
    painter->fParent  = this;
    painter->fMainPad = fMainPad;
    painter->fSubPads = fSubPads;
    fSubPainters.push_back(painter);
  }

  void Painter::Paint() {
    if (!HasParent()) {
      if (!fPainted) {
        MakePad();
        MakeSubPads();
      }
      LockPad();
      for (auto x : fSubPainters)
        x->fMainPad = fMainPad;
      TryPaint();
      UpdateAllSubPads();
      UnlockPad();
    } else {
      GetAncestor()->Paint();
    }
  }

  void Painter::SetFlag(Int_t bit, Bool_t state) {
    if (state)
      SETBIT(fDrawFlags, bit);
    else
      CLRBIT(fDrawFlags, bit);
  }

  void Painter::GenerateSubPads(Int_t x, Int_t y) {
    LockPad();
    if (fMainPad) {
      fMainPad->Divide(x, y);
      int count = 0;
      for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
          AddSubPad(fMainPad->cd(++count));
        }
      }
    }
    UnlockPad();
  }

  void Painter::UpdateAllSubPads() {
    if (fSubPads->size() == 0) {
      if (fMainPad) {
        fMainPad->Modified(true);
        fMainPad->Update();
      }
      return;
    }
    for (auto x : *fSubPads) {
      if (fPadStyle && fOwnPad) { fPadStyle->Apply(x); }
      x->Modified(1);
      x->Update();
    }
  }

  void Painter::TryPaint() {
    if (fPainted) {
      InnerRepaint();
    } else {
      fPainted = kTRUE;
      InnerPaint();
    }
    for (auto x : fSubPainters)
      x->TryPaint();
  }

  Painter::Painter(TVirtualPad* mainPad) { fMainPad = mainPad; }

  void Painter::SetOption(TString option) {
    if (Hal::Std::FindParam(option, "skip")) return;
    if (Hal::Std::FindParam(option, "default!")) {
      SetDefaultFlag();
      return;
    }
    if (Hal::Std::FindParam(option, "grid")) {
      Hal::PadStyle style;
      style.SetGridx();
      style.SetGridy();
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

  void Painter::MakePad() {
    if (gPad == nullptr) {
      fCanvas = new TCanvas();
      fOwnPad = kTRUE;
    }
    fMainPad = gPad;
    if (fMainPad->InheritsFrom("TCanvas")) { fCanvas = (TCanvas*) fMainPad; }
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

  void Painter::GotoSubPad(Int_t no) {
    if (no <= 0) {
      fMainPad->cd();
    } else {
      (*fSubPads)[no - 1]->cd();
    }
  }

  void Painter::ClearMainPad() {
    if (fMainPad) fMainPad->Clear();
    fSubPads->clear();
  }

  TVirtualPad* Painter::GetPad(Int_t index) const {
    if (index <= 0) return fMainPad;
    return (*fSubPads)[index - 1];
  }

  Painter* Painter::GetAncestor() const {
    if (HasParent()) { return GetParent(); }
    return GetAncestor();
  }

  void Painter::SetGlobalPadStyle(Hal::PadStyle& pad) { fPadStyle = new Hal::PadStyle(pad); }


} /* namespace Hal */
