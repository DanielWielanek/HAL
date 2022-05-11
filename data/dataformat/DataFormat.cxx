/*
 * DataFormat.cxx
 *
 *  Created on: 25 lis 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DataFormat.h"

#include <TMath.h>
#include <TString.h>
namespace Hal {
  NamespaceImp(DetectorID) namespace DetectorID {
    UInt_t GetDetectorID(TString pattern) {
      if (pattern.Length() > 6) return 0;
      TString newPattern = pattern;
      for (int i = pattern.Length(); i < 6; i++) {
        newPattern = "0" + newPattern;
      }
      ULong_t detectorID = 0;
      for (int i = 0; i < newPattern.Length(); i++) {
        Char_t x    = newPattern[i];
        UInt_t base = 0;
        if (x < 58) {  // this is 0-9
          base = x - 48;
        } else if (x < 91) {  // upper case
          base = x - 55;
        } else {  // lower case
          base = x - 87;
        }
        UInt_t power = (UInt_t) TMath::Power(36, 5 - i);
        detectorID   = detectorID + base * power;
      }
      return detectorID;
    }
  }
}  // namespace Hal
