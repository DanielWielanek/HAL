#include "Cut.h"

#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"

#include <TObjArray.h>

namespace Hal {
  Cut::Cut(const Int_t size, ECutUpdate update, TString group_flag) :
    TNamed(),
    fPassed(0),
    fFailed(0),
    fTotal(0),
    fSubCut(size),
    fCutSize(size),
    fInit(kFALSE),
    fIsCloned(kFALSE),
    fState(0),
    fCollectionID(0),
    fLabel(-1),
    fUpdateRatio(update),
    fWeight(1.0),
    fGroupFlag(group_flag) {}

  Cut::Cut(const Cut& cut) : TNamed(cut), fCutSize(cut.fCutSize), fUpdateRatio(cut.fUpdateRatio), fGroupFlag(cut.fGroupFlag) {
    this->fPassed       = cut.fPassed;
    this->fFailed       = cut.fFailed;
    this->fTotal        = cut.fTotal;
    this->fSubCut       = cut.fSubCut;
    this->fInit         = cut.fInit;
    this->fIsCloned     = cut.fIsCloned;
    this->fState        = cut.fState;
    this->fCollectionID = cut.fCollectionID;
    this->fWeight       = cut.fWeight;
    this->fLabel        = cut.fLabel;
  }

  void Cut::Reset() { fPassed = fFailed = fTotal = 0; }

  Bool_t Cut::Validate() {
    fTotal++;
    fState = fSubCut.Validate();
    if (fState) {
      fPassed++;
      return kTRUE;
    } else {
      fFailed++;
      return kFALSE;
    }
  }

  Bool_t Cut::AntiValidate() {
    fTotal++;
    fState = !fSubCut.Validate();
    if (fState) {
      fPassed++;
      return kTRUE;
    } else {
      fFailed++;
      return kFALSE;
    }
  }

  Bool_t Cut::ValidateAbs() {
    fTotal++;
    fState = fSubCut.ValidateAbs();
    if (fState) {
      fPassed++;
      return kTRUE;
    } else {
      fFailed++;
      return kFALSE;
    }
  }

  Bool_t Cut::ForcedUpdate(Bool_t state) {
    if (state) {
      ++fPassed;
    } else {
      ++fFailed;
    }
    ++fTotal;
    return state;
  }

  void Cut::SetMinAndMax(Double_t val, Int_t i) { SetMinMax(val, val, i); }

  void Cut::SetMinMax(Double_t min, Double_t max, Int_t i) {
    if (i > fSubCut.GetSize() || i < 0) {
      Cout::PrintInfo(Form("Seting wrong parameter id (%i) in %s", i, ClassName()), EInfo::kError);
      return;
    }
    fSubCut.SetMin(min, i);
    fSubCut.SetMax(max, i);
  }

  Cut** Cut::MultiClone(Int_t no, Bool_t increment_collections) {
    Cut** array = new Cut*[no];
    for (int i = 0; i < no; i++) {
      array[i] = this->MakeCopy();
    }
    if (increment_collections) {
      for (int i = 0; i < no; i++) {
        array[i]->SetCollectionID(i);
      }
    }
    return array;
  }

  Package* Cut::Report() const {
    Package* pack = new Package(this);
    pack->AddObject(new ParameterInt("UpdateRatio", static_cast<Int_t>(fUpdateRatio)));
    pack->AddObject(new ParameterInt("CutSize", fCutSize));
    for (Int_t i = 0; i < fCutSize; i++) {
      pack->AddObject(new ParameterString(Form("UnitName_%i", i), fSubCut.GetUnit(i)));
      pack->AddObject(new ParameterDouble(Form("MinCut_%i", i), fSubCut.GetMin(i)));
      pack->AddObject(new ParameterDouble(Form("MaxCut_%i", i), fSubCut.GetMax(i)));
    }
    pack->AddObject(new ParameterULong64("Passed", fPassed, '+'));
    pack->AddObject(new ParameterULong64("Failed", fFailed, '+'));
    pack->AddObject(new ParameterULong64("Total", fTotal, '+'));
    pack->AddObject(new ParameterInt("CollectionID", fCollectionID));
    return pack;
  }

