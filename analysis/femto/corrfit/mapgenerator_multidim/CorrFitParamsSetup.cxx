/*
 * CorrFitDatabase.cxx
 *
 *  Created on: 22 sty 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitParamsSetup.h"


#include <TApplication.h>
#include <TChain.h>
#include <TClass.h>
#include <TFile.h>
#include <TMath.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TSQLServer.h>
#include <TSQLStatement.h>
#include <TSystem.h>
#include <fstream>
#include <iostream>

#include "CorrFitDumpedPairAna.h"
#include "Cout.h"
#include "FemtoCorrFuncSimple.h"
#include "FemtoFreezeoutGenerator.h"
#include "FemtoPairKinematics.h"
#include "FemtoSourceModel.h"
#include "FemtoWeightGenerator.h"
#include "FemtoWeightGeneratorLednicky.h"
#include "XMLNode.h"

//==================================================
namespace Hal {
  CorrFitParamsSetup::CorrFitParamsSetup(Int_t params) : fSize(params) {
    fMin.MakeBigger(fSize);
    fMax.MakeBigger(fSize);
    fNpoints.MakeBigger(fSize);
    fNames = new TString[fSize];
  }

  CorrFitParamsSetup::CorrFitParamsSetup(TString xmlFile) {
    XMLFile file(xmlFile);
    XMLNode* root       = file.GetRootNode();
    XMLNode* parameters = root->GetChild("Parameters");
    fSize               = parameters->GetNChildren();
    fMin.MakeBigger(fSize);
    fMax.MakeBigger(fSize);
    fNpoints.MakeBigger(fSize);
    fNames = new TString[fSize];
    for (int i = 0; i < fSize; i++) {
      XMLNode* par = parameters->GetChild(i);
      fMin[i]      = par->GetAttrib("min")->GetValue().Atof();
      fMax[i]      = par->GetAttrib("max")->GetValue().Atof();
      double step  = par->GetAttrib("step")->GetValue().Atof();
      fNpoints[i]  = TMath::Ceil((fMax[i] - fMin[i]) / double(step)) + 1;
      fMax[i]      = fMin[i] + double(fNpoints[i] - 1) * step;
      fNames[i]    = par->GetAttrib("name")->GetValue();
    }
  }

  Int_t CorrFitParamsSetup::GetNJobs() const {
    Int_t jobs = 1;
    for (int i = 0; i < GetNParams(); i++) {
      jobs = jobs * GetNPoints(i);
    }
    return jobs;
  }

  std::vector<int> CorrFitParamsSetup::GetDimensions() const {
    std::vector<int> dims(fSize);
    for (int i = 0; i < fSize; i++) {
      dims[i] = fNpoints.Get(i);
    }
    return dims;
  }

  void CorrFitParamsSetup::SetParameter(Int_t par_id, Double_t min, Double_t max, Int_t points, TString name) {
    if (par_id >= fSize) {
      Int_t old_size = fSize;
      fSize          = par_id + 1;
      fMin.MakeBigger(fSize);
      fMax.MakeBigger(fSize);
      fNpoints.MakeBigger(fSize);
      TString* temp = fNames;
      fNames        = new TString[fSize];
      for (int i = 0; i < old_size; i++)
        fNames[i] = temp[i];
      delete[] temp;
    }
    fMin[par_id]     = min;
    fMax[par_id]     = max;
    fNpoints[par_id] = points;
    fNames[par_id]   = name;
  }

  CorrFitParamsSetup::CorrFitParamsSetup(const CorrFitParamsSetup& other) :
    TObject(other), fSize(other.fSize), fMin(other.fMin), fMax(other.fMax), fNpoints(other.fNpoints) {
    fNames = new TString[fSize];
    for (int i = 0; i < fSize; i++)
      fNames[i] = other.fNames[i];
  }

  CorrFitParamsSetup& CorrFitParamsSetup::operator=(const CorrFitParamsSetup& rhs) {
    if (&rhs == this) return *this;
    fSize    = rhs.fSize;
    fMin     = rhs.fMin;
    fMax     = rhs.fMax;
    fNpoints = rhs.fNpoints;
    if (fNames) delete[] fNames;
    fNames = new TString[fSize];
    for (int i = 0; i < fSize; i++) {
      fNames[i] = rhs.fNames[i];
    }
    return *this;
  }

  Double_t CorrFitParamsSetup::GetStepSize(Int_t par_id) const {
    Double_t steps = fNpoints.Get(par_id) - 1.0;
    if (steps == 0) return 0;
    return (fMax.Get(par_id) - fMin.Get(par_id)) / steps;
  }

  void CorrFitParamsSetup::Print(Option_t* /*opt*/) const {
    Cout::Text("CorrFitDbParamsSetup", "L", kWhite);
    for (int i = 0; i < GetNParams(); i++) {
      TString name = Form("%s min: %4.2f max: %4.2f steps %i", GetParName(i).Data(), GetMin(i), GetMax(i), GetNSteps(i));
      Cout::Text(name, "L", kWhite);
    }
  }

  Bool_t CorrFitParamsSetup::TestMapFile(Int_t jobId) {
    TString name = Form("files/corrfit_map_%i.root", jobId);
    std::ifstream testFile;
    testFile.open(name);
    if (!testFile.good()) {
      testFile.close();
      return kFALSE;
    }
    testFile.close();
    TFile* f = new TFile(name);
    if (f->IsZombie()) {
      delete f;
      gSystem->Exec(Form("rm %s", name.Data()));
      return kFALSE;
    }
    TTree* t = (TTree*) f->Get("map");
    if (t == nullptr) {
      delete f;
      gSystem->Exec(Form("rm %s", name.Data()));
      return kFALSE;
    }
    delete f;
    return kTRUE;
  }

  Int_t CorrFitParamsSetup::GetParId(TString name) const {
    for (int i = 0; i < fSize; i++) {
      if (fNames[i] == name) return i;
    }
    return -1;
  }

}  // namespace Hal
