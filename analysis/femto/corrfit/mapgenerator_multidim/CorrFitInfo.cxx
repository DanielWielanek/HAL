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

#include <iostream>

namespace Hal {
  CorrFitInfo::CorrFitInfo() : TNamed(), fCF(nullptr), fMacroText(""), fPairFile("") { SetName("Info"); }

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
  }
}  // namespace Hal
