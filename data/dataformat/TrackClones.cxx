/*
 * TrackClonesTemplate.cxx
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackClones.h"

#include "Cout.h"
#include "DataManager.h"
#include "IOManager.h"

namespace Hal {
  TrackClones::TrackClones(TString className, TString branchname, TString dirname) :
    fBranchName(branchname), fDirName(dirname), fClassName(className) {}

  void TrackClones::Register(Bool_t write) {
    DataManager* mngr = DataManager::Instance();
    auto clones       = new TClonesArray(fClassName);
    fPointer          = mngr->Register(clones, fBranchName, write);
  }

  void TrackClones::GetFromTree() {
    fPointer = (ObjectDoublePointer*) DataManager::Instance()->GetDoublePointer(fBranchName);
    if (fPointer == nullptr) { Cout::PrintInfo(Form("Branch %s not found!", fBranchName.Data()), EInfo::kError); }
  }

  TrackClones::~TrackClones() {}

  Bool_t TrackClones::ExistInTree() const {
    DataManager* mngr = DataManager::Instance();
    return mngr->CheckBranch(fBranchName);
  }

  void TrackClones::Compress(Int_t* map, Int_t /*map_size*/) {
    Int_t good_pos = 0;
    auto clones    = GetClones();
    for (int i = 0; i < clones->GetEntriesFast(); i++) {
      if (i == map[good_pos]) {
        good_pos++;
        continue;
      }
      clones->RemoveAt(i);
    }
    clones->Compress();
  }

  void TrackClones::DeleteClones() { fPointer.DeletePointer(); }
}  // namespace Hal
