/*
 * TrackSmeared.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSMEAREDTRACK_H_
#define HALSMEAREDTRACK_H_


#include "McTrack.h"

namespace Hal {
  class SmearedEvent;
  /**
   * class for storing smeared tracks
   */
  class SmearedTrack : public McTrack {
    friend class SmearedEvent;
    Track* fTrack;
    TLorentzVector* fTrueMomenta;
    SmearedTrack(Track* track);

  public:
    SmearedTrack();
    void CopyData(Track* other);
    inline TLorentzVector* GetTrueMomentum() const { return fTrueMomenta; };
    virtual ~SmearedTrack();
    ClassDef(SmearedTrack, 1)
  };
}  // namespace Hal
#endif /* HALSMEAREDTRACK_H_ */
