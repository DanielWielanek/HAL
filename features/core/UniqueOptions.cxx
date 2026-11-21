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

#include <iostream>

namespace Hal {
  void UniqueOptions::OverwriteTag(TString newStr, TString oldStr) {
    for (auto& i : fOpts) {
      if (i == oldStr) {
        i = newStr;
        return;
      }
    }
  }

  void UniqueOptions::AddConflicts(std::initializer_list<TString> list) {
    auto vec = Hal::Std::GetVector(list);
    for (auto& p : vec)
      p.ToLower();
    fConflicts.push_back(vec);
  }

  Bool_t UniqueOptions::AddTag(TString tag, Bool_t overwrite) {
    tag.ToLower();
    for (auto conf : fConflicts) {
      for (auto potConf : conf) {
        if (potConf == tag) {  // potential conflict found
          for (auto potConf2 : conf) {
            if (CheckTag(potConf2)) {
              if (overwrite) {
                OverwriteTag(tag, potConf2);
                return kTRUE;
              }
              return kFALSE;
            }
          }
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

  void UniqueOptions::Print(Option_t* /*option*/) const {
    std::cout << "Conflicts" << std::endl;
    for (auto i : fConflicts) {
      std::cout << "\t";
      for (auto j : i) {
        std::cout << j << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "Tags" << std::endl;
    for (auto i : fOpts) {
      std::cout << "\t" << i << std::endl;
    }
  }

} /* namespace Hal */
