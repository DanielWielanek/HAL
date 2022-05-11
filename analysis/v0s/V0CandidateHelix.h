/*
 * V0CandidateHelix.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALV0CANDIDATEHELIX_H_
#define HALV0CANDIDATEHELIX_H_

#include "V0Candidate.h"
/**
 * create V0 candidate for experiments with constant magnetic field
 * where helix track can be used
 */

namespace Hal {
  class V0CandidateHelix : public V0Candidate {
  public:
    V0CandidateHelix();
    void MakeV0(Double_t mPos, Double_t mNeg, Int_t pidThis, Int_t pidPos, Int_t pidNeg);
    virtual ~V0CandidateHelix();
    ClassDef(V0CandidateHelix, 1)
  };
}  // namespace Hal
#endif /* HALV0CANDIDATEHELIX_H_ */