  TObjArray* Cut::Split(Int_t n, Int_t i, Double_t shift) const {
    if (i >= fCutSize) {
      Cout::PrintInfo(Form("Canot split %s along %i", this->ClassName(), n), EInfo::kLowWarning);
      return NULL;
    }
    if (n <= 1) {
      Cout::PrintInfo(Form("Canot split %s into %i partts", this->ClassName(), i), EInfo::kLowWarning);
      return NULL;
    }
    TObjArray* cuts_array = new TObjArray();
    Double_t tmin         = GetMin(i);
    Double_t tmax         = GetMax(i);
    Double_t step         = (tmax - tmin) / ((Double_t) n);
    for (int j = 0; j < n; j++) {
      Cut* copy    = this->MakeCopy();
      Double_t min = tmin + j * step;
      Double_t max = tmin + (j + 1) * step;
      if (shift > 0 && j != 0) {
        min += shift;
      } else if (shift < 0 && j != n - 1) {
        max += shift;
      }
      copy->SetMinMax(min, max, i);
      cuts_array->AddLast(copy);
    }
    return cuts_array;
  }

  void Cut::Print(Option_t* /*option*/) const {
    Cout::InStars(Form("Report %s", this->ClassName()));
    Cout::Text("Cut ranges", "M");
    Cout::Database(3, "Parameter name", "Min", "Max");
    for (int i = 0; i < GetCutSize(); i++) {
      Cout::Database(3, GetUnit(i).Data(), Form("%4.2f", GetMin(i)), Form("%4.2f", GetMax(i)));
    }
    Cout::Text(Form("Passed %llu", GetPassed()), "M");
    Cout::Text(Form("Failed %llu", GetFailed()), "M");
  }

  Cut& Cut::operator=(const Cut& other) {
    if (this == &other) return *this;
    if (other.fCutSize != this->fCutSize) return *this;
    if (other.fUpdateRatio != this->fUpdateRatio) return *this;
    fPassed       = other.fPassed;
    fFailed       = other.fFailed;
    fTotal        = other.fTotal;
    fSubCut       = other.fSubCut;
    fInit         = other.fInit;
    fIsCloned     = other.fIsCloned;
    fState        = other.fState;
    fCollectionID = other.fCollectionID;
    fWeight       = other.fWeight;
    fLabel        = other.fLabel;
    return *this;
  }

  Bool_t Cut::FormatEquals(TString format, Int_t format_id, EFormatDepth depth) const {
    if (depth == EFormatDepth::kAll) {  // default depth, check manually
      if (this->InheritsFrom("Hal::TwoTrackCut"))
        depth = EFormatDepth::kBuffered;
      else
        depth = EFormatDepth::kNonBuffered;
    }
    const Event* ev   = DataFormatManager::Instance()->GetFormat(format_id, depth);
    TString classname = ev->ClassName();
    if (classname.EqualTo(format))
      return kTRUE;
    else
      return kFALSE;
  }

  Bool_t Cut::FormatInhertis(TString format, Int_t format_id, EFormatDepth depth) const {
    if (depth == EFormatDepth::kAll) {  // default depth, check manually
      if (this->InheritsFrom("Hal::TwoTrackCut"))
        depth = EFormatDepth::kBuffered;
      else
        depth = EFormatDepth::kNonBuffered;
    }
    const Event* ev = DataFormatManager::Instance()->GetFormat(format_id, depth);
    if (ev->InheritsFrom(format)) return kTRUE;
    return kFALSE;
  }

  TString Cut::CutName(Option_t* opt) const {
    TString option = opt;
    TString name;
    TString className = ClassName();
    if (fLabel >= 0) className = Form("%s_%i", className.Data(), fLabel);
    if (option.EqualTo("re")) {
      if (InheritsFrom("Hal::EventCut")) {
        name = Form("Hal::EventRealCut(%s)", className.Data());
      } else if (InheritsFrom("Hal::TrackCut")) {
        name = Form("Hal::TrackRealCut(%s)", className.Data());
      } else if (InheritsFrom("Hal::TwoTrackCut")) {
        name = Form("Hal::TwoTrackRealCut(%s)", className.Data());
      }
    } else if (option.EqualTo("im")) {
      if (InheritsFrom("Hal::EventCut")) {
        name = Form("Hal::EventImaginaryCut(%s)", className.Data());
      } else if (InheritsFrom("Hal::TrackCut")) {
        name = Form("Hal::TrackImaginaryCut(%s)", className.Data());
      } else if (InheritsFrom("Hal::TwoTrackCut")) {
        name = Form("Hal::TwoTrackImaginaryCut(%s)", className.Data());
      }
    }
    if (name.Length() == 0) name = className;
    return name;
  }

  Bool_t Cut::InLimits(Int_t par) const {
    if (GetValue(par) < GetMin(par)) return kFALSE;
    if (GetValue(par) > GetMax(par)) return kFALSE;
    return kTRUE;
  }

  std::vector<std::pair<TString, Double_t>> Cut::GetBinLabels(Int_t /*int1*/) const {
    std::vector<std::pair<TString, Double_t>> x;
    return x;
  }
}  // namespace Hal
