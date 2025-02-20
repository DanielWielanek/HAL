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
}  // namespace Hal
