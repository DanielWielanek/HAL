/*
 * TwoTrack.cxx
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrack.h"
namespace Hal {
  TwoTrack::TwoTrack() : fTrack1(NULL), fTrack2(NULL), fType(PairType::kOther), fSame(kTRUE) {}

  TwoTrack::~TwoTrack() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
