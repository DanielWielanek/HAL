/*
 * FemtoWeightGeneratorScottPratt.h
 *
 *  Created on: 30 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORSCOTTPRATT_H_
#define ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORSCOTTPRATT_H_


#include "FemtoWeightGenerator.h"

namespace Hal {
  namespace ScottPratt {
    class CWaveFunction;
  }  // namespace ScottPratt
  class FemtoWeightGeneratorScottPratt : public FemtoWeightGenerator {
    Hal::ScottPratt::CWaveFunction* fWave;  //!
    double fPa[4], fPb[4];
    double fXa[4], fXb[4];

  public:
    FemtoWeightGeneratorScottPratt(TString pairFile, Bool_t phase);
    FemtoWeightGeneratorScottPratt(const FemtoWeightGeneratorScottPratt& other);
    virtual void SetPairType(Femto::EPairType aPairType);
    virtual FemtoWeightGenerator* MakeCopy() const { return new FemtoWeightGeneratorScottPratt(*this); };
    virtual Double_t GenerateWeight(FemtoPair* pair);
    virtual ~FemtoWeightGeneratorScottPratt();
    ClassDef(FemtoWeightGeneratorScottPratt, 1)
  };

} /* namespace Hal */

#endif /* ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORSCOTTPRATT_H_ */
