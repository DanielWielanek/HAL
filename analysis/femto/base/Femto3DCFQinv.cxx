/*
 * Femto3DCFQinv.cxx
 *
 *  Created on: 19 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto3DCFQinv.h"
#include "FemtoConst.h"
#include "FemtoPair.h"
#include <TBrowser.h>
#include <TCollection.h>
#include <TH3.h>
#include <TMathBase.h>
#include <TString.h>
#include <TVirtualPad.h>

namespace Hal {
  Femto3DCFQinv::Femto3DCFQinv(TString name, Femto::EKinematics frame) : Femto3DCF(name, frame), fPureNum(NULL), fQinvNum(NULL) {
    if (GetNum()) {
      fPureNum = (TH3D*) GetNum()->Clone("qinvN");
      fQinvNum = (TH3D*) GetNum()->Clone("qinvD");
    }
  }

  Femto3DCFQinv::Femto3DCFQinv(TString name,
                               Int_t binsX,
                               Double_t minX,
                               Double_t maxX,
                               Int_t binsY,
                               Double_t minY,
                               Double_t maxY,
                               Int_t binsZ,
                               Double_t minZ,
                               Double_t maxZ,
                               Femto::EKinematics frame) :
    Femto3DCF(name, binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ, frame) {
    fPureNum = (TH3D*) GetNum()->Clone("qinvN");
    fQinvNum = (TH3D*) GetNum()->Clone("qinvD");
  }

  Femto3DCFQinv::Femto3DCFQinv(const Femto3DCFQinv& other) : Femto3DCF(other) {
    fPureNum = (TH3D*) other.fPureNum->Clone("qinvN");
    fQinvNum = (TH3D*) other.fQinvNum->Clone("qinvD");
  }

  void Femto3DCFQinv::FillNumObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    Double_t x      = pair->GetX();
    Double_t y      = pair->GetY();
    Double_t z      = pair->GetZ();
    if (pair->IsAbs()) {
      x = TMath::Abs(x);
      y = TMath::Abs(y);
      z = TMath::Abs(z);
    }
    ((TH3*) fNum)->Fill(x, y, z, pair->GetWeight());
    ((TH3*) fPureNum)->Fill(x, y, z);
    ((TH3*) fQinvNum)->Fill(x, y, z, pair->GetT());
  }

  TH3D* Femto3DCFQinv::GetQinvHist(TString opt) const {
    if (opt == "av") {
      TH3D* div = (TH3D*) fQinvNum->Clone("ratio");
      div->Divide(fPureNum);
      return div;
    }
    return NULL;
  }

  Long64_t Femto3DCFQinv::Merge(TCollection* collection) {
    if (collection) {
      Femto3DCFQinv* pack = NULL;
      TIter iterator(collection);
      while ((pack = (Femto3DCFQinv*) iterator())) {
        Add(pack);
      }
    }
    return 1;
  }

  Femto3DCFQinv& Femto3DCFQinv::operator=(const Femto3DCFQinv& other) {
    if (&other != this) {
      DividedHisto3D::operator=(other);
      if (fPureNum) delete fPureNum;
      if (fQinvNum) delete fQinvNum;
      fPureNum = (TH3D*) other.fPureNum->Clone();
      fQinvNum = (TH3D*) other.fQinvNum->Clone();
    }
    return *this;
  }

  void Femto3DCFQinv::Add(const Object* h) {
    Femto3DCF::Add(h);
    const Femto3DCFQinv* cf = dynamic_cast<const Femto3DCFQinv*>(h);
    if (cf) {
      fPureNum->Add(cf->fPureNum);
      fQinvNum->Add(cf->fQinvNum);
    }
  }

  void Femto3DCFQinv::AddNum(TH1* num, Option_t* opt) {
    Femto3DCF::AddNum(num, opt);
    if (fPureNum) {
      delete fPureNum;
      delete fQinvNum;
    }
    fPureNum = (TH3D*) GetNum()->Clone("qinvN");
    fQinvNum = (TH3D*) GetNum()->Clone("qinvD");
  }

  void Femto3DCFQinv::AddDen(TH1* den, Option_t* opt) { Femto3DCF::AddDen(den, opt); }

  void Femto3DCFQinv::Browse(TBrowser* b) {
    TVirtualPad* c1 = gPad;
    gPad->Clear();
    Draw("all");
    gPad = c1;
    b->SetDrawOption("colz");
    b->Add(fNum);
    b->Add(fDen);
    b->Add(GetQinvHist("av"));
  }

  Femto3DCFQinv::Femto3DCFQinv() : Femto3DCF(), fPureNum(nullptr), fQinvNum(nullptr) {}

  Femto3DCFQinv::~Femto3DCFQinv() {
    if (fQinvNum) {
      delete fQinvNum;
      delete fPureNum;
    }
  }
}  // namespace Hal
