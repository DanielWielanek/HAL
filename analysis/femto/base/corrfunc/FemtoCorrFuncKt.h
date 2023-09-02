/*
 * CorrFuncCollectionKt.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOCORRFUNCCOLLECTIONKT_H_
#define HALFEMTOCORRFUNCCOLLECTIONKT_H_

#include "FemtoCorrFunc1D.h"
/**
 * collection of correlation functions binned by kT value
 */
class TH1D;
namespace Hal {
  class FemtoCorrFuncKt : public FemtoCorrFunc1D {
  protected:
    virtual Double_t GetPairValDenRot(Hal::FemtoPair* pair) const;
    virtual Double_t GetPairValDenHemi(Hal::FemtoPair* pair) const;
    virtual Double_t GetPairValNum(Hal::FemtoPair* pair) const;

  public:
    FemtoCorrFuncKt() {};
    /**
     *
     * @param h
     * @param init
     * @param ktBins number of kt bins
     * @param ktMin
     * @param ktMax
     */
    FemtoCorrFuncKt(const DividedHisto1D& h, const std::initializer_list<double>& init = {0, 10}, Int_t ktBins = 100);
    virtual ~FemtoCorrFuncKt() {};
    ClassDef(FemtoCorrFuncKt, 3)
  };
}  // namespace Hal

#endif /* HALFEMTOCORRFUNCCOLLECTIONKT_H_ */
