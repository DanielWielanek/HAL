/*
 * TrackPdgBinCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKPDGBINCUT_H_
#define HALTRACKPDGBINCUT_H_

#include "TrackCut.h"

namespace Hal {
  /**
   * class that convert PDG codes to more "compressed" values
   */
  class TrackPdgBinCut : public TrackCut {
    std::unordered_map<int, int> fMap;
    std::vector<TString> fNames;
    std::vector<Int_t> fIndexes;

  public:
    /**
     * default ctor
     * @param autoload if yes then basic PID bins will be added
     */
    TrackPdgBinCut(Bool_t autoload = kTRUE);
    /**
     * add new pid code
     * @param pid pid code
     * @param name name of particle (if not specified HAL will look in own PDG table)
     */
    void SafelyAddToMap(Int_t pid, TString name);
    virtual Bool_t Pass(Track* track);
    virtual std::vector<TString> GetAxisBinLabels(Int_t par = 0) const;
    virtual Hal::Package* Report() const;
    virtual ~TrackPdgBinCut() {};
    ClassDef(TrackPdgBinCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKPDGBINCUT_H_ */
