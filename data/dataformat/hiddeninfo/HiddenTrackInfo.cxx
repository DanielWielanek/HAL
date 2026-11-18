/*
 * HiddenInfo.cxx
 *
 *  Created on: 24 lis 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HiddenTrackInfo.h"

#include "CompressionMap.h"

namespace Hal {
  void HiddenTrackInfo::CopyData(HiddenTrackInfo* object) { fData = object->fData; }

  void HiddenTrackInfo::Reset() {
    for (auto& i : fData)
      i = -1;
  }

  void HiddenTrackInfo::TranslateLinks(const CompressionMap& map, Int_t start) {
    for (int i = start; i < (int) fData.size(); i++) {
      fData[i] = map.GetNewIndex(fData[i]);
    }
  }

} /* namespace Hal */
