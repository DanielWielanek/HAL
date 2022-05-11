/*
 * TrackToF.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTOFTRACK_H_
#define HALTOFTRACK_H_

#include "DetectorTrack.h"
/**
 * class for representation of particle registered by Time of Flight detector
 */
namespace Hal {
  class ToFTrack : public DetectorTrack {
    Int_t fToFNHits;
    Float_t fToFTime;
    Float_t fToFMass2;
    Float_t fToFBeta;
    Float_t fToFEnergy;

  public:
    /**
     * default ctor
     */
    ToFTrack();
    /**
     * copy ctor
     * @param other
     */
    ToFTrack(const ToFTrack& other) = default;
    /**
     * Assignment operator
     * @param track
     * @return
     */
    ToFTrack& operator=(const ToFTrack& track) = default;
    /**
     *
     * @return value of beta . mass2, energyfor dummy partile
     */
    static Float_t DummyVal() { return -1E+6; };
    /**
     *
     * @return number of hits in ToF
     */
    inline Int_t GetNHits() const { return fToFNHits; };
    /**
     *
     * @return time of flight
     */
    inline Float_t GetTime() const { return fToFTime; };
    /**
     *
     * @return square of mass (should it be negative number for particles that
     * don't have ToF id?)
     */
    inline Float_t GetMass2() const { return fToFMass2; };
    /**
     *
     * @return beta in ToF
     */
    inline Float_t GetBeta() const { return fToFBeta; }
    /**
     *
     * @return energy from ToF
     */
    inline Float_t GetEnergy() const { return fToFEnergy; };
    /**
     *
     * @param nToFHits set number of ToF hits
     */
    void SetNHits(Int_t nToFHits) { fToFNHits = nToFHits; }
    /**
     *
     * @param toFBeta set ToF beta
     */
    void SetBeta(Float_t toFBeta) { fToFBeta = toFBeta; }
    /**
     *
     * @param toFEnergy set ToF energy
     */
    void SetEnergy(Float_t toFEnergy) { fToFEnergy = toFEnergy; }
    /**
     *
     * @param toFMass2 set tof mass2
     */
    void SetMass2(Float_t toFMass2) { fToFMass2 = toFMass2; }
    /**
     *
     * @param toFTime set time of flight
     */
    void SetTime(Float_t toFTime) { fToFTime = toFTime; }
    virtual ~ToFTrack();
    ClassDef(ToFTrack, 1)
  };
}  // namespace Hal
#endif /* HALTOFTRACK_H_ */
