/*
 * TrackInterface.cxx
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackInterface.h"
namespace Hal {
  TrackInterface::TrackInterface(Int_t obj) : fRawObjNo(obj) { fRawObject = new TObject*[fRawObjNo]; }

  TrackInterface::~TrackInterface() { delete[] fRawObject; }
}  // namespace Hal
