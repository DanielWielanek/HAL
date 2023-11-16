/*
 * CorrFitMapRebin.cxx
 *
 *  Created on: 8 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitMapRebin.h"

#include "CorrFitInfo.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoSHCF.h"
#include "StdMath.h"

#include <TFile.h>
#include <TTree.h>

#include <iostream>

namespace Hal {
  CorrFitMapRebin::CorrFitMapRebin() :
    fInFile(nullptr),
    fOutFile(nullptr),
    fInTree(nullptr),
    fOutTree(nullptr),
    fInfo(nullptr),
    fRebinX(0),
    fRebinY(0),
    fRebinZ(0) {}

  Bool_t CorrFitMapRebin::Compress(TString outFile) {
    fInFile         = new TFile(fInFileName, "read");
    fInfo           = (CorrFitInfo*) fInFile->Get("Info");
    fInTree         = (TTree*) fInFile->Get("map");
    fOutFileName    = outFile;
    TObject* obj    = fInfo->GetCf();
    Femto3DCF* cf3d = dynamic_cast<Femto3DCF*>(obj);
    Femto1DCF* cf1d = dynamic_cast<Femto1DCF*>(obj);
    FemtoSHCF* cfsh = dynamic_cast<FemtoSHCF*>(obj);
    if (cf3d != nullptr) {
      return Rebin3D();
    } else if (cfsh != nullptr) {
      return Rebin1D();
    } else if (cf1d != nullptr) {
      return Rebin1D();
    } else {
      std::cout << "Unknown CF" << std::endl;
    }

    return kFALSE;
  }

  CorrFitMapRebin::~CorrFitMapRebin() {
    if (fInFile) {
      fInFile->Close();
      delete fInFile;
    }
    if (fOutFile) {
      fOutFile->Close();
      delete fOutFile;
    }
  }

  Bool_t CorrFitMapRebin::Rebin1D() {
    DividedHisto1D* obj = (DividedHisto1D*) fInfo->GetCf();
    Int_t sizeX         = obj->GetNum()->GetNbinsX();
    if (sizeX % fRebinX != 0) {
      std::cout << Form("Cannot rebin X-axis in %s", obj->ClassName()) << std::endl;
      return kFALSE;
    }

    TH1D* den = (TH1D*) obj->GetDen();

    Array_1<Float_t>* data = nullptr;
    fInTree->SetBranchAddress("data", &data);
    fOutFile                  = new TFile(fOutFileName, "recreate");
    fOutTree                  = new TTree("map", "");
    Array_1<Float_t>* newData = new Array_1<Float_t>();
    fOutTree->Branch("data", &newData);
    fInTree->GetEntry(0);
    newData->MakeBigger(data->GetSize() / fRebinX);

    Array_1<Float_t> weight(sizeX);

    Float_t* temp = new Float_t[fRebinX];
    // Int_t newBins = sizeX / fRebinX;
    for (int i = 0; i < sizeX / fRebinX; i++) {
      Float_t sum = 0;
      for (int j = 0; j < fRebinX; j++) {
        temp[j] = den->GetBinContent(i * fRebinX + j);
        sum += temp[j];
      }
      for (int j = 0; j < fRebinX; j++) {
        weight[i * fRebinX + j] = temp[j] / sum;
        if (sum == 0) weight[i * fRebinX + j] = 1.0 / ((Float_t) fRebinX);
      }
    }
    delete[] temp;


    for (int i = 0; i < fInTree->GetEntries(); i++) {
      fInTree->GetEntry(i);
      for (int j = 0; j < newData->GetSize(); j++) {
        Float_t sum = 0;
        for (int k = 0; k < fRebinX; k++) {
          sum += (*data)[j * fRebinX + k] * weight[j * fRebinX + k];
        }
        (*newData)[j] = sum;
      }
      fOutTree->Fill();
    }

    fOutTree->Write();
    CorrFitInfo* newInfo = new CorrFitInfo(*fInfo);
    ((DividedHisto1D*) newInfo->GetCf())->Rebin(fRebinX, "x");
    newInfo->Write();
    fOutFile->Close();
    delete fOutFile;
    fOutFile = nullptr;
    return kTRUE;
  }

  Bool_t CorrFitMapRebin::Rebin3D() {
    Femto3DCF* obj = (Femto3DCF*) fInfo->GetCf();
    Int_t sizeX    = obj->GetNum()->GetNbinsX();
    Int_t sizeY    = obj->GetNum()->GetNbinsY();
    Int_t sizeZ    = obj->GetNum()->GetNbinsZ();
    TH3* den       = (TH3*) obj->GetDen();
    if (sizeX % fRebinX != 0) {
      std::cout << "Cannot rebin X-axis in 3DCF" << std::endl;
      return kFALSE;
    }
    if (sizeY % fRebinY != 0) {
      std::cout << "Cannot rebin Y-axis in 3DCF" << std::endl;
      return kFALSE;
    }
    if (sizeZ % fRebinZ != 0) {
      std::cout << "Cannot rebin Z-axis in 3DCF" << std::endl;
      return kFALSE;
    }
    Array_1<Float_t>* data = nullptr;
    fInTree->SetBranchAddress("data", &data);
    fOutFile                  = new TFile(fOutFileName, "recreate");
    fOutTree                  = new TTree("map", "");
    Array_1<Float_t>* newData = new Array_1<Float_t>();
    fOutTree->Branch("data", &newData);
    fInTree->GetEntry(0);
    newData->MakeBigger(data->GetSize() / fRebinX / fRebinY / fRebinZ);

    Array_3<Int_t> mapL(sizeX, sizeY, sizeZ);
    Array_3<Float_t> mapW(sizeX, sizeY, sizeZ);
    Array_3<Int_t> mapS(sizeX / fRebinX, sizeY / fRebinY, sizeZ / fRebinZ);
    for (int i = 0; i < sizeX; i++) {
      for (int j = 0; j < sizeY; j++) {
        for (int k = 0; k < sizeZ; k++) {
          mapL[i][j][k] = Hal::Std::Bin3dToBin1d(sizeX, sizeY, i, j, k, kFALSE);
        }
      }
    }
    Int_t sizeXs        = sizeX / fRebinX;
    Int_t sizeYs        = sizeY / fRebinY;
    Int_t sizeZs        = sizeZ / fRebinZ;
    Double_t dummyScale = 1.0 / (Float_t)(fRebinX * fRebinY * fRebinZ);
    Array_3<Float_t> temp(fRebinX, fRebinY, fRebinZ);
    for (int i = 0; i < sizeXs; i++) {
      for (int j = 0; j < sizeYs; j++) {
        for (int k = 0; k < sizeZs; k++) {
          mapS[i][j][k] = Hal::Std::Bin3dToBin1d(sizeXs, sizeYs, i, j, k, kFALSE);
          Float_t sum   = 0;
          for (int a = 0; a < fRebinX; a++) {
            const Int_t tX = i * fRebinX + a;
            for (int b = 0; b < fRebinY; b++) {
              const Int_t tY = j * fRebinY + b;
              for (int c = 0; c < fRebinZ; c++) {
                const Int_t tZ = k * fRebinZ + c;
                // Int_t bin      = mapL[tX][tY][tZ];
                sum += den->GetBinContent(tX, tY, tZ);
              }
            }
          }
          for (int a = 0; a < fRebinX; a++) {
            const Int_t tX = i * fRebinX + a;
            for (int b = 0; b < fRebinY; b++) {
              const Int_t tY = j * fRebinY + b;
              for (int c = 0; c < fRebinZ; c++) {
                const Int_t tZ   = k * fRebinZ + c;
                mapW[tX][tY][tZ] = den->GetBinContent(tX, tY, tZ) / sum;
                if (sum == 0) { mapW[tX][tY][tZ] = dummyScale; }
              }
            }
          }
        }
      }
    }
    Int_t step = fInTree->GetEntries() / 100;
    for (int iEv = 0; iEv < fInTree->GetEntries(); iEv++) {
      fInTree->GetEntry(iEv);
      if (iEv % step) { Cout::ProgressBar(iEv, fInTree->GetEntries()); }
      for (int i = 0; i < sizeXs; i++) {
        for (int j = 0; j < sizeYs; j++) {
          for (int k = 0; k < sizeZs; k++) {
            Float_t sum = 0;
            for (int a = 0; a < fRebinX; a++) {
              const Int_t tX = i * fRebinX + a;
              for (int b = 0; b < fRebinY; b++) {
                const Int_t tY = j * fRebinY + b;
                for (int c = 0; c < fRebinZ; c++) {
                  const Int_t tZ = k * fRebinZ + c;
                  Int_t bin      = mapL[tX][tY][tZ];
                  sum += (*data)[bin] * mapW[tX][tY][tZ];
                }
              }
            }
            (*newData)[mapS[i][j][k]] = sum;
          }
        }
      }
      fOutTree->Fill();
    }
    fOutTree->Write();
    CorrFitInfo* newInfo = new CorrFitInfo(*fInfo);
    ((DividedHisto3D*) newInfo->GetCf())->Rebin(fRebinX, "x");
    ((DividedHisto3D*) newInfo->GetCf())->Rebin(fRebinY, "y");
    ((DividedHisto3D*) newInfo->GetCf())->Rebin(fRebinZ, "z");
    newInfo->Write();
    fOutFile->Close();
    delete fOutFile;
    fOutFile = nullptr;
    return kTRUE;
  }

  CorrFitMapRebin::CorrFitMapRebin(TString inFile, Int_t nbins) : CorrFitMapRebin() {
    fInFileName = inFile;
    fRebinX = fRebinY = fRebinZ = nbins;
  }
}  // namespace Hal
