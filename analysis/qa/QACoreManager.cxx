/*
 * QACoreManager.cxx
 *
 *  Created on: 4 maj 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "QACoreManager.h"

#include "Const.h"
#include "TrackAna.h"
#include "TrackOnlyPrimariesCut.h"
#include "TrackPdgCut.h"

namespace Hal {
  void QACoreManager::SetMcTrackCut(TrackAna* ana, ePidCut cut, eParticleType primary, TString flag) {
    Int_t pid = 0;
    switch (cut) {
      case ePidCut::kPionPlus: {
        pid = Const::PionPlusPID();
      } break;
      case ePidCut::kPionMinus: {
        pid = -Const::PionPlusPID();
      } break;
      case ePidCut::kKaonPlus: {
        pid = Const::KaonPlusPID();
      } break;
      case ePidCut::kKaonMinus: {
        pid = -Const::KaonPlusPID();
      } break;
      case ePidCut::kProton: {
        pid = Const::ProtonPID();
      } break;
      case ePidCut::kAntiProton: {
        pid = -Const::ProtonPID();
      } break;
    }
    TrackPdgCut pdgCut;
    pdgCut.SetMinAndMax(pid);
    ana->AddCut(pdgCut, flag);
    switch (primary) {
      case eParticleType::kPrimaryOnly: {
        TrackOnlyPrimariesCut prim;
        ana->AddCut(prim, flag);
      } break;
      case eParticleType::kSecondaryOnly: {
        TrackOnlyPrimariesCut prim;
        prim.AcceptOnlyNonPrimaries();
        ana->AddCut(prim, flag);
      } break;
    }
  }
  Event* QACoreManager::GetFormat(eFormatType /*type*/, eAnaType /*ana*/) { return nullptr; }

  FairRunAna* QACoreManager::GetRunAna(TString outFile, TString simFile, TString recoFile, TString parFile) {
    if (simFile.Length() == 0) {
      simFile  = recoFile;
      recoFile = "";
    }
    FairRunAna* run      = new FairRunAna();
    FairFileSource* file = new FairFileSource(simFile);
    if (recoFile != "") { file->AddFriend(recoFile); }
    if (parFile != "") {
      FairRuntimeDb* rtdb       = run->GetRuntimeDb();
      FairParRootFileIo* parIo1 = new FairParRootFileIo();
      parIo1->open(parFile.Data());
      rtdb->setFirstInput(parIo1);
    }
    run->SetSource(file);
    run->SetOutputFile(outFile);
    return run;
  }

  void QACoreManager::SetRecoTrackCut(TrackAna* /*ana*/, ePidCut /*cut*/, eParticleType /*primary*/, TString /*flag*/) {}

  void QACoreManager::SetEventCut(TrackAna* /*ana*/, Int_t /*col*/, TString /*flag*/) {};

  void QACoreManager::SetPairCut(TwoTrackAna* /*ana*/, ePidCut /*pid1*/, ePidCut /*pid2*/) {};

  QACoreManager::ePidCut QACoreManager::ToPid(Int_t i) {
    switch (i) {
      case 0: return QACoreManager::ePidCut::kPionPlus; break;
      case 1: return QACoreManager::ePidCut::kPionMinus; break;
      case 2: return QACoreManager::ePidCut::kKaonPlus; break;
      case 3: return QACoreManager::ePidCut::kKaonMinus; break;
      case 4: return QACoreManager::ePidCut::kProton; break;
      case 5: return QACoreManager::ePidCut::kAntiProton; break;
      default: return QACoreManager::ePidCut::kUnkown;
    }
  }
}  // namespace Hal
