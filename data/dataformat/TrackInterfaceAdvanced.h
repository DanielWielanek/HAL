/*
 * TrackInterfaceAdvanced.h
 *
 *  Created on: 25 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_DATA_DATAFORMAT_TRACKINTERFACEADVANCED_H_
#define HAL_DATA_DATAFORMAT_TRACKINTERFACEADVANCED_H_

#include "TrackInterface.h"

/**
 * class for advanced holdin data, currently not used
 */
namespace Hal {

  class TrackInterfaceAdvanced : public TrackInterface {
  public:
    TrackInterfaceAdvanced();
    virtual ~TrackInterfaceAdvanced();
    ClassDef(TrackInterfaceAdvanced, 1)
  };

} /* namespace Hal */

#endif /* HAL_DATA_DATAFORMAT_TRACKINTERFACEADVANCED_H_ */
