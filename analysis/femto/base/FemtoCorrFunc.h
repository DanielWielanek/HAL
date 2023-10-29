/*
 * CorrFuncCollection.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFUNCCOLLECTION_H_
#define HALCORRFUNCCOLLECTION_H_


#include "Array.h"
#include "DividedHisto.h"
#include "HtmlFile.h"
#include "ObjectMatrix.h"
#include "Parameter.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <initializer_list>
#include <iostream>

namespace Hal {
  class FemtoPair;

  /**
   * basic class for storing correlation function (binned)
   */
  class FemtoCorrFunc : public Object {
    ParameterInt fEventCol;
    ParameterInt fPairCol;
    TString fComment;

  protected:
    ObjectMatrix_1* fArray = {nullptr};
    /**
     * deafult c-tor
     * @param h correlation function to duplication
     * @param size number of correlation function bins (total)
     */
    FemtoCorrFunc(const DividedHisto1D* h, Int_t size = 0);

  public:
    FemtoCorrFunc();
    FemtoCorrFunc(const DividedHisto1D& h, Int_t size = 0);
    FemtoCorrFunc(const FemtoCorrFunc& other);
    FemtoCorrFunc& operator=(const FemtoCorrFunc& other);
    /**
     * additional check of sizes, ranges
     * @return
     */
    virtual Bool_t Check() = 0;
    /**
     * add those objects
     * @param pack
     */
    virtual void Add(const Object* pack);
    virtual void Browse(TBrowser* b);
    /**
     * fill numerator
     * @param pair
     */
    virtual void FillNum(FemtoPair* pair) = 0;
    virtual void FillDenPerfect(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenRotated(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenMixed(FemtoPair* pair) = 0;
    virtual void FillDenHemisphere(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenCharged(FemtoPair* pair) { FillDenMixed(pair); };
    /**
     * set comment for this object
     * @param comment
     */
    void SetComment(TString comment) { fComment = comment; };
    /**
     * set event collection ID
     * @param no
     */
    void SetEventCollID(Int_t no) { fEventCol.SetValue(no); };
    /**
     * set pair collection ID
     * @param no
     */
    void SetPairCollID(Int_t no) { fPairCol.SetValue(no); };
    /**
     *
     * @return event collection ID
     */
    Int_t GetEventCollID() const { return fEventCol.GetValue(); };
    /**
     *
     * @return pair collection ID
     */
    Int_t GetPairCollID() const { return fPairCol.GetValue(); };
    /**
     *
     * @return total number of subfunction
     */
    Int_t GetEntries() const { return fArray->GetSize(); };
    /**
     *
     * @return comment
     */
    TString GetComment() const { return fComment; }
    /**
     * return correlation function at given position
     * @param i
     * @return
     */
    DividedHisto1D* GetCF(Int_t i) const { return (DividedHisto1D*) fArray->At(i); };
    virtual ~FemtoCorrFunc();
    ClassDef(FemtoCorrFunc, 1)
  };

}  // namespace Hal
#endif /* HALFEMTO_ANALYSIS_FEMTO_BASE_HALCORRFUNCCOLLECTION_H_ */
