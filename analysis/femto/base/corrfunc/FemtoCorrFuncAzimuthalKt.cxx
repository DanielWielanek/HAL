/*
 * CorrFuncAzimuthalKt.cxx
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoCorrFuncAzimuthalKt.h"

#include "Array.h"
#include "DividedHisto.h"
#include "Event.h"
#include "FemtoPair.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlTable.h"
#include "ObjectMatrix.h"
#include "Track.h"

#include <FairLogger.h>
#include <TMath.h>
#include <TString.h>
#include <TSystem.h>
#include <initializer_list>
#include <iostream>


namespace Hal {
  FemtoCorrFuncAzimuthalKt::FemtoCorrFuncAzimuthalKt(const DividedHisto1D* h,
                                                     const std::initializer_list<double>& init,
                                                     Int_t phi) :
    FemtoCorrFunc2D(h, "k_{T} [GeV/c]", "#phi_{pair}") {
    fPhiCorrection[0] = 0;
    fPhiCorrection[1] = TMath::TwoPi();
    if (phi == 0) return;
    if (fRangeX.GetSize() != (Int_t) init.size()) {
      LOG(ERROR) << "Wrong size of array during call SetKtBins" << fRangeX.GetSize() << " " << init.size() << std::endl;
      return;
    }
    std::initializer_list<double>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      fRangeX[pos++] = *it;
    }
    fRangeY.MakeBigger(phi + 1);
    Double_t width = TMath::TwoPi() / ((Double_t) phi);
    for (int i = 0; i <= phi; i++) {
      fRangeY[i] = width * i - width * 0.5;
    }
  }

  Bool_t FemtoCorrFuncAzimuthalKt::Check() {
    fPhiCorrection[0] = fRangeY[0];
    fPhiCorrection[1] = fPhiCorrection[0] + TMath::TwoPi();
    return FemtoCorrFunc2D::Check();
  }

  void FemtoCorrFuncAzimuthalKt::FillNum(FemtoPair* pair) {
    Double_t px        = pair->GetPx1() + pair->GetPx2();
    Double_t py        = pair->GetPy1() + pair->GetPy2();
    Double_t event_phi = pair->GetTrack1()->GetEvent()->GetPhi();
    ((DividedHisto1D*) fArray->At(CalculateBin(px, py, event_phi)))->FillNumObj(pair);
  }

  void FemtoCorrFuncAzimuthalKt::FillDenMixed(FemtoPair* pair) {
    Double_t px        = pair->GetPx1() + pair->GetPx2();
    Double_t py        = pair->GetPy1() + pair->GetPy2();
    Double_t event_phi = pair->GetTrack1()->GetEvent()->GetPhi();
    ((DividedHisto1D*) fArray->At(CalculateBin(px, py, event_phi)))->FillDenObj(pair);
  }

  Int_t FemtoCorrFuncAzimuthalKt::CalculateBin(Double_t px, Double_t py, Double_t event_phi) {
    Double_t kt      = TMath::Sqrt(px * px + py * py) * 0.5;
    Int_t kt_bin     = -1;
    const Int_t size = fRangeX.GetSize();
    if (kt < fRangeX.Get(0)) return -1;
    for (int i = 1; i < size; i++) {
      if (kt < fRangeX.Get(i)) {
        kt_bin = i - 1;
        break;
      }
    }
    if (kt_bin < 0) return -1;
    Double_t phi = TMath::ATan2(py, px);
    phi -= event_phi;
    while (phi < fPhiCorrection[0])
      phi += TMath::TwoPi();
    while (phi > fPhiCorrection[1])
      phi -= TMath::TwoPi();
    Int_t phi_bin        = -1;
    const Int_t size_phi = fRangeY.GetSize();
    if (kt < fRangeX.Get(0)) return -1;
    for (int i = 1; i < size_phi; i++) {
      if (kt < fRangeX.Get(i)) {
        phi_bin = i - 1;
        break;
      }
    }
    if (phi_bin < 0) return -1;
    return kt_bin + phi_bin * (fRangeX.GetSize() - 1);
  }

  void FemtoCorrFuncAzimuthalKt::FillDenRotated(FemtoPair* pair) {
    Double_t px        = pair->GetPx1() - pair->GetPx2();
    Double_t py        = pair->GetPy1() - pair->GetPy2();
    Double_t event_phi = pair->GetTrack1()->GetEvent()->GetPhi();
    ((DividedHisto1D*) fArray->At(CalculateBin(px, py, event_phi)))->FillDenObj(pair);
  }

  TString FemtoCorrFuncAzimuthalKt::HTMLExtract(Int_t no, TString dir) const {
    TString path = Form("%s/corrfunc_%i", dir.Data(), no);
    gSystem->MakeDirectory(dir);
    TString filename = Form("%s/corrfunc.html", path.Data());
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "nicatable", "");
    HtmlRow row;
    row.AddContent(HtmlCell(Form("%s/%s", fLabelX.Data(), fLabelY.Data())));
    for (int i = 0; i < fRangeY.GetSize() - 1; i++) {
      row.AddContent(HtmlCell(Form("%4.2f - %4.2f ", fRangeY.Get(i), fRangeY.Get(i + 1))));
    }
    table.AddContent(row);
    const Int_t y_size = fRangeY.GetSize() - 1;
    for (int i = 0; i < fRangeX.GetSize() - 1; i++) {
      HtmlRow row1;
      row1.AddContent(HtmlCell(Form("%4.2f - %4.2f ", fRangeX.Get(i), fRangeX.Get(i + 1))));
      for (int j = 0; j < fRangeY.GetSize() - 1; j++) {
        Int_t pos         = j * y_size + i;
        DividedHisto1D* h = (DividedHisto1D*) fArray->At(pos);
        row.AddContent(HtmlCell(h->HTMLExtract(pos, path)));
      }
      table.AddContent(row1);
    }
    file.AddContent(table);
    file.Save();
    return HtmlCore::GetUrl(Form("corrfunc_%i/corrfunc.html", no), this->ClassName());
  }

  FemtoCorrFuncAzimuthalKt::~FemtoCorrFuncAzimuthalKt() {
    // TODO Auto-generated destructor stub
  }

  FemtoCorrFuncAzimuthalKt::FemtoCorrFuncAzimuthalKt(const DividedHisto1D& h,
                                                     const std::initializer_list<double>& init,
                                                     Int_t phi) :
    FemtoCorrFunc2D(h, "k_{T} [GeV/c]", "#phi_{pair}") {
    fPhiCorrection[0] = 0;
    fPhiCorrection[1] = TMath::TwoPi();
    if (phi == 0) return;
    if (fRangeX.GetSize() != (Int_t) init.size()) {
      LOG(ERROR) << "Wrong size of array during call SetKtBins" << fRangeX.GetSize() << " " << init.size() << std::endl;
      return;
    }
    std::initializer_list<double>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      fRangeX[pos++] = *it;
    }
    fRangeY.MakeBigger(phi + 1);
    Double_t width = TMath::TwoPi() / ((Double_t) phi);
    for (int i = 0; i <= phi; i++) {
      fRangeY[i] = width * i - width * 0.5;
    }
  }
}  // namespace Hal
