/*
 * TrackMC.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALMCTRACK_H_
#define HALMCTRACK_H_

#include "Track.h"
/**
 * basic class for stroing MC tracks something like "Particle"
 */
namespace Hal {
  class McTrack : public Track {

  protected:
    TLorentzVector fFreez;
    TLorentzVector fStart;
    Int_t fPDG;

  public:
    McTrack();
    McTrack& operator=(const McTrack& other) = default;
    /**
     *
     * @return track pid code
     */
    virtual Int_t GetPdg() const { return fPDG; };
    /**
     * @return freezout position
     */
    inline const TLorentzVector& GetFreezoutPosition() const { return fFreez; };
    /**
     *
     * @return start position of particle
     */
    inline const TLorentzVector& GetStartPosition() const { return fStart; };
    virtual void CopyData(Track* other);
    void Boost(Double_t x, Double_t y, Double_t z);
    void SetPdg(Int_t pdg) { fPDG = pdg; };
    /**
     * set freezout position
     * @param x
     * @param y
     * @param z
     * @param t
     */
    inline void SetFreezoutPosition(Double_t x, Double_t y, Double_t z, Double_t t) { fFreez.SetXYZT(x, y, z, t); }
    /**
     * set start point position
     * @param fieldID
     * @return
     */
    inline void SetStartPosition(Double_t x, Double_t y, Double_t z, Double_t t) { fStart.SetXYZT(x, y, z, t); }
    virtual Float_t GetFieldVal(Int_t fieldID) const;
    virtual TString GetFieldName(Int_t fieldID) const;
    virtual ~McTrack();
    ClassDef(McTrack, 1)
  };
}  // namespace Hal
#endif /* HALMCTRACK_H_ */
