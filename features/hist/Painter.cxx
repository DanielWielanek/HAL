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
#include <vector>

#include "Cout.h"  //KURWA
#include "PadStyle.h"
#include "StdString.h"
#include "Style.h"

#include <TCanvas.h>
#include <TROOT.h>

namespace Hal {
  const int Painter::kHtmlBit                  = 0;
  const int Painter::kPadBit                   = 1;
  const int Painter::kCanvasBit                = 2;
  const int Painter::kSameBit                  = 3;
  const int Painter::kBrowserBit               = 4;
  const int Painter::kLastBitPainter           = 4;
  Painter::commonPointers Painter::gCommonData = commonPointers();

  Painter::Painter() {
    fCommonData.fCanvases = new std::vector<TCanvas*>();
    fCommonData.fPads     = new std::vector<std::vector<TVirtualPad*>>();
    fCommonData.fParent   = this;
  }

  Painter::~Painter() {
    CleanCommonData();
    for (auto x : fSubPainters)
      delete x;
  }

  void Painter::AddPainter(Painter* painter) {
    painter->CleanCommonData();
    painter->fCommonData = fCommonData;
    Bool_t replicate     = kFALSE;
    for (auto i : fSubPainters) {
      if (i == painter) replicate = kTRUE;
    }
    if (!replicate) { fSubPainters.push_back(painter); }
  }

  void Painter::AddAsSubPainter() {
    auto parent = GetAncestor();
    if (!parent) return;
    for (auto i : parent->fSubPainters) {
      if (i == this) return;
    }
    parent->fSubPainters.push_back(this);
  }

