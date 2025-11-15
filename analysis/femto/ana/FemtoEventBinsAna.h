/*
 * FemtoEventBinsAna.h
 *
 *  Created on: 4 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_ANA_HALFEMTOEVENTBINSANA_H_
#define HALFEMTO_ANALYSIS_FEMTO_ANA_HALFEMTOEVENTBINSANA_H_

#include "FemtoBasicAna.h"

#include "Array.h"

#include <Rtypes.h>
#include <RtypesCore.h>


namespace Hal {
  class EventBinningCut;

  class FemtoEventBinsAna : public FemtoBasicAna {
  protected:
    virtual Bool_t InitArray();
    virtual void ProcessFemtoPair();
    virtual void ProcessFemtoPair_Perfect();
    virtual void ProcessFemtoPair_Rotated();
    virtual void ProcessFemtoPair_Hemisphere();
    virtual void ProcessFemtoPair_Mixed();
    virtual void ProcessFemtoPair_Charged();

  public:
    FemtoEventBinsAna();
    virtual ~FemtoEventBinsAna();
    ClassDef(FemtoEventBinsAna, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_ANALYSIS_FEMTO_ANA_HALFEMTOEVENTBINSANA_H_ */
