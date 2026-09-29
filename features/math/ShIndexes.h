/*
 * ShIndexes.h
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MATH_SHINDEXES_H_
#define HAL_FEATURES_MATH_SHINDEXES_H_

#include <RtypesCore.h>

#include "Array.h"

namespace Hal {

  namespace Sh {
    /**
     * indexing of SH from l,m -> to one dimensional array and vice versa
     */
    class Indexes : public TObject {
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
      Indexes(Int_t maxL = 5);
      /**
       *  copy constructor
       */
      Indexes(const Indexes& other);
      /**
       * assignement operator
       * @param other
       * @return
       */
      Indexes& operator=(const Indexes& other);
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
      virtual void Print(Option_t* option = "") const;
      /**
       * destructor
       */
      virtual ~Indexes();
      ClassDef(Indexes, 1)
    };

    /**
     * simliar to FemtoYlmIndexes but ignores m<0
     */
    class IndexesShort : public TObject {
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
      IndexesShort(Int_t L = 5);
      IndexesShort(const IndexesShort& other);
      /**
       * assignement operator
       * @param other
       * @return
       */
      IndexesShort& operator=(const IndexesShort& other);
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
      virtual void Print(Option_t* option = "") const;
      /**
       * destructor
       */
      virtual ~IndexesShort();
      ClassDef(IndexesShort, 1)
    };

  }  // namespace Sh

} /* namespace Hal */

#endif /* HAL_FEATURES_MATH_SHINDEXES_H_ */
