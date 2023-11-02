/*
 * CompressionMap.cxx
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CompressionMap.h"

#include <iostream>

namespace Hal {

  CompressionMap::CompressionMap() : fSize(0), fAllocatedSize(0), fCounter(0), fOldToNewIndex(nullptr), fNewToOldIndex(nullptr) {}

  CompressionMap::CompressionMap(const CompressionMap& other) :
    TObject(other), fSize(other.fSize), fAllocatedSize(other.fAllocatedSize), fCounter(other.fCounter), fOldToNewIndex(nullptr) {
    if (fAllocatedSize > 0) {
      fOldToNewIndex = new Int_t[fAllocatedSize];
      fNewToOldIndex = new Int_t[fAllocatedSize];
    }
    std::cout << "WARNING COPY MAP" << std::endl;
  }

  void CompressionMap::Reset(Int_t size) {
    CheckSize(size);
    fSize    = size;
    fCounter = 0;
    for (int i = 0; i < fSize; i++) {
      fOldToNewIndex[i] = -1;
    }
  }

  void CompressionMap::Recalculate() {
    for (int i = 0; i < fSize; i++) {
      if (fOldToNewIndex[i] == 0) {
        fOldToNewIndex[i]        = fCounter;
        fNewToOldIndex[fCounter] = i;
        fCounter++;
      }
    }
  }

  void CompressionMap::CheckSize(Int_t size) {
    if (fAllocatedSize <= size) {
      Int_t newSize = size * 2;
      if (fOldToNewIndex) delete[] fOldToNewIndex;
      if (fNewToOldIndex) delete[] fNewToOldIndex;
      fOldToNewIndex = new Int_t[newSize];
      fNewToOldIndex = new Int_t[newSize];
      fAllocatedSize = newSize;
    }
  }

  CompressionMap::~CompressionMap() {
    if (fOldToNewIndex) delete[] fOldToNewIndex;
    if (fNewToOldIndex) delete[] fNewToOldIndex;
  }

} /* namespace Hal */
