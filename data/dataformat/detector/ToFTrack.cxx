/*
 * ToFTrack.cxx
 *
 *  Created on: 28 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "ToFTrack.h"
namespace Hal {
  ToFTrack::ToFTrack() : fToFNHits(0), fToFTime(0), fToFMass2(0), fToFBeta(0), fToFEnergy(0) {}

  ToFTrack::~ToFTrack() {}
}  // namespace Hal
