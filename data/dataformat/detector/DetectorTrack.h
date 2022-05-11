/*
 * DetectorTrack.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALDETECTORTRACK_H_
#define HALDETECTORTRACK_H_

#include <TObject.h>


/**
 * class for holding information about information about track from single
 * detector
 */
namespace Hal {
  class DetectorTrack : public TObject {
    Int_t fFlag;

  public:
    DetectorTrack();
    /**
     * copy ctor
     * @param other
     */
    DetectorTrack(const DetectorTrack& other) = default;
    /**
     * Assignment operator
     * @param track
     * @return
     */
    DetectorTrack& operator=(const DetectorTrack& track) = default;
    /**
     *
     * @return flag, 1 if this is correct track, 0 otherwise
     */
    inline Int_t GetFlag() const { return fFlag; };
    inline void SetFlag(Int_t flag) { fFlag = flag; };
    inline void SetFlagGood() { fFlag = 1; };
    inline void SetFlagBad() { fFlag = 0; }
    virtual ~DetectorTrack();
    ClassDef(DetectorTrack, 1)
  };
}  // namespace Hal
#endif /* HALDETECTORTRACK_H_ */
