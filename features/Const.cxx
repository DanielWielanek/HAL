/*
 * HalConst.cxx
 *
 *  Created on: 1 lip 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Const.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>

namespace Hal {
  Const::Const() {}

  Const::~Const() {}

  void Const::ExtendPdgDatabase(TString inFile) {
    if (inFile.Length() == 0) {
      auto PDG = TDatabasePDG::Instance();
      if (!PDG->GetParticle(Const::DeuteronPID())) {
        PDG->AddParticle("Deuteron", "Deuteron", Const::DeuteronMass(), kTRUE, 0, 1, "Ion", Const::DeuteronPID(), 0);
        PDG->AddParticle("Anti-Deuteron", "Anti-Deuteron", Const::DeuteronMass(), kTRUE, 0, -1, "Ion", -Const::DeuteronPID(), 1);
      }
    }
  }

  TString Const::PdgToName(Int_t pid) {
    const int apdg    = std::abs(pid);
    const bool isAnti = pid < 0;

    if (apdg == PionPlusPID()) return isAnti ? "pion-" : "pion+";
    if (apdg == PionZeroPID()) return "pion0";
    if (apdg == KaonPlusPID()) return isAnti ? "K-" : "K+";
    if (apdg == KaonZeroPID()) return isAnti ? "K0bar" : "K0";
    if (apdg == KaonZeroLongPID()) return "K_L0";
    if (apdg == KaonZeroShortPID()) return "K_S0";
    if (apdg == ProtonPID()) return isAnti ? "pbar" : "p";
    if (apdg == NeutronPID()) return isAnti ? "nbar" : "n";
    if (apdg == ElectronPID()) return isAnti ? "e+" : "e-";
    if (apdg == ElectronNeutrinoPID()) return isAnti ? "anti-nu_e" : "nu_e";
    if (apdg == MuonPID()) return isAnti ? "mu+" : "mu-";
    if (apdg == PhotonPID()) return "gamma";
    if (apdg == DeuteronPID()) return isAnti ? "dbar" : "d";
    if (apdg == TritonPID()) return isAnti ? "tbar" : "t";
    if (apdg == Helium3PID()) return isAnti ? "He3bar" : "He3";
    if (apdg == Helium4PID()) return isAnti ? "He4bar" : "He4";
    if (apdg == HyperTritonPID()) return isAnti ? "HyperTbar" : "HyperT";
    if (apdg == HyperHydrogen4PID()) return isAnti ? "HyperH4bar" : "HyperH4";
    if (apdg == HyperHelium4PID()) return isAnti ? "HyperHe4bar" : "HyperHe4";
    if (apdg == LambdaPID()) return isAnti ? "Lambdabar" : "Lambda";
    if (apdg == DeltaPlusPID()) return isAnti ? "Delta+_bar" : "Delta+";
    if (apdg == DeltaPlusPlusPID()) return isAnti ? "Delta++_bar" : "Delta++";
    if (apdg == DeltaZeroPID()) return isAnti ? "Delta0_bar" : "Delta0";
    if (apdg == DeltaMinusPID()) return isAnti ? "Delta-_bar" : "Delta-";
    if (apdg == SigmaZeroPID()) return isAnti ? "Sig0bar" : "Sig0";
    if (apdg == SigmaPlusPID()) return isAnti ? "Sig+bar" : "Sig+";
    if (apdg == SigmaMinusPID()) return isAnti ? "Sig-bar" : "Sig-";
    if (apdg == XiZeroPID()) return isAnti ? "Xi0bar" : "Xi0";
    if (apdg == XiMinusPID()) return isAnti ? "Xi-bar" : "Xi-";

    return "unknown";
  }

}  // namespace Hal
