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

#include "FemtoCorrFunc.h"
/**
 * collection of correlation functions binned by kT value
 */
class TH1D;
namespace Hal {
  class FemtoCorrFuncKt : public FemtoCorrFunc1D {
    TH1D* fKtNum;
    TH1D* fKtDen;

  protected:
    virtual void AddHTMLCode(HtmlFile& file) const;

  public:
    FemtoCorrFuncKt();
    /**
     *
     * @param h
     * @param init
     * @param ktBins number of kt bins
     * @param ktMin
     * @param ktMax
     */
    FemtoCorrFuncKt(const DividedHisto1D& h,
                    const std::initializer_list<double>& init = {0, 10},
                    Int_t ktBins                              = 100,
                    Double_t ktMin                            = 0,
                    Double_t ktMax                            = 2);
    FemtoCorrFuncKt(const FemtoCorrFuncKt& other);
    TH1D* GetKtNum() const { return fKtNum; };
    TH1D* GetKtDen() const { return fKtDen; };
    virtual Bool_t Check();
    virtual void Add(const Object* pack);
    virtual void FillNum(FemtoPair* pair);
    virtual void FillDenPerfect(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenRotated(FemtoPair* pair);
    virtual void FillDenMixed(FemtoPair* pair);
    virtual void FillDenHemisphere(FemtoPair* pair) { FillDenRotated(pair); };
    virtual void FillDenCharged(FemtoPair* pair) { FillDenMixed(pair); };
    virtual TString HTMLExtract(Int_t no, TString dir = "") const;
    virtual ~FemtoCorrFuncKt();
    ClassDef(FemtoCorrFuncKt, 2)
  };
}  // namespace Hal

#endif /* HALFEMTOCORRFUNCCOLLECTIONKT_H_ */
