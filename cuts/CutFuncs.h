/*
 * CutFuncs.h
 *
 *  Created on: 25 lis 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CUTS_CUTFUNCS_H_
#define HAL_CUTS_CUTFUNCS_H_

#include <vector>

#include "Object.h"
#include "Std.h"
#include <RtypesCore.h>
#include <TString.h>

namespace Hal {
  class Cut;

  namespace Cuts {
    /**
     * calculate collection Id's from expression
     * @param option
     * @return
     */
    std::vector<Int_t> GetCollectionsFlags(Int_t startCol, TString option);
    /**
     * make copy of cut
     * @param cu cut to copy
     * @param flag re for real, other create img flag
     * @param acceptNulls for img if true than accept nulls
     * @return
     */
    Hal::Cut* MakeCutCopy(const Hal::Cut& cut, TString flag, Bool_t acceptNulls = kTRUE);
    /**
     * return cut update ratio name
     * @param upd
     * @return
     */
    TString GetCutUpdateRatioName(Hal::ECutUpdate upd);
  }  // namespace Cuts

} /* namespace Hal */

#endif /* HAL_CUTS_CUTFUNCS_H_ */
