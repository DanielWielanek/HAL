/*
 * TwoTrackRadialDistanceCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 *		modified AliFemtoTwoTrackCutRadialDistance
 */
#ifndef HALTWOTRACKRADIALDISTANCECUT_H_
#define HALTWOTRACKRADIALDISTANCECUT_H_
#include "TwoTrackCut.h"

#include <TDatabasePDG.h>
#include <TVector2.h>

#define mag_const 0.15
/**
 * check radial distance cut in barbell like detector where is constant magnetic
 * field in Z direction
 */
namespace Hal {
  class TwoTrackRadialDistanceCut : public TwoTrackCut {
    Double_t fMinRad;
    Double_t fMaxRad;
    TDatabasePDG* fPDG;
    Double_t fMagSign, fMagField;
    Double_t fMagConst;

  public:
    TwoTrackRadialDistanceCut();
    /**
     * set TPC/detector radius
     * @param min inner radius
     * @param max outer radius
     */
    void SetRadius(Double_t min, Double_t max);
    /**
     * set magnetic field
     * @param field magnetic field
     */
    void SetMagFiled(Double_t field);
    /**
     * set magnetic field sign
     * @param sign
     */
    void SetMagFieldSign(Double_t sign);
    Bool_t Init(Int_t task_id = 0);
    Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackRadialDistanceCut();
    ClassDef(TwoTrackRadialDistanceCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKRADIALDISTANCECUT_H_ */
