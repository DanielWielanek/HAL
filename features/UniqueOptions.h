/*
 * UniqueOptions.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_UNIQUEOPTIONS_H_
#define HAL_FEATURES_UNIQUEOPTIONS_H_

#include <TObject.h>
#include <TString.h>
#include <vector>
/**
 * holds uniqe options
 */
namespace Hal {

  class UniqueOptions : public TObject {
    std::vector<TString> fOpts;
    std::vector<std::vector<TString>> fConflicts;
    void OverwriteTag(TString newStr, TString oldStr);

  public:
    UniqueOptions() {};
    /**
     * add conflicts
     * @param list
     */
    void AddConflicts(std::initializer_list<TString> list);
    /**
     * add tag if possible
     * @param tag
     * @param overwrite if true then tag overwrites conflicts, in such case return true
     * otherwise do nothing and return false
     * @return
     */
    Bool_t AddTag(TString tag, Bool_t overwrite = kTRUE);
    /**
     * removes tag
     * @param tag
     * @return true if tag was found
     */
    Bool_t RemoveTag(TString tag);
    /**
     *
     * @param tag tag to found
     * @return true if tag found
     */
    Bool_t CheckTag(TString tag) const;
    virtual void Print(Option_t* option = "") const;
    virtual ~UniqueOptions() {};
    ClassDef(UniqueOptions, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_UNIQUEOPTIONS_H_ */
