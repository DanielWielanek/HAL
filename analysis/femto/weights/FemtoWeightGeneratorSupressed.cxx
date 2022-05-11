/*
 * FemtoWeightSupressed.cxx
 *
 *  Created on: 22 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoWeightGeneratorSupressed.h"

#include "Package.h"
#include "Parameter.h"

#include <RtypesCore.h>
#include <TRandom.h>

namespace Hal {
  FemtoWeightGeneratorSupressed& FemtoWeightGeneratorSupressed::operator=(const FemtoWeightGeneratorSupressed& other) {
    if (this != &other) {
      FemtoWeightGenerator::operator=(other);
      if (fWeightGenerator) delete fWeightGenerator;
      fWeightGenerator = (FemtoWeightGenerator*) other.fWeightGenerator->Clone();
    }
    return *this;
  }

  Double_t FemtoWeightGeneratorSupressed::GenerateWeight(FemtoPair* pair) {
    if (gRandom->Rndm() > fSupressionFactor) { return fWeightGenerator->GenerateWeight(pair); }
    return 1;
  }

  FemtoWeightGeneratorSupressed::FemtoWeightGeneratorSupressed(const FemtoWeightGenerator& weight) :
    FemtoWeightGenerator(), fSupressionFactor(0) {
    fWeightGenerator = weight.MakeCopy();
  }

  void FemtoWeightGeneratorSupressed::SetSupressionFactor(Double_t factor) {
    if (factor >= 0 && factor <= 1) fSupressionFactor = factor;
  }

  Bool_t FemtoWeightGeneratorSupressed::Init(Int_t task_id, FemtoPair* pair) {
    if (fWeightGenerator == nullptr) return kFALSE;
    return fWeightGenerator->Init(task_id, pair);
  }

  FemtoWeightGeneratorSupressed::~FemtoWeightGeneratorSupressed() {
    if (fWeightGenerator) delete fWeightGenerator;
  }

  Package* FemtoWeightGeneratorSupressed::Report() const {
    Package* report = new Package(this);
    report->AddObject(fWeightGenerator->Report());
    report->AddObject(new ParameterDouble("SupressionFactor", fSupressionFactor));
    return report;
  }

  FemtoWeightGeneratorSupressed::FemtoWeightGeneratorSupressed(const FemtoWeightGeneratorSupressed& other) :
    FemtoWeightGenerator(other), fWeightGenerator(nullptr) {
    if (other.fWeightGenerator) { fWeightGenerator = other.fWeightGenerator->MakeCopy(); }
    fSupressionFactor = other.fSupressionFactor;
  }
}  // namespace Hal
