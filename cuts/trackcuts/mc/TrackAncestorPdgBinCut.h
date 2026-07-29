/*
 * TrackMotherPdgBinCut.h
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_MC_TRACKANCESTORPDGBINCUT_H_
#define HAL_CUTS_TRACKCUTS_MC_TRACKANCESTORPDGBINCUT_H_

#include <RtypesCore.h>
#include <TString.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Const.h"
#include "TrackMcCut.h"


namespace Hal {

  /**
   * return pdg binned of ancestor
   */
  class TrackAncestorPdgBinCut : public TrackMcCut {
    std::unordered_map<int, int> fMap;
    std::vector<TString> fNames;
    std::vector<Int_t> fIndexes;

  public:
    /**
     * default ctor
     * @param autoload if yes then basic PID bins will be added
     */
    TrackAncestorPdgBinCut(Int_t pid = Hal::Const::DummyPID());
    /**
     * add new pid code
     * @param pid pid code
     * @param name name of particle (if not specified HAL will look in own PDG table)
     */
    void SafelyAddToMap(Int_t pid, TString name);
    virtual Bool_t Pass(Track* track);
    virtual std::vector<TString> GetAxisBinLabels(Int_t par = 0) const;
    virtual Hal::Package* Report() const;
    virtual ~TrackAncestorPdgBinCut() {};
    ClassDef(TrackAncestorPdgBinCut, 1)
  };
} /* namespace Hal */

#endif /* HAL_CUTS_TRACKCUTS_MC_TRACKANCESTORPDGBINCUT_H_ */
