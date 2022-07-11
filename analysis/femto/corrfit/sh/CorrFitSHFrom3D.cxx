/*
 * CorrFitSHFrom3D.cxx
 *
 *  Created on: 7 lip 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitSHFrom3D.h"

#include "FemtoSHCF.h"
#include "FemtoYlm.h"
#include "FemtoYlmIndexes.h"

#include <complex>

#include <RtypesCore.h>
#include <TF1.h>
#include <TF2.h>
#include <TMath.h>

namespace Hal {

  CorrFitSHFrom3D::CorrFitSHFrom3D(Int_t par) : CorrFitSHCF(par), fHarmonic(nullptr) {
    fHarmonic = new TF2("Harm", this, &CorrFitSHFrom3D::Legendres_Fx, -10, 10, -10, 10, GetParametersNo() + 2);
  }

  Double_t CorrFitSHFrom3D::Legendres_Fx(Double_t* x, Double_t* par) const {
    Double_t cartesianQ[3];
    Double_t sphericalQ[3] = {par[1], x[0], x[1]};
    ShToCart(cartesianQ, sphericalQ);  // r costheta, phi
    Double_t fx                     = Calc3DCartesian(cartesianQ, par + 2);
    std::complex<double>* YlmBuffer = Hal::FemtoYlm::YlmUpToL(fLmVals.GetL(), x[0], x[1]);
    Int_t index                     = par[0];
    Double_t val                    = fx * YlmBuffer[index].real();
    return val;
  }

  void CorrFitSHFrom3D::ShToCart(Double_t* cart, Double_t* sph) const {
    Double_t cost = sph[1];
    Double_t rho  = sph[0];
    Double_t sint = TMath::Sqrt(1 - cost * cost);
    cart[0]       = TMath::Cos(sph[2]) * sint * rho;
    cart[1]       = TMath::Sin(sph[2]) * sint * rho;
    cart[2]       = cost * rho;
  }

  Double_t CorrFitSHFrom3D::Calc3DCartesian(Double_t* x, Double_t* par) const {
    Double_t qx = x[0];
    Double_t qy = x[1];
    Double_t qz = x[2];
    Double_t Ro = par[0];
    Double_t Rs = par[1];
    Double_t Rl = par[2];
    Double_t rx = qx * qx * Ro * Ro;
    Double_t ry = qy * qy * Rs * Rs;
    Double_t rz = qz * qz * Rl * Rl;
    Double_t s  = 5.0760003;
    return 1 + par[Lambda()] * TMath::Exp(-(rx + ry + rz) * s * s);
  }

  Double_t CorrFitSHFrom3D::Integral(Double_t q, Double_t ilm) const {
    const Double_t norm = 1.0 / TMath::Sqrt(4.0 * TMath::Pi());
    fHarmonic->FixParameter(1, q);
    fHarmonic->FixParameter(0, ilm);
    Double_t v = fHarmonic->Integral(-1, 1, -TMath::Pi(), TMath::Pi()) * norm;
    return v;
  }

  CorrFitSHFrom3D::~CorrFitSHFrom3D() {
    if (fHarmonic) delete fHarmonic;
    // TODO Auto-generated destructor stub
  }

  Double_t CorrFitSHFrom3D::CalculateCF(const Double_t* x, const Double_t* params) const {

    // set "global" parameters
    for (int i = 0; i < GetParametersNo(); i++) {
      fHarmonic->FixParameter(i + 2, params[i]);
    }
    // x[0] - q inv , params - parameters
    for (int i = 0; i < fMaxJM; i++) {
      fYlmBuffer[i].real(Integral(x[0], i));
    }
    return 0;
  }


} /* namespace Hal */
