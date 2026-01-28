/*
 * CorrFitFunctor.cxx
 *
 *  Created on: 18 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitFunctor.h"

#include "CorrFitInfo.h"
#include "Cout.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TFile.h>
#include <TTree.h>

#include <iostream>

namespace Hal {
  CorrFitFunctor::CorrFitFunctor(TString map_file) :
    fMapFile(map_file), fInfo(nullptr), fSetup(), fChain(nullptr), fFile(nullptr), fParams(0), fData(nullptr) {
    fFile  = new TFile(fMapFile);
    fChain = (TTree*) fFile->Get("map");
    fInfo  = (CorrFitInfo*) fFile->Get("Info");
    fChain->SetBranchAddress("data", &fData);
    fSetup     = fInfo->GetSetup();
    fParams    = fSetup.GetNParams();
    fDimsArray = fSetup.GetDimensions();
    fPosArray.resize(fSetup.GetNParams());
  }

  Bool_t CorrFitFunctor::GetParameterConfig(TString name, Double_t& min, Double_t& max, Int_t& point) const {
    int paramId = -1;
    for (int i = 0; i < fSetup.GetNParams(); i++) {
      if (fSetup.GetParName(i) == name) {
        paramId = i;
        break;
      }
    }
    if (paramId == -1) return kFALSE;
    min   = fSetup.GetMin(paramId);
    max   = fSetup.GetMax(paramId);
    name  = fSetup.GetParName(paramId);
    point = fSetup.GetNPoints(paramId);
    return kTRUE;
  }

  Int_t CorrFitFunctor::GetNParams() const { return fInfo->GetSetup().GetNParams(); }

  void CorrFitFunctor::PrintInfo() {
    Cout::Text("CorrFitFunctor", "M", kWhite);
    fInfo->GetCf()->Print();
    Cout::Text("Params conf", "L", kWhite);
    Cout::Database({"Name", "Steps", "Nmin", "Nmax"});
    for (int i = 0; i < fSetup.GetNParams(); i++) {
      TString steps = Form("%i", fSetup.GetNSteps(i));
      TString min   = Form("%4.3f", fSetup.GetMin(i));
      TString max   = Form("%4.3f", fSetup.GetMax(i));
      Cout::Database({fSetup.GetParName(i), steps, min, max});
    }
  }

  Array_1<Float_t>* CorrFitFunctor::GetData(Double_t* params) {
    Int_t entry_val = 0;
    // Int_t prestep   = 1;
    for (int i = 0; i < fSetup.GetNParams(); i++) {
      Double_t par      = params[i];
      Double_t dif      = par - fSetup.GetMin(i);
      Double_t stepSize = fSetup.GetStepSize(i);
      Int_t bin         = dif / stepSize;
      if (stepSize == 0) bin = 0;
      fPosArray[i] = bin;
      /*Int_t step_val = bin * prestep;
      entry_val += step_val;
      prestep *= fSetup.GetNPoints(i);*/
    }
    entry_val = Hal::Std::MultiToOneDimIndex(fDimsArray, fPosArray);
    fChain->GetEntry(entry_val);
    return fData;
  }

  CorrFitFunctor::~CorrFitFunctor() {
    if (fFile) { fFile->Close(); }
  }
}  // namespace Hal
