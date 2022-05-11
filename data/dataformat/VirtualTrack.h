/*
 * TrackVirtual.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALVIRTUALTRACK_H_
#define HALVIRTUALTRACK_H_
#include "Track.h"
namespace Hal {
  class VirtualTrack : public Track {
  public:
    VirtualTrack();
    virtual ~VirtualTrack();
    ClassDef(VirtualTrack, 1)
  };
}  // namespace Hal
#endif /* HALVIRTUALTRACK_H_ */
