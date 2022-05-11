/*
 * TrackExpCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKEXPCUT_H_
#define HALTRACKEXPCUT_H_

#include "TrackCut.h"
/**
 * basic class for all track cuts that use reconstructed tracks
 */
namespace Hal {
  class TrackExpCut : public TrackCut {
  protected:
    /**
     *
     * @param format_id
     * @return true if tracks return "tpc-track"
     */
    Bool_t TpcAvaiable(Int_t format_id) const;
    /**
     *
     * @param format_id
     * @return true if track return "tof-track"
     */
    Bool_t ToFAvaiable(Int_t format_id) const;

  public:
    TrackExpCut(Int_t size = 1);
    virtual Bool_t Init(Int_t format_id = 0);
    virtual ~TrackExpCut();
    ClassDef(TrackExpCut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKEXPCUT_H_ */
