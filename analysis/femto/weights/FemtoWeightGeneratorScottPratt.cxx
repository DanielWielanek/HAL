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

  FemtoWeightGeneratorScottPratt::FemtoWeightGeneratorScottPratt(TString pairFile) : fParFile(pairFile) {
    std::string str(pairFile);
    InitWeights();
  }

  void FemtoWeightGeneratorScottPratt::SetPairType(Femto::EPairType aPairType) {
    fPairType = aPairType;
    switch (fPairType) {
      case Femto::EPairType::kUnknown: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kPionPlusPionPlus: {
        switch (fPref) {
          case EWeightApprox::kOther: {
            fCurWeight = GetWave(1, EWeightApprox::kOther);
          } break;
          case EWeightApprox::kPhase: {
            fCurWeight = GetWave(9, EWeightApprox::kPhase);
          } break;
          default: {
            fCurWeight = GetWave(2, EWeightApprox::kSquare);
          } break;
        }
      } break;
      case Femto::EPairType::kPionPlusPionMinus: {
        switch (fPref) {
          case EWeightApprox::kOther: {
            fCurWeight = GetWave(2, EWeightApprox::kOther);
          } break;
          case EWeightApprox::kPhase: {
            fCurWeight = GetWave(8, EWeightApprox::kPhase);
          } break;
          default: {
            fCurWeight = GetWave(0, EWeightApprox::kSquare);
          } break;
        };
      } break;
      case Femto::EPairType::kKaonPlusKaonPlus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kKaonPlusKaonMinus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonProton: {
        switch (fPref) {
          case EWeightApprox::kPhase: {
            fCurWeight = GetWave(6, EWeightApprox::kPhase);
          } break;
          default: {
            fCurWeight = GetWave(7, EWeightApprox::kSquare);
          } break;
        }
      }; break;
      case Femto::EPairType::kProtonAntiproton: {
        fCurWeight = GetWave(0, EWeightApprox::kOther);
      }; break;
      case Femto::EPairType::kPionPlusKaonPlus: {
        fCurWeight = GetWave(3, EWeightApprox::kSquare);
      }; break;
      case Femto::EPairType::kPionPlusKaonMinus: {
        switch (fPref) {
          case EWeightApprox::kPhase: {
            fCurWeight = GetWave(5, EWeightApprox::kPhase);
          } break;
          default: {
            fCurWeight = GetWave(1, EWeightApprox::kSquare);
          } break;
        }
      }; break;
      case Femto::EPairType::kPionPlusProton: {
        switch (fPref) {
          case EWeightApprox::kPhase: {
            fCurWeight = GetWave(1, EWeightApprox::kPhase);
          } break;
          default: {
            fCurWeight = GetWave(5, EWeightApprox::kSquare);
          } break;
        }
      }; break;
      case Femto::EPairType::kPionPlusAntiproton: {
        fCurWeight = GetWave(6, EWeightApprox::kSquare);
      } break;

      case Femto::EPairType::kKaonPlusProton: {
        switch (fPref) {
          case EWeightApprox::kPhase: {
            fCurWeight = GetWave(0, EWeightApprox::kPhase);
          } break;
          default: {
            fCurWeight = GetWave(4, EWeightApprox::kSquare);
          } break;
        };
      } break;
      case Femto::EPairType::kKaonPlusAntiproton: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonLambda: {
        fCurWeight = GetWave(4, EWeightApprox::kPhase);
      }; break;
      case Femto::EPairType::kLambdaLambda: {
        fCurWeight = GetWave(10, EWeightApprox::kPhase);
      }; break;
      case Femto::EPairType::kKaonZeroKaonZero: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kKaonZeroKaonZeroBar: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kNeutronNeutron: {
        fCurWeight = GetWave(7, EWeightApprox::kPhase);
      }; break;
      case Femto::EPairType::kNeutronProton: {
        fCurWeight = GetWave(3, EWeightApprox::kPhase);
      }; break;
      case Femto::EPairType::kPionZeroPionZero: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kNeutronLambda: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonSigmaPlus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonAntiSigmaPlus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonAntiLambda: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kSigmaPlusSigmaPlus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kSigmaPlusAntiSigmaPlus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonXiZero: {
        fCurWeight = GetWave(2, EWeightApprox::kPhase);
      }; break;
      case Femto::EPairType::kNeutronXiMinus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonXIMInus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kNeutronXiZero: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonSigmaZero: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kSigmaZeroSigmaZero: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kLambdaSigmaZero: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kLambdaAntiLambda: {
        fCurWeight = nullptr;
      }; break;
    };
  }

  Hal::ScottPratt::CWaveFunction* FemtoWeightGeneratorScottPratt::InitSquareWave(Int_t n) {
    std::string str(fParFile);
    switch (n) {
      case 0: fWaveSquares[0] = new ScottPratt::CWaveFunction_pipluspiminus_sqwell(str); break;
      case 1: fWaveSquares[1] = new ScottPratt::CWaveFunction_kpluspiminus_sqwell(str); break;
      case 2: fWaveSquares[2] = new ScottPratt::CWaveFunction_pipluspiplus_sqwell(str); break;
      case 3: fWaveSquares[3] = new ScottPratt::CWaveFunction_kpluspiplus_sqwell(str); break;
      case 4: fWaveSquares[4] = new ScottPratt::CWaveFunction_pkplus_sqwell(str); break;
      case 5: fWaveSquares[5] = new ScottPratt::CWaveFunction_ppiplus_sqwell(str); break;
      case 6: fWaveSquares[6] = new ScottPratt::CWaveFunction_ppiminus_sqwell(str); break;
      case 7: fWaveSquares[7] = new ScottPratt::CWaveFunction_pp_schrod(str); break;
      default: return nullptr; break;
    }
    return fWaveSquares[n];
  }

  Hal::ScottPratt::CWaveFunction* FemtoWeightGeneratorScottPratt::InitPhaseWave(Int_t n) {
    std::string str(fParFile);
    switch (n) {
      case 0: fWavePhases[0] = new ScottPratt::CWaveFunction_pkplus_phaseshift(str); break;
      case 1: fWavePhases[1] = new ScottPratt::CWaveFunction_ppiplus_phaseshift(str); break;
      case 2: fWavePhases[2] = new ScottPratt::CWaveFunction_Xipi_phaseshift(str); break;
      case 3: fWavePhases[3] = new ScottPratt::CWaveFunction_pn_phaseshift(str); break;
      case 4: fWavePhases[4] = new ScottPratt::CWaveFunction_plambda_phaseshift(str); break;
      case 5: fWavePhases[5] = new ScottPratt::CWaveFunction_kpluspiminus_phaseshift(str); break;
      case 6: fWavePhases[6] = new ScottPratt::CWaveFunction_pp_phaseshift(str); break;
      case 7: fWavePhases[7] = new ScottPratt::CWaveFunction_nn_phaseshift(str); break;
      case 8: fWavePhases[8] = new ScottPratt::CWaveFunction_pipluspiminus_phaseshift(str); break;
      case 9: fWavePhases[9] = new ScottPratt::CWaveFunction_pipluspiplus_phaseshift(str); break;
      case 10: fWavePhases[10] = new ScottPratt::CWaveFunction_lambdalambda_phaseshift(str); break;
      case 11: fWavePhases[11] = new ScottPratt::CWaveFunction_lambdalambdaantiparspin_phaseshift(str); break;
      case 12: fWavePhases[12] = new ScottPratt::CWaveFunction_lambdalambdaparspin_phaseshift(str); break;
      default: break;
    }
    return fWavePhases[n];
  }

  Hal::ScottPratt::CWaveFunction* FemtoWeightGeneratorScottPratt::InitOtherWave(Int_t n) {
    std::string str(fParFile);
    switch (n) {
      case 0: fWave[0] = new ScottPratt::CWaveFunction_ppbar_nocoulomb(str); break;
      case 1: fWave[1] = new ScottPratt::CWaveFunction_pipluspiplus_nostrong(str); break;
      case 2: fWave[2] = new ScottPratt::CWaveFunction_pipluspiminus_nostrong(str); break;
      case 3: fWave[3] = new ScottPratt::CWaveFunction_ppbar_nocoulomb(str); break;
      default: break;
    }
    return fWave[n];
  }

  Double_t FemtoWeightGeneratorScottPratt::GenerateWeight(FemtoPair* pair) {
    Double_t scale = 1000.0;
    auto oldPair   = fPairType;
    SetPairTypeFromPair(pair);
    if (oldPair != fPairType) { SetPairType(fPairType); }
    if (!fCurWeight) return 1;
    fPa[3] = pair->TruePx1() * scale;
    fPa[2] = pair->TruePy1() * scale;
    fPa[1] = pair->TruePz1() * scale;
    fPa[0] = pair->TrueE1() * scale;
    fPb[3] = pair->TruePx2() * scale;
    fPb[2] = pair->TruePy2() * scale;
    fPb[1] = pair->TruePz2() * scale;
    fPb[0] = pair->TrueE2() * scale;

    fXa[3] = pair->GetX1();
    fXa[2] = pair->GetY1();
    fXa[1] = pair->GetZ1();
    fXa[0] = pair->GetT1();

    fXb[3] = pair->GetX2();
    fXb[2] = pair->GetY2();
    fXb[1] = pair->GetZ2();
    fXb[0] = pair->GetT2();

    return fCurWeight->GetPsiSquared(fPa, fXa, fPb, fXb);
  }

  FemtoWeightGeneratorScottPratt::FemtoWeightGeneratorScottPratt(const FemtoWeightGeneratorScottPratt& other) {
    fParFile = other.fParFile;
    fPref    = other.fPref;
    std::string str(fParFile);
    InitWeights();
  }

  void FemtoWeightGeneratorScottPratt::InitWeights() {
    std::string str(fParFile);
    fWavePhases = new ScottPratt::CWaveFunction*[13];
    for (int i = 0; i < 13; i++) {
      fWavePhases[i] = nullptr;
    }

    fWaveSquares = new ScottPratt::CWaveFunction*[8];
    for (int i = 0; i < 8; i++) {
      fWaveSquares[i] = nullptr;
    }
    fWave = new ScottPratt::CWaveFunction*[4];
    for (int i = 0; i < 4; i++) {
      fWave[i] = nullptr;
    }
  }

  Hal::ScottPratt::CWaveFunction* FemtoWeightGeneratorScottPratt::GetWave(Int_t n, EWeightApprox eWeightApprox) {
    switch (eWeightApprox) {
      case EWeightApprox::kSquare: {
        if (fWaveSquares[n]) return fWaveSquares[n];
        return InitSquareWave(n);
      } break;
      case EWeightApprox::kPhase: {
        if (fWavePhases[n]) return fWavePhases[n];
        return InitPhaseWave(n);
      } break;
      case EWeightApprox::kOther: {
        if (fWave[n]) return fWave[n];
        return InitOtherWave(n);
      } break;
    }
    return nullptr;
  }

  FemtoWeightGeneratorScottPratt::~FemtoWeightGeneratorScottPratt() {
    if (fWave) {
      for (int i = 0; i < 4; i++) {
        if (fWave[i]) delete fWave[i];
      }
      delete[] fWave;
    }
    if (fWaveSquares) {
      for (int i = 0; i < 8; i++) {
        if (fWaveSquares[i]) delete fWaveSquares[i];
      }
      delete[] fWaveSquares;
    }
    if (fWavePhases) {
      for (int i = 0; i < 13; i++) {
        if (fWavePhases[i]) delete fWavePhases[i];
      }
      delete[] fWavePhases;
    }
  }

} /* namespace Hal */
