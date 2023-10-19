/*
 * FemtoYlmIndexes.h
 *
 *  Created on: 8 lip 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_FEMTOYLMINDEXES_H_
#define HAL_ANALYSIS_FEMTO_BASE_FEMTOYLMINDEXES_H_

#include <TObject.h>

#include "Array.h"

/**
 * class for holding L,M indexes for spherical harmonics
 */

namespace Hal {

  class FemtoYlmIndexes : public TObject {
    Int_t fL;
    Int_t fMaxJM;
    Double_t* fEls;  //[fMaxJM]
    Double_t* fEms;  //[fMaxJM]
    Int_t* fElsi;    //[fMaxJM]
    Int_t* fEmsi;    //[fMaxJM]
    Array_2<Int_t> fIndexes;

  public:
    /**
     * default construtor
     * @param L max L
     */
    FemtoYlmIndexes(Int_t L = 5);
    FemtoYlmIndexes(const FemtoYlmIndexes& other);
    /**
     * assignement operator
     * @param other
     * @return
     */
    FemtoYlmIndexes& operator=(const FemtoYlmIndexes& other);
    /**
     * changes max L to newL
     * @param newL
     */
    void Resize(Int_t newL);
    /**
     *
     * @param i
     * @return L for given index (as double)
     */
    inline Double_t GetEls(Int_t i) const { return fEls[i]; }
    /**
     *
     * @param i
     * @return L for given index (as int)
     */
    inline Int_t GetElsi(Int_t i) const { return fElsi[i]; }
    /**
     *
     * @param i
     * @return M for given index (as double)
     */
    inline Double_t GetEms(Int_t i) const { return fEms[i]; }
    /**
     *
     * @param i
     * @return M for given index (as int)
     */
    inline Int_t GetEmsi(Int_t i) const { return fEmsi[i]; }
    /**
     *
     * @return maxL
     */
    inline Int_t GetMaxL() const { return fL; }
    /**
     *
     * @return (L+1)^2
     */
    inline Int_t GetMaxJM() const { return fMaxJM; }
    /**
     *
     * @param l
     * @param m
     * @return index for given l and m
     */
    inline Int_t GetIndex(Int_t l, Int_t m) const { return fIndexes.Get(l, fL + m); };
    /**
     * return pad id for drawing SH coefficients
     * @param l
     * @param m
     * @return
     */
    Int_t GetPadId(Int_t l, Int_t m) const;
    /**
     * destructor
     */
    virtual ~FemtoYlmIndexes();
    ClassDef(FemtoYlmIndexes, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_FEMTOYLMINDEXES_H_ */
