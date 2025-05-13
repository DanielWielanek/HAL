/*
 * NicaUnigenTrack.h
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAUNIGENTRACK_H_
#define NICAUNIGENTRACK_H_

#include <iostream>

#include "McTrack.h"
#include "UParticle.h"

/**
 * class for representation of track from unigen in "fake" format
 */
namespace HalUni {
  class UnigenTrack : public Hal::McTrack {
  public:
    UnigenTrack();
    virtual ~UnigenTrack();
    ClassDef(UnigenTrack, 1)
  };
}  // namespace HalUni
#endif /* NICAUNIGENTRACK_H_ */
