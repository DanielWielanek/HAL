/*
 * TrackTPC.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTPCTRACK_H_
#define HALTPCTRACK_H_
#include "DetectorTrack.h"
#include <iostream>
/**
 * basic class for representation of particles registered by TPC
 */
namespace Hal {
  class TpcTrack : public DetectorTrack {
    Int_t fTpcNHits;
    Float_t fTpcSigmaPion;
    Float_t fTpcSigmaKaon;
    Float_t fTpcSigmaProton;
    Float_t fTpcSigmaElectron;
    Float_t fTpcPidProbPion;
    Float_t fTpcPidProbKaon;
    Float_t fTpcPidProbProton;
    Float_t fTpcPidProbElectron;
    Float_t fTpcDeDx;

  public:
    /**
     * default ctor
     */
    TpcTrack();
    /**
     * copy ctor
     * @param other
     */
    TpcTrack(const TpcTrack& other) = default;
    /**
     * Assignment operator
     * @param track
     * @return
     */
    TpcTrack& operator=(const TpcTrack& track) = default;
    /**
     *
     * @return number of TPC hist
     */
    inline Int_t GetNHits() const { return fTpcNHits; };
    /**
     *
     * @return pion sigma
     */
    inline Float_t GetSigmaPion() const { return fTpcSigmaPion; };
    /**
     *
     * @return kaon sigma
     */
    inline Float_t GetSigmaKaon() const { return fTpcSigmaKaon; };
    /**
     *
     * @return protonc sigma
     */
    inline Float_t GetSigmaProton() const { return fTpcSigmaProton; };
    /**
     *
     * @return electron sigma
     */
    inline Float_t GetSigmaElectron() const { return fTpcSigmaElectron; }
    /**
     *
     * @return probablity that this in pion (calculated only by TPC
     */
    inline Float_t GetProbPion() const { return fTpcPidProbPion; };
    /**
     *
     * @return probablity that this in kaon (calculated only by TPC
     */
    inline Float_t GetProbKaon() const { return fTpcPidProbKaon; };
    /**
     *
     * @return probablity that this in proton (calculated only by TPC
     */
    inline Float_t GetProbProton() const { return fTpcPidProbProton; };
    /**
     *
     * @return probablity that this in electron (calculated only by TPC
     */
    inline Float_t GetProbElectron() const { return fTpcPidProbElectron; };
    /**
     *
     * @return dEdX
     */
    inline Float_t GetDeDx() const { return fTpcDeDx; };
    /**
     *
     * @param deDx set dEd
     */
    inline void SetDeDx(Float_t deDx) { fTpcDeDx = deDx; }
    /**
     * set TCP sigma's
     * @param pion  pion sigma
     * @param kaons kaon sigma
     * @param proton proton sigma
     * @param electron electron sigma
     */
    void SetSigma(Float_t pion, Float_t kaons, Float_t proton, Float_t electron);
    /**
     * set TPC probability that this is:
     * @param pion  pion
     * @param kaon kaon
     * @param proton proton
     * @param electron electron
     */
    void SetPidProb(Float_t pion, Float_t kaon, Float_t proton, Float_t electron);
    /**
     * set number of tpc hits
     * @param tpcHits
     */
    inline void SetNHits(Int_t tpcHits) { fTpcNHits = tpcHits; }
    virtual ~TpcTrack();
    ClassDef(TpcTrack, 1)
  };
}  // namespace Hal
#endif /* HALTPCTRACK_H_ */
