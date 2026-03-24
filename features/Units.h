/*
 * Units.h
 *
 *  Created on: 23 mar 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_UNITS_H_
#define HAL_FEATURES_UNITS_H_

#include <RtypesCore.h>

#include "Const.h"


namespace Hal {
  /**
   * all units in this framework are cm and sec except freezeout coordinates expressed in fm / fm/c
   */
  namespace Units {
    inline Double_t m() { return 100.0; }
    inline Double_t cm() { return 1.0; }
    inline Double_t mm() { return 0.1; }
    inline Double_t um() { return 0.01; }
    inline Double_t fm() { return 1.E-13; };

    inline Double_t s() { return 1.0; }
    inline Double_t fm_c() { return fm() / Hal::Const::c(); }

    inline Double_t GeV() { return 1.0; }
    inline Double_t MeV() { return 0.001; }
    /** tesla **/
    inline Double_t T() { return 1.0; }
  }  // namespace Units
}  // namespace Hal


#endif /* HAL_FEATURES_UNITS_H_ */
