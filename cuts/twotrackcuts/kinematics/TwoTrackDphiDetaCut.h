/*
 * TwoTrackDphiDetaCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKDPHIDETACUT_H_
#define HALTWOTRACKDPHIDETACUT_H_
#include "TwoTrackCut.h"

#include <TDatabasePDG.h>
#include <TVector2.h>


/** check if pair is inside the square with given d-phi* and d-eta*/
namespace Hal {
  class TwoTrackDphiDetaCut : public TwoTrackCut {
  public:
    TwoTrackDphiDetaCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackDphiDetaCut();
    ClassDef(TwoTrackDphiDetaCut, 1)
  };
  /** check if pair is inside the square with given d-phi* and d-eta*/

  class TwoTrackDphistarDetaCut : public TwoTrackCut {
    Double_t fMinRad;
    Double_t fMaxRad;
    TDatabasePDG* fPDG;
    Double_t fMagSign, fMagField, fMagScale;
    const Double_t fMagConst;

  public:
    TwoTrackDphistarDetaCut();
    /**
     * set detector radii
     * @param min inner radius in m
     * @param max outer radius in m
     */
    void SetRadius(Double_t min, Double_t max) {
      fMinRad = min;
      fMaxRad = max;
    }
    /**
     *
     * @param sign magnetic field sing
     */
    void SetMagSing(Double_t sign);
    /**
     *
     * @param field magnetic field
     */
    void SetMagField(Double_t field);
    virtual Bool_t Pass(TwoTrack* pair);
    virtual Bool_t Init(Int_t task_id = 0);
    virtual Package* Report() const;
    virtual ~TwoTrackDphistarDetaCut();
    ClassDef(TwoTrackDphistarDetaCut, 1)
  };
}  // namespace Hal

#endif /* HALTWOTRACKDPHIDETACUT_H_ */
