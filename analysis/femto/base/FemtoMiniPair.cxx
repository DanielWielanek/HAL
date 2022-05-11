/*
 * FemtoMiniPair.cxx
 *
 *  Created on: 9 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoMiniPair.h"
#include <iostream>

namespace Hal {
  FemtoMiniPair::FemtoMiniPair() :
    FemtoMicroPair(), fX1(0.0), fY1(0.0), fZ1(0.0), fT1(0.0), fX2(0.0), fY2(0.0), fZ2(0.0), fT2(0.0) {}

  FemtoMiniPair::~FemtoMiniPair() {
    // TODO Auto-generated destructor stub
  }

  void FemtoMiniPair::PrintInfo() const {
    std::cout << "pair info" << std::endl;
    Double_t m1 = TMath::Sqrt(fE1 * fE1 - fPx1 * fPx1 - fPy1 * fPy1 - fPz1 * fPz1);
    Double_t m2 = TMath::Sqrt(fE2 * fE2 - fPx2 * fPx2 - fPy2 * fPy2 - fPz2 * fPz2);
    printf("momenta (%.4f %.4f %.4f %.4f) (%.4f %.4f %.4f %.4f) m1 = %.4f m2 = %.4f "
           "\n",
           GetPx1(),
           GetPy1(),
           GetPz1(),
           GetE1(),
           GetPx2(),
           GetPy2(),
           GetPz2(),
           GetE2(),
           m1,
           m2);
    printf("True momenta (%.4f %.4f %.4f %.4f) (%.4f %.4f %.4f %.4f) m1 = %.4f m2 = "
           "%.4f \n",
           TruePx1(),
           TruePy1(),
           TruePz1(),
           TrueE1(),
           TruePx2(),
           TruePy2(),
           TruePz2(),
           TrueE2(),
           m1,
           m2);
    printf("coords (%.4f %.4f %.4f %.4f) (%.4f %.4f %.4f %.4f)\n",
           GetX1(),
           GetY1(),
           GetZ1(),
           GetT1(),
           GetX2(),
           GetY2(),
           GetZ2(),
           GetT2());
  }

  FemtoMiniPair& FemtoMiniPair::operator=(const FemtoPair& other) {
    fPx1  = other.TruePx1();
    fPx2  = other.TruePx2();
    fPy1  = other.TruePy1();
    fPy2  = other.TruePy2();
    fPz1  = other.TruePz1();
    fPz2  = other.TruePz2();
    fE1   = other.TrueE1();
    fE2   = other.TrueE2();
    fpx1  = other.GetPx1();
    fpx2  = other.GetPx2();
    fpy1  = other.GetPy1();
    fpy2  = other.GetPy2();
    fpz1  = other.GetPz1();
    fpz2  = other.GetPz2();
    fe1   = other.TrueE1();
    fe2   = other.TrueE2();
    fPdg1 = other.GetPdg1();
    fPdg2 = other.GetPdg2();
    fX1   = other.GetX1();
    fX2   = other.GetX2();
    fY1   = other.GetY1();
    fY2   = other.GetY2();
    fZ1   = other.GetZ1();
    fZ2   = other.GetZ2();
    fT1   = other.GetT1();
    fT2   = other.GetT2();
    return *this;
  }

  //==================================================
  FemtoPicoPair::FemtoPicoPair() : fPx1(0.0), fPy1(0.0), fPz1(0.0), fE1(0.0), fPx2(0.0), fPy2(0.0), fPz2(0.0), fE2(0.0) {}

  FemtoPicoPair& FemtoPicoPair::operator=(const FemtoPicoPair& other) {
    fPx1 = other.TruePx1();
    fPx2 = other.TruePx2();
    fPy1 = other.TruePy1();
    fPy2 = other.TruePy2();
    fPz1 = other.TruePz1();
    fPz2 = other.TruePz2();
    fE1  = other.TrueE1();
    fE2  = other.TrueE2();
    return *this;
  }

  FemtoPicoPair::~FemtoPicoPair() {}

  //==================================================
  FemtoMicroPair::FemtoMicroPair() :
    FemtoPicoPair(), fpx1(0.0), fpy1(0.0), fpz1(0.0), fe1(0.0), fpx2(0.0), fpy2(0.0), fpz2(0.0), fe2(0.0), fPdg1(0), fPdg2(0) {}

  void FemtoMicroPair::SetTrueMomenta1(Float_t px, Float_t py, Float_t pz, Float_t e) {
    fPx1 = px;
    fPy1 = py;
    fPz1 = pz;
    fE1  = e;
  }

  void FemtoMicroPair::SetTrueMomenta2(Float_t px, Float_t py, Float_t pz, Float_t e) {
    fPx2 = px;
    fPy2 = py;
    fPz2 = pz;
    fE2  = e;
  }

  void FemtoMicroPair::SetMomenta1(Float_t px, Float_t py, Float_t pz, Float_t e) {
    fpx1 = px;
    fpy1 = py;
    fpz1 = pz;
    fe1  = e;
  }

  void FemtoMicroPair::SetMomenta2(Float_t px, Float_t py, Float_t pz, Float_t e) {
    fpx2 = px;
    fpy2 = py;
    fpz2 = pz;
    fe2  = e;
  }

  FemtoMicroPair& FemtoMicroPair::operator=(const FemtoPair& other) {
    fPx1  = other.TruePx1();
    fPx2  = other.TruePx2();
    fPy1  = other.TruePy1();
    fPy2  = other.TruePy2();
    fPz1  = other.TruePz1();
    fPz2  = other.TruePz2();
    fE1   = other.TrueE1();
    fE2   = other.TrueE2();
    fpx1  = other.GetPx1();
    fpx2  = other.GetPx2();
    fpy1  = other.GetPy1();
    fpy2  = other.GetPy2();
    fpz1  = other.GetPz1();
    fpz2  = other.GetPz2();
    fe1   = other.TrueE1();
    fe2   = other.TrueE2();
    fPdg1 = other.GetPdg1();
    fPdg2 = other.GetPdg2();
    return *this;
  }

  FemtoMicroPair::~FemtoMicroPair() {}
}  // namespace Hal
