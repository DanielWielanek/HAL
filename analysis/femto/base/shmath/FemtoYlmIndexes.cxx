/*
 * FemtoYlmIndexes.cpp
 *
 *  Created on: 8 lip 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoYlmIndexes.h"

#include "Cout.h"

namespace Hal {


  FemtoYlmIndexes::~FemtoYlmIndexes() {
    if (fEls) delete[] fEls;
    if (fEms) delete[] fEms;
    if (fElsi) delete[] fElsi;
    if (fEmsi) delete[] fEmsi;
  }

  FemtoYlmIndexes& FemtoYlmIndexes::operator=(const FemtoYlmIndexes& other) {
    if (this == &other) return *this;
    if (this->fL == other.fL) return *this;
    Resize(other.fL);
    return *this;
  }

  FemtoYlmIndexes::FemtoYlmIndexes(Int_t L) :
    fL(L), fMaxJM((L + 1) * (L + 1)), fEls(nullptr), fEms(nullptr), fElsi(nullptr), fEmsi(nullptr) {
    Resize(L);
  }

  void FemtoYlmIndexes::Resize(Int_t newL) {
    if (fEls) {
      delete[] fEls;
      delete[] fEms;
      delete[] fElsi;
      delete[] fEmsi;
      fEls  = nullptr;
      fEms  = nullptr;
      fElsi = nullptr;
      fEmsi = nullptr;
    }
    fL = newL;
    if (fL == 0) return;
    if (fL > 6) { Cout::PrintInfo("Creating YLM for L>6!", EInfo::kError); }
    fMaxJM = (fL + 1) * (fL + 1);
    if (fL > 0) {
      fEls     = new Double_t[fMaxJM];
      fEms     = new Double_t[fMaxJM];
      fElsi    = new Int_t[fMaxJM];
      fEmsi    = new Int_t[fMaxJM];
      fIndexes = Array_2<Int_t>(fL + 1, 2 * fL + 1);
      for (int i = 0; i <= fL; i++) {
        for (int j = 0; j < 2 * fL + 1; j++) {
          fIndexes[i][j] = -1;
        }
      }
    }
    int el = 0;
    int em = 0;
    int il = 0;
    do {
      fEls[il]              = el;
      fEms[il]              = em;
      fElsi[il]             = (int) el;
      fEmsi[il]             = (int) em;
      fIndexes[el][fL + em] = il;
#ifdef _FINISH_DEBUG_
      std::cout << "il el em " << il << " " << fElsi[il] << " " << fEmsi[il] << std::endl;
#endif
      em++;
      il++;
      if (em > el) {
        el++;
        em = -el;
      }
    } while (el <= fL);
  }

  FemtoYlmIndexes::FemtoYlmIndexes(const FemtoYlmIndexes& other) : FemtoYlmIndexes(other.fL) {}

  Int_t FemtoYlmIndexes::GetPadId(Int_t l, Int_t m) const {
    Array_2<Int_t> map;
    map.MakeBigger(fL + 1, fL + 1);
    Int_t pad = 1;
    for (int i = 0; i <= fL; i++) {
      for (int j = 0; j <= fL; j++) {
        map[i][j] = pad++;
      }
    }
    Int_t x = l;
    Int_t y = 0;
    if (m >= 0) {
      y = l - m;
    } else {
      y = l;
      x = l + m;
    }
    return map[x][y];
  }


} /* namespace Hal */
