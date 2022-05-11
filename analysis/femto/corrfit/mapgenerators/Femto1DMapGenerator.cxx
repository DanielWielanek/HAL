/*
 * Femto1DMapGenerator.cxx
 *
 *  Created on: 26 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Femto1DMapGenerator.h"

#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "FemtoFreezoutGenerator.h"
#include "FemtoPair.h"
#include "FemtoWeightGenerator.h"
#include "HalStd.h"

#include <TDatabasePDG.h>
#include <TFile.h>
#include <iostream>

namespace Hal {
  Femto1DMapGenerator::Femto1DMapGenerator(Bool_t fake) :
    fRMin(0.5),
    fRMax(10.5),
    fKStarMin(0),
    fKStarMax(1.0),
    fMass12(-1),
    fMass22(-1),
    fRBins(10),
    fKStarBins(100),
    fPid1(0),
    fPid2(0),
    fIgnoreSign(kFALSE),
    fUseFake(fake),
    fKinematics(Femto::EKinematics::kPRF),
    fPair(nullptr),
    fGenerator(nullptr),
    fWeight(nullptr),
    fMap(nullptr) {}

  Femto1DMapGenerator::Femto1DMapGenerator() : Femto1DMapGenerator(kFALSE) {}

  void Femto1DMapGenerator::SetCorrFctn(const Femto1DCF& cf) {
    fKinematics = cf.GetFrame();
    fKStarBins  = cf.GetNum()->GetXaxis()->GetNbins();
    fKStarMin   = cf.GetNum()->GetXaxis()->GetBinLowEdge(1);
    fKStarMax   = cf.GetNum()->GetXaxis()->GetBinUpEdge(fKStarBins);
  }

  void Femto1DMapGenerator::SetPid(Int_t pid1, Int_t pid2) {
    TDatabasePDG* pid = TDatabasePDG::Instance();
    TParticlePDG* p1  = pid->GetParticle(pid1);
    TParticlePDG* p2  = pid->GetParticle(pid2);
    if (p1 == nullptr) {
      Cout::PrintInfo(Form("Femto1DMapGenerator::SetPid cannot find PID1 = %i", pid1), EInfo::kLessWarning);
      return;
    };
    if (p2 == nullptr) {
      Cout::PrintInfo(Form("Femto1DMapGenerator::SetPid cannot find PID1 = %i", pid2), EInfo::kLessWarning);
      return;
    };
    fPid1   = pid1;
    fPid2   = pid2;
    fMass12 = p1->Mass() * p1->Mass();
    fMass22 = p2->Mass() * p2->Mass();
  }

  void Femto1DMapGenerator::SetRBins(Int_t bins, Double_t rmin, Double_t rmax, Bool_t center) {
    if (center) {
      fRBins = bins + 1;
      fRadiiBins.MakeBigger(bins);
      Double_t dr = (rmax - rmin) / (Double_t)(bins);
      fRMin       = rmin - dr * 0.5;
      fRMax       = rmax + dr * 0.5;
      for (int i = 0; i < fRBins; i++) {
        fRMin = rmin + dr * i;
      }
    } else {
      fRBins = bins;
      fRadiiBins.MakeBigger(bins);
      fRMin       = rmin;
      fRMax       = rmax;
      Double_t dr = (fRMax - fRMin) / (Double_t)(bins - 1);
      for (int i = 0; i < fRBins; i++) {
        fRadiiBins[i] = fRMin + dr * i + dr * 0.5;
      }
    }
  }

  void Femto1DMapGenerator::SetGenerator(FemtoFreezoutGenerator& gen) {
    if (fGenerator) delete fGenerator;
    fGenerator = gen.MakeCopy();
  }

  void Femto1DMapGenerator::SetWeightGenerator(FemtoWeightGenerator& calc) {
    if (fWeight) delete fWeight;
    fWeight = calc.MakeCopy();
  }

  Bool_t Femto1DMapGenerator::Init() {
    fPair = Femto::MakePair(fKinematics, fUseFake);
    fMap  = new DividedHisto2D("map", fKStarBins, fKStarMin, fKStarMax, fRBins, fRMin, fRMax, 'D');
    if (fWeight == nullptr) {
      Cout::PrintInfo("Femto1DMapGenerator::Init lack of weight", EInfo::kImportantError);
      return kFALSE;
    }
    if (fGenerator == nullptr) {
      Cout::PrintInfo("Femto1DMapGenerator::Init lack of generator", EInfo::kImportantError);
      return kFALSE;
    }
    fPair->SetPdg1(fPid1);
    fPair->SetPdg2(fPid2);
    if (fIgnoreSign) fPair->UseAbs();
    return kTRUE;
  }

  void Femto1DMapGenerator::SaveMap(TString filename) {
    TFile* f                     = new TFile(filename, "recreate");
    CorrFitMapKstarRstarDiv* map = new CorrFitMapKstarRstarDiv(*fMap, fKinematics);
    map->Write("map");
    f->Close();
  }

  Femto1DMapGenerator::~Femto1DMapGenerator() {
    if (fPair) delete fPair;
    if (fGenerator) delete fGenerator;
    if (fWeight) delete fWeight;
    if (fMap) delete fMap;
  }
}  // namespace Hal
