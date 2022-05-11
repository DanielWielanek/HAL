/*
 * ToFBasicCut.h
 *
 *  Created on: 26 wrz 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKBASICTOFCUT_H_
#define HALTRACKBASICTOFCUT_H_

#include "TrackExpCut.h"
namespace Hal {
  class TrackBasicToFCut : public TrackExpCut {
    static const Int_t fgMass2;
    static const Int_t fgBeta;
    static const Int_t fgFlag;

  public:
    TrackBasicToFCut();
    Bool_t Init(Int_t formad_id);
    static Int_t Beta() { return fgBeta; };
    static Int_t Mass2() { return fgMass2; };
    static Int_t Flag() { return fgFlag; };
    virtual Bool_t Pass(Track* tr);
    virtual ~TrackBasicToFCut();
    ClassDef(TrackBasicToFCut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKBASICTOFCUT_H_ */
