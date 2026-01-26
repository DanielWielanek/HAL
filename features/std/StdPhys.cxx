/*
 * StdPhys.cxx
 *
 *  Created on: 11 gru 2025
 *      Author: daniel
 */

#include "StdPhys.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>

#include "Cout.h"
#include "Std.h"

namespace Hal {
  namespace Std {
    namespace LorentzGenerator {
      TLorentzVector PtY(Double_t pt, Double_t y, Int_t pid, Double_t phi) {
        auto pdgTrack = TDatabasePDG::Instance()->GetParticle(pid);
        if (!pdgTrack) {
          Hal::Cout::PrintInfo("Hal::Std::GenerateParticleYpt (in StPhysics) cannot find pdg!", Hal::EInfo::kError);
          return TLorentzVector(0, 0, 0, 0);
        }
        double mass = pdgTrack->Mass();
        return PtYM(pt, y, mass, phi);
      }

      TLorentzVector PtYM(Double_t pt, Double_t y, Double_t m, Double_t phi) {
        if (phi == -100) { phi = gRandom->Uniform(-TMath::Pi(), TMath::Pi()); }
        double px = pt * TMath::Cos(phi);
        double py = pt * TMath::Sin(phi);
        double mT = TMath::Sqrt(m * m + pt * pt);
        double pz = mT * TMath::SinH(y);
        TLorentzVector p;
        p.SetXYZM(px, py, pz, m);
        return p;
      }

      TLorentzVector PtEta(Double_t pt, Double_t eta, Int_t pid, Double_t phi) {
        auto pdgTrack = TDatabasePDG::Instance()->GetParticle(pid);
        if (!pdgTrack) {
          Hal::Cout::PrintInfo("Hal::Std::GenerateParticleYeta (in StPhysics) cannot find pdg!", Hal::EInfo::kError);
          return TLorentzVector(0, 0, 0, 0);
        }
        double mass = pdgTrack->Mass();
        return PtEtaM(pt, eta, mass, phi);
      }

      TLorentzVector PtEtaM(Double_t pt, Double_t eta, Double_t m, Double_t phi) {
        if (phi == -100) { phi = gRandom->Uniform(-TMath::Pi(), TMath::Pi()); }
        Double_t px = pt * TMath::Cos(phi);
        Double_t py = pt * TMath::Sin(phi);
        Double_t pz = TMath::Sqrt(px * px + py * py) * TMath::SinH(eta);
        TLorentzVector p;
        p.SetXYZM(px, py, pz, m);
        return p;
      }
    }  // namespace LorentzGenerator

  }  // namespace Std
}  // namespace Hal
