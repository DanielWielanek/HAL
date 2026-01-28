/*
 * CorrFitWielanek.h
 *
 *  Created on: 28 sty 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITWIELANEK_H_
#define HALCORRFITWIELANEK_H_

#include "CorrFitKisiel.h"

#include <Rtypes.h>
#include <TString.h>


/**
 * class for fitting correlation functions by using "wielanek maps"
 * this class is for testing conception only
 */
namespace Hal {
  class CorrFitWielanek : public CorrFitKisiel {
    Int_t fExtrapolationsSteps;
    TH2D* fFSIMap;
    TF1* fSource;

  protected:
    /**
     * create map that is compatible with CorrFitKisiel
     */
    void CreateMap();
    virtual void Check();

  public:
    CorrFitWielanek();
    /**
     * set source emission function, function must fulfill following criteria:
     * parameter q - is radius
     * parameter 1 - is to normalization (or not present)
     * @param func source emission function to set
     */
    void SetSourceEmissionFunction(TF1* func);
    /**
     * set source emission function by name
     * @param opt name of source emission function, currently only "gaus" is
     * supported
     */
    void SetSourceEmissionFunstion(Option_t* opt);
    void LoadMapFromFile(TString name, Int_t kt_bin = 0);
    /**
     * set extrapotlation steps, step in radii map is equalt to
     * (Rmax-Rmin)/extrapolation steps
     * @param step number of steps
     */
    void SetExtrapolationsSteps(Int_t step);
    virtual ~CorrFitWielanek();
    ClassDef(CorrFitWielanek, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITWIELANEK_H_ */
