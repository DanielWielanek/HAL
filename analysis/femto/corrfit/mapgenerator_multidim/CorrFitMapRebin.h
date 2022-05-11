/*
 * CorrFitMapRebin.h
 *
 *  Created on: 8 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITMAPREBIN_H_
#define HALCORRFITMAPREBIN_H_

#include "Array.h"

#include <TObject.h>


/**
 * class for compression of multidimensional maps
 */

class TFile;
class TTree;
namespace Hal {
  class CorrFitInfo;
  class DividedHisto1D;
  class DividedHisto3D;
  class CorrFitMapRebin : public TObject {
    TFile* fInFile;
    TFile* fOutFile;
    TTree* fInTree;
    TTree* fOutTree;
    CorrFitInfo* fInfo;
    TString fInFileName;
    TString fOutFileName;
    Int_t fRebinX, fRebinY, fRebinZ;
    Bool_t Rebin1D();
    Bool_t Rebin3D();

  public:
    CorrFitMapRebin();
    /**
     *
     * @param inFile name of the input file
     * @param nbins rebinning value (the same for all axes)
     */
    CorrFitMapRebin(TString inFile, Int_t nbins);
    /** set rebin separatelly for each axis **/
    void SetRebin3D(Int_t binsX, Int_t binsY, Int_t binsZ) {
      fRebinX = binsX;
      fRebinY = binsY;
      fRebinZ = binsZ;
    }
    /**
     * compress map and write to file
     * @param outFile file with compressed map
     * @return status of compression (false if some problem occured)
     */
    Bool_t Compress(TString outFile);
    virtual ~CorrFitMapRebin();
    CorrFitMapRebin& operator=(const CorrFitMapRebin& other) = delete;
    ClassDef(CorrFitMapRebin, 1)
  };
}  // namespace Hal

#endif /* HALCORRFITMAPREBIN_H_ */
