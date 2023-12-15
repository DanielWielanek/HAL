/*
 * QAFairCoreManager.cxx
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "QAManagerBasic.h"

#include <FairFileSource.h>
#include <FairParRootFileIo.h>
#include <FairRun.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>

namespace Hal {
  namespace Fair {
    QAManagerBasic::QAManagerBasic() {
    }

    QAManagerBasic::~QAManagerBasic() {
    }

    FairRunAna* QAManagerBasic::GetRunAna(TString outFile, TString simFile, TString recoFile, TString parFile) {
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
  }  // namespace Fair
} /* namespace Hal */
