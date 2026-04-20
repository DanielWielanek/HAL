/*
 * LorentzSmearing.cxx
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#include "LorentzSmearing.h"

#include <TAxis.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>

#include "Cout.h"
#include "StdHist.h"

namespace Hal {

  //=============================== LorentzSmearing

  LorentzSmearing::LorentzSmearing(Int_t pid) : fPid(pid) {
    auto db = TDatabasePDG::Instance();
    auto tr = db->GetParticle(fPid);
    if (tr) { fMass = tr->Mass(); }
  }

  LorentzSmearing::LorentzSmearing(const LorentzSmearing& other) : fPid(other.fPid), fMass(other.fMass), fKeepE(other.fKeepE) {}

  void LorentzSmearing::SetPid(Int_t pid) {
    auto db = TDatabasePDG::Instance();
    auto tr = db->GetParticle(fPid);
    if (tr) { fMass = tr->Mass(); }
  }


  Bool_t LorentzSmearing::Init() { return true; }

  //================================ LorentzSmearingTF1

  LorentzSmearingTF1::LorentzSmearingTF1(const LorentzSmearingTF1& other) : LorentzSmearing(other) {
    if (other.fFuncP) { LorentzSmearingTF1::SetResolution(*other.fFuncP, *other.fFuncPhi, *other.fFuncTheta); }
  }


  void LorentzSmearingTF1::SetResolution(const TF1& p, const TF1& phi, const TF1& theta) {
    if (fFuncP) {
      delete fFuncP;
      delete fFuncPhi;
      delete fFuncTheta;
    }
    fFuncP     = (TF1*) p.Clone();
    fFuncPhi   = (TF1*) phi.Clone();
    fFuncTheta = (TF1*) theta.Clone();
  }

  void LorentzSmearingTF1::Smear(TLorentzVector& smeared, const TLorentzVector& unsmeared) const {
    Double_t p         = unsmeared.P();
    Double_t phi       = unsmeared.Phi();
    Double_t theta     = unsmeared.Theta();
    Double_t p_sim     = p + fScale * gRandom->Gaus(0, fFuncP->Eval(p));
    Double_t phi_sim   = phi + fScale * gRandom->Gaus(0, fFuncPhi->Eval(p));
    Double_t theta_sim = theta + fScale * gRandom->Gaus(0, fFuncTheta->Eval(p));
    double pt_sim      = p_sim * TMath::Sin(theta_sim);
    double px_sim      = pt_sim * TMath::Cos(phi_sim);
    double py_sim      = pt_sim * TMath::Sin(phi_sim);
    double pz_sim      = p_sim * TMath::Cos(theta_sim);
    if (fKeepE) {
      smeared.SetX(px_sim);
      smeared.SetY(py_sim);
      smeared.SetZ(pz_sim);
    } else {
      smeared.SetXYZM(px_sim, py_sim, pz_sim, fMass);
    }
  }

  Bool_t LorentzSmearingTF1::Init() {
    if (!fFuncP) return false;
    return LorentzSmearing::Init();
  }

  LorentzSmearingTF1::~LorentzSmearingTF1() {
    if (fFuncP) delete fFuncP;
    if (fFuncPhi) delete fFuncPhi;
    if (fFuncTheta) delete fFuncTheta;
  }

  //==================================== LorentzSmearingTH2

  LorentzSmearingTH2::LorentzSmearingTH2(const LorentzSmearingTH2& other) : LorentzSmearing(other) {
    if (other.fResoP2d) { LorentzSmearingTH2::SetResolution(*other.fResoP2d, *other.fResoPhi2d, *other.fResoTheta2d); }
  }

  void LorentzSmearingTH2::SetResolution(const TH2D& p, const TH2D& phi, const TH2D& theta) {
    if (fResoP2d) delete fResoP2d;
    if (fResoPhi2d) delete fResoPhi2d;
    if (fResoTheta2d) delete fResoTheta2d;
    fResoP2d     = (TH2D*) p.Clone();
    fResoPhi2d   = (TH2D*) phi.Clone();
    fResoTheta2d = (TH2D*) theta.Clone();
  }

  void LorentzSmearingTH2::Smear(TLorentzVector& smeared, const TLorentzVector& unsmeared) const {
    Double_t p         = unsmeared.P();
    Double_t phi       = unsmeared.Phi();
    Double_t theta     = unsmeared.Theta();
    Int_t pBin         = fResoP2d->GetXaxis()->FindBin(p);
    Double_t p_sim     = p + fScale * fResoP[pBin]->GetRandom();
    Double_t phi_sim   = phi + fScale * fResoPhi[pBin]->GetRandom();
    Double_t theta_sim = theta + fScale * fResoTheta[pBin]->GetRandom();
    double pt_sim      = p_sim * TMath::Sin(theta_sim);
    double px_sim      = pt_sim * TMath::Cos(phi_sim);
    double py_sim      = pt_sim * TMath::Sin(phi_sim);
    double pz_sim      = p_sim * TMath::Cos(theta_sim);
    if (fKeepE) {
      smeared.SetX(px_sim);
      smeared.SetY(py_sim);
      smeared.SetZ(pz_sim);
    } else {
      smeared.SetXYZM(px_sim, py_sim, pz_sim, fMass);
    }
  }

  Bool_t LorentzSmearingTH2::Init() {
    if (!fResoP2d) return false;

    for (int i = 0; i <= fResoP2d->GetNbinsX() + 1; i++) {
      auto pp     = Hal::Std::GetProjection1D(fResoP2d, i, i, "bins+y");
      auto pphi   = Hal::Std::GetProjection1D(fResoPhi2d, i, i, "bins+y");
      auto ptheta = Hal::Std::GetProjection1D(fResoTheta2d, i, i, "bins+y");
      fResoP.push_back(pp);
      fResoPhi.push_back(pphi);
      fResoTheta.push_back(ptheta);
    }
    CheckVector(fResoP, "P");
    CheckVector(fResoPhi, "Phi");
    CheckVector(fResoTheta, "Theta");
    return LorentzSmearing::Init();
  }

  void LorentzSmearingTH2::CheckVector(std::vector<TH1D*> vec, TString name) {
    const int n = vec.size();
    std::vector<int> flags(n, -2);
    auto NotEmpty = [](TH1D* h) {
      if (h->GetEffectiveEntries() == 0) return false;
      if (h->GetEntries() == 0) return false;
      return true;
    };
    for (int i = n - 1; i >= 0; i--) {
      auto h = vec[i];
      if (NotEmpty(h)) continue;
      int left  = -1;
      int right = -1;
      int best  = -1;
      for (int j = i - 1; j >= 0; --j) {
        if (NotEmpty(vec[j])) {
          left = j;
          break;
        }
      }
      for (int j = i + 1; j < n; ++j) {
        if (NotEmpty(vec[j])) {
          right = j;
          break;
        }
      }
      if (left == -1 && right == -1) {
        best = -1;
      } else if (left == -1) {
        best = right;
      } else if (right == -1) {
        best = left;
      } else {
        if ((i - left) <= (right - i))
          best = left;
        else
          best = right;
      }
      flags[i] = best;
    }
    for (int i = 0; i < flags.size(); i++) {
      if (flags[i] != -2) {
        Hal::Cout::PrintInfo(Form("%s - detected empty bins %s", ClassName(), name.Data()), Hal::EInfo::kLowWarning);
        break;
      }
    }
    for (int i = 0; i < n; i++) {
      if (flags[i] == -1) {
        vec[i]->Fill(0);
      } else if (flags[i] >= 0) {
        vec[i]->Add(vec[flags[i]]);
      }
    }
    for (int i = 0; i < flags.size(); i++) {
      double mom = fResoP2d->GetXaxis()->GetBinCenter(i);
      if (flags[i] == -1) {
        Hal::Cout::PrintInfo(Form("Empty bin p = %4.4f filled with 0", mom), Hal::EInfo::kDebugInfo);
      } else if (flags[i] >= 0) {
        int idx = i;
        while (flags[idx] >= 0) {
          idx = flags[idx];
          if (flags[idx] == -2) break;
        }
        double mom2 = fResoP2d->GetXaxis()->GetBinCenter(idx);
        Hal::Cout::PrintInfo(Form("Empty bin p = %4.4f filled with resolution from %4.4f", mom, mom2), Hal::EInfo::kDebugInfo);
      }
    }
  }

  LorentzSmearingTH2::~LorentzSmearingTH2() {
    if (fResoP2d) delete fResoP2d;
    if (fResoPhi2d) delete fResoPhi2d;
    if (fResoTheta2d) delete fResoTheta2d;
    for (auto i : fResoP)
      delete i;
    for (auto i : fResoPhi)
      delete i;
    for (auto i : fResoTheta)
      delete i;
  }
  //================================

  LorentzSmearingPercent::LorentzSmearingPercent(const LorentzSmearingPercent& other) : LorentzSmearing(other) {
    fPerc = other.fPerc;
  }

  void LorentzSmearingPercent::Smear(TLorentzVector& smeared, const TLorentzVector& unsmeared) const {
    double px     = unsmeared.X();
    double py     = unsmeared.Y();
    double pz     = unsmeared.Z();
    double px_sim = gRandom->Gaus(px, fScale * px * fPerc);
    double py_sim = gRandom->Gaus(py, fScale * py * fPerc);
    double pz_sim = gRandom->Gaus(pz, fScale * pz * fPerc);
    if (fKeepE) {
      smeared.SetX(px_sim);
      smeared.SetY(py_sim);
      smeared.SetZ(pz_sim);
    } else {
      smeared.SetXYZM(px_sim, py_sim, pz_sim, fMass);
    }
  }

  Bool_t LorentzSmearingPercent::Init() { return kTRUE; }

} /* namespace Hal */
