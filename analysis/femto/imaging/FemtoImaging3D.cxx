/*
 * FemtoImaging3D.cxx
 *
 *  Created on: 21 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoImaging3D.h"

#include <TAxis.h>
#include <TH1.h>
#include <TH3.h>
#include <TMath.h>
#include <TString.h>

#include "Cout.h"
#include "FemtoConst.h"
#include "HalStd.h"

namespace Hal {
  FemtoImaging3D::FemtoImaging3D() :
    fQMax {1.0, 1.0, 1.0},
    fQMin {0, 0, 0},
    fRMax {20, 20, 20},
    fRMin {0, 0, 0},
    fBins {20, 20, 20},
    fKinematics(Femto::EKinematics::kLCMS),
    fCF(NULL),
    fR(NULL),
    fSource(NULL) {
    // TODO improve this method
  }

  void FemtoImaging3D::Init() {
    TH3* CF         = (TH3*) fCF->GetHist(kTRUE);
    Double_t min_x  = CF->GetXaxis()->GetBinLowEdge(1);
    Double_t max_x  = CF->GetXaxis()->GetBinUpEdge(CF->GetNbinsX());
    Double_t bins_x = CF->GetNbinsX();
    Double_t min_y  = CF->GetYaxis()->GetBinLowEdge(1);
    Double_t max_y  = CF->GetYaxis()->GetBinUpEdge(CF->GetNbinsY());
    Double_t bins_y = CF->GetNbinsY();
    Double_t min_z  = CF->GetZaxis()->GetBinLowEdge(1);
    Double_t max_z  = CF->GetZaxis()->GetBinUpEdge(CF->GetNbinsZ());
    Double_t bins_z = CF->GetNbinsZ();
    fR              = new TH3D("R", "R", bins_x, min_x, max_x, bins_y, min_y, max_y, bins_z, min_z, max_z);
    for (int i = 1; i <= bins_x; i++) {
      for (int j = 1; j <= bins_y; j++) {
        for (int k = 1; k <= bins_z; k++) {
          fR->SetBinContent(i, j, k, CF->GetBinContent(i, j, k) - 1.0);
          if (CF->GetBinContent(i, j, k) != 0) {
            fR->SetBinError(i, j, k, CF->GetBinError(i, j, k) * fR->GetBinContent(i, j, k) / CF->GetBinContent(i, j, k));
          }
        }
      }
    }
    delete CF;
    fSource =
      new TH3D("Source", "Source", fBins[0], fRMin[0], fRMax[0], fBins[1], fRMin[1], fRMax[1], fBins[2], fRMin[2], fRMax[2]);
    if (fCF->GetFrame() == Femto::EKinematics::kPRF) {
      fSource->GetXaxis()->SetTitle("r*_{out}");
      fSource->GetYaxis()->SetTitle("r*_{side}");
      fSource->GetZaxis()->SetTitle("r*_{long}");
      fKinematics = Femto::EKinematics::kPRF;
    } else if (fCF->GetFrame() == Femto::EKinematics::kLCMS) {
      Cout::PrintInfo("Imaging doesn't work for LCMS ", EInfo::kImportantWarning);
      return;
      fSource->GetXaxis()->SetTitle("r_{out}");
      fSource->GetYaxis()->SetTitle("r_{side}");
      fSource->GetZaxis()->SetTitle("r_{long}");
      fKinematics = Femto::EKinematics::kLCMS;
    }
  }

  void FemtoImaging3D::InverseTransofrm(Option_t* opt) {
    TString option = opt;
    Bool_t fast    = kFALSE;
    if (option.EqualTo("fast")) fast = kTRUE;
    Int_t zeroX = fSource->GetXaxis()->FindBin(0.0);
    Int_t zeroY = fSource->GetYaxis()->FindBin(0.0);
    Int_t zeroZ = fSource->GetZaxis()->FindBin(0.0);
    Int_t isZeroX, isZeroY, isZeroZ;

    for (int i = 1; i <= fSource->GetNbinsX(); i++) {
      Double_t rx = fSource->GetXaxis()->GetBinCenter(i) * Femto::FmToGeV();
      if (i == zeroX) {
        isZeroX = 1;
      } else {
        isZeroX = 0;
      }
      for (int j = 1; j <= fSource->GetNbinsY(); j++) {
        Double_t ry = fSource->GetYaxis()->GetBinCenter(j) * Femto::FmToGeV();
        if (j == zeroY) {
          isZeroY = 1;
        } else {
          isZeroY = 0;
        }
        for (int k = 1; k <= fSource->GetNbinsZ(); k++) {
          if (fast) {
            if (k == zeroZ) {
              isZeroZ = 1;
            } else {
              isZeroZ = 0;
            }
            if ((isZeroX + isZeroY + isZeroZ) < 2) { continue; }
          }

          Double_t rz = fSource->GetZaxis()->GetBinCenter(k) * Femto::FmToGeV();
          // compute SR for signle cell
          Double_t sum    = 0.0;
          Double_t errors = 0.0;
          // integral over momentum
          for (int a = fR->GetXaxis()->FindBin(fQMin[0]); a <= fR->GetXaxis()->FindBin(fQMax[0]); a++) {
            Double_t qx = fR->GetXaxis()->GetBinCenter(a);
            Double_t QX = qx * rx;
            for (int b = fR->GetYaxis()->FindBin(fQMin[1]); b <= fR->GetYaxis()->FindBin(fQMax[1]); b++) {
              Double_t qy = fR->GetYaxis()->GetBinCenter(b);
              Double_t QY = qy * ry;
              for (int c = fR->GetZaxis()->FindBin(fQMin[2]); c <= fR->GetZaxis()->FindBin(fQMax[2]); c++) {
                Double_t qz = fR->GetZaxis()->GetBinCenter(c);
                Double_t QZ = qz * rz;
                Double_t R  = fR->GetBinContent(a, b, c);
                if (R == 0) continue;
                Double_t dR       = fR->GetBinError(a, b, c);
                Double_t integral = TMath::Cos(2.0 * (QX + QY + QZ));
                sum += R * integral;
                errors = dR * integral;
              }
            }
          }
          //------------- end of integral ------------
          Double_t ex = rx * fR->GetXaxis()->GetBinWidth(1);
          Double_t ey = ry * fR->GetYaxis()->GetBinWidth(1);
          Double_t ez = rz * fR->GetZaxis()->GetBinWidth(1);
          if (sum < 0) { sum = 0; }
          Double_t dFourierX = (0.05 + ex * 0.22 + ex * ex + 0.084) * sum;
          Double_t dFourierY = (0.05 + ey * 0.22 + ey * ey + 0.084) * sum;
          Double_t dFourierZ = (0.05 + ez * 0.22 + ez * ez + 0.084) * sum;
          //	std::cout<<sum<<std::endl;
          errors = TMath::Sqrt(errors * errors + dFourierX * dFourierX + dFourierY * dFourierY + dFourierZ * dFourierZ);
          fSource->SetBinContent(i, j, k, sum);
          fSource->SetBinError(i, j, k, errors);
        }
      }
      Cout::ProgressBar(i, fSource->GetNbinsX());
    }
    delete fR;
  }

  TH3D* FemtoImaging3D::Inverse(Femto3DCF* cf, Option_t* opt) {
    fCF = cf;
    Init();
    InverseTransofrm(opt);
    Double_t norm = fSource->Integral(1, fSource->GetNbinsX(), 1, fSource->GetNbinsY(), 1, fSource->GetNbinsZ(), "width");
    fSource->Scale(1.0 / norm);
    TH3D* histo = (TH3D*) fSource->Clone();
    return histo;
  }

  void FemtoImaging3D::SetRAxis(Int_t bins, Double_t min, Double_t max, Option_t* opt) {
    Int_t option = 0;
    if (opt[0] == 'x') {
      option = 0;
    } else if (opt[0] == 'y') {
      option = 1;
    } else if (opt[0] == 'z') {
      option = 2;
    } else {
      return;
    }
    fBins[option] = bins;
    fRMin[option] = min;
    fRMax[option] = max;
  }

  void FemtoImaging3D::SetQMax(Double_t qmax, Option_t* opt) {
    Int_t option = 0;
    if (opt[0] == 'x') {
      option = 0;
    } else if (opt[0] == 'y') {
      option = 1;
    } else if (opt[0] == 'z') {
      option = 2;
    } else {
      return;
    }
    fQMax[option] = qmax;
  }

  void FemtoImaging3D::SetQMin(Double_t qmin, Option_t* opt) {
    Int_t option = 0;
    if (opt[0] == 'x') {
      option = 0;
    } else if (opt[0] == 'y') {
      option = 1;
    } else if (opt[0] == 'z') {
      option = 2;
    } else {
      return;
    }
    fQMin[option] = qmin;
  }

  FemtoImaging3D::~FemtoImaging3D() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
