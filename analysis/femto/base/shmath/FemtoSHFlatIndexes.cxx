/*
 * FemtoSHFlatIndexes.cxx
 *
 *  Created on: 20 lis 2026
 *      Author: daniel
 */

#include "FemtoSHFlatIndexes.h"


namespace Hal {

  FemtoSHFlatIndexes::FemtoSHFlatIndexes(int maxL, int bins) :
    fIndexes(maxL), fBins(bins), fMaxL(maxL), fBinStep((fMaxL + 1) * (fMaxL + 1) * 2), fImStep((fMaxL + 1) * (fMaxL + 1)) {}

  int FemtoSHFlatIndexes::GetRawIndexRe(int l, int m, int bin) const {
    const int step = fBinStep * bin;
    const int ilm  = fIndexes.GetIndex(l, m);
    return step + ilm;
  }

  int FemtoSHFlatIndexes::GetRawIndexIm(int l, int m, int bin) const { return GetRawIndexRe(l, m, bin) + fImStep; }

  int FemtoSHFlatIndexes::GetRawIndexRe(int lmIndex, int bin) const {
    const int step = fBinStep * bin;
    return step + lmIndex;
  }

  int FemtoSHFlatIndexes::GetRawIndexIm(int lmIndex, int bin) const {
    const int step = fBinStep * bin;
    return step + lmIndex + fImStep;
  }

  void FemtoSHFlatIndexes::GetIndexesRe(int rawIndex, int& l, int& m, int& bin) const {
    bin                = rawIndex / fBinStep;
    const int ilmIndex = rawIndex - fBinStep * bin;
    l                  = fIndexes.GetElsi(ilmIndex);
    m                  = fIndexes.GetEmsi(ilmIndex);
  }

  int FemtoSHFlatIndexes::GetTotalIndexes() const { return fBins * fBinStep; }

  void FemtoSHFlatIndexes::GetIndexesIm(int rawIndex, int& l, int& m, int& bin) const {
    bin                = rawIndex / fBinStep;
    const int ilmIndex = rawIndex - fBinStep * bin - fImStep;
    l                  = fIndexes.GetElsi(ilmIndex);
    m                  = fIndexes.GetEmsi(ilmIndex);
  }


} /* namespace Hal */
