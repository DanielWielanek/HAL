/*
 * Task.cxx
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Task.h"

#include "Package.h"
#include "Parameter.h"
#include "StdString.h"

#include <vector>

#include <TDirectory.h>
#include <TFile.h>
#include <TObject.h>
#include <TString.h>

namespace Hal {
  Task::Task() {
    // TODO Auto-generated constructor stub
  }

  void Task::GoToDir(TString name) {
    TDirectory* dir            = (TDirectory*) gFile;
    std::vector<TString> array = Hal::Std::ExplodeString(name, '/');
    for (int i = 0; i < (int) array.size(); i++) {
      TDirectory* newdir = (TDirectory*) dir->FindObject(array[i]);
      if (!newdir) { dir->mkdir(array[i]); }
      dir->cd(array[i]);
    }
  }

  Task::~Task() {
    // TODO Auto-generated destructor stub
  }

}  // namespace Hal
