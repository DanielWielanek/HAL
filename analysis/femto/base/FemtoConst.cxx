/*
 * FemtoConst.h
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoConst.h"
#include "Const.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoDPhiDEta.h"
#include "FemtoPairKinematics.h"
#include "FemtoSHCF.h"
#include "FemtoWeightGenerator.h"
#include "FemtoWeightGeneratorLednicky.h"
#include "StdString.h"
#include "XMLNode.h"

#include <TClass.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TRandom.h>
#include <iostream>

namespace Hal {
  namespace Femto {
    Bool_t IsPairIdentical(EPairType pt) {
      switch (pt) {
        case EPairType::kPionPlusPionPlus: {
          return kTRUE;
        } break;
        case EPairType::kPionZeroPionZero: {
          return kTRUE;
        } break;
        case EPairType::kKaonZeroKaonZero: {
          return kTRUE;
        } break;
        case EPairType::kPionPlusKaonPlus: {
          return kTRUE;
        } break;
        case EPairType::kProtonProton: {
          return kTRUE;
        } break;
        case EPairType::kNeutronNeutron: {
          return kTRUE;
        } break;
        case EPairType::kLambdaLambda: {
          return kTRUE;
        } break;
        case EPairType::kSigmaPlusSigmaPlus: {
          return kTRUE;
        } break;
        case EPairType::kSigmaZeroSigmaZero: {
          return kTRUE;
        } break;
        default: return kFALSE; break;
      }
    }

    std::pair<Int_t, Int_t> PairTypeToPid(EPairType pt) {
      std::pair<Int_t, Int_t> pair;
      switch (pt) {
        case EPairType::kUnknown: {
        } break;
        case EPairType::kPionPlusPionPlus: {
          pair.first  = Const::PionPlusPID();
          pair.second = Const::PionPlusPID();
        } break;
        case EPairType::kPionPlusPionMinus: {
          pair.first  = Const::PionPlusPID();
          pair.second = -Const::PionPlusPID();
        } break;
        case EPairType::kKaonPlusKaonPlus: {
          pair.first  = Const::KaonPlusPID();
          pair.second = Const::KaonPlusPID();
        } break;
        case EPairType::kKaonPlusKaonMinus: {
          pair.first  = Const::KaonPlusPID();
          pair.second = -Const::KaonPlusPID();
        } break;

        case EPairType::kProtonProton: {
          pair.first  = Const::ProtonPID();
          pair.second = Const::ProtonPID();
        } break;
        case EPairType::kProtonAntiproton: {
          pair.first  = Const::ProtonPID();
          pair.second = -Const::ProtonPID();
        } break;
        case EPairType::kPionPlusKaonPlus: {
          pair.first  = Const::PionPlusPID();
          pair.second = Const::KaonPlusPID();
        } break;
        case EPairType::kPionPlusKaonMinus: {
          pair.first  = Const::PionPlusPID();
          pair.second = -Const::KaonPlusPID();
        } break;

        case EPairType::kPionPlusProton: {
          pair.first  = Const::PionPlusPID();
          pair.second = Const::ProtonPID();
        } break;
        case EPairType::kPionPlusAntiproton: {
          pair.first  = Const::PionPlusPID();
          pair.second = -Const::ProtonPID();
        } break;
        case EPairType::kKaonPlusProton: {
          pair.first  = Const::KaonPlusPID();
          pair.second = Const::ProtonPID();
        } break;
        case EPairType::kKaonPlusAntiproton: {
          pair.first  = Const::KaonPlusPID();
          pair.second = -Const::ProtonPID();
        } break;

        case EPairType::kProtonLambda: {
          pair.first  = Const::ProtonPID();
          pair.second = Const::LambdaPID();
        } break;
        case EPairType::kLambdaLambda: {
          pair.first  = Const::LambdaPID();
          pair.second = Const::LambdaPID();
        } break;
        case EPairType::kKaonZeroKaonZero: {
          pair.first  = 310;
          pair.second = 310;
        } break;
        case EPairType::kKaonZeroKaonZeroBar: {
          pair.first  = 310;
          pair.second = -310;
        } break;


        case EPairType::kNeutronNeutron: {
          pair.first  = Const::NeutronPID();
          pair.second = Const::NeutronPID();
        } break;
        case EPairType::kNeutronProton: {
          pair.first  = Const::NeutronPID();
          pair.second = Const::ProtonPID();
        } break;
        case EPairType::kPionZeroPionZero: {
          pair.first  = Const::PionZeroPID();
          pair.second = Const::PionZeroPID();
        } break;
        case EPairType::kNeutronLambda: {
          pair.first  = Const::NeutronPID();
          pair.second = Const::LambdaPID();
        } break;
        //
        case EPairType::kProtonSigmaPlus: {
          pair.first  = Const::ProtonPID();
          pair.second = Const::SigmaPlusPID();
        } break;
        case EPairType::kProtonAntiSigmaPlus: {
          pair.first  = Const::NeutronPID();
          pair.second = -Const::SigmaPlusPID();
        } break;
        case EPairType::kProtonAntiLambda: {
          pair.first  = Const::ProtonPID();
          pair.second = -Const::LambdaPID();
        } break;
        case EPairType::kSigmaPlusSigmaPlus: {
          pair.first  = Const::SigmaPlusPID();
          pair.second = Const::SigmaPlusPID();
        } break;
        case EPairType::kSigmaPlusAntiSigmaPlus: {
          pair.first  = Const::SigmaPlusPID();
          pair.second = -Const::SigmaPlusPID();
        } break;
        case EPairType::kProtonXiZero: {
          pair.first  = Const::ProtonPID();
          pair.second = Const::XiZeroPID();
        } break;
        case EPairType::kNeutronXiMinus: {
          pair.first  = Const::NeutronPID();
          pair.second = Const::XiMinusPID();
        } break;
        case EPairType::kProtonXIMInus: {
          pair.first  = Const::ProtonPID();
          pair.second = Const::XiMinusPID();
        } break;

        case EPairType::kNeutronXiZero: {
          pair.first  = Const::NeutronPID();
          pair.second = Const::XiZeroPID();
        } break;
        case EPairType::kProtonSigmaZero: {
          pair.first  = Const::ProtonPID();
          pair.second = Const::SigmaZeroPID();
        } break;
        case EPairType::kSigmaZeroSigmaZero: {
          pair.first  = Const::SigmaZeroPID();
          pair.second = Const::SigmaZeroPID();
        } break;
        case EPairType::kLambdaSigmaZero: {
          pair.first  = Const::LambdaPID();
          pair.second = Const::SigmaZeroPID();
        } break;
        case EPairType::kLambdaAntiLambda: {
          pair.first  = Const::LambdaPID();
          pair.second = -Const::LambdaPID();
        } break;
      }
      return pair;
    }  // namespace Femto

    EPairType PidToPairType(Int_t pid1, Int_t pid2) {
      if (pid1 < 0) {
        pid1 = -pid1;
        pid2 = -pid2;
      }
      switch (pid1) {
        case 211: {  // pion
          switch (pid2) {
            case 211: return EPairType::kPionPlusPionPlus; break;
            case -211: return EPairType::kPionPlusPionMinus; break;
            case 321: return EPairType::kPionPlusKaonPlus; break;
            case -321: return EPairType::kPionPlusKaonMinus; break;
            case 2212: return EPairType::kPionPlusProton; break;
            case -2212: return EPairType::kPionPlusAntiproton; break;
          }
        } break;
        case 321: {  // kaon
          switch (pid2) {
            case 321: return EPairType::kKaonPlusKaonPlus; break;
            case -321: return EPairType::kKaonPlusKaonMinus; break;
            case 2212: return EPairType::kKaonPlusProton; break;
            case -2212: return EPairType::kKaonPlusAntiproton; break;
          }
        } break;

        case 2212: {  // proton
          switch (pid2) {
            case 2212: return EPairType::kProtonProton; break;
            case -2212: return EPairType::kProtonAntiproton; break;
            case 3122: return EPairType::kProtonLambda; break;
            case 2112: return EPairType::kNeutronProton; break;
            case 3222: return EPairType::kProtonSigmaPlus; break;
            case -3222: return EPairType::kProtonAntiSigmaPlus; break;
            case -3122: return EPairType::kProtonAntiLambda; break;
            case 3322: return EPairType::kProtonXiZero; break;
            case 3312: return EPairType::kProtonXIMInus; break;
            case 3212: return EPairType::kProtonSigmaZero; break;
          }
        } break;
        case 2112: {  // neutron
          switch (pid2) {
            case 2112: return EPairType::kNeutronNeutron; break;
            case 3122: return EPairType::kNeutronLambda; break;
            case 2212: return EPairType::kNeutronProton; break;  // why?
            case 3312: return EPairType::kNeutronXiMinus; break;
            case 3322: return EPairType::kNeutronXiZero; break;
          }
        } break;
        case 3122: {
          switch (pid2) {
            case 3122: return EPairType::kLambdaLambda; break;
            case -3122: return EPairType::kLambdaAntiLambda; break;
            case 3212: return EPairType::kLambdaSigmaZero; break;
          }
        } break;
        case 310: {
          switch (pid2) {
            case 310: return EPairType::kKaonZeroKaonZero; break;
            case -310: return EPairType::kKaonZeroKaonZeroBar; break;
          }
          break;
        }
        case 111: {
          if (pid2 == 111) return EPairType::kPionZeroPionZero;
          break;
        } break;
        case 3222: {
          switch (pid2) {
            case -3222: {
              return EPairType::kSigmaPlusAntiSigmaPlus;
            } break;
            case 3222: {
              return EPairType::kSigmaPlusSigmaPlus;
            } break;
          }
        } break;
        case 3212: {
          if (pid2 == 3212) return EPairType::kSigmaZeroSigmaZero;
        } break;
      }
      return EPairType::kUnknown;
    }

    TString PairTypeToString(EPairType type) {
      switch (type) {
        case EPairType::kUnknown: {
        } break;
        case EPairType::kPionPlusPionPlus: {
          return "pi+pi+";
        } break;
        case EPairType::kPionPlusPionMinus: {
          return "pi+pi-";
        } break;
        case EPairType::kKaonPlusKaonPlus: {
          return "K+K+";
        } break;
        case EPairType::kKaonPlusKaonMinus: {
          return "K+K-";
        } break;

        case EPairType::kProtonProton: {
          return "pp";
        } break;
        case EPairType::kProtonAntiproton: {
          return "ppb";
        } break;
        case EPairType::kPionPlusKaonPlus: {
          return "pi+K+";
        } break;
        case EPairType::kPionPlusKaonMinus: {
          return "pi+K-";
        } break;

        case EPairType::kPionPlusProton: {
          return "pi+p";
        } break;
        case EPairType::kPionPlusAntiproton: {
          return "pi+pb";
        } break;
        case EPairType::kKaonPlusProton: {
          return "K+p";
        } break;
        case EPairType::kKaonPlusAntiproton: {
          return "K+pb";
        } break;

        case EPairType::kProtonLambda: {
          return "pL";
        } break;
        case EPairType::kLambdaLambda: {
          return "LL";
        } break;
        case EPairType::kKaonZeroKaonZero: {
          return "K0K0";
        } break;
        case EPairType::kKaonZeroKaonZeroBar: {
          return "K0K0b";
        } break;
        case EPairType::kNeutronNeutron: {
          return "nn";
        } break;
        case EPairType::kNeutronProton: {
          return "np";
        } break;
        case EPairType::kPionZeroPionZero: {
          return "pi0pi0";
        } break;
        case EPairType::kNeutronLambda: {
          return "nLam";
        } break;
        case EPairType::kProtonSigmaPlus: {
          return "pSig+";
        } break;
        case EPairType::kProtonAntiSigmaPlus: {
          return "pAntiSig+";
        } break;
        case EPairType::kProtonAntiLambda: {
          return "pLb";
        } break;
        case EPairType::kSigmaPlusSigmaPlus: {
          return "Sig+Sig+";
        } break;
        case EPairType::kSigmaPlusAntiSigmaPlus: {
          return "SIg+AntiSig+";
        } break;
        case EPairType::kProtonXiZero: {
          return "pXi0";
        } break;
        case EPairType::kNeutronXiMinus: {
          return "nXi-";
        } break;
        case EPairType::kProtonXIMInus: {
          return "pXi-";
        } break;

        case EPairType::kNeutronXiZero: {
          return "nXi0";
        } break;
        case EPairType::kProtonSigmaZero: {
          return "pSig0";
        } break;
        case EPairType::kSigmaZeroSigmaZero: {
          return "Sig0Sig0";
        } break;
        case EPairType::kLambdaSigmaZero: {
          return "LSig0";
        } break;
        case EPairType::kLambdaAntiLambda: {
          return "LLb";
        } break;
      }
      return "uknonwn";
    }

    FemtoPair* MakePair(EKinematics frame, Bool_t use_fake) {
      switch (frame) {
        case EKinematics::kPRF: return new FemtoPairPRF(use_fake); break;
        case EKinematics::kLCMS: return new FemtoPairLCMS(use_fake); break;
        case EKinematics::kSH_LCMS: return new FemtoPairLCMS_SH(use_fake); break;
        case EKinematics::kSH_PRF: return new FemtoPairPRF_SH(use_fake); break;
        case EKinematics::kPHIETA: return new FemtoPairDPhiDEta(use_fake); break;
        case EKinematics::kPRFL: return new FemtoPairPRFL(use_fake); break;
        default: return NULL; break;
      }
    }

    TString KinematicsToLabel(EKinematics kin) {
      switch (kin) {
        case EKinematics::kPRF: return "prf"; break;
        case EKinematics::kLCMS: return "lcms"; break;
        case EKinematics::kSH_LCMS: return "sh_lcms"; break;
        case EKinematics::kSH_PRF: return "sh_prf"; break;
        case EKinematics::kPHIETA: return "phieta"; break;
        case EKinematics::kPRFL: return "prfl"; break;
        default: return ""; break;
      }
    }

    EKinematics LabelToKinematics(TString label) {
      if (label.EqualTo("prf")) {
        return EKinematics::kPRF;
      } else if (label.EqualTo("lcms")) {
        return EKinematics::kLCMS;
      } else if (label.EqualTo("sh_prf")) {
        return EKinematics::kSH_PRF;
      } else if (label.EqualTo("sh_lcms")) {
        return EKinematics::kSH_LCMS;
      } else if (label.EqualTo("dphideta")) {
        return EKinematics::kPHIETA;
      }
      return EKinematics::kLCMS;
    }

    DividedHisto1D* GetHistoFromXML(XMLNode* nod) {
      if (nod == nullptr) return nullptr;
      XMLNode* frame = nod->GetChild("Frame");
      XMLNode* name  = nod->GetChild("Name");
      XMLNode* type  = nod->GetChild("Type");

      Int_t bins[3]   = {100, 100, 100};
      Double_t min[3] = {0, 0, 0}, max[3] = {1, 1, 1};
      TString axes[3] = {"Xaxis", "Xaxis", "Zaxis"};
      for (int i = 0; i < 3; i++) {
        XMLNode* xaxis = nod->GetChild(axes[i]);
        if (xaxis != nullptr) {
          XMLAttrib* binsXml = xaxis->GetAttrib("bins");
          XMLAttrib* minXml  = xaxis->GetAttrib("min");
          XMLAttrib* maxXml  = xaxis->GetAttrib("max");
          if (binsXml) bins[i] = binsXml->GetValue().Atoi();
          if (minXml) min[i] = minXml->GetValue().Atof();
          if (maxXml) max[i] = maxXml->GetValue().Atof();
        }
      }

      EKinematics Frame = EKinematics::kLCMS;
      if (frame) Frame = CodeLabelToKinematics(frame->GetValue());

      TString cfName = "cf";
      if (name != nullptr) cfName = name->GetValue();
      if (type == nullptr) return nullptr;
      TString classType = type->GetValue();
      if (classType.EqualTo("Femto1DCF")) {
        return new Femto1DCF(cfName, bins[0], min[0], max[0], Frame);
      } else if (classType.EqualTo("Femto3DCF")) {
        return new Femto3DCF(cfName, bins[0], min[0], max[0], bins[1], min[1], max[1], bins[2], min[2], max[2], Frame);
      } else if (classType.EqualTo("FemtoSHCF")) {
        XMLNode* lXml = nod->GetChild("L");
        Int_t L       = 3;
        if (lXml) L = lXml->GetValue().Atoi();
        return new FemtoSHCF(cfName, L, bins[0], min[0], max[0], Frame);
      } else if (classType.EqualTo("FemtoDPhiDEta")) {
        return new FemtoDPhiDEta(cfName, bins[0], bins[1], min[1], max[1]);
      }
      std::cout << "Cannot create correlation function from this node" << std::endl;
      return nullptr;
    }

    EKinematics CodeLabelToKinematics(TString label) {
      if (label.Contains("Hal::")) { label.ReplaceAll("Hal::", ""); }
      if (label.Contains("Femto::")) { label.ReplaceAll("Femto::", ""); }
      if (label.Contains("EKinematics::")) { label.ReplaceAll("EKinematics::", ""); }
      if (label.EqualTo("kPRF")) {
        return EKinematics::kPRF;
      } else if (label.EqualTo("kLCMS")) {
        return EKinematics::kLCMS;
      } else if (label.EqualTo("kSH_PRF")) {
        return EKinematics::kSH_PRF;
      } else if (label.EqualTo("kSH_LCMS")) {
        return EKinematics::kSH_LCMS;
      } else if (label.EqualTo("kPHIETA")) {
        return EKinematics::kPHIETA;
      }
      return EKinematics::kLCMS;
    }

    TString KinematicsToAxisLabel(EKinematics kin, Int_t ax, Int_t ndim) {
      switch (kin) {
        case EKinematics::kPRF: {
          switch (ndim) {
            case 1: {
              switch (ax) {
                case 0: return "k* [GeV/c]"; break;
                case 1: return "dN_{pairs}/dk*"; break;
                case 3: return "CF(k*)"; break;
              }
            } break;
            case 3: {
              switch (ax) {
                case 0: return "k*_{out} [GeV/c]"; break;
                case 1: return "k*_{side} [GeV/c]"; break;
                case 2: return "k*_{long} [GeV/c]"; break;
                case 3: return "CF(k*)"; break;
              }
            } break;
          }
        } break;
        case EKinematics::kLCMS: {
          switch (ndim) {
            case 1: {
              switch (ax) {
                case 0: return "q_{inv} [GeV/c]"; break;
                case 1: return "dN_{pairs}/dq_{inv}"; break;
                case 3: return "CF(q_{inv})"; break;
              }
            } break;
            case 3: {
              switch (ax) {
                case 0: return "q_{out} [GeV/c]"; break;
                case 1: return "q_{side} [GeV/c]"; break;
                case 2: return "q_{long} [GeV/c]"; break;
                case 3: return "CF(q)"; break;
              }
            } break;
          }
        } break;
        case EKinematics::kSH_PRF: {
          switch (ax) {
            case 0: return "k* [GeV/c]"; break;
            case 1: return "#phi*"; break;
            case 2: return "#theta*"; break;
            case 3: return "CF(k*)"; break;
          }
        } break;
        case EKinematics::kSH_LCMS: {
          switch (ax) {
            case 0: return "q_{inv} [GeV/c]"; break;
            case 1: return "#phi_{inv}"; break;
            case 2: return "#theta_{inv}"; break;
            case 3: return "CF(q_{inv})"; break;
          }
        } break;
        case EKinematics::kPHIETA: {
          switch (ax) {
            case 0: return "#Delta#phi"; break;
            case 1: return "#Delta#eta"; break;
            case 2: return "dN_{pairs}/dN_{#Delta#phi#Delta#eta}"; break;
            case 3: return "C(#Delta#phi,#Delta#eta)"; break;
          }
          break;
        }
        case EKinematics::kPRFL: {
          switch (ndim) {
            case 1: {
              switch (ax) {
                case 0: return "k* [GeV/c]";
                case 1: return "dN_{pairs}/dk*";
                case 3: return "CF(k*)";
              }
            } break;
            case 3: {
              switch (ax) {
                case 0: return "k*_{out} [GeV/c]";
                case 1: return "k*_{side} [GeV/c]";
                case 2: return "k*_{long} [GeV/c]";
                case 3: return "CF(k*)";
              }
            } break;
          }
        } break;
      }
      return "";
    }
    void FillRandomPair(FemtoPair* p, Int_t pid1, Int_t pid2, Double_t sigmaq, Double_t sigmar) {
      auto pdg    = TDatabasePDG::Instance();
      Double_t m1 = pdg->GetParticle(pid1)->Mass();
      Double_t m2 = pdg->GetParticle(pid2)->Mass();
      TLorentzVector p1, p2;
      auto rgaus = [](Double_t s) { return gRandom->Gaus(0, s); };
      p1.SetXYZM(rgaus(sigmaq), rgaus(sigmaq), rgaus(sigmaq), m1);
      p2.SetXYZM(rgaus(sigmaq), rgaus(sigmaq), rgaus(sigmaq), m2);
      p->SetTrueMomenta(p1, p2);
      p->SetMomenta(p1, p2);
      TLorentzVector x1(rgaus(sigmar), rgaus(sigmar), rgaus(sigmar), 0);
      TLorentzVector x2(rgaus(sigmar), rgaus(sigmar), rgaus(sigmar), 0);
      p->SetFreezouts(x1, x2);
    }

    Array_1<Float_t>* ExportToFlat(TObject* obj) {
      if (!obj) return nullptr;
      auto shcf = dynamic_cast<Hal::FemtoSHCF*>(obj);
      if (shcf) {
        shcf->RecalculateCF();
        return shcf->ExportToFlatNum();
      }
      auto cf1d = dynamic_cast<Hal::Femto1DCF*>(obj);
      if (cf1d) return cf1d->ExportToFlatNum();
      auto cf3d = dynamic_cast<Hal::Femto3DCF*>(obj);
      if (cf3d) return cf3d->ExportToFlatNum();
      return nullptr;
    }
    /**
     * return CF type if object can be convereted in CF
     * @param obj
     * @return cf type kUnkown if failed to export
     */
    ECFType GetCFType(TObject* obj) {
      if (!obj) return ECFType::kUnkown;
      if (dynamic_cast<Hal::FemtoSHCF*>(obj)) { return ECFType::kSpherical; }
      if (dynamic_cast<Hal::Femto1DCF*>(obj)) { return ECFType::kOneDim; }
      if (dynamic_cast<Hal::Femto3DCF*>(obj)) { return ECFType::kThreeDim; }
      if (dynamic_cast<Hal::FemtoDPhiDEta*>(obj)) { return ECFType::kPhiEta; }
      return ECFType::kUnkown;
    }

    FemtoWeightGenerator* GetWeightGeneratorFromXLM(XMLNode* nod) {
      XMLNode* weightType = nod->GetChild("Type");
      if (!weightType) return nullptr;
      TClass* weightClass     = TClass::GetClass(weightType->GetValue(), 1, 0);
      FemtoWeightGenerator* w = static_cast<FemtoWeightGenerator*>(weightClass->New());
      if (!w) return nullptr;
      XMLNode* pairType        = nod->GetChild("PairType");
      TString val              = pairType->GetValue();
      std::vector<TString> str = Hal::Std::ExplodeString(val, ';');
      if (str.size() == 2) {
        Int_t pid1 = str[0].Atoi();
        Int_t pid2 = str[1].Atoi();
        w->SetPairType(Femto::PidToPairType(pid1, pid2));
      }
      FemtoWeightGeneratorLednicky* lednicky = dynamic_cast<FemtoWeightGeneratorLednicky*>(w);
      if (lednicky) {
        lednicky->SetStrongOff();
        lednicky->SetCoulOff();
        lednicky->SetQuantumOff();

        XMLNode* quantum = nod->GetChild("QuantumOn");
        XMLNode* strong  = nod->GetChild("StrongOn");
        XMLNode* coul    = nod->GetChild("CoulombOn");
        if (quantum) {
          if (quantum->GetValue() == "kTRUE") lednicky->SetQuantumOn();
        }
        if (strong) {
          if (strong->GetValue() == "kTRUE") lednicky->SetStrongOn();
        }
        if (coul) {
          if (coul->GetValue() == "kTRUE") lednicky->SetCoulOn();
        }
      }
      return w;
    }
  }  // namespace Femto
}  // namespace Hal
