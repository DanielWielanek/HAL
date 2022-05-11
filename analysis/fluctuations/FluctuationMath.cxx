/*
 * FluctuationMath.cxx
 *
 *  Created on: 31 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FluctuationMath.h"

#include <TMath.h>
#include <iostream>

namespace Hal {
  FluctuationMath::FluctuationMath() :
    fN(0),
    fTollerance(5),
    fNError(0),
    fOmega(0),
    fSSigma(0),
    fKappaSigma(0),
    fOmegaError(0),
    fSigmaError(0),
    fKappaError(0),
    fNSample(10) {
    fM[0] = fM[1] = 0;
  }

  void FluctuationMath::GetVals(Double_t& min, Double_t& max, Double_t val, Double_t error) {
    min = val - TMath::Abs(error);
    max = val + TMath::Abs(error);
  }

  void FluctuationMath::SetHistoTH1D(TH1D* h, TString opt) {
    if (opt == "") {
      CalcError(h);
    } else {
      CalcErrorMagic(h);
    }
  }

  void FluctuationMath::Calc(TH1D* h) {
    fN = fM[2] = fM[3] = fM[4] = 0.0;
    Double_t entries           = h->GetEntries();
    // fHisto->Scale(1.0/entries);
    for (int i = 1; i <= h->GetNbinsX(); i++) {
      Double_t pN = ((Double_t) h->GetBinContent(i));
      Double_t N  = (Double_t) h->GetBinCenter(i);
      fN += N * pN;
    }
    fN = fN / entries;
    for (int i = 1; i <= h->GetNbinsX(); i++) {
      Double_t pN = ((Double_t) h->GetBinContent(i));
      Double_t N  = (Double_t) h->GetBinCenter(i);
      for (int j = 2; j <= 4; j++) {
        fM[j] += TMath::Power(N - fN, j) * pN;
      }
    }
    for (int j = 2; j <= 4; j++) {
      fM[j] = fM[j] / entries;
    }
    fOmega      = GetCentralMoment(2) / fN;
    fSSigma     = GetCentralMoment(3) / GetCentralMoment(2);
    fKappaSigma = GetCentralMoment(4) / GetCentralMoment(2) - 3.0 * GetCentralMoment(2);
  }

  Double_t FluctuationMath::GetCentralMoment(Int_t i) const {
    if (i < 2 || i > 4) {
      std::cout << "wrong mu index" << std::endl;
      return 0;
    } else {
      return fM[i];
    }
  }

  void FluctuationMath::CalcError(TH1D* h) {
    TH1D* hh              = (TH1D*) h->Clone("temp_boostrap");
    FluctuationMath* stat = new FluctuationMath();
    Double_t range_min[4], range_max[4];
    GetVals(range_min[0], range_max[0], GetMean(), GetMean() * 2.5);
    GetVals(range_min[1], range_max[1], GetOmega(), GetOmega() * 2.5);
    GetVals(range_min[2], range_max[2], GetSSigma(), GetSSigma() * 2.5);
    GetVals(range_min[3], range_max[3], GetKappaSigma(), GetKappaSigma() * 2.5);
    Bool_t stat_ok = kFALSE;
    while (!stat_ok) {
      TH1D* mean  = new TH1D("mean", "mean", 1000, range_min[0], range_max[0]);
      TH1D* omega = new TH1D("omega", "omega", 1000, range_min[1], range_max[1]);
      TH1D* sigma = new TH1D("sigma", "sigma", 1000, range_min[2], range_max[2]);
      TH1D* kappa = new TH1D("kapa", "kappa", 1000, range_min[3], range_max[3]);
      for (int i = 0; i < fNSample; i++) {
        hh->Reset();
        for (int j = 0; j < h->GetEntries(); j++) {
          hh->Fill(h->GetRandom());
        }
        stat->Calc(hh);
        omega->Fill(stat->GetOmega());
        sigma->Fill(stat->GetSSigma());
        kappa->Fill(stat->GetKappaSigma());
        mean->Fill(stat->GetMean());
      }
      fOmegaError = omega->GetRMS();
      fSigmaError = sigma->GetRMS();
      fKappaError = kappa->GetRMS();
      fNError     = mean->GetRMS();
      stat_ok     = kTRUE;
      if (fNError > (1.0 / fTollerance) * TMath::Abs(range_min[0] - range_max[0])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of mean" << std::endl;
        Double_t step = TMath::Abs(range_min[0] - range_max[0]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[0],
                          range_max[0],
                          step,
                          fNError)
                  << std::endl;
        GetVals(range_min[0], range_max[0], GetMean(), GetMeanError() * fTollerance * 2.0);
        step = TMath::Abs(range_min[0] - range_max[0]);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[0], range_max[0], step) << std::endl;
      }
      if (fOmegaError > (1.0 / fTollerance) * TMath::Abs(range_min[1] - range_max[1])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of omega" << std::endl;
        Double_t step = TMath::Abs(range_min[1] - range_max[1]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[1],
                          range_max[1],
                          step,
                          fOmegaError)
                  << std::endl;
        GetVals(range_min[1], range_max[1], GetOmega(), GetOmegaError() * fTollerance * 2.0);
        step = TMath::Abs(range_min[1] - range_max[1]);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[1], range_max[1], step) << std::endl;
      }
      if (fSigmaError > (1.0 / fTollerance) * TMath::Abs(range_min[2] - range_max[2])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of sigma" << std::endl;
        Double_t step = TMath::Abs(range_min[2] - range_max[2]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[2],
                          range_max[2],
                          step,
                          fSigmaError)
                  << std::endl;
        step = TMath::Abs(range_min[2] - range_max[2]);
        GetVals(range_min[2], range_max[2], GetSSigma(), GetSSigmaError() * fTollerance * 2.0);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[2], range_max[2], step) << std::endl;
      }
      if (fKappaError > (1.0 / fTollerance) * TMath::Abs(range_min[3] - range_max[3])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of kappa" << std::endl;
        Double_t step = TMath::Abs(range_min[3] - range_max[3]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[3],
                          range_max[3],
                          step,
                          fKappaError)
                  << std::endl;
        GetVals(range_min[3], range_max[3], GetKappaSigma(), GetKappaSigmaError() * fTollerance * 2.0);
        step = TMath::Abs(range_min[3] - range_max[3]);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[3], range_max[3], step) << std::endl;
      }
      delete omega;
      delete mean;
      delete sigma;
      delete kappa;
    }
    delete stat;
    delete hh;
  }

  void FluctuationMath::CalcErrorMagic(TH1D* h) {
    TH1D* hh              = (TH1D*) h->Clone("temp_boostrap");
    FluctuationMath* stat = new FluctuationMath();
    Double_t range_min[4], range_max[4];
    GetVals(range_min[0], range_max[0], GetMean(), GetMean() * 2.5);
    GetVals(range_min[1], range_max[1], GetOmega(), GetOmega() * 2.5);
    GetVals(range_min[2], range_max[2], GetSSigma(), GetSSigma() * 2.5);
    GetVals(range_min[3], range_max[3], GetKappaSigma(), GetKappaSigma() * 2.5);
    Bool_t stat_ok = kFALSE;
    while (!stat_ok) {
      TH1D* mean  = new TH1D("mean", "mean", 1000, range_min[0], range_max[0]);
      TH1D* omega = new TH1D("omega", "omega", 1000, range_min[1], range_max[1]);
      TH1D* sigma = new TH1D("sigma", "sigma", 1000, range_min[2], range_max[2]);
      TH1D* kappa = new TH1D("kapa", "kappa", 1000, range_min[3], range_max[3]);
      for (int i = 0; i < h->GetNbinsX(); i++) {
        hh->Reset();
        Double_t scale    = h->GetEntries();
        Double_t bin_cont = h->GetBinContent(i);
        scale             = scale / (scale - bin_cont);
        for (int j = 0; j <= h->GetNbinsX(); j++) {
          hh->SetBinContent(j, h->GetBinContent(j) * scale);
        }
        hh->SetBinContent(i, 0);
        stat->Calc(hh);
        omega->Fill(stat->GetOmega(), bin_cont);
        sigma->Fill(stat->GetSSigma(), bin_cont);
        kappa->Fill(stat->GetKappaSigma(), bin_cont);
        mean->Fill(stat->GetMean(), bin_cont);
      }
      fOmegaError = omega->GetRMS();
      fSigmaError = sigma->GetRMS();
      fKappaError = kappa->GetRMS();
      fNError     = mean->GetRMS();
      stat_ok     = kTRUE;
      if (fNError > (1.0 / fTollerance) * TMath::Abs(range_min[0] - range_max[0])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of mean" << std::endl;
        Double_t step = TMath::Abs(range_min[0] - range_max[0]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[0],
                          range_max[0],
                          step,
                          fNError)
                  << std::endl;
        GetVals(range_min[0], range_max[0], GetMean(), GetMeanError() * fTollerance * 2.0);
        step = TMath::Abs(range_min[0] - range_max[0]);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[0], range_max[0], step) << std::endl;
      }
      if (fOmegaError > (1.0 / fTollerance) * TMath::Abs(range_min[1] - range_max[1])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of omega" << std::endl;
        Double_t step = TMath::Abs(range_min[1] - range_max[1]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[1],
                          range_max[1],
                          step,
                          fOmegaError)
                  << std::endl;
        GetVals(range_min[1], range_max[1], GetOmega(), GetOmegaError() * fTollerance * 2.0);
        step = TMath::Abs(range_min[1] - range_max[1]);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[1], range_max[1], step) << std::endl;
      }
      if (fSigmaError > (1.0 / fTollerance) * TMath::Abs(range_min[2] - range_max[2])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of sigma" << std::endl;
        Double_t step = TMath::Abs(range_min[2] - range_max[2]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[2],
                          range_max[2],
                          step,
                          fSigmaError)
                  << std::endl;
        step = TMath::Abs(range_min[2] - range_max[2]);
        GetVals(range_min[2], range_max[2], GetSSigma(), GetSSigmaError() * fTollerance * 2.0);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[2], range_max[2], step) << std::endl;
      }
      if (fKappaError > (1.0 / fTollerance) * TMath::Abs(range_min[3] - range_max[3])) {
        stat_ok = kFALSE;
        std::cout << " need to recalcuate because of kappa" << std::endl;
        Double_t step = TMath::Abs(range_min[3] - range_max[3]);
        std::cout << Form(" used range %4.3f - %4.3f [%4.3f ] calculated error "
                          "was %4.3f ",
                          range_min[3],
                          range_max[3],
                          step,
                          fKappaError)
                  << std::endl;
        GetVals(range_min[3], range_max[3], GetKappaSigma(), GetKappaSigmaError() * fTollerance * 2.0);
        step = TMath::Abs(range_min[3] - range_max[3]);
        std::cout << Form(" new  range %4.3f - %4.3f [%4.3f ] ", range_min[3], range_max[3], step) << std::endl;
      }
      delete omega;
      delete mean;
      delete sigma;
      delete kappa;
    }
    delete stat;
    delete hh;
  }

  FluctuationMath::~FluctuationMath() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
