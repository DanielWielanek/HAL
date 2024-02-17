/*
 * FemtoPainterOptions.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoDrawOptions.h"

#include "Std.h"
#include "StdString.h"
#include "Style.h"

#include <initializer_list>
#include <iterator>

#include <RtypesCore.h>
#include <TString.h>


namespace Hal {

  /**
   * configure drawing of functions
   */


  void FemtoDrawOptions::SetCfRanges(std::initializer_list<Double_t> vals) { fYRanges = Hal::Std::GetVector(vals); }

  FemtoDrawOptions::FemtoDrawOptions(TString opt) {
    AddConflicts({"cf", "num", "re"});
    AddConflicts({"re", "im"});
    AddConflicts({"diag1", "diag2"});
    AddConflicts({"separate", "skipneg"});
  }

  void FemtoDrawOptions::SetQRanges(Double_t min, Double_t max) {
    if (min == max) {
      fXRanges.clear();
    } else {
      if (fXRanges.size() < 2) fXRanges.resize(2);
      fXRanges[0] = min;
      fXRanges[1] = max;
    }
  }

  void FemtoDrawOptions::SetOptions(TString opts) {
    auto vec = Hal::Std::ExplodeString(opts, '+');
    for (auto i : vec) {
      AddTag(i);
    }
  }

  FemtoDrawOptions::FemtoDrawOptions(const FemtoDrawOptions& other) : fXRanges(other.fXRanges), fYRanges(other.fXRanges) {
    if (other.fHistoStyle) fHistoStyle = new Hal::HistoStyle(*other.fHistoStyle);
    if (other.fHistoStyle2) fHistoStyle2 = new Hal::HistoStyle(*other.fHistoStyle2);
    if (other.fPadStyle) fHistoStyle = new Hal::HistoStyle(*other.fHistoStyle);
  }

  FemtoDrawOptions& FemtoDrawOptions::operator=(const FemtoDrawOptions& other) {
    if (this == &other) return *this;
    fXRanges = other.fXRanges;
    fYRanges = other.fYRanges;

    if (fHistoStyle) {
      delete fHistoStyle;
      fHistoStyle = nullptr;
    }
    if (fHistoStyle2) {
      delete fHistoStyle2;
      fHistoStyle2 = nullptr;
    }
    if (fHistoStyle) {
      delete fHistoStyle;
      fHistoStyle = nullptr;
    }

    if (other.fHistoStyle) fHistoStyle = new Hal::HistoStyle(*other.fHistoStyle);
    if (other.fHistoStyle2) fHistoStyle2 = new Hal::HistoStyle(*other.fHistoStyle2);
    if (other.fPadStyle) fHistoStyle = new Hal::HistoStyle(*other.fHistoStyle);
    return *this;
  }

  void FemtoDrawOptions::SetHistoStyle(const Hal::HistoStyle& style, Int_t no) {
    if (no != 0) {
      if (fHistoStyle2) delete fHistoStyle2;
      fHistoStyle2 = new Hal::HistoStyle(style);
    } else {
      if (fHistoStyle) delete fHistoStyle;
      fHistoStyle = new Hal::HistoStyle(style);
    }
  }

  void FemtoDrawOptions::SetPadStyle(const Hal::PadStyle& style) {
    if (fPadStyle) delete fPadStyle;
    fPadStyle = new Hal::PadStyle(style);
  }

  FemtoDrawOptions::~FemtoDrawOptions() {
    if (fHistoStyle) delete fHistoStyle;
    if (fHistoStyle2) delete fHistoStyle2;
    if (fPadStyle) delete fPadStyle;
  }

} /* namespace Hal */
