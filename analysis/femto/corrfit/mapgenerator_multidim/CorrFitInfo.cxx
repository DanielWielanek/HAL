/*
 * CorrFitInfo.cxx
 *
 *  Created on: 10 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitInfo.h"

#include "Cout.h"

#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"

#include <iostream>

namespace Hal {
  CorrFitInfo::CorrFitInfo() : TNamed(), fCF(nullptr), fMacroText(""), fPairFile(""), fVertical(kFALSE) { SetName("Info"); }

  CorrFitInfo::~CorrFitInfo() {
    if (fCF) delete fCF;
  }

  void CorrFitInfo::Print(Option_t* /*option*/) const {
    Cout::InStars("CorrFitInfo", kWhite);
    fCF->Print();
    fSetup.Print("");
    Cout::Text(Form("Pair File : %s", fPairFile.Data()), "R", kWhite);
    Cout::InStars("Macro text", kWhite);
    std::cout << fMacroText << std::endl;
  }

  CorrFitInfo::CorrFitInfo(const CorrFitInfo& other) : TNamed() {
    SetName(other.GetName());
    fCF        = other.fCF->Clone();
    fMacroText = other.fMacroText;
    fPairFile  = other.fMacroText;
    fSetup     = other.fSetup;
    fVertical  = other.fVertical;
  }

  Int_t CorrFitInfo::GetMainBins() const {
    if (!fCF) return 0;
    if (dynamic_cast<Hal::FemtoSHCF*>(fCF)) { return ((Hal::FemtoSHCF*) fCF)->GetNum()->GetNbinsX(); }
    if (dynamic_cast<Hal::Femto1DCF*>(fCF)) { return ((Hal::Femto1DCF*) fCF)->GetNum()->GetNbinsX(); }
    if (dynamic_cast<Hal::Femto3DCF*>(fCF)) { return ((Hal::Femto3DCF*) fCF)->GetNum()->GetNbinsZ(); }
    return 0;
  }

}  // namespace Hal
