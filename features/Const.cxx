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
      if (!PDG->GetParticle(Const::DummyPID())) {
        PDG->AddParticle("DummyParticle", "DummyParticle", 0, kTRUE, 0, 1, "Ion", Const::DummyPID(), 0);
        PDG->AddParticle("Anti-DummyParticle", "Anti-DummyParticle", 0, kTRUE, 0, -1, "Ion", -Const::DummyPID(), 1);
      }
    }
  }

  TString Const::PdgToName(Int_t pid) {
    const int apdg    = std::abs(pid);
    const bool isAnti = pid < 0;

    if (apdg == PionPlusPID()) return isAnti ? "#pi^{-}" : "#pi^{+}";
    if (apdg == PionZeroPID()) return "#pi^{0}";
    if (apdg == KaonPlusPID()) return isAnti ? "K^{-}" : "K^{+}";
    if (apdg == KaonZeroPID()) return isAnti ? "#bar{K^{0}}" : "K^{0}";
    if (apdg == KaonZeroLongPID()) return "K_{L}^{0}";
    if (apdg == KaonZeroShortPID()) return "K_{S}^{0}";
    if (apdg == ProtonPID()) return isAnti ? "#bar{p}" : "p";
    if (apdg == NeutronPID()) return isAnti ? "#bar{n}" : "n";
    if (apdg == ElectronPID()) return isAnti ? "e^{+}" : "e^{-}";
    if (apdg == ElectronNeutrinoPID()) return isAnti ? "#bar{#nu_{e}}" : "#nu_{e}";
    if (apdg == MuonPID()) return isAnti ? "#mu^{+}" : "#mu^{-}";
    if (apdg == PhotonPID()) return "#gamma";
    if (apdg == DeuteronPID()) return isAnti ? "#bar{d}" : "d";
    if (apdg == TritonPID()) return isAnti ? "#bar{t}" : "t";
    if (apdg == Helium3PID()) return isAnti ? "#bar{He3}" : "He3";
    if (apdg == Helium4PID()) return isAnti ? "#bar{He4}" : "He4";
    if (apdg == HyperTritonPID()) return isAnti ? "HyperTbar" : "HyperT";
    if (apdg == HyperHydrogen4PID()) return isAnti ? "HyperH4bar" : "HyperH4";
    if (apdg == HyperHelium4PID()) return isAnti ? "HyperHe4bar" : "HyperHe4";
    if (apdg == LambdaPID()) return isAnti ? "#bar{#Lambda}" : "#Lambda";
    if (apdg == DeltaPlusPID()) return isAnti ? "#bar{Delta^{+}}" : "#Delta^{+}";
    if (apdg == DeltaPlusPlusPID()) return isAnti ? "#bar{#Delta^{++}}" : "#Delta^{++}";
    if (apdg == DeltaZeroPID()) return isAnti ? "#bar{#Delta^{0}}" : "#Delta^{0}";
    if (apdg == DeltaMinusPID()) return isAnti ? "#bar{#Delta^{-}}" : "Delta^{-}";
    if (apdg == SigmaZeroPID()) return isAnti ? "#bar{#Sigma^{0}}" : "#Sigma^{0}";
    if (apdg == SigmaPlusPID()) return isAnti ? "#bar{#Sigma^{+}}" : "#Sigma^{+}";
    if (apdg == SigmaMinusPID()) return isAnti ? "#bar{#Sigma^{-}}" : "#Sigma^{-}";
    if (apdg == XiZeroPID()) return isAnti ? "#bar{#Xi^{0}}" : "#Xi^{0}";
    if (apdg == XiMinusPID()) return isAnti ? "#bar{#Xi^{-}" : "#Xi^{-}";
    if (apdg == DummyPID()) return isAnti ? "#bar{#aleph}" : "#aleph";
    return "unknown";
  }

}  // namespace Hal
