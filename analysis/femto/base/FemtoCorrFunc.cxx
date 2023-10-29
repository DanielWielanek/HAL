/*
 * CorrFuncCollection.cxx
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoCorrFunc.h"

#include "Cout.h"
#include "HtmlCore.h"
#include "HtmlTable.h"
#include "Std.h"

#include <TBrowser.h>
#include <TCanvas.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TSystem.h>


namespace Hal {
  FemtoCorrFunc::FemtoCorrFunc(const DividedHisto1D* h, Int_t size) {
    fEventCol.SetName("EventColID");
    fPairCol.SetName("TrackColID");
    if (h != NULL && size > 0) {
      fArray = new ObjectMatrix_1();
      fArray->Init(size, h);
    }
  }

  FemtoCorrFunc::FemtoCorrFunc(const DividedHisto1D& h, Int_t size) {
    fEventCol.SetName("EventColID");
    fPairCol.SetName("TrackColID");
    if (size > 0) {
      fArray = new ObjectMatrix_1();
      fArray->Init(size, &h);
    }
  }

  void FemtoCorrFunc::Add(const Object* pack) {
    FemtoCorrFunc* other = (FemtoCorrFunc*) pack;
    if (other->fArray->GetSize() != fArray->GetSize()) {
      Cout::PrintInfo("Cannot merge CorrFunc, sizes are different", EInfo::kError);
      return;
    } else {
      for (int i = 0; i < fArray->GetSize(); i++) {
        DividedHisto1D* this_func  = (DividedHisto1D*) fArray->At(i);
        DividedHisto1D* other_func = (DividedHisto1D*) other->fArray->At(i);
        this_func->Add(other_func);
      }
    }
  }

  void FemtoCorrFunc::Browse(TBrowser* b) {
    b->Add(&fEventCol);
    b->Add(&fPairCol);
    for (int i = 0; i < fArray->GetSize(); i++) {
      b->Add(fArray->At(i));
    }
  }

  FemtoCorrFunc::FemtoCorrFunc(const FemtoCorrFunc& other) {
    if (other.fArray) { fArray = (ObjectMatrix_1*) other.fArray->Clone(); }
    fEventCol = other.fEventCol;
    fPairCol  = other.fPairCol;
  }

  FemtoCorrFunc& FemtoCorrFunc::operator=(const FemtoCorrFunc& other) {
    if (this == &other) return *this;
    if (fArray) delete fArray;
    if (other.fArray) { fArray = (ObjectMatrix_1*) other.fArray->Clone(); }
    fEventCol = other.fEventCol;
    fPairCol  = other.fPairCol;
    return *this;
  }

  FemtoCorrFunc::~FemtoCorrFunc() {
    if (fArray) delete fArray;
  }

}  // namespace Hal
