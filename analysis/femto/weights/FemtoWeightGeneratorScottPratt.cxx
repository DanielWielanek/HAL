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
            fCurWeight = fWave[1];
          } break;
          case EWeightApprox::kPhase: {
            fCurWeight = fWavePhases[9];
          } break;
          default: {
            fCurWeight = fWaveSquares[2];
          }
        }
        break;
      }
    };
    break;
    case Femto::EPairType::kPionPlusPionMinus: {
      switch (fPref) {
        case EWeightApprox::kOther: {
          fCurWeight = fWave[2];
        } break;
        case EWeightApprox::kPhase: {
          fCurWeight = fWavePhases[8];
        } break;
        default: {
          fCurWeight = fWaveSquares[0];
        } break;
      };
      break;
      case Femto::EPairType::kKaonPlusKaonPlus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kKaonPlusKaonMinus: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonProton: {
        switch (fPref) {
          case EWeightApprox::kPhase: {
            fCurWeight = fWavePhases[6];
          } break;
          default: {
            fCurWeight = fWaveSquares[7];
          } break;
        }
      }; break;
      case Femto::EPairType::kProtonAntiproton: {
        fCurWeight = fWave[0];
      }; break;
      case Femto::EPairType::kPionPlusKaonPlus: {
        fCurWeight = fWaveSquares[3];
      }; break;
      case Femto::EPairType::kPionPlusKaonMinus: {
        switch (fPref) {
          case EWeightApprox::kPhase: {
            fCurWeight = fWavePhases[5];
          } break;
          default: {
            fCurWeight = fWaveSquares[1];
          } break;
        }
      }; break;
      case Femto::EPairType::kPionPlusProton: {
        switch (fPref) {
          case EWeightApprox::kPhase: {
            fCurWeight = fWavePhases[1];
          } break;
          default: {
            fCurWeight = fWaveSquares[5];
          } break;
        }
      }; break;
      case Femto::EPairType::kPionPlusAntiproton: {
        fCurWeight = fWaveSquares[6];
      }
    }; break;
    case Femto::EPairType::kKaonPlusProton: {
      switch (fPref) {
        case EWeightApprox::kPhase: {
          fCurWeight = fWavePhases[0];
        } break;
        default: {
          fCurWeight = fWaveSquares[4];
        } break;
      };
      break;
      case Femto::EPairType::kKaonPlusAntiproton: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kProtonLambda: {
        fCurWeight = fWavePhases[4];
      }; break;
      case Femto::EPairType::kLambdaLambda: {
        fCurWeight = fWavePhases[10];
      }; break;
      case Femto::EPairType::kKaonZeroKaonZero: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kKaonZeroKaonZeroBar: {
        fCurWeight = nullptr;
      }; break;
      case Femto::EPairType::kNeutronNeutron: {
        fCurWeight = fWavePhases[7];
      }; break;
      case Femto::EPairType::kNeutronProton: {
        fCurWeight = fWavePhases[3];
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
        fCurWeight = fWavePhases[2];
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
    fWavePhases     = new ScottPratt::CWaveFunction*[13];
    fWavePhases[0]  = new ScottPratt::CWaveFunction_pkplus_phaseshift(str);
    fWavePhases[1]  = new ScottPratt::CWaveFunction_ppiplus_phaseshift(str);
    fWavePhases[2]  = new ScottPratt::CWaveFunction_Xipi_phaseshift(str);
    fWavePhases[3]  = new ScottPratt::CWaveFunction_pn_phaseshift(str);
    fWavePhases[4]  = new ScottPratt::CWaveFunction_plambda_phaseshift(str);
    fWavePhases[5]  = new ScottPratt::CWaveFunction_kpluspiminus_phaseshift(str);
    fWavePhases[6]  = new ScottPratt::CWaveFunction_pp_phaseshift(str);
    fWavePhases[7]  = new ScottPratt::CWaveFunction_nn_phaseshift(str);
    fWavePhases[8]  = new ScottPratt::CWaveFunction_pipluspiminus_phaseshift(str);
    fWavePhases[9]  = new ScottPratt::CWaveFunction_pipluspiplus_phaseshift(str);
    fWavePhases[10] = new ScottPratt::CWaveFunction_lambdalambda_phaseshift(str);
    fWavePhases[11] = new ScottPratt::CWaveFunction_lambdalambdaantiparspin_phaseshift(str);
    fWavePhases[12] = new ScottPratt::CWaveFunction_lambdalambdaparspin_phaseshift(str);

    fWaveSquares    = new ScottPratt::CWaveFunction*[8];
    fWaveSquares[0] = new ScottPratt::CWaveFunction_pipluspiminus_sqwell(str);
    fWaveSquares[1] = new ScottPratt::CWaveFunction_kpluspiminus_sqwell(str);
    fWaveSquares[2] = new ScottPratt::CWaveFunction_pipluspiplus_sqwell(str);
    fWaveSquares[3] = new ScottPratt::CWaveFunction_kpluspiplus_sqwell(str);
    fWaveSquares[4] = new ScottPratt::CWaveFunction_pkplus_sqwell(str);
    fWaveSquares[5] = new ScottPratt::CWaveFunction_ppiplus_sqwell(str);
    fWaveSquares[6] = new ScottPratt::CWaveFunction_ppiminus_sqwell(str);
    fWaveSquares[7] = new ScottPratt::CWaveFunction_pp_schrod(str);

    fWave    = new ScottPratt::CWaveFunction*[4];
    fWave[0] = new ScottPratt::CWaveFunction_ppbar_nocoulomb(str);
    fWave[1] = new ScottPratt::CWaveFunction_pipluspiplus_nostrong(str);
    fWave[2] = new ScottPratt::CWaveFunction_pipluspiminus_nostrong(str);
    fWave[3] = new ScottPratt::CWaveFunction_ppbar_nocoulomb(str);
  }

  FemtoWeightGeneratorScottPratt::~FemtoWeightGeneratorScottPratt() {
    if (fWave) {
      for (int i = 0; i < 4; i++) {
        delete fWave[i];
      }
      delete[] fWave;
    }
    if (fWaveSquares) {
      for (int i = 0; i < 8; i++) {
        delete fWaveSquares[i];
      }
      delete[] fWaveSquares;
    }
    if (fWavePhases) {
      for (int i = 0; i < 13; i++) {
        delete fWavePhases[i];
      }
      delete[] fWavePhases;
    }
  }

} /* namespace Hal */
