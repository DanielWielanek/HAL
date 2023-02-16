/*
 * CorrFitDatabase.h
 *
 *  Created on: 22 sty 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITPARAMSSETUP_H_
#define HALCORRFITPARAMSSETUP_H_

#include "Array.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

namespace Hal {
  /**
   * store information about number of parameters combination stored in
   * XML.
   */

  class CorrFitParamsSetup : public TObject {
    Int_t fSize;
    Array_1<Double_t> fMin;
    Array_1<Double_t> fMax;
    Array_1<Int_t> fNpoints;
    TString* fNames;  //[fSize]
  public:
    CorrFitParamsSetup(Int_t params = 1);
    CorrFitParamsSetup(TString xmlFile);
    CorrFitParamsSetup(const CorrFitParamsSetup& other);
    CorrFitParamsSetup& operator=(const CorrFitParamsSetup& rhs);
    virtual void Print(Option_t* opt = "") const;
    void SetParameter(Int_t par_id, Double_t min, Double_t max, Int_t points, TString name);
    static Bool_t TestMapFile(Int_t jobId);
    Int_t GetNJobs() const;
    Int_t GetNParams() const { return fSize; }
    Int_t GetNSteps(Int_t par_id) const { return fNpoints.Get(par_id) - 1; };
    Int_t GetNPoints(Int_t par_id) const { return fNpoints.Get(par_id); };
    Double_t GetMin(Int_t par_id) const { return fMin.Get(par_id); };
    Double_t GetMax(Int_t par_id) const { return fMax.Get(par_id); };
    Double_t GetStepSize(Int_t par_id) const;
    std::vector<int> GetDimensions() const;
    TString GetParName(Int_t par_id) const { return fNames[par_id]; };
    virtual ~CorrFitParamsSetup() { delete[] fNames; }
    ClassDef(CorrFitParamsSetup, 2)
  };
}  // namespace Hal


#endif /* HALCORRFITPARAMSSETUP_H_                                                                                               \
        */
