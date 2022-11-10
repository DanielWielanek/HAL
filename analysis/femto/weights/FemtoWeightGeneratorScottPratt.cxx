/*
 * FemtoWeightGeneratorScottPratt.cxx
 *
 *  Created on: 30 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoWeightGeneratorScottPratt.h"

#include "FemtoPair.h"
#include "wavefunction.h"
#include <string>

namespace Hal {

  FemtoWeightGeneratorScottPratt::FemtoWeightGeneratorScottPratt(TString pairFile, Bool_t phase) {
    std::string str(pairFile);
    if (phase) {
      fWave = new ScottPratt::CWaveFunction_pp_schrod(str);
    } else {
      fWave = new ScottPratt::CWaveFunction_pp_phaseshift(str);
    }
  }

  void FemtoWeightGeneratorScottPratt::SetPairType(Femto::EPairType aPairType) {}

  Double_t FemtoWeightGeneratorScottPratt::GenerateWeight(FemtoPair* pair) {
    Double_t scale = 1000.0;
    fPa[3]         = pair->TruePx1() * scale;
    fPa[2]         = pair->TruePy1() * scale;
    fPa[1]         = pair->TruePz1() * scale;
    fPa[0]         = pair->TrueE1() * scale;
    fPb[3]         = pair->TruePx2() * scale;
    fPb[2]         = pair->TruePy2() * scale;
    fPb[1]         = pair->TruePz2() * scale;
    fPb[0]         = pair->TrueE2() * scale;

    fXa[3] = pair->GetX1();
    fXa[2] = pair->GetY1();
    fXa[1] = pair->GetZ1();
    fXa[0] = pair->GetT1();

    fXb[3] = pair->GetX2();
    fXb[2] = pair->GetY2();
    fXb[1] = pair->GetZ2();
    fXb[0] = pair->GetT2();
    return fWave->GetPsiSquared(fPa, fXa, fPb, fXb);
  }

  FemtoWeightGeneratorScottPratt::FemtoWeightGeneratorScottPratt(const FemtoWeightGeneratorScottPratt& other) {}

  FemtoWeightGeneratorScottPratt::~FemtoWeightGeneratorScottPratt() { delete fWave; }

} /* namespace Hal */
