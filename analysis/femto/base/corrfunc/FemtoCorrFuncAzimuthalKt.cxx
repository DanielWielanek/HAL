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
#include "Cout.h"
#include "DividedHisto.h"
#include "Event.h"
#include "FemtoPair.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlTable.h"
#include "ObjectMatrix.h"
#include "Track.h"

#include <TMath.h>
#include <TString.h>
#include <TSystem.h>
#include <initializer_list>
#include <iostream>


namespace Hal {
  /*
    FemtoCorrFuncAzimuthalKt::FemtoCorrFuncAzimuthalKt(const DividedHisto1D* h,
                                                       const std::initializer_list<double>& init,
                                                       Int_t phi) :
      FemtoCorrFunc2D(h, "k_{T} [GeV/c]", "#phi_{pair}", init.size() - 1, phi) {
      Double_t dphi     = TMath::TwoPi() / Double_t(phi);
      fPhiCorrection[0] = -0.5 * dphi;
      fPhiCorrection[1] = TMath::TwoPi() - 0.5 * dphi;
      if (phi == 0) return;
      if (fRangeX.GetSize() != (Int_t) init.size()) {
        Cout::PrintInfo(Form("Wrong size of array during call SetKtBins %i %i", fRangeX.GetSize(), (int) init.size()),
                        EInfo::kError);
        return;
      }
      std::initializer_list<double>::iterator it;
      Int_t pos = 0;
      for (it = init.begin(); it != init.end(); ++it) {
        fRangeX[pos++] = *it;
      }
      fRangeY.MakeBigger(phi + 1);
      for (int i = 0; i <= phi; i++) {
        fRangeY[i] = dphi * i - dphi * 0.5;
      }
    }
  */
  Bool_t FemtoCorrFuncAzimuthalKt::Check() {
    fPhiCorrection[0] = fRangeY[0];
    fPhiCorrection[1] = fPhiCorrection[0] + TMath::TwoPi();
    return FemtoCorrFunc2D::Check();
  }

  FemtoCorrFuncAzimuthalKt::~FemtoCorrFuncAzimuthalKt() {
    // TODO Auto-generated destructor stub
  }

  FemtoCorrFuncAzimuthalKt::FemtoCorrFuncAzimuthalKt(const DividedHisto1D& h,
                                                     const std::initializer_list<double>& init,
                                                     Int_t phi,
                                                     Int_t nbins) :
    FemtoCorrFunc2D(h, "k_{T} [GeV/c]", "#phi_{pair}", init.size() - 1, phi) {
    fPhiCorrection[0] = 0;
    fPhiCorrection[1] = TMath::TwoPi();
    if (phi == 0) return;
    if (fRangeX.GetSize() != (Int_t) init.size()) {
      Cout::PrintInfo(Form("Wrong size of array during call SetKtBins %i %i", fRangeX.GetSize(), (int) init.size()),
                      EInfo::kError);
      return;
    }
    std::initializer_list<double>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      fRangeX[pos++] = *it;
    }
    Double_t width = TMath::TwoPi() / ((Double_t) phi);
    for (int i = 0; i <= phi; i++) {
      fRangeY[i] = width * double(i) - width * 0.5;
    }
  }

  std::pair<double, double> FemtoCorrFuncAzimuthalKt::GetPairValNum(Hal::FemtoPair* pair) const {
    Double_t tpx = pair->GetPx1() + pair->GetPx2();
    Double_t tpy = pair->GetPy1() + pair->GetPy2();
    Double_t phi = TMath::ATan2(tpy, tpx);
    phi -= (fPhiCorrection[0]);
    phi -= 0;  // pair->GetTrack1()->GetEvent()->GetPhi();
    while (phi < fPhiCorrection[0])
      phi += TMath::TwoPi();
    while (phi > fPhiCorrection[1])
      phi -= TMath::TwoPi();
    Double_t kt = TMath::Sqrt(tpx * tpx + tpy * tpy) * 0.5;
    std::pair<double, double> res;
    res.first  = kt;
    res.second = phi;  // TODO is order good?
    return res;
  }

  std::pair<double, double> FemtoCorrFuncAzimuthalKt::GetPairValDenRot(Hal::FemtoPair* pair) const {
    Double_t tpx = pair->GetPx1() - pair->GetPx2();
    Double_t tpy = pair->GetPy1() - pair->GetPy2();
    Double_t phi = TMath::ATan2(tpy, tpx);
    phi -= fPhiCorrection[0];
    phi -= pair->GetTrack1()->GetEvent()->GetPhi();
    while (phi < fPhiCorrection[0])
      phi += TMath::TwoPi();
    while (phi > fPhiCorrection[1])
      phi -= TMath::TwoPi();
    Double_t kt = TMath::Sqrt(tpx * tpx + tpy * tpy) * 0.5;
    std::pair<double, double> res;
    res.first  = kt;
    res.second = phi;  // TODO is order good?
    return res;
  }

  std::pair<double, double> FemtoCorrFuncAzimuthalKt::GetPairValDenHemi(Hal::FemtoPair* pair) const {
    return GetPairValDenRot(pair);
  }

  Bool_t FemtoCorrFuncAzimuthalKt::Angle(Char_t opt) const {
    if (opt == 'y') return kTRUE;
    return kFALSE;
  }

}  // namespace Hal
