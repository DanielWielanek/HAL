/*
 * FemtoDebug2DCF.h
 *
 *  Created on: 25 lis 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_FEMTODEBUG2DCF_H_
#define HAL_ANALYSIS_FEMTO_BASE_FEMTODEBUG2DCF_H_
#include "DividedHisto.h"

namespace Hal {
  class FemtoPair;
  class FemtoDebug2DCF : public DividedHisto2D {
    static std::function<void(TH1*, Hal::FemtoPair*)>* fMagicFunction;  //!
  protected:
    virtual void SetAxisNames(TH1* h);

  public:
    static void SetFillFunction(std::function<void(TH1*, Hal::FemtoPair*)>* func) { fMagicFunction = func; }
    /**
     * constructor for Streamer
     */
    FemtoDebug2DCF();
    /**
     * main constructor
     * @param name
     * @param phibins
     * @param eta_bins
     * @param min
     * @param max
     */
    FemtoDebug2DCF(TString name, Int_t phibins, Int_t eta_bins);
    FemtoDebug2DCF(const FemtoDebug2DCF& other);
    /**
     * draw this object
     * @param opt if "num" then only numerator is drawn, if "den" only denominator
     * is drawn, if "all" current pad is divided and numertor, by default when
     * "all" is called then histograms are rebinned for performance optimization,
     * this rebinning can be ingored by adding "hd", othrerwise only divided
     * histogram is drawn denominator and CF are drawn
     */
    virtual void Draw(Option_t* opt);
    virtual void Browse(TBrowser* b);
    virtual void FillNumObj(TObject* obj);
    virtual void FillDenObj(TObject* obj);
    TObject* Clone(const char* newname = "") const;
    virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
    virtual ~FemtoDebug2DCF();
    ClassDef(FemtoDebug2DCF, 2)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_FEMTODEBUG2DCF_H_ */
