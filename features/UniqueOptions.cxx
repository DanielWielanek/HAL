/*
 * UniqueOptions.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "UniqueOptions.h"

#include "Std.h"

namespace Hal {
  void UniqueOptions::AddConflicts(std::initializer_list<TString> list) {
    auto vec = Hal::Std::GetVector(list);
    for (auto& p : vec)
      p.ToLower();
    fConflicts.push_back(vec);
  }

  Bool_t UniqueOptions::AddTag(TString tag, Bool_t overwrite) {
    tag.ToLower();
    for (unsigned int i = 0; i < fConflicts.size(); i++) {
      auto conf = fConflicts[i];
      for (auto j : conf) {
        if (j == tag) {  // conflict found
          if (overwrite)
            RemoveTag(j);
          else
            return kFALSE;
        }
      }
    }
    fOpts.push_back(tag);
    return kTRUE;
  }

  Bool_t UniqueOptions::RemoveTag(TString tag) {
    tag.ToLower();
    for (unsigned int i = 0; i < fOpts.size(); i++) {
      TString opt = fOpts[i];
      if (tag == opt) {
        fOpts.erase(fOpts.begin() + i);
        return kTRUE;
      }
    }
    return kFALSE;
  }

  Bool_t UniqueOptions::CheckTag(TString tag) const {
    tag.ToLower();
    for (auto i : fOpts) {
      if (i == tag) return kTRUE;
    }
    return kFALSE;
  }


} /* namespace Hal */
