/*
 * CorrFitSHCF.cxx
 *
 *  Created on: 23 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitSHCF.h"
#include "CorrFit.h"
#include "Cout.h"

#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TF1.h>
#include <TH1.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TRandom.h>
#include <TString.h>

#include "FemtoSHCF.h"
#include "FemtoYlm.h"
namespace Hal {
  const Int_t CorrFitSHCF::fgRout   = 0;
  const Int_t CorrFitSHCF::fgRside  = 1;
  const Int_t CorrFitSHCF::fgRlong  = 2;
  const Int_t CorrFitSHCF::fgLambda = 3;
  const Int_t CorrFitSHCF::fgNorm   = 4;

  CorrFitSHCF::CorrFitSHCF(CorrFit3DCF* f) :
    CorrFitFunc(f->GetParametersNo(), 10),
    fL(0),
    fMaxJM(0),
    fSamples(10000),
    fBins(0),
    fIntegralsRe(NULL),
    fIntegralsIm(NULL),
    fX(NULL),
    fY(NULL),
    fZ(NULL),
    fBinMin(NULL),
    fBinMax(NULL),
    fEls(NULL),
    fEms(NULL),
    fElsi(NULL),
    fEmsi(NULL),
    fYlmBuffer(NULL),
    f3d(f) {
    for (int i = 0; i < GetParametersNo(); i++) {
      SetParameterName(i, f3d->GetParameterName(i));
      SetRange(0, 1);
    }
    fCFHistogramsRe = new TClonesArray("TH1D");
    fCFHistogramsIm = new TClonesArray("TH1D");
    fNumeratorsRe   = new TClonesArray("TH1D");
    fNumeratorsIm   = new TClonesArray("TH1D");
    fBinRange[0] = fBinRange[1] = 0;
    fBinMin = fBinMax = NULL;
  }

  void CorrFitSHCF::EstimateActiveBins() {
    fActiveBins = 0;
    for (int i = fBinRange[0]; i < fBinRange[1]; i++) {
      TH1D* T = (TH1D*) fCFHistogramsRe->UncheckedAt(0);
      if (T->GetBinContent(i) < fThreshold) continue;
      for (int j = 0; j < fMaxJM; j++) {
        if (i < fBinMin->Get(j)) continue;
        if (i > fBinMax->Get(j)) continue;
        fActiveBins += 2;
      }
    }
    Double_t free_parameters = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!fParameters[i].IsFixed()) free_parameters++;
    }
    fNDF = fActiveBins - free_parameters;
  }

  double CorrFitSHCF::GetChiTF(const double* par) const {
    Double_t f = 0.0;
    Double_t A, C;
    Double_t e, chi; /* FIXME */
    for (int i = 0; i < GetParametersNo(); i++) {
      fTempParamsEval[i] = par[i];
      if (f3d) f3d->fTempParamsEval[i] = par[i];
    }
    if (f3d) f3d->ParametersChanged();
    ParametersChanged();

    for (int i = fBinRange[0]; i < fBinRange[1]; i++) {
      TH1D* T = (TH1D*) fCFHistogramsRe->UncheckedAt(0);
      Double_t R[1];
      R[0] = T->GetXaxis()->GetBinCenter(i);
      if (T->GetBinContent(i) < fThreshold) continue;
      CalculateCF(R, par);  // calculate CF(el,elm) at R
      for (int j = 0; j < fMaxJM; j++) {
        if (i < fBinMin->Get(j)) continue;
        if (i > fBinMax->Get(j)) continue;
        TH1D* re = (TH1D*) fCFHistogramsRe->UncheckedAt(j);
        TH1D* im = (TH1D*) fCFHistogramsIm->UncheckedAt(i);
        A        = re->GetBinContent(i);
        e        = re->GetBinError(i);
        C        = fYlmBuffer[j].real();
        chi      = A - C;
        f += chi * chi / e * e;

        A   = re->GetBinContent(i);
        e   = re->GetBinError(i);
        C   = fYlmBuffer[j].real();
        chi = A - C;
        f += chi * chi / e * e;

        A   = im->GetBinContent(i);
        e   = im->GetBinError(i);
        C   = fYlmBuffer[j].real();
        chi = A - C;
        f += chi * chi / e * e;
      }
    }
    return f;
  }

  void CorrFitSHCF::ReadParametersName() {
    switch (fKinematics) {
      case Femto::EKinematics::kLCMS:
        // do noting
        break;
      case Femto::EKinematics::kPRF:
        // TODO Improve this replacement
        if (fParameters[Rout()].GetParName().EqualTo("R_{out}")) SetParameterName(Rout(), "R*_{out}");
        if (fParameters[Rside()].GetParName().EqualTo("R_{side}")) SetParameterName(Rside(), "R*_{side}");
        if (fParameters[Rlong()].GetParName().EqualTo("R_{long}")) SetParameterName(Rlong(), "R*_{long}");
        break;
      default: break;
    }
  }

  TF1* CorrFitSHCF::GetFittingFunction(Option_t* /*opt*/) const {
    return new TF1(
      "funcFitted", this, &CorrFitSHCF::GetFun, fRange.Get(0), fRange.Get(1), GetParametersNo() + 3, this->ClassName(), "GetFun");
  }

  Double_t CorrFitSHCF::GetFun(Double_t* x, Double_t* params) const {
    Int_t l  = params[GetParametersNo()];
    Int_t m  = params[GetParametersNo() + 1];
    Int_t re = params[GetParametersNo() + 2];
    Double_t R[1];
    R[0] = x[0];
    CalculateCF(R, params);  // calculate CF(el,elm) at R
    if (re == 1) {
      return fYlmBuffer[GetIndexForLM(l, m)].real();
    } else {
      return fYlmBuffer[GetIndexForLM(l, m)].imag();
    }
  }

  void CorrFitSHCF::SetFuncRange(Double_t x_min, Double_t x_max, Int_t el, Int_t em) {
    Int_t l           = GetIndexForLM(el, em);
    fRange[l * 2]     = x_min;
    fRange[l * 2 + 1] = x_max;
  }

  void CorrFitSHCF::DrawAdv(Option_t* draw_option, Int_t /*width*/) {
    for (int i = 0; i < GetParametersNo(); i++)
      fTempParamsEval[i] = GetParameter(i);
    RecalculateFunction();
    TString option = draw_option;
    if (gPad == NULL) {
      new TCanvas();
      gPad->Clear();
      gPad->Divide(fL + 1, fL + 1);
    }
    TVirtualPad* pad = gPad;
    ((FemtoSHCF*) fCF)->Draw();
    //	Bool_t full = kTRUE;
    Int_t pad_id = 0;
    for (int i = 0; i <= fL; i++) {
      int I = i;
      for (int j = 0; j <= fL; j++) {
        int J = j - i;
        if (j <= i) I = i;
        pad->cd(pad_id++);
        gPad->SetBottomMargin(0.025);
        gPad->SetTopMargin(0.025);
        gPad->SetLeftMargin(0.025);
        gPad->SetRightMargin(0.025);
        TF1* f_re = GetFittingFunction(I, J, kTRUE);
        f_re->SetLineColor(kBlue);
        TF1* f_im = GetFittingFunction(I, J, kTRUE);
        f_im->SetLineColor(kBlue);
        f_re->Draw("SAME");
        f_im->Draw("SAME");
        I++;
      }
    }
  }

  Double_t CorrFitSHCF::Eval(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) { return 0; }

  void CorrFitSHCF::Fit(TObject* histo) {
    fL     = ((FemtoSHCF*) histo)->GetL();
    fMaxJM = (fL + 1) * (fL + 1);
    fCFHistogramsRe->Clear();
    fCFHistogramsIm->Clear();
    fCF = histo;
    PrepareIntegrals();
    CorrFitFunc::Fit(histo);
  }

  void CorrFitSHCF::PrepareIntegrals() {
    Cout::DebugInfo(4);
    if (fIntegralsRe) {
      delete fIntegralsRe;
      delete fIntegralsIm;
      delete fX;
      delete fY;
      delete fZ;
      delete fYlmBuffer;
      delete fBinMin;
      delete fBinMax;
      delete fYlmBuffer;
      delete[] fEls;
      delete[] fEms;
      delete[] fElsi;
      delete[] fEmsi;
    }
    Cout::DebugInfo(5);
    fEls    = new Double_t[fMaxJM];
    fEms    = new Double_t[fMaxJM];
    fElsi   = new Double_t[fMaxJM];
    fEmsi   = new Double_t[fMaxJM];
    int elx = 0;
    int emx = 0;
    int ilx = 0;
    Cout::DebugInfo(6);
    do {
      fEls[ilx]  = elx;
      fEms[ilx]  = emx;
      fElsi[ilx] = (int) elx;
      fEmsi[ilx] = (int) emx;
      emx++;
      ilx++;
      if (emx > elx) {
        elx++;
        emx = -elx;
      }
    } while (elx <= fL);
    Cout::DebugInfo(7);
    fBinMin      = new Array_1<Int_t>(fMaxJM);
    fBinMax      = new Array_1<Int_t>(fMaxJM);
    fBinRange[0] = 1E+4;
    Cout::DebugInfo(8);
    Int_t pos = 0;
    for (int el = 0; el < fL; el++) {
      for (int em = -el; em <= el; em++) {
        TH1D* im                                       = ((FemtoSHCF*) fCF)->GetCFIm(el, em);
        TH1D* re                                       = ((FemtoSHCF*) fCF)->GetCFRe(el, em);
        TH1D* imn                                      = ((FemtoSHCF*) fCF)->GetNumIm(el, em);
        TH1D* ren                                      = ((FemtoSHCF*) fCF)->GetNumRe(el, em);
        *((TH1D*) fCFHistogramsIm->ConstructedAt(pos)) = *im;
        *((TH1D*) fCFHistogramsRe->ConstructedAt(pos)) = *re;
        *((TH1D*) fNumeratorsRe->ConstructedAt(pos))   = *imn;
        *((TH1D*) fNumeratorsIm->ConstructedAt(pos))   = *ren;
        (*fBinMin)[pos]                                = im->GetXaxis()->FindBin(fRange[pos * 2]);
        (*fBinMax)[pos]                                = im->GetXaxis()->FindBin(fRange[pos * 2 + 1]);
        delete imn;
        delete ren;
        delete im;
        delete re;
        fBinRange[0] = TMath::Min(fBinRange[0], (*fBinMin)[pos]);
        fBinRange[1] = TMath::Max(fBinRange[1], (*fBinMax)[pos]);
        pos++;
      }
    }
    Cout::DebugInfo(9);
    fYlmBuffer   = new std::complex<double>[fMaxJM];
    fIntegralsRe = new Array_2<Double_t>(fMaxJM, fSamples);
    fIntegralsIm = new Array_2<Double_t>(fMaxJM, fSamples);
    fX           = new Array_1<Double_t>(fSamples);
    fY           = new Array_1<Double_t>(fSamples);
    fZ           = new Array_1<Double_t>(fSamples);
    Double_t x, y, z;
    Cout::DebugInfo(19);
    for (int i = 0; i < fSamples; i++) {
      gRandom->Sphere(x, y, z, 1.0);
      (*fX)[i]                        = x;
      (*fY)[i]                        = y;
      (*fZ)[i]                        = z;
      std::complex<double>* YlmBuffer = FemtoYlm::YlmUpToL(fL, x, y, z);
      for (int j = 0; j < fMaxJM; j++) {
        (*fIntegralsRe)[j][i] = real(YlmBuffer[j]);
        (*fIntegralsIm)[j][i] = -imag(YlmBuffer[j]);
      }
    }
  }

  Double_t CorrFitSHCF::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t R = x[0] / TMath::Sqrt(3.0);
    Double_t X[3];
    for (int j = 0; j < fMaxJM; j++) {
      Double_t Re = 0;
      Double_t Im = 0;
      for (int i = 0; i < fSamples; i++) {
        X[0]        = fX->Get(i) * R;
        X[1]        = fY->Get(i) * R;
        X[2]        = fZ->Get(i) * R;
        Double_t cf = f3d->CalculateCF(X, params);
        Re += cf * (*fIntegralsRe)[j][i];
        Im += cf * (*fIntegralsRe)[j][i];
      }
      fYlmBuffer[j].real(Re);
      fYlmBuffer[j].real(Im);
    }
    return 0;
  }

  Int_t CorrFitSHCF::GetIndexForLM(int el, int em) const {
    for (int iter = 0; iter < fMaxJM; iter++)
      if ((el == fElsi[iter]) && (em == fEmsi[iter])) return iter;
    return -1;
  }

  TF1* CorrFitSHCF::GetFittingFunction(Int_t el, Int_t em, Bool_t re) const {
    TF1* r = GetFittingFunction("");
    r->FixParameter(GetParametersNo(), el);
    r->FixParameter(GetParametersNo() + 1, em);
    if (re) {
      r->FixParameter(GetParametersNo() + 2, 1);
    } else {
      r->FixParameter(GetParametersNo() + 2, 0);
    }
    return r;
  }

  CorrFitSHCF::~CorrFitSHCF() {
    if (fIntegralsRe) {
      delete fIntegralsRe;
      delete fIntegralsIm;
      delete fX;
      delete fY;
      delete fZ;
      delete fYlmBuffer;
      delete fBinMin;
      delete fBinMax;
      delete[] fEls;
      delete[] fEms;
      delete[] fElsi;
      delete[] fEmsi;
    }
    delete fCFHistogramsRe;
    delete fCFHistogramsIm;
    delete fNumeratorsIm;
    delete fNumeratorsRe;
  }
}  // namespace Hal