  void Painter::Paint() {
    if (CheckOpt(kSameBit)) {
      fCommonData = gCommonData;
      auto parent = fCommonData.fParent;
      if (parent != this) { AddAsSubPainter(); }
    }
    Bool_t batch = gROOT->IsBatch();
    if (CheckOpt(kHtmlBit)) gROOT->SetBatch(kTRUE);
    auto pad = gPad;
    if (!HasParent()) {
      if (!fPainted) { MakePadsAndCanvases(); }
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
    if (pad)
      pad->cd();
    else
      gPad = nullptr;
    gCommonData = this->fCommonData;
    if (CheckOpt(kHtmlBit)) gROOT->SetBatch(batch);
  }

  void Painter::SetFlag(Int_t bit, Bool_t state) {
    if (state)
      SETBIT(fDrawFlags, bit);
    else
      CLRBIT(fDrawFlags, bit);
  }

  void Painter::InnerRepaint() { std::cout << ClassName() << " does not support repaint" << std::endl; }

  void Painter::OwnCanvasDivide(TCanvas* c, Int_t x, Int_t y, Int_t canvasNo) { c->Divide(x, y); }

  void Painter::MakeCanvasPads(Int_t x, Int_t y, Int_t canvasNo) {
    auto dividePads = [&]() {
      auto canva = GetCanvas(canvasNo);
      int count  = 0;
      if (!CheckOpt(kPadBit)) {
        OwnCanvasDivide(canva, x, y, canvasNo);
      } else {  // reuse pads
        bool cont = true;
        auto gpad = gPad;
        do {
          auto pad = canva->cd(++count);
          if (gpad == pad) {
            cont = false;
            count--;
          }
          if (count > 100) cont = false;
        } while (cont);
      }
      for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
          (*fCommonData.fPads)[canvasNo].push_back(canva->cd(++count));
          if (fPadStyle) fPadStyle->Apply(gPad);
        }
      }
    };
    LockPad();
    if (CheckOpt(kSameBit)) {
    } else if (fCommonData.fCanvases->size() > canvasNo) {
      dividePads();
    } else if (CheckOpt(kBrowserBit) && canvasNo == 0) {
      TCanvas* newCanv = dynamic_cast<TCanvas*>(gPad);
      fCommonData.fCanvases->push_back(newCanv);
      std::vector<TVirtualPad*> pads;
      pads.push_back(newCanv);
      fCommonData.fPads->push_back(pads);
      dividePads();
    } else {
      for (int i = fCommonData.fCanvases->size(); i <= canvasNo; i++) {
        TCanvas* newCanv = nullptr;
        if (CheckOpt(kPadBit) || CheckOpt(kCanvasBit)) {
          newCanv = dynamic_cast<TCanvas*>(gPad);
          if (!newCanv) {
            auto pad = gPad;
            newCanv  = pad->GetCanvas();
            gPad     = pad;
          }
        }
        if (!newCanv) newCanv = new TCanvas();
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
    if (HasParent()) return;
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
    TString styleName;
    if (Hal::Std::FindExpressionEqual(option, "style", styleName, kTRUE)) {
      fPredefinedStyle = styleName;
      fPredefinedStyle = fPredefinedStyle.ReplaceAll("&", "+");
    }
    if (fPredefinedStyle.Length()) { fPadStyle = new Hal::PadStyle(fPredefinedStyle); }
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
    Bool_t logs[] = {kFALSE, kFALSE, kFALSE};
    if (Hal::Std::FindParam(option, "logx", true)) logs[0] = kTRUE;
    if (Hal::Std::FindParam(option, "logy", true)) logs[1] = kTRUE;
    if (Hal::Std::FindParam(option, "logy", true)) logs[2] = kTRUE;
    for (int i = 0; i < 3; i++) {
      if (logs[i] == kFALSE) continue;
      if (!fPadStyle) {
        Hal::PadStyle style;
        SetGlobalPadStyle(style);
      }
      if (i == 0) fPadStyle->SetLogx(1);
      if (i == 1) fPadStyle->SetLogy(1);
      if (i == 2) fPadStyle->SetLogz(1);
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
    if (Hal::Std::FindParam(option, "canvas", kTRUE)) {
      fOptionsChanged = kTRUE;
      SETBIT(defFlags, kCanvasBit);
      CLRBIT(defFlags, kPadBit);
      CLRBIT(defFlags, kSameBit);
    }
    auto ranges = Hal::Std::FindBrackets(option, kTRUE, kTRUE);
    for (auto range : ranges) {
      std::vector<double> res;
      auto foundx = GetPatterns(range, "margin", res);
      if (res.size() == 4 && foundx)
        if (!fPadStyle) {
          Hal::PadStyle style(res[0], res[1], res[2], res[3]);
          SetGlobalPadStyle(style);
        } else {
          fPadStyle->SetLeftMargin(res[0]);
          fPadStyle->SetBottomMargin(res[1]);
          fPadStyle->SetRightMargin(res[2]);
          fPadStyle->SetTopMargin(res[3]);
        }
    }
    if (Hal::Std::FindParam(option, "pad", kTRUE)) {
      fOptionsChanged = kTRUE;
      SETBIT(defFlags, kPadBit);
      CLRBIT(defFlags, kCanvasBit);
      CLRBIT(defFlags, kSameBit);
    }
    if (Hal::Std::FindParam(option, "same", kTRUE)) {
      fOptionsChanged = kTRUE;
      CLRBIT(defFlags, kPadBit);
      CLRBIT(defFlags, kCanvasBit);
      SETBIT(defFlags, kSameBit);
    }
    if (Hal::Std::FindParam(option, "browser", kTRUE)) {
      CLRBIT(defFlags, kPadBit);
      CLRBIT(defFlags, kCanvasBit);
      CLRBIT(defFlags, kSameBit);
      CLRBIT(defFlags, kHtmlBit);
      SETBIT(defFlags, kBrowserBit);
    }

    auto newOpt = SetOptionInternal(option, defFlags);
    if (newOpt != fDrawFlags) {
      fOptionsChanged = kTRUE;
      fDrawFlags      = newOpt;
    }
  }

  Bool_t Painter::HasParent() const {
    if (fCommonData.fParent != this) return kTRUE;
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

  Painter* Painter::GetAncestor() {
    if (HasParent()) { return GetParent()->GetAncestor(); }
    return this;
  }

  void Painter::SetGlobalPadStyle(Hal::PadStyle& pad) { fPadStyle = new Hal::PadStyle(pad); }

  Bool_t Painter::CanvasExist(Int_t canvasNo) const {
    if (fCommonData.fCanvases->size() > canvasNo) return kTRUE;
    return kFALSE;
  }

  void Painter::CleanCommonData() {
    if (HasParent()) return;
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

  void Painter::ResetFewBits(ULong64_t& flag, std::initializer_list<Int_t> bits, Int_t set) const {
    for (auto ibit : bits)
      CLRBIT(flag, ibit);
    if (set >= 0) SETBIT(flag, set);
  }

  void SimplePainter::MakePadsAndCanvases() { MakeCanvasPads(1, 1, 0); }

  void Painter::ApplyGlobalPadStyle() const {
    if (gPad && fPadStyle) fPadStyle->Apply(gPad);
  }

} /* namespace Hal */
