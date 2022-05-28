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
#include <TNamed.h>
#include <TSystem.h>


namespace Hal {
  FemtoCorrFunc::FemtoCorrFunc(const DividedHisto1D* h, Int_t size) : fArray(NULL) {
    fEventCol.SetName("EventColID");
    fPairCol.SetName("TrackColID");
    if (h != NULL && size > 0) {
      fArray = new ObjectMatrix_1();
      fArray->Init(size, h);
    }
  }

  FemtoCorrFunc::FemtoCorrFunc(const DividedHisto1D& h, Int_t size) : fArray(nullptr) {
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

  FemtoCorrFunc::~FemtoCorrFunc() { delete fArray; }

  FemtoCorrFunc1D::FemtoCorrFunc1D(const DividedHisto1D* h, TString label, Int_t size) : FemtoCorrFunc(h, size), fLabel(label) {
    fRange.MakeBigger(size + 1);
  }

  FemtoCorrFunc1D::FemtoCorrFunc1D(const DividedHisto1D& h, TString label, Int_t size) : FemtoCorrFunc(h, size), fLabel(label) {
    fRange.MakeBigger(size + 1);
  }

  TString FemtoCorrFunc1D::HTMLExtract(Int_t counter, TString dir) const {
    TString path = Form("%s/corrfunc_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    TString filename = Form("%s/corrfunc.html", path.Data());
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "haltable", "");
    HtmlRow row("", "dark_blue", "");
    row.AddContent(HtmlCellCol(fLabel, 2));
    row.AddContent(HtmlCell("value"));
    table.AddContent(row);
    for (int i = 0; i < fArray->GetSize(); i++) {
      DividedHisto1D* h = (DividedHisto1D*) fArray->At(i);
      HtmlRow row1("", "light_blue", "");
      row1.AddContent(HtmlCell(Form("%4.2f", fRange.Get(i))));
      row1.AddContent(HtmlCell(Form("%4.2f", fRange.Get(i + 1))));
      row1.AddContent(HtmlCell(h->HTMLExtract(i, path)));
      table.AddContent(row1);
    }
    file.AddContent(table);
    AddHTMLCode(file);
    file.Save();
    return HtmlCore::GetUrl(Form("corrfunc_%i/corrfunc.html", counter), this->ClassName());
  }

  Bool_t FemtoCorrFunc1D::Check() {
    Int_t x_bins = fRange.GetSize() - 1;
    for (int i = 0; i < x_bins; i++) {
      DividedHisto1D* h = (DividedHisto1D*) fArray->At(i);
      TString histname  = h->GetName();
      h->SetName(Form("%s[%i]", histname.Data(), i));
    }
    return kTRUE;
  }

  void FemtoCorrFunc1D::SetBins(const std::initializer_list<double>& init) {
    if (fRange.GetSize() != (Int_t) init.size()) {
      Cout::PrintInfo(Form("Wrong size of array during call SetKtBins %i %i", fRange.GetSize(), (int) init.size()),
                      EInfo::kError);
      return;
    }
    std::initializer_list<double>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      fRange[pos++] = *it;
    }
  }

  FemtoCorrFunc1D::~FemtoCorrFunc1D() {}

  FemtoCorrFunc2D::FemtoCorrFunc2D(const DividedHisto1D* h, TString labelX, TString labelY, Int_t sizeX, Int_t sizeY) :
    FemtoCorrFunc(h, sizeX * sizeY), fLabelX(labelX), fLabelY(labelY) {
    fRangeX.MakeBigger(sizeX);
    fRangeY.MakeBigger(sizeY);
  }
  FemtoCorrFunc2D::FemtoCorrFunc2D(const DividedHisto1D& h, TString labelX, TString labelY, Int_t sizeX, Int_t sizeY) :
    FemtoCorrFunc(h, sizeX * sizeY), fLabelX(labelX), fLabelY(labelY) {
    fRangeX.MakeBigger(sizeX);
    fRangeY.MakeBigger(sizeY);
  }

  TString FemtoCorrFunc2D::HTMLExtract(Int_t counter, TString dir) const {
    TString path = Form("%s/corrfunc_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    TString filename = Form("%s/corrfunc.html", path.Data());
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "haltable", "");
    HtmlRow row1;
    row1.AddContent(HtmlCell(Form("%s/%s", fLabelX.Data(), fLabelY.Data())));

    for (int i = 0; i < fRangeY.GetSize() - 1; i++) {
      row1.AddContent(HtmlCell(Form("%4.2f - %4.2f ", fRangeY.Get(i), fRangeY.Get(i + 1))));
    }
    table.AddContent(row1);
    const Int_t y_size = fRangeY.GetSize() - 1;
    for (int i = 0; i < fRangeX.GetSize() - 1; i++) {
      HtmlRow row2;
      row2.AddContent(HtmlCell(Form("%4.2f - %4.2f ", fRangeX.Get(i), fRangeX.Get(i + 1))));
      for (int j = 0; j < fRangeY.GetSize() - 1; j++) {
        Int_t pos         = j * y_size + i;
        DividedHisto1D* h = (DividedHisto1D*) fArray->At(pos);
        HtmlCell cell(h->HTMLExtract(pos, path));
        row2.AddContent(cell);
      }
      table.AddContent(row2);
    }
    file.AddContent(table);
    file.Save();
    return HtmlCore::GetUrl(Form("corrfunc_%i/corrfunc.html", counter), this->ClassName());
  }

  Bool_t FemtoCorrFunc2D::Check() {
    for (int i = 1; i < fRangeX.GetSize(); i++) {
      if (fRangeX[i] < fRangeX[i - 1]) {
        Cout::PrintInfo(Form("Wrong order in X-axis in %s", this->ClassName()), EInfo::kCriticalError);
        return kFALSE;
      }
    }
    for (int i = 1; i < fRangeY.GetSize(); i++) {
      if (fRangeY[i] < fRangeY[i - 1]) {
        Cout::PrintInfo(Form("Wrong order in Y-axis in %s", this->ClassName()), EInfo::kCriticalError);
        return kFALSE;
      }
    }
    Int_t x_bins = fRangeX.GetSize() - 1;
    for (int i = 0; i < x_bins; i++) {
      for (int j = 0; j < fRangeY.GetSize() - 1; j++) {
        DividedHisto1D* h = (DividedHisto1D*) fArray->At(i + j * x_bins);
        TString histname  = h->GetName();
        histname.ReplaceAll(Form("[%i]", i + j * x_bins), Form("[%i][%i]", i, j));
      }
    }
    return kTRUE;
  }

  FemtoCorrFunc2D::~FemtoCorrFunc2D() {}

  DividedHisto1D* FemtoCorrFunc2D::GetCF(Int_t i, Int_t j) const {
    Int_t pos = i + j * (fRangeX.GetSize() - 1);
    { return (DividedHisto1D*) fArray->At(pos); };
  }

  FemtoCorrFunc::FemtoCorrFunc() : Object(), fArray(nullptr) {}
}  // namespace Hal
