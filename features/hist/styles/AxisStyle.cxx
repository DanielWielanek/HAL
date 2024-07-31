/*
 * AxisStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "AxisStyle.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TString.h>

#include "XMLNode.h"

namespace Hal {
  const unsigned short int AxisStyle::kTitleOffset = 0;
  const unsigned short int AxisStyle::kLabelOffset = 1;
  const unsigned short int AxisStyle::kTitleSize   = 2;
  const unsigned short int AxisStyle::kLabelSize   = 3;
  const unsigned short int AxisStyle::kTickLength  = 4;
  const unsigned short int AxisStyle::kNdivisions  = 5;
  const unsigned short int AxisStyle::kAxisColor   = 6;
  const unsigned short int AxisStyle::kLabelColor  = 7;
  const unsigned short int AxisStyle::kTitleColor  = 8;
  const unsigned short int AxisStyle::kTitleFont   = 9;
  const unsigned short int AxisStyle::kCenterTitle = 10;
  const unsigned short int AxisStyle::kRangeMin    = 11;
  const unsigned short int AxisStyle::kRangeMax    = 12;
  const unsigned short int AxisStyle::kTitle       = 13;

  void AxisStyle::SetTitleOffset(Float_t val) { SetF(kTitleOffset, val); }

  void AxisStyle::SetLabelOffset(Float_t val) { SetF(kLabelOffset, val); }

  void AxisStyle::SetTitleSize(Float_t val) { SetF(kTitleSize, val); }

  void AxisStyle::SetLabelSize(Float_t val) { SetF(kLabelSize, val); }

  void AxisStyle::SetTickLength(Float_t val) { SetF(kTickLength, val); }

  void AxisStyle::SetNdivisions(Int_t val) { SetI(kNdivisions, val); }

  void AxisStyle::SetAxisColor(Int_t val) { SetI(kAxisColor, val); }

  void AxisStyle::SetLabelColor(Int_t val) { SetI(kLabelColor, val); }

  void AxisStyle::SetTitleColor(Int_t val) { SetI(kTitleColor, val); }

  void AxisStyle::SetTitleFont(Int_t val) { SetI(kTitleFont, val); }

  void AxisStyle::SetCenterTitle(Int_t val) { SetI(kCenterTitle, val); }

  Float_t AxisStyle::GetTitleOffset() const { return GetF(kTitleOffset); }

  Float_t AxisStyle::GetLabelOffset() const { return GetF(kLabelOffset); }

  Float_t AxisStyle::GetTitleSize() const { return GetF(kTitleSize); }

  Float_t AxisStyle::GetLabelSize() const { return GetF(kLabelSize); }

  Float_t AxisStyle::GetTickLength() const { return GetF(kTickLength); }

  Int_t AxisStyle::GetNDivisions() const { return GetI(kNdivisions); }

  Int_t AxisStyle::GetAxisColor() const { return GetI(kAxisColor); }

  Int_t AxisStyle::GetLabelColor() const { return GetI(kLabelColor); }

  Int_t AxisStyle::GetTitleColor() const { return GetI(kTitleColor); }

  Int_t AxisStyle::GetTitleFont() const { return GetI(kTitleFont); }

  Int_t AxisStyle::GetCenterTitle() const { return GetI(kCenterTitle); }

  void AxisStyle::Apply(TAxis& obj) const {
    if (Find(kTitleOffset)) obj.SetTitleOffset(GetF(kTitleOffset));
    if (Find(kLabelOffset)) obj.SetLabelOffset(GetF(kLabelOffset));
    if (Find(kTitleSize)) obj.SetTitleSize(GetF(kTitleSize));
    if (Find(kLabelSize)) obj.SetLabelSize(GetF(kLabelSize));
    if (Find(kTickLength)) obj.SetTickLength(GetF(kTickLength));
    if (Find(kNdivisions)) obj.SetNdivisions(GetI(kNdivisions));
    if (Find(kAxisColor)) obj.SetAxisColor(GetI(kAxisColor));
    if (Find(kLabelColor)) obj.SetLabelColor(GetI(kLabelColor));
    if (Find(kTitleColor)) obj.SetTitleColor(GetI(kTitleColor));
    if (Find(kTitleFont)) obj.SetTitleFont(GetI(kTitleFont));
    if (Find(kCenterTitle)) obj.CenterTitle(GetI(kCenterTitle));
    if (Find(kTitle)) obj.SetTitle(fTitle);
    if (Find(kRangeMin) && Find(kRangeMin)) obj.SetRangeUser(GetF(kRangeMin), GetF(kRangeMax));
  };

  void AxisStyle::SetTitle(TString val) {
    SetI(kTitle, 0);
    fTitle = val;
  }

  void AxisStyle::ExportToXML(XMLNode* node) const {
    if (Find(kTitleOffset)) node->AddAttrib(new Hal::XMLAttrib("TitleOffset", Form("%4.4f", GetF(kTitleOffset))));
    if (Find(kLabelOffset)) node->AddAttrib(new Hal::XMLAttrib("LabelOffset", Form("%4.4f", GetF(kLabelOffset))));
    if (Find(kTitleSize)) node->AddAttrib(new Hal::XMLAttrib("TitleSize", Form("%4.4f", GetF(kTitleSize))));
    if (Find(kLabelSize)) node->AddAttrib(new Hal::XMLAttrib("LabelSize", Form("%4.4f", GetF(kLabelSize))));
    if (Find(kTickLength)) node->AddAttrib(new Hal::XMLAttrib("TickLength", Form("%4.4f", GetF(kTickLength))));
    if (Find(kNdivisions)) node->AddAttrib(new Hal::XMLAttrib("Ndivisions", Form("%i", GetI(kNdivisions))));
    if (Find(kAxisColor)) node->AddAttrib(new Hal::XMLAttrib("AxisColor", Form("%i", GetI(kAxisColor))));
    if (Find(kLabelColor)) node->AddAttrib(new Hal::XMLAttrib("LabelColor", Form("%i", GetI(kLabelColor))));
    if (Find(kTitleColor)) node->AddAttrib(new Hal::XMLAttrib("TitleColor", Form("%i", GetI(kTitleColor))));
    if (Find(kTitleFont)) node->AddAttrib(new Hal::XMLAttrib("TitleFont", Form("%i", GetI(kTitleFont))));
    if (Find(kCenterTitle)) node->AddAttrib(new Hal::XMLAttrib("CenterTitle", Form("%i", GetI(kCenterTitle))));
    if (Find(kTitle)) node->AddAttrib(new Hal::XMLAttrib("Title", fTitle));
    if (Find(kRangeMin)) node->AddAttrib(new Hal::XMLAttrib("RangeMin", Form("%4.4f", GetF(kRangeMin))));
    if (Find(kRangeMax)) node->AddAttrib(new Hal::XMLAttrib("RangeMax", Form("%4.4f", GetF(kRangeMax))));
  }

  void AxisStyle::SetRangeUser(Float_t min, Float_t max) {
    SetF(kRangeMin, min);
    SetF(kRangeMax, max);
  }

  void AxisStyle::ImportFromXML(XMLNode* node) {

    if (auto atr = node->GetAttrib("TitleOffset")) {
      float x = atr->GetValue().Atof();
      SetTitleOffset(x);
    }
    if (auto atr = node->GetAttrib("LabelOffset")) {
      float x = atr->GetValue().Atof();
      SetLabelOffset(x);
    }
    if (auto atr = node->GetAttrib("TitleSize")) {
      float x = atr->GetValue().Atof();
      SetTitleSize(x);
    }
    if (auto atr = node->GetAttrib("LabelSize")) {
      float x = atr->GetValue().Atof();
      SetLabelSize(x);
    }
    if (auto atr = node->GetAttrib("TickLength")) {
      float x = atr->GetValue().Atof();
      SetTickLength(x);
    }
    if (auto atr = node->GetAttrib("Ndivisions")) {
      int x = atr->GetValue().Atoi();
      SetNdivisions(x);
    }
    if (auto atr = node->GetAttrib("AxisColor")) {
      int x = atr->GetValue().Atoi();
      SetAxisColor(x);
    }
    if (auto atr = node->GetAttrib("LabelColor")) {
      int x = atr->GetValue().Atoi();
      SetLabelColor(x);
    }
    if (auto atr = node->GetAttrib("TitleColor")) {
      int x = atr->GetValue().Atoi();
      SetTitleColor(x);
    }
    if (auto atr = node->GetAttrib("TitleFont")) {
      int x = atr->GetValue().Atoi();
      SetTitleFont(x);
    }
    if (auto atr = node->GetAttrib("CenterTitle")) {
      int x = atr->GetValue().Atoi();
      SetCenterTitle(x);
    }

    if (node->GetAttrib("RangeMin") && node->GetAttrib("RangeMin")) {
      auto atr1  = node->GetAttrib("RangeMin");
      auto atr2  = node->GetAttrib("RangeMax");
      float mini = atr1->GetValue().Atof();
      float maxi = atr2->GetValue().Atof();
      SetRangeUser(mini, maxi);
    }

    if (auto atr = node->GetAttrib("CenterTitle")) {
      int x = atr->GetValue().Atoi();
      SetCenterTitle(x);
    }
    if (auto atr = node->GetAttrib("Title")) {
      TString x = atr->GetValue();
      SetTitle(x);
    }
  }

  AxisStyle::AxisStyle() {}

  AxisStyle::AxisStyle(Double_t titleSize, Double_t labelSize, Double_t titleOffset, Double_t labelOffset) {
    SetTitleSize(titleSize);
    SetLabelSize(labelSize);
    SetTitleOffset(titleOffset);
    SetLabelOffset(labelOffset);
  }

} /* namespace Hal */
