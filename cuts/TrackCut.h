/*
 * TrackCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 */

#ifndef HALTRACKCUT_H_
#define HALTRACKCUT_H_

#include "Cut.h"
/**
 * abstract class for all track cuts
 */
namespace Hal {
  class Track;
  class TrackComplexCut;
  class TrackCut : public Cut {
  protected:
  public:
    /**
     * default constructor
     * @param i number of checked parameters
     */
    TrackCut(const Int_t i = 1);
    virtual ~TrackCut();
    /**
     *
     * @param track
     * @return true if track pass, false otherwise
     */
    virtual Bool_t Pass(Track* track) = 0;
    /**
     *
     * @param opt - option of making complex cut, by default
     * cut contains imaginary and real part (like complex(this,this), however by
     * specify "re" or "im" you can enforce creation cut (this,virtual) or
     * (virtual, this) rescpectively
     * @return complex cut from this cut
     * i
     */
    TrackComplexCut* MakeComplexCut(Option_t* opt) const;
    virtual Package* Report() const;
    ClassDef(TrackCut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKCUT_H_ */
