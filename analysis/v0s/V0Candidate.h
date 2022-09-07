/*
 * V0Candidate.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALV0CANDIDATE_H_
#define HALV0CANDIDATE_H_

#include "TwoTrack.h"
#include "V0Track.h"
#include <TVector3.h>
/**
 * abstract class for V0 candidate
 */

namespace Hal {
  class V0Candidate : public TwoTrack {
  protected:
    V0Track fV0Track;
    TVector3 fDCA;

  public:
    V0Candidate();
    /**
     * make V0Track from this class
     * @param mPos mass of positive daughter
     * @param mNeg mass of negative daughter
     */
    virtual void MakeV0(Double_t mPos, Double_t mNeg, Int_t pidThis, Int_t pidPos, Int_t pidNeg) = 0;
    /**
     *
     * @return reference to V0Track
     */
    const V0Track& GetV0Track() { return fV0Track; };
    /**
     * return DCA
     * @return
     */
    const TVector3& GetDCA() const { return fDCA; }
    virtual ~V0Candidate();
    ClassDef(V0Candidate, 1)
  };
}  // namespace Hal
#endif /* HALV0CANDIDATE_H_ */
