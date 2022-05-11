/*
 * FemtoSourceModelGausKt.h
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOSOURCEMODELGAUSMT_H_
#define HALFEMTOSOURCEMODELGAUSMT_H_

#include "FemtoPair.h"
#include "FemtoSourceModel.h"

namespace Hal {
  class FemtoSourceModelGausMt : public FemtoSourceModel3D {
  public:
    FemtoSourceModelGausMt();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModelGausMt(const FemtoSourceModelGausMt& model);
    void SetOutParams(Double_t a, Double_t b);
    void SetSideParams(Double_t a, Double_t b);
    void SetLongParams(Double_t a, Double_t b);
    void SetM2(Double_t par);
    virtual FemtoSourceModel* MakeCopy() const;
    virtual void GenerateCoordinates(FemtoPair* Pair);
    virtual Package* Report() const;
    virtual ~FemtoSourceModelGausMt();
    ClassDef(FemtoSourceModelGausMt, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOSOURCEMODELGAUSMT_H_ */
