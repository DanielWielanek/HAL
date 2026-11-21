/*
 * FemtoSHFlatIndexes.h
 *
 *  Created on: 20 lis 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOSHFLATINDEXES_H_
#define HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOSHFLATINDEXES_H_

#include <RtypesCore.h>

#include "ShIndexes.h"


namespace Hal {

  class FemtoSHFlatIndexes {
    Hal::Sh::Indexes fIndexes;
    const Int_t fBins;
    const Int_t fMaxL;
    const Int_t fBinStep;
    const Int_t fImStep;

  public:
    FemtoSHFlatIndexes(int maxL = 1, int bins = 1);
    int GetIndexesPerBin() const { return fBinStep; }
    int GetTotalIndexes() const;
    int GetRawIndexRe(int l, int m, int bin) const;
    int GetRawIndexIm(int l, int m, int bin) const;
    int GetRawIndexRe(int lmIndex, int bin) const;
    int GetRawIndexIm(int lmIndex, int bin) const;
    void GetIndexesRe(int rawIndex, int& l, int& m, int& bin) const;
    void GetIndexesIm(int rawIndex, int& l, int& m, int& bin) const;
    virtual ~FemtoSHFlatIndexes() {};
    ClassDef(FemtoSHFlatIndexes, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_SHMATH_FEMTOSHFLATINDEXES_H_ */
