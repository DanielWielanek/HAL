/*
 * WielanekMapGenerator.h
 *
 *  Created on: 28 sty 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALWIELANEKMAPGENERATOR_H_
#define HALWIELANEKMAPGENERATOR_H_
#include "Femto1DMapGenerator.h"

#include <TString.h>

#include "FemtoWeightGenerator.h"
class TH2D;

/**
 * alternative map generator this code generate maps that are not compatible
 * with CorrFitKisiel! this code works only with PRF frame
 */

namespace Hal {
  class FemtoWielanekMapGenerator : public Femto1DMapGenerator {
    Int_t fPairsNo;
    Double_t* fWeightArray;  //[fPairsNo]
    Double_t* fAngleArray;   //[fPairsNo]
    Double_t GeneratePairs(FemtoPair* pair, Double_t R, Double_t K);

  public:
    /**
     * default constructor
     */
    FemtoWielanekMapGenerator();
    /**
     * initialize mapper
     */
    Bool_t Init();
    /**
     * create map
     * @param pairs_per_bin number of pairs in single bin
     */
    void Exec(Int_t pairs_per_bin);
    virtual ~FemtoWielanekMapGenerator();
    ClassDef(FemtoWielanekMapGenerator, 1)
  };
}  // namespace Hal
#endif /* HALWIELANEKMAPGENERATOR_H_ */
