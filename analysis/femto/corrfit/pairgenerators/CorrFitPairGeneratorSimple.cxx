/*
 * CorrFitPairGeneratorSimple.cxx
 *
 *  Created on: 27 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitPairGeneratorSimple.h"
#include "CorrFitMapGroupConfig.h"

#include "Array.h"
#include "CorrFitPairFile.h"
#include "Cout.h"
#include "FemtoMiniPair.h"

#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TRandom.h>

namespace Hal {

  void CorrFitPairGeneratorSimple::GenerateEvent() {
    if (fLimitsN[0] > fBinLimit) return;
    fLimitsN[0] += 100;
    Double_t min   = fGrouping.GetMin();
    Double_t max   = fGrouping.GetMax();
    Double_t scale = 1.0;
    if (fFrame == Femto::EKinematics::kLCMS) { scale = 0.5; }
    min = min * scale;
    max = max * scale;
    TLorentzVector p1, p2;
    Double_t x, y, z;
    FemtoMicroPair* pair = nullptr;
    auto build           = [&]() {
      p1.SetXYZM(x, y, z, fM1);
      p2.SetXYZM(-x, -y, -z, fM2);
      p1.Boost(0.2, 0, 0.0);
      p2.Boost(0.2, 0, 0.0);
      pair->SetMomenta1(p1.X(), p1.Y(), p1.Z(), p1.E());
      pair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.E());
      pair->SetMomenta2(p2.X(), p2.Y(), p2.Z(), p2.E());
      pair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.E());
      pair->SetPdg1(fPid1);
      pair->SetPdg2(fPid2);
    };


    if (fGrouping.GroupByKStar()) {
      const Int_t defSize = 100;
      for (int bin = 0; bin < fGrouping.GetNbins(); bin++) {
        const Double_t kstar = fCentersX[bin] * scale;
        auto vec             = fPairFile->GetSignal(bin);
        for (int i = 0; i < defSize; i++) {
          pair = (FemtoMicroPair*) vec->UncheckedAt(i);
          gRandom->Sphere(x, y, z, kstar);
          build();
        }
      }
    } else {
      const Int_t defSize = 1;
      const int binY      = fCentersY.GetSize();
      const int binX      = fCentersX.GetSize();
      for (int bin = 0; bin < fGrouping.GetNbins(); bin++) {
        auto vec = fPairFile->GetSignal(bin);
        vec->ExpandCreateFast(defSize * binY * binX);
        int count = 0;
        for (int biny = 0; biny < binY; biny++) {
          for (int binx = 0; binx < binX; binx++) {
            pair = (FemtoMicroPair*) vec->UncheckedAt(count++);
            z    = fCentersZ[bin] * scale;
            y    = fCentersY[biny] * scale;
            x    = fCentersX[binx] * scale;
            if (gRandom->Uniform() < 0.5) x = -x;
            if (gRandom->Uniform() < 0.5) y = -y;
            if (gRandom->Uniform() < 0.5) z = -z;
            build();
          }
        }
      }
    } /* finished 3d generating*/
  }

  Bool_t CorrFitPairGeneratorSimple::Init() {
    auto val = CorrFitPairGenerator::Init();
    if (fFrame != Hal::Femto::EKinematics::kPRF)
      Hal::Cout::PrintInfo(Form("CorrFitPairGeneratorSimple works correctly only with PRF frame"), EInfo::kError);
    return val;
  }

} /* namespace Hal */
