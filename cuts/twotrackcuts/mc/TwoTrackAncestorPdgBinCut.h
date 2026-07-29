/*
 * TwoTrackAncestorBinCut.h
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#ifndef HAL_CUTS_TWOTRACKCUTS_MC_TWOTRACKANCESTORBINCUT_H_
#define HAL_CUTS_TWOTRACKCUTS_MC_TWOTRACKANCESTORBINCUT_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Const.h"
#include "TwoTrackMcCut.h"

namespace Hal {
  class Track;
} /* namespace Hal */


namespace Hal {

  class TwoTrackAncestorPdgBinCut : public TwoTrackMcCut {
    std::unordered_map<int, int> fMap;
    std::vector<TString> fNames;
    std::vector<Int_t> fIndexes;

    Int_t GetPid(Hal::Track* track);

  public:
    enum ParID { FirstParticle = 0, SecondParticle = 1 };
    TwoTrackAncestorPdgBinCut(Int_t pid = Hal::Const::DummyPID());
    /**
     * add new pid code
     * @param pid pid code
     * @param name name of particle (if not specified HAL will look in own PDG table)
     */
    void SafelyAddToMap(Int_t pid, TString name);
    virtual Bool_t Pass(TwoTrack* pair);
    virtual std::vector<TString> GetAxisBinLabels(Int_t par = 0) const;
    virtual Hal::Package* Report() const;
    virtual ~TwoTrackAncestorPdgBinCut() {};
    ClassDef(TwoTrackAncestorPdgBinCut, 1)
  };
} /* namespace Hal */

#endif /* HAL_CUTS_TWOTRACKCUTS_MC_TWOTRACKANCESTORBINCUT_H_ */
