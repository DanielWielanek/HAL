/*
 * FemtoImaging1D.cxx
 *
 *  Created on: 20 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoImaging1D.h"

#include "Cout.h"

#include "TMath.h"
#ifndef DISABLE_GSL
#include <gsl/gsl_sf_bessel.h>
#endif
#include <iostream>
namespace Hal {
  FemtoImaging1D::FemtoImaging1D() :
    fQMax(0.5),
    fQMin(0.0),
    fRMax(50),
    fRMin(0),
    fBins(100),
    fKinematics(Femto::EKinematics::kLCMS),
    fCF(NULL),
    fR(NULL),
    fSource(NULL) {}

  void FemtoImaging1D::Init() {
    TH1* CF       = fCF->GetHist(kTRUE);
    Double_t min  = CF->GetXaxis()->GetBinLowEdge(1);
    Double_t max  = CF->GetXaxis()->GetBinUpEdge(CF->GetNbinsX());
    Double_t bins = CF->GetNbinsX();
    fR            = new TH1D("R", "R", bins, min, max);
    for (int i = 1; i <= bins; i++) {
      fR->SetBinContent(i, (CF->GetBinContent(i) - 1.0));
      if (CF->GetBinContent(i) != 0) fR->SetBinError(i, CF->GetBinError(i) * fR->GetBinContent(i) / CF->GetBinContent(i));
    }
    delete CF;
    // fRMax = TMath::Pi()/fR->GetBinWidth(1)/2.0/FM_TO_GEV;
    fSource = new TH1D("Source", "Source", fBins, fRMin, fRMax);
    if (fCF->GetFrame() == Femto::EKinematics::kPRF) {
      fSource->GetYaxis()->SetTitle("S(r*)");
      fSource->GetXaxis()->SetTitle("r*");
      fKinematics = Femto::EKinematics::kPRF;
    } else if (fCF->GetFrame() == Femto::EKinematics::kLCMS) {
      fSource->GetYaxis()->SetTitle("S(r)");
      fSource->GetXaxis()->SetTitle("r");
      fKinematics = Femto::EKinematics::kLCMS;
    }
  }

  TH1D* FemtoImaging1D::Inverse(Femto1DCF* cf) {
    fCF = cf;
    Init();
    InverseTransofrm();
    Double_t norm = fSource->Integral(1, fSource->GetNbinsX(), "width");
    //	norm = norm *fSource->GetBinWidth(1);
    fSource->Scale(1.0 / norm);
    TH1D* histo = (TH1D*) fSource->Clone();
    delete fSource;
    fSource = NULL;
    //	histo->SetDirectory(0);
    return histo;
  }

  void FemtoImaging1D::SetRAxis(Int_t bins, Double_t min, Double_t max) {
    fBins = bins;
    fRMin = min;
    fRMax = max;
  }

  TH1D* FemtoImaging1D::Inverse(TH1D* CF, Bool_t normalize) {
    Double_t min  = CF->GetXaxis()->GetBinLowEdge(1);
    Double_t max  = CF->GetXaxis()->GetBinUpEdge(CF->GetNbinsX());
    Double_t bins = CF->GetNbinsX();

    fR = new TH1D("R", "R", bins, min, max);
    for (int i = 1; i <= bins; i++) {
      fR->SetBinContent(i, (CF->GetBinContent(i) - 1.0));
      if (CF->GetBinContent(i) != 0) fR->SetBinError(i, CF->GetBinError(i) * fR->GetBinContent(i) / CF->GetBinContent(i));
    }
    // fRMax = TMath::Pi()/fR->GetBinWidth(1)/2.0/FM_TO_GEV;
    fSource = new TH1D("Source", "Source", fBins, fRMin, fRMax);
    InverseTransofrm();
    Double_t norm = fSource->Integral(1, fSource->GetNbinsX(), "width");
    //	norm = norm *fSource->GetBinWidth(1);
    if (normalize) fSource->Scale(1.0 / norm);
    TH1D* histo = (TH1D*) fSource->Clone();
    delete fSource;
    fR      = NULL;
    fSource = NULL;
    //	histo->SetDirectory(0);
    return histo;
  }

  TH1D* FemtoImaging1D::MagicInverse(TH1D* CF, Double_t l, Bool_t /*normalize*/) {
    Double_t min  = CF->GetXaxis()->GetBinLowEdge(1);
    Double_t max  = CF->GetXaxis()->GetBinUpEdge(CF->GetNbinsX());
    Double_t bins = CF->GetNbinsX();

    fR = new TH1D("R", "R", bins, min, max);
    for (int i = 1; i <= bins; i++) {
      fR->SetBinContent(i, (CF->GetBinContent(i)));
      if (CF->GetBinContent(i) != 0) fR->SetBinError(i, CF->GetBinError(i) * fR->GetBinContent(i) / CF->GetBinContent(i));
    }
    // fRMax = TMath::Pi()/fR->GetBinWidth(1)/2.0/FM_TO_GEV;
    fSource = new TH1D("Source", "Source", fBins, fRMin, fRMax);
    MagicInverseTransform(l);
    TH1D* histo = (TH1D*) fSource->Clone();
    delete fSource;
    fR      = NULL;
    fSource = NULL;
    //	histo->SetDirectory(0);
    return histo;
  }

  void FemtoImaging1D::MagicInverseTransform(Int_t l) {
    Double_t power = TMath::Power(-1, l / 2) * TMath::Power(TMath::Pi(), 3);
    for (double i = 1; i <= fSource->GetNbinsX(); i++) {
      if (l % 2 == 1) continue;
      Double_t r      = fSource->GetBinCenter(i) * Femto::FmToGeV();
      Double_t sum    = 0.0;
      Double_t errors = 0.0;
      for (int j = fR->GetXaxis()->FindBin(fQMin); j < fR->GetXaxis()->FindBin(fQMax); j++) {
        Double_t q = fR->GetBinCenter(j);
        if (fKinematics == Femto::EKinematics::kPRF) q += q;
        Double_t R = fR->GetBinContent(j);
        //	if(TMath::Abs(R)<=0.01) continue;
        Double_t dR       = fR->GetBinError(j);
        Double_t integral = q * q * BesselJn(l, 2 * q * r) / Femto::FmToGeV() / Femto::FmToGeV();
        sum += R * integral;
        errors += dR * integral;
      }
      // Double_t end = fR->GetBinCenter(fR->FindBin(fQMax));
      // Double_t numerical_end =TMath::Ceil( fQMax/TMath::Pi()/2.0/r);
      if (r != 0) {
        // Double_t correction =
        // -1.0/r*(TMath::Cos(r*numerical_end)-TMath::Cos(r*end)); sum+=correction;
      }

      // Double_t ep  = r*fR->GetBinWidth(1);
      // empiric estimation of  error
      if (sum < 0) { sum = 0; }
      // Double_t dFourierError = (0.05+ep*0.22+ep*ep*0.084)*sum;
      //	errors= TMath::Sqrt(errors*errors+dFourierError*dFourierError);
      //	if(r!=0){
      fSource->SetBinContent(i, sum * power);
      fSource->SetBinError(i, errors);
      //	}
    }
    delete fR;
    fR = NULL;
  }

  Double_t FemtoImaging1D::BesselJ0(double x) const {
#ifndef DISABLE_GSL
    return gsl_sf_bessel_j0(x);
#else
    std::cout << "GSL NOT SUPPORTED" << std::endl;
    return 0;
#endif
  }

  Double_t FemtoImaging1D::BesselJn(int n, double x) const {
    //	gsl_sf_bessel_j
    //	return gsl_sf_bessel_Jn(n,x);
    // gsl_sf_bessel_Inu()
#ifndef DISABLE_GSL
    return jn(n, x);
#else
    std::cout << "GSL NOT SUPPORTED" << std::endl;
    return 0;
#endif
  }

  double FemtoImaging1D::BesselJ1(double x) const {
#ifndef DISABLE_GSL
    return gsl_sf_bessel_j1(x);
#else
    std::cout << "GSL NOT SUPPORTED" << std::endl;
    return 0;
#endif
  }

  FemtoImaging1D::~FemtoImaging1D() {}

  void FemtoImaging1D::InverseTransofrm() {
    for (double i = 1; i <= fSource->GetNbinsX(); i++) {
      Double_t r      = fSource->GetBinCenter(i) * Femto::FmToGeV();
      Double_t sum    = 0.0;
      Double_t errors = 0.0;
      for (int j = fR->GetXaxis()->FindBin(fQMin); j < fR->GetXaxis()->FindBin(fQMax); j++) {
        Double_t q = fR->GetBinCenter(j);
        if (fKinematics == Femto::EKinematics::kPRF) q += q;
        Double_t R = fR->GetBinContent(j);
        if (R == 0) continue;
        Double_t dR       = fR->GetBinError(j);
        Double_t integral = q * TMath::Sin(q * r * 2);
        sum += R * integral;
        errors += dR * integral;
      }
      // Double_t end = fR->GetBinCenter(fR->FindBin(fQMax));
      // Double_t numerical_end =TMath::Ceil( fQMax/TMath::Pi()/2.0/r);
      if (r != 0) {
        // Double_t correction =
        // -1.0/r*(TMath::Cos(r*numerical_end)-TMath::Cos(r*end)); sum+=correction;
      }

      Double_t ep = r * fR->GetBinWidth(1);
      // empiric estimation of  error
      if (sum < 0) { sum = 0; }
      Double_t dFourierError = (0.05 + ep * 0.22 + ep * ep * 0.084) * sum;
      errors                 = TMath::Sqrt(errors * errors + dFourierError * dFourierError);
      if (r != 0) {
        fSource->SetBinContent(i, sum / r);
        fSource->SetBinError(i, errors / r);
      }
    }
    delete fR;
    fR = NULL;
  }
}  // namespace Hal
