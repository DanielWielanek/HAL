/*
 * FemtoFreezoutGenerator.cxx
 *
 *  Created on: 26-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoFreezoutGenerator.h"
#include "FemtoPair.h"
#include <TLorentzVector.h>
#include <TMath.h>

#include "Cout.h"
#include "FemtoSourceModelGauss.h"
#include "Std.h"
#include "Package.h"
#include "Parameter.h"

namespace Hal {
  FemtoFreezoutGenerator::FemtoFreezoutGenerator() : fModel(nullptr) {}

  FemtoFreezoutGenerator::FemtoFreezoutGenerator(const FemtoSourceModel& source) {
    fModel = (FemtoSourceModel*) source.MakeCopy();
  }

  void FemtoFreezoutGenerator::SetSourceModel(const FemtoSourceModel& model) {
    if (fModel) delete fModel;
    fModel = model.MakeCopy();
  }

  void FemtoFreezoutGenerator::GenerateFreezoutCooordinates(FemtoPair* Pair) {
    fModel->GenerateCoordinates(Pair);
    Boost(Pair);
  }

  void FemtoFreezoutGenerator::Init() {
    if (fModel == NULL) {
      Cout::PrintInfo("No freezout model !", EInfo::kLowWarning);
      fModel = new FemtoSourceModelGauss1D();
    }
  }

  Package* FemtoFreezoutGenerator::Report() const {
    Package* pack = new Package(this);
    pack->AddObject(fModel->Report());
    return pack;
  }

  FemtoFreezoutGenerator::~FemtoFreezoutGenerator() {
    if (fModel) delete fModel;
  }

  FemtoFreezoutGeneratorLCMS::FemtoFreezoutGeneratorLCMS() {}

  void FemtoFreezoutGeneratorLCMS::Boost(FemtoPair* pair) {
    // like in AliFemtoModelGausLCMSFreezeOutGenerator
    // std::cout<<"boost lscm"<<std::endl;
    // std::cout<<pair->Px2()<<std::endl;
    Double_t tPx = pair->TruePx1() + pair->TruePx2();
    Double_t tPy = pair->TruePy1() + pair->TruePy2();
    Double_t tPz = pair->TruePz1() + pair->TruePz2();
    Double_t tE1 = pair->TrueE1();
    Double_t tE2 = pair->TrueE2();
    Double_t tEs = tE1 + tE2;
    // std::cout<<"booster "<<std::endl;
    // pair->PrintInfo();
    Double_t tPt    = sqrt(tPx * tPx + tPy * tPy);
    Double_t tRout  = fModel->GetROut();
    Double_t tRside = fModel->GetRSide();
    Double_t tRlong = fModel->GetRLong();
    // std::cout<<" too boost "<<tRout<<" "<<tRside<<" "<<tRlong<<std::endl;
    //-----------------------------------------------------------------
    // tPy = -tPy;
    // X1.Boost(0,0,tPz/tEs);
    // tRout = gRandom->Gaus(0,1);
    Double_t tXout  = (tPx * tRout + tPy * tRside) / tPt;
    Double_t tXside = (tPy * tRout - tPx * tRside) / tPt;
    // why ?
    // tXout = (tRout*tPx - tRside*tPy)/tPt;
    // tXside = (tRside*tPx + tRout*tPy)/tPt;
    Double_t tBetaz  = tPz / tEs;
    Double_t tGammaz = 1.0 / TMath::Sqrt(1 - tBetaz * tBetaz);

    Double_t tXlong = tGammaz * (tRlong + tBetaz * 0.0);
    Double_t tXtime = tGammaz * (0.0 + tBetaz * tRlong);

    pair->SetX1(0);
    pair->SetY1(0);
    pair->SetZ1(0);
    pair->SetT1(0);
    pair->SetX2(tXout);
    pair->SetY2(tXside);
    pair->SetZ2(tXlong);
    pair->SetT2(tXtime);

#ifdef _kinetic_debug
    std::cout << "source generator" << std::endl;
    pair->PrintInfo();
#endif
  }

  Package* FemtoFreezoutGeneratorLCMS::Report() const {
    Package* pack = FemtoFreezoutGenerator::Report();
    pack->AddObject(new ParameterString("source kinematics", "LCMS"));
    return pack;
  }

  FemtoFreezoutGeneratorLCMS::~FemtoFreezoutGeneratorLCMS() {}

  FemtoFreezoutGeneratorPRF::FemtoFreezoutGeneratorPRF() {
    // Cout::CriticalError("PRF freezeout generator Not ready yet");
  }

  void FemtoFreezoutGeneratorPRF::Boost(FemtoPair* pair) {
    Double_t tPx = pair->TruePx1() + pair->TruePx2();
    Double_t tPy = pair->TruePy1() + pair->TruePy2();
    Double_t tPz = pair->TruePz1() + pair->TruePz2();
    Double_t tE1 = pair->TrueE1();
    Double_t tE2 = pair->TrueE2();
    Double_t tEs = tE1 + tE2;

    Double_t tRout  = fModel->GetROut();
    Double_t tRside = fModel->GetRSide();
    Double_t tRlong = fModel->GetRLong();
    // hardcore going to lab coordintates
    // TODO speed up this part
    TLorentzVector P(tPx, tPy, tPz, tEs);
    P.Boost(0, 0, -tPz / tEs);
    TLorentzVector X(-tRout, -tRside, -tRlong, 0);
    X.Boost(P.Pt() / P.E(), 0, 0);
    X.RotateZ(TMath::ATan2(tPy, tPx));
    X.Boost(0, 0, tPz / tEs);
    pair->SetX1(0);
    pair->SetY1(0);
    pair->SetZ1(0);
    pair->SetT1(0);
    pair->SetX2(X.X());
    pair->SetY2(X.Y());
    pair->SetZ2(X.Z());
    pair->SetT2(X.T());
    // Xx.Print();
    // X.Print();
  }

  Package* FemtoFreezoutGeneratorPRF::Report() const {
    Package* pack = FemtoFreezoutGenerator::Report();
    pack->AddObject(new ParameterString("source kinematics", "PRF"));
    return pack;
  }

  FemtoFreezoutGeneratorPRF::~FemtoFreezoutGeneratorPRF() {}

  FemtoFreezoutGenerator* FemtoFreezoutGeneratorLCMS::MakeCopy() const { return new FemtoFreezoutGeneratorLCMS(*this); }

  FemtoFreezoutGenerator* FemtoFreezoutGeneratorPRF::MakeCopy() const { return new FemtoFreezoutGeneratorPRF(*this); }

  FemtoFreezoutGenerator::FemtoFreezoutGenerator(const FemtoFreezoutGenerator& generator) : TObject(generator) {
    if (generator.fModel) fModel = generator.fModel->MakeCopy();
  }

  FemtoFreezoutGeneratorLCMS::FemtoFreezoutGeneratorLCMS(const FemtoFreezoutGeneratorLCMS& generator) :
    FemtoFreezoutGenerator(generator) {}

  FemtoFreezoutGeneratorLCMS::FemtoFreezoutGeneratorLCMS(const FemtoSourceModel& model) : FemtoFreezoutGenerator(model) {}

  FemtoFreezoutGeneratorPRF::FemtoFreezoutGeneratorPRF(const FemtoFreezoutGeneratorPRF& generator) :
    FemtoFreezoutGenerator(generator) {}

  FemtoFreezoutGeneratorPRF::FemtoFreezoutGeneratorPRF(const FemtoSourceModel& model) : FemtoFreezoutGenerator(model) {}

  void FemtoFreezoutGenerator::Print(Option_t* option) const {
    Cout::Text(ClassName(), "L");
    if (fModel) {
      fModel->Print();
    } else {
      Cout::Text("No model!", "L");
    }
  }
}  // namespace Hal
