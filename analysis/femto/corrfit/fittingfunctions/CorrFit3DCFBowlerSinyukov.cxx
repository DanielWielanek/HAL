/*
 * CorrFit3DCFBowlerSinyukov.cxx
 *
 *  Created on: 22 sty 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit3DCFBowlerSinyukov.h"


#include <TAxis.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TString.h>
#include <iostream>

#include "Array.h"
#include "CorrFit.h"
#include "Cout.h"
#include "Femto3DCF.h"
#include "Femto3DCFQinv.h"
#include "StdHist.h"
#include "StdString.h"


namespace Hal {
  Double_t CorrFit3DCFBowlerSinyukov::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t cf_free = 0;
    Double_t kq      = 0;

    switch (fFrame) {
      case Femto::EKinematics::kLCMS: {
        Double_t k_star = 0;
        k_star          = 0.5 * fQinvMap->Eval(x[0], x[1], x[2]);
        kq              = fCFs->Eval(k_star, fRinv);
        cf_free =
          1.
          + fQSMode
              * TMath::Exp(-25.76578
                           * (x[0] * x[0] * params[Rout()] * params[Rout()] + x[1] * x[1] * params[Rside()] * params[Rside()]
                              + x[2] * x[2] * params[Rlong()] * params[Rlong()]));
      } break;
      case Femto::EKinematics::kPRF: {
        Double_t k_star = TMath::Sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
        cf_free =
          1.
          + fQSMode
              * TMath::Exp(-25.76578 * 4.0
                           * (x[0] * x[0] * params[Rout()] * params[Rout()] + x[1] * x[1] * params[Rside()] * params[Rside()]
                              + x[2] * x[2] * params[Rlong()] * params[Rlong()]));  // x4 cause PRF
        kq = fCFs->Eval(k_star, fRinv);
      } break;
      default:
        // r_out = params[Rout()];
        break;
    }
    if (kq < 0 || TMath::IsNaN(kq)) kq = 0;
    return params[Norm()] * (1.0 - params[Lambda()] + params[Lambda()] * kq * cf_free);
  }

  CorrFit3DCFBowlerSinyukov::CorrFit3DCFBowlerSinyukov() :
    CorrFit3DCF(6),
    fCFs(NULL),
    fQinvMap(NULL),
    fTStarZero(kFALSE),
    fRinv(0),
    fAverageBeta(0),
    fQSMode(0),
    fSqrt3(1.0 / TMath::Sqrt(3.0)),
    fFrame(Femto::EKinematics::kLCMS) {
    SetParameterName(Tau(), "#tau");
    FixParameter(Tau(), 0);
  }
  CorrFit3DCFBowlerSinyukov::CorrFit3DCFBowlerSinyukov(Int_t extra_params) :
    CorrFit3DCF(6 + extra_params),
    fCFs(NULL),
    fQinvMap(NULL),
    fTStarZero(kFALSE),
    fRinv(0),
    fAverageBeta(0),
    fQSMode(0),
    fSqrt3(1.0 / TMath::Sqrt(3.0)),
    fFrame(Femto::EKinematics::kLCMS) {
    SetParameterName(Tau(), "#tau");
    FixParameter(Tau(), 0);
  }
  void CorrFit3DCFBowlerSinyukov::LoadMap(TH2D* h, Option_t* opt) {
    TString option = opt;
    if (fCFs) delete fCFs;
    if (Hal::Std::FindParam(option, "krmap")) {
      fCFs = new Spline2D(h);
      //		h->RebinY(4);
      //		h->RebinX(2);
      Cout::Text(Form("Safe range of k* is %4.2f", h->GetXaxis()->GetBinUpEdge(h->GetNbinsX())), "M", kWhite);
      Cout::Text(Form("Safe range of r* is %4.2f", 0.125 * TMath::Abs(h->GetYaxis()->GetBinUpEdge(h->GetNbinsY()))), "M", kWhite);
      // make integration over R from 0.5 to 0.2 r* map with step 0.1 fm
      Double_t map_step = 0.5;
      //		Double_t r_Min = h->GetYaxis()->GetBinLowEdge(1);
      //		Double_t r_Max = h->GetYaxis()->GetBinUpEdge(h->GetNbinsY());
      Double_t r_max = h->GetYaxis()->GetBinUpEdge(h->GetNbinsY()) * 0.125;
      Int_t r_bins   = r_max / map_step;
      TH2D* cf_map   = new TH2D("cf_map",
                              "cf_map",
                              h->GetNbinsX(),
                              h->GetXaxis()->GetBinLowEdge(1),
                              h->GetXaxis()->GetBinUpEdge(h->GetNbinsX()),
                              r_bins,
                              0,
                              r_max);
      Cout::Text("calculating integrals", "M", kWhite);
      Double_t dr2     = fCFs->GetYaxis()->GetBinWidth(1) * 0.5;  // 0.5 dR
      Double_t r_steps = fCFs->GetYaxis()->GetNbins();

      Double_t sqrtpi = TMath::Sqrt(TMath::Pi());
      for (int r_bin = 0; r_bin <= r_bins; r_bin++) {
        Double_t R    = cf_map->GetYaxis()->GetBinCenter(r_bin);
        Double_t norm = 1.0 / (sqrtpi * TMath::Power(R, 3) * 4.0);

        //			Double_t exp_min =
        // TMath::Exp(r_Min*r_Min/(4.0*R*R))*sqrtpi*R; 			Double_t exp_max =
        // TMath::Exp(r_Max*r_Max/(4.0*R*R))*sqrtpi*R; 			Double_t min_int =
        // 0.5*( 1.0-r_Min/exp_min +TMath::Erf(r_Min/(2*R)) ); 			Double_t max_int =
        // 0.5*(r_Max/exp_max +TMath::Erfc(r_Max/(2.*R)));
        for (int i = 1; i <= cf_map->GetNbinsX(); i++) {
          Double_t k     = cf_map->GetXaxis()->GetBinCenter(i);
          Double_t integ = 0;
          for (int j = 1; j <= r_steps; j++) {
            Double_t r_temp = fCFs->GetYaxis()->GetBinCenter(j);
            Double_t m_dr   = fCFs->Eval(k, r_temp - dr2) * norm * Gaus(r_temp - dr2, R);
            Double_t dr     = fCFs->Eval(k, r_temp) * norm * Gaus(r_temp, R);
            Double_t p_dr   = fCFs->Eval(k, r_temp + dr2) * norm * Gaus(r_temp + dr2, R);
            /*
             Double_t m_dr =1*norm*Gaus(r_temp-dr2, R);
             Double_t dr = 1*norm*Gaus(r_temp,R);
             Double_t p_dr = 1*norm*Gaus(r_temp+dr2,R);
             */
            integ += (m_dr + 2.0 * dr + p_dr) * dr2 * 0.5;
            // integ +=2.0*dr*dr2;
          }
          //				Double_t min_r =
          // fCFs->GetYaxis()->GetBinCenter(1); 				Double_t max_r =
          // fCFs->GetYaxis()->GetBinCenter(fCFs->GetYaxis()->GetNbins());

          //				Double_t dif =
          // integ-(TMath::Exp(-4*R*R*25.76578*k*k)+1);
          /*				std::cout<<"m\t"<<min_int<<std::endl;
           std::cout<<"M\t"<<max_int<<std::endl;
           integ+=min_int*fCFs->Eval(min_r, k);
           integ+=max_int*fCFs->Eval(max_r, k);
           */
          cf_map->SetBinContent(i, r_bin, integ);
        }
        Cout::ProgressBar(r_bin, r_bins);
      }
      if (cf_map->GetXaxis()->GetBinUpEdge(0) == 1)
        for (int r_bin = 0; r_bin <= r_bins; r_bin++) {
          cf_map->SetBinContent(0, r_bin, 0);  // zero at q ==0
        }
      Cout::Text("Integrals done", "M", kWhite);
      delete fCFs;
      fCFs = new Spline2D(cf_map, "linear");
      fCFs->Refit();
      if (Hal::Std::FindParam(option, "dump")) {
        TFile* dump = new TFile("dump.root", "recreate");
        cf_map->Write();
        dump->Close();
      } else {
        delete cf_map;
      }
    } else {
      if (Hal::Std::FindParam(option, "lcms")) {
        TH2D* h2 = new TH2D("kstarMap",
                            "kstarMap",
                            h->GetNbinsX(),
                            h->GetXaxis()->GetBinLowEdge(1) * 0.5,
                            h->GetXaxis()->GetBinUpEdge(h->GetNbinsX()) * 0.5,
                            h->GetNbinsY(),
                            h->GetYaxis()->GetBinLowEdge(1),
                            h->GetYaxis()->GetBinUpEdge(h->GetNbinsY()));
        h2->GetYaxis()->SetTitle("R*");
        h2->GetXaxis()->SetTitle("k*");
        for (int i = 0; i <= h->GetNbinsX() + 1; i++) {
          for (int j = 0; j <= h->GetNbinsY() + 1; j++) {
            h2->SetBinContent(i, j, h->GetBinContent(i, j));
            h2->SetBinError(i, j, h->GetBinError(i, j));
          }
        }
        fCFs = new Spline2D(h2);
        delete h2;
      } else {
        TH2D* temp = (TH2D*) h->Clone();
        Hal::Std::HistogramEdges(temp, "x+uv", 0);
        Hal::Std::HistogramEdges(temp, "y+uv", 0);
        Hal::Std::HistogramEdges(temp, "z+uv", 0);
        fCFs = new Spline2D(temp);
        delete temp;
      }
      fCFs->Refit();
      Cout::Text(Form("Safe range of k* is %4.2f", h->GetXaxis()->GetBinUpEdge(h->GetNbinsX())), "M", kWhite);
      Cout::Text(Form("Safe range of r* is %4.2f", TMath::Abs(h->GetYaxis()->GetBinUpEdge(h->GetNbinsY()))), "M", kWhite);
    }
  }

  void CorrFit3DCFBowlerSinyukov::RecalculateFunction() const {
    switch (fFrame) {
      case Femto::EKinematics::kPRF: {
        fRinv =
          TMath::Sqrt(fTempParamsEval[Rout()] * fTempParamsEval[Rout()] + fTempParamsEval[Rside()] * fTempParamsEval[Rside()]
                      + fTempParamsEval[Rlong()] * fTempParamsEval[Rlong()])
          * fSqrt3;
      } break;
      case Femto::EKinematics::kLCMS: {
        Double_t t = fTempParamsEval[Tau()];
        if (fTStarZero) { t = fTempParamsEval[Rout()] * fAverageBeta; }
        Double_t r_out = GetRoutPRF(fTempParamsEval[Rout()], t);
        fRinv          = TMath::Sqrt(r_out * r_out + fTempParamsEval[Rside()] * fTempParamsEval[Rside()]
                            + fTempParamsEval[Rlong()] * fTempParamsEval[Rlong()])
                * fSqrt3;
      } break;
      default: fRinv = 0; break;
    }
  }

  CorrFit3DCFBowlerSinyukov::~CorrFit3DCFBowlerSinyukov() {
    if (fCFs) delete fCFs;
    if (fQinvMap) delete fQinvMap;
  }

  void CorrFit3DCFBowlerSinyukov::Fit(TObject* histo) {
    Femto3DCF* cf = (Femto3DCF*) histo;
    fFrame        = cf->GetFrame();
    switch (fFrame) {
      case Femto::EKinematics::kPRF: break;
      case Femto::EKinematics::kLCMS: {
        Femto3DCFQinv* cfq = dynamic_cast<Femto3DCFQinv*>(cf);
        if (cfq == NULL) {
          if (fQinvMap) delete fQinvMap;
          fQinvMap = NULL;
          Cout::Text("No Qinv Map in LCMS !", "M", kOrange);
        } else {
          TH3D* temp = (TH3D*) cfq->GetQinvHist()->Clone();
          fQinvMap   = new Spline3D(temp, "yes+mid");
          delete temp;
        }
      } break;
      default: break;
    }
    CorrFit3DCF::Fit(histo);
    if (fTStarZero && fFrame == Femto::EKinematics::kLCMS) {
      Double_t t = GetParameter(Rout()) * fAverageBeta;
      fParameters[Tau()].SetFittedValue(t);
      fTempParamsEval[Tau()] = t;
      FixParameter(Tau(), t);
    }
  }

  void CorrFit3DCFBowlerSinyukov::SetAveragePairBeta(Double_t m1, Double_t m2, Double_t avkt) {
    Double_t mpair = m1 + m2;
    avkt           = avkt * 2.0;
    Double_t epair = TMath::Sqrt(avkt * avkt + mpair * mpair);
    fAverageBeta   = avkt / epair;
    fGamma         = 1.0 / (TMath::Sqrt(1 - fAverageBeta * fAverageBeta));
  }

  void CorrFit3DCFBowlerSinyukov::SetAveragePairBeta(Double_t beta) {
    fAverageBeta = beta;
    fGamma       = 1.0 / (TMath::Sqrt(1 - fAverageBeta * fAverageBeta));
  }

  Double_t CorrFit3DCFBowlerSinyukov::GetRoutPRF(Double_t r_out, Double_t tau) const {
    return fGamma * (r_out - fAverageBeta * tau);
  }

  Femto3DCF* CorrFit3DCFBowlerSinyukov::UnBowlerize() const {
    if (fCF == NULL) return NULL;
    Femto3DCF* unbowler = new Femto3DCF(*(Femto3DCF*) fCF);
    ParametersChanged();
    Double_t x[3];
    Double_t* params = new Double_t[GetParametersNo()];
    for (int i = 0; i < GetParametersNo(); i++) {
      params[i] = GetParameter(i);
    }
    params[Norm()] = 1;
    TH3* num       = (TH3*) unbowler->GetNum();
    for (fBinX = 1; fBinX <= unbowler->GetNum()->GetXaxis()->GetNbins(); fBinX++) {
      x[0] = num->GetXaxis()->GetBinCenter(fBinX);
      for (fBinY = 1; fBinY <= unbowler->GetNum()->GetYaxis()->GetNbins(); fBinY++) {
        x[1] = num->GetYaxis()->GetBinCenter(fBinY);
        for (fBinZ = 1; fBinZ <= unbowler->GetNum()->GetZaxis()->GetNbins(); fBinZ++) {
          x[2]         = num->GetZaxis()->GetBinCenter(fBinZ);
          Double_t val = num->GetBinContent(fBinX, fBinY, fBinZ);
          Double_t cf  = EvalCF(x, params);
          if (cf < 0) continue;
          val = val / cf;
          num->SetBinContent(fBinX, fBinY, fBinZ, val);
        }
      }
    }
    delete[] params;
    return unbowler;
  }

  void CorrFit3DCFBowlerSinyukov::FitDummy(TObject* histo) {
    Femto3DCF* cf = (Femto3DCF*) histo;
    fFrame        = cf->GetFrame();
    switch (fFrame) {
      case Femto::EKinematics::kPRF: break;
      case Femto::EKinematics::kLCMS: {
        Femto3DCFQinv* cfq = dynamic_cast<Femto3DCFQinv*>(cf);
        if (cfq == NULL) {
          if (fQinvMap) delete fQinvMap;
          fQinvMap = NULL;
          Cout::Text("No Qinv Map in LCMS !", "M", kOrange);
        } else {
          TH3D* temp = (TH3D*) cfq->GetQinvHist()->Clone();
          fQinvMap   = new Spline3D(temp, "yes+mid");
          delete temp;
        }
      } break;
      default: break;
    }
    CorrFit3DCF::FitDummy(histo);
    if (fTStarZero && fFrame == Femto::EKinematics::kLCMS) {
      Double_t t = GetParameter(Rout()) * fAverageBeta;
      fParameters[Tau()].SetFittedValue(t);
      fTempParamsEval[Tau()] = t;
      FixParameter(Tau(), t);
    }
  }
}  // namespace Hal
