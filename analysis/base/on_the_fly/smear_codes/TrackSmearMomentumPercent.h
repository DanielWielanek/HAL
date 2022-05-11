/*
 * TrackSmearMomentumPercent.h
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKSMEARMOMENTUMPERCENT_H_
#define HALTRACKSMEARMOMENTUMPERCENT_H_

#include "SmearAlgorithm.h"


/**
 * class for smearing momentum of track
 */

namespace Hal {
  class Package;
  class TrackSmearMomentumPercent : public TrackSmear {
    Double_t fPxSmear;
    Double_t fPySmear;
    Double_t fPzSmear;
    Double_t fPtSmear;
    Int_t fSmearMode;

  public:
    TrackSmearMomentumPercent();
    /**
     * copy constructor
     * @param obj
     */
    TrackSmearMomentumPercent(const TrackSmearMomentumPercent& obj);
    /**
     * set momenum smear  only px an py are smeared
     * @param pt transverse momentum smear in percent
     */
    void SetMomentumSmear(Double_t pt);
    /**
     * set momentum smear
     * @param pt smear on pt (in percent)
     * @param pz smear on pz (in percent)
     */
    void SetMomentumSmear(Double_t pt, Double_t pz);
    /**
     * set momentu smear
     * @param px px smear on pt (in percent)
     * @param py py smear on pt (in percent)
     * @param pz pt smear on pt (in percent)
     */
    void SetMomentumSmear(Double_t px, Double_t py, Double_t pz);
    Task::EInitFlag Init();
    void ModifyTrack(SmearedTrack* mod);
    virtual SmearAlgorithm* MakeCopy() const { return new TrackSmearMomentumPercent(*this); };
    virtual Package* Report() const;
    virtual ~TrackSmearMomentumPercent();
    ClassDef(TrackSmearMomentumPercent, 1)
  };
}  // namespace Hal

#endif /* HALTRACKSMEARMOMENTUMPERCENT_H_ */
