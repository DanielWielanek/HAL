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
      TLorentzVector YPt(Double_t pt, Double_t y, Int_t pid, Double_t phi) {
        if (phi == -100) { phi = gRandom->Uniform(-TMath::Pi(), TMath::Pi()); }
        double px     = pt * TMath::Cos(phi);
        double py     = pt * TMath::Sin(phi);
        auto pdgTrack = TDatabasePDG::Instance()->GetParticle(pid);
        if (!pdgTrack) {
          Hal::Cout::PrintInfo("Hal::Std::GenerateParticleYpt (in StPhysics) cannot find pdg!", Hal::EInfo::kError);
          return TLorentzVector(0, 0, 0, 0);
        }
        double mass = pdgTrack->Mass();
        double mT   = TMath::Sqrt(mass * mass + pt * pt);
        double pz   = mT * TMath::SinH(y);
        TLorentzVector p;
        p.SetXYZM(px, py, pz, mass);
        return p;
      }
    }  // namespace LorentzGenerator

  }  // namespace Std
}  // namespace Hal
