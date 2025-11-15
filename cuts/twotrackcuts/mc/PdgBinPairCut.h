/*
 * PdgBinPairCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPDGBINPAIRCUT_H_
#define HALPDGBINPAIRCUT_H_

#include "McTrack.h"
#include "TwoTrackCut.h"

namespace Hal {
  class PdgBinPairCut : public TwoTrackCut {
    std::unordered_map<int, int> fMap;
    std::vector<TString> fNames;
    std::vector<Int_t> fIndexes;

    Int_t GetPid(Hal::Track* track);

  public:
    enum ParID { FirstParticle = 0, SecondParticle = 1 };
    PdgBinPairCut(Bool_t autoload = kTRUE);
    /**
     * add new pid code
     * @param pid pid code
     * @param name name of particle (if not specified HAL will look in own PDG table)
     */
    void SafelyAddToMap(Int_t pid, TString name);
    virtual Bool_t Pass(TwoTrack* pair);
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t par = 0) const;
    virtual Hal::Package* Report() const;
    virtual ~PdgBinPairCut();
    ClassDef(PdgBinPairCut, 1)
  };
}  // namespace Hal
#endif /* HALPDGBINPAIRCUT_H_ */
