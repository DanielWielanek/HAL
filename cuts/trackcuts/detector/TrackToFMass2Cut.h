/*
 * TrackToFMass2Cut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKTOFMASS2CUT_H_
#define HALTRACKTOFMASS2CUT_H_
#include "TrackExpCut.h"
/**
 * class for cut on ToF mass2
 * user can set standard minium and maximum cut (like in basic to cut)
 * and by SetPolyLIneUp/Down apply additional polynomial-like cut that can be
 * expressed as a + b*p + c*p^2
 */
namespace Hal {
  class TrackToFMass2Cut : public TrackExpCut {
  protected:
    Double_t fLowLine[3];
    Double_t fHighLine[3];
    void
    GetPoly(Double_t& a, Double_t& b, Double_t& c, Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t x3, Double_t y3);
    TrackToFMass2Cut(Int_t nParams);

  public:
    TrackToFMass2Cut();
    TrackToFMass2Cut(const TrackToFMass2Cut& other);
    void SetPolyLineDown(Double_t a, Double_t b, Double_t c);
    void SetPolyLineUp(Double_t a, Double_t b, Double_t c);
    void GetPolyUp(Double_t& a, Double_t& b, Double_t& c) const;
    void GetPolyDown(Double_t& a, Double_t& b, Double_t& c) const;
    void SetUpPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t x3, Double_t y3);
    void SetDownPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t x3, Double_t y3);
    virtual Bool_t Init(Int_t formad_id);
    virtual Bool_t Pass(Track* tr);
    virtual ~TrackToFMass2Cut();
    virtual Package* Report() const;
    ClassDef(TrackToFMass2Cut, 1)
  };
  /**
   * cut like  @see TrackToFMass2Cut but also reject all particles with
   * mass==0
   */
  class TrackTofMass2CutNoZeros : public TrackToFMass2Cut {
  public:
    TrackTofMass2CutNoZeros() {};
    TrackTofMass2CutNoZeros(const TrackToFMass2Cut& other);
    virtual Bool_t Pass(Track* tr);
    virtual ~TrackTofMass2CutNoZeros() {};
    ClassDef(TrackTofMass2CutNoZeros, 1)
  };
}  // namespace Hal
#endif /* HALTRACKTOFMASS2CUT_H_ */
