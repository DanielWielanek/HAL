/*
 * Femto1DCFAnaMapMC.h
 *
 *  Created on: 2 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO1DCFANAMAPMC_H_
#define HALFEMTO1DCFANAMAPMC_H_

#include "Femto1DMapGenerator.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "Array.h"
#include "FemtoConst.h"
#include "FemtoFreezoutGenerator.h"
#include "FemtoWeightGenerator.h"


namespace Hal {
  class DividedHisto2D;
  class Femto1DCF;

  /**
   * generate 2-dim CF map for 1D CF,
   */


  class Femto1DCFAnaMapMC : public Femto1DMapGenerator {
  protected:
    Femto1DCFAnaMapMC(Bool_t useFake) : Femto1DMapGenerator(useFake) {};

  public:
    Femto1DCFAnaMapMC();
    virtual Bool_t Init();
    /**
     * create map
     * @param pairs_per_bin number of pairs in single bin
     */
    virtual void Run(Int_t pairs_per_bin);
    virtual ~Femto1DCFAnaMapMC();
    ClassDef(Femto1DCFAnaMapMC, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO1DCFANAMAPMC_H_ */
