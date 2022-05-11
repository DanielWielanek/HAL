/*
 * TwoTrackSailorCowboyCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKSAILORCOWBOYCUT_H_
#define HALTWOTRACKSAILORCOWBOYCUT_H_

#include "Helix.h"
#include "TwoTrackCut.h"

namespace Hal {
  class TwoTrackSailorCowboyCut : public TwoTrackCut {
    Double_t fThreshold;
    HelixZ fHz1, fHz2;

  protected:
    Int_t Sailor() const { return 1; };
    Int_t Cowboy() const { return -1; };
    Double_t NormalizeAngle(const TVector3 pos, Double_t x, Double_t y) const;
    Bool_t AreBetween(Double_t phi, Double_t sign) const;
    Bool_t Check();

  public:
    TwoTrackSailorCowboyCut();
    TwoTrackSailorCowboyCut(const TwoTrackSailorCowboyCut& other);
    TwoTrackSailorCowboyCut& operator=(const TwoTrackSailorCowboyCut& other);
    virtual Bool_t Pass(TwoTrack* pair);
    /**
     * if tracks cross at less than r_min then threat them as sailros
     * @param r_min
     */
    void SetThreshold(Double_t r_min) { fThreshold = r_min; };
    void Cowboys() { SetMinAndMax(-1); };
    void Sailors() { SetMinAndMax(1); }
    Bool_t Init(Int_t formatId);
    virtual ~TwoTrackSailorCowboyCut();
    virtual Package* Report() const;
    ClassDef(TwoTrackSailorCowboyCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKSAILORCOWBOYCUT_H_ */
