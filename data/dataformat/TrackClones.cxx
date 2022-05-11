/*
 * TrackClonesTemplate.cxx
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackClones.h"

#include "DataManager.h"
#include "IOManager.h"

#include <FairLogger.h>

namespace Hal {
  TrackClones::TrackClones(TString className, TString branchname, TString dirname) :
    fBranchName(branchname), fDirName(dirname), fClones(NULL) {
    if (className != "") { fClones = new TClonesArray(className); }
  }

  void TrackClones::Register(Bool_t write) {
    DataManager* mngr = DataManager::Instance();
    mngr->Register(fBranchName, fDirName, fClones, write);
  }

  void TrackClones::GetFromTree() {
    if (fClones) { delete fClones; }
    fClones = NULL;
    fClones = (TClonesArray*) DataManager::Instance()->GetObject(fBranchName);
    if (fClones == NULL) { LOG(ERROR) << "Branch " << fBranchName << " not found !"; }
  }

  TrackClones::~TrackClones() {
    if (fClones) delete fClones;
  }

  Bool_t TrackClones::ExistInTree() const {
    DataManager* mngr = DataManager::Instance();
    return mngr->CheckBranch(fBranchName);
  }

  void TrackClones::Compress(Int_t* map, Int_t /*map_size*/) {
    Int_t good_pos = 0;
    for (int i = 0; i < fClones->GetEntriesFast(); i++) {
      if (i == map[good_pos]) {
        good_pos++;
        continue;
      }
      fClones->RemoveAt(i);
    }
    fClones->Compress();
  }

  void TrackClones::DeleteClones() {
    if (fClones) delete fClones;
    fClones = NULL;
  }
}  // namespace Hal
